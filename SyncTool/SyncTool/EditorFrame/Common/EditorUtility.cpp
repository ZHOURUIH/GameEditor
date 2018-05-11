#include "EditorUtility.h"
#include "EditorFrame.h"
#include "OKDialog.h"
#include "YesNoDialog.h"

void EditorUtility::logError(const std::string& info)
{
	OKDialog* ok = new OKDialog("错误");
	ok->setInfo(info);
	ok->ShowModal();
	delete ok;
}

void EditorUtility::logInfo(const std::string& info)
{
	OKDialog* ok = new OKDialog("提示");
	ok->setInfo(info);
	ok->ShowModal();
	delete ok;
}

int EditorUtility::askYesNODialog(const std::string& info)
{
	YesNODialog* yesNO = new YesNODialog("提示");
	yesNO->setInfo(info);
	int ret = yesNO->ShowModal();
	delete yesNO;
	return ret;
}