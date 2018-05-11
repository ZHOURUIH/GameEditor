#include "txEngineDefine.h"

#include "txTexture.h"
#include "txEngineRoot.h"
#include "txFreeTypeFont.h"
#include "InputSystem.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "EditBoxProperty.h"
#include "LayoutUtility.h"

	EditBox::EditBox(const std::string& name, const WINDOW_TYPE& type)
		:
		ComplexWindow(name, type),
		mBackground(NULL),
		mCaret(NULL),
		mTextWindow(NULL),
		mShowCaret(true),
		mTimeCount(0.0f),
		mMaxTextLength(32),
		mNeedUpdateCaret(true)
	{}

	void EditBox::init(Layout* layout)
	{
		ComplexWindow::init(layout);
		mBackground = (TextureWindow*)createInnerChild("TextureWindow", mName + "_background");
		mTextWindow = (TextWindow*)createInnerChild("TextWindow", mName + "_text");
		mCaret = (TextureWindow*)createInnerChild("TextureWindow", mName + "_caret");
		mTextWindowTip = (TextureWindow*)createInnerChild("TextureWindow", mName + "_texttip");
		addInnerChild(mBackground);
		mBackground->addChild(mTextWindow);
		mCaret->setVisible(false);
		mTextWindow->addChild(mCaret);
		mTextWindow->addChild(mTextWindowTip);
	}

	void EditBox::initProperty()
	{
		ComplexWindow::initProperty();
		addProperty<EditBoxPropertyShowCaret>();
		addProperty<EditBoxPropertyMaxTextLength>();
		addProperty<EditBoxPropertyEditText>();
	}

	void EditBox::update(const float& elapsedTime)
	{
		if (!isVisible())
		{
			return;
		}

		ComplexWindow::update(elapsedTime);
		if (mNeedUpdateCaret)
		{
			updateCaretPos();
			mNeedUpdateCaret = false;
		}
		if (mCaret->isVisible())
		{
			float alpha = std::sin((mTimeCount + 0.25f) * 2.0f * txMath::MATH_PI);
			alpha = (alpha + 1.0f) / 2.0f;
			mCaret->setAlpha(alpha);
			mTimeCount += elapsedTime;
		}
	}
	void EditBox::processKey(const float& elapsedTime)
	{
		// 控制按键
		if (mInputSystem->isKeyCurrentDown(VK_BACK))
		{
			eraseLastText(false);
		}
		else if (mInputSystem->isKeyCurrentDown(VK_RETURN))
		{
			pushWindowEvent(WE_RETURN);
		}
		else if (mInputSystem->isKeyCurrentDown(VK_TAB))
		{
			pushWindowEvent(WE_TABLE);
		}
		// 字符
		auto& curCharList = mInputSystem->getCurCharList();
		auto iterChar = curCharList.begin();
		auto iterCharEnd = curCharList.end();
		FOR_STL(curCharList, ; iterChar != iterCharEnd; ++iterChar)
		{
			int key = *iterChar;
			// 字符输入事件,找出其中的字符按键,从32到127的都是字符
			if (key >= 32 && key <= 127)
			{
				char text[2] = { (char)key, '\0' };
				appendText(std::string(text), false);
			}
		}
		END_FOR_STL(curCharList);
	}
	void EditBox::setText(const std::string& text, const bool& async)
	{
		std::string newText = text;
		if ((int)newText.length() > mMaxTextLength)
		{
			newText = newText.substr(0, mMaxTextLength);
		}
		mTextWindow->setText(newText, async);
		mTextWindowTip->setVisible(newText.length() == 0);
		updateCaretPos();
	}

	void EditBox::clearText(const bool& async)
	{
		setText(EMPTY_STRING, async);
	}

	void EditBox::appendText(const std::string& text, const bool& async)
	{
		if ((int)mTextWindow->getText().length() + (int)text.length() > mMaxTextLength)
		{
			return;
		}
		setText(mTextWindow->getText() + text, async);
	}

	void EditBox::eraseLastText(const bool& async)
	{
		const std::string& curText = mTextWindow->getText();
		std::wstring unicodeText = txStringUtility::ANSIToUnicode(curText);
		if (unicodeText.length() > 0)
		{
			unicodeText = unicodeText.substr(0, unicodeText.length() - 1);
			std::string newText = txStringUtility::UnicodeToANSI(unicodeText);
			setText(newText, async);
		}
	}
	void EditBox::updateCaretPos()
	{
		txPoint caretPos = mCaret->getPosition();
		// 这里需要去除由于自适应缩放而引起的位置大小改变
		VECTOR2 textOriSize = mTextWindow->getPixelSize();
		if (txMath::isFloatZero(textOriSize.x))
		{
			textOriSize = LayoutUtility::getWindowSize(mTextWindow);
		}
		const VECTOR2& textFinalSize = mTextWindow->getFinalSize();
		float widthScale = textFinalSize.x / textOriSize.x;
		if (txMath::isFloatZero(textFinalSize.x))
		{
			widthScale = 1.0f;
		}
		caretPos.x.mAbsolute = (int)((float)mTextWindow->getContentWidth() / widthScale);
		mCaret->setPosition(caretPos);
	}
	void EditBox::setFocus(const bool& focus)
	{
		ComplexWindow::setFocus(focus);
		mCaret->setVisible(mShowCaret && focus);
		mCaret->setAlpha(1.0f);
		mTimeCount = 0.0f;
	}
	void EditBox::onLeftDown(const VECTOR2& position)
	{
		ComplexWindow::onLeftDown(position);
		bool mouseInEdit = mTextWindow->isPointInRect(position, false);
		SetCursor(mouseInEdit ? LoadCursor(NULL, IDC_IBEAM) : LoadCursor(NULL, IDC_ARROW));
	}
	void EditBox::onRightDown(const VECTOR2& position)
	{
		ComplexWindow::onRightDown(position);
		bool mouseInEdit = mTextWindow->isPointInRect(position, false);
		SetCursor(mouseInEdit ? LoadCursor(NULL, IDC_IBEAM) : LoadCursor(NULL, IDC_ARROW));
	}

	void EditBox::onMouseLeave()
	{
		ComplexWindow::onMouseLeave();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	void EditBox::onMouseMove(const VECTOR2& position)
	{
		ComplexWindow::onMouseMove(position);
		bool mouseInEdit = mTextWindow->isPointInRect(position, false);
		SetCursor(mouseInEdit ? LoadCursor(NULL, IDC_IBEAM) : LoadCursor(NULL, IDC_ARROW));
	}

	const std::string& EditBox::getText()
	{
		if (NULL != mTextWindow)
		{
			return mTextWindow->getText();
		}
		return EMPTY_STRING;
	}
	void EditBox::renameInnerChildren(const std::string& namePrefix)
	{
		mBackground->rename(namePrefix + "_background");
		mTextWindow->rename(namePrefix + "_text");
		mCaret->rename(namePrefix + "_caret");
		mTextWindowTip->rename(namePrefix + "_texttip");
	}