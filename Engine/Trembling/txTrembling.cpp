#include "Utility.h"
#include "txTrembling.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"

int txTrembling::mDimensionNameSeed = 0;

bool txTrembling::init(const std::string& filePath)
{
	mTremblingNodeList.clear();
	return readFile(filePath);
}

void txTrembling::destroy()
{
	int size = mTremblingNodeList.size();
	FOR_STL(mTremblingNodeList, int i = 0; i < size; ++i)
	{
		TRACE_DELETE(mTremblingNodeList[i]);
	}
	END_FOR_STL(mTremblingNodeList);
	mTremblingNodeList.clear();
}

void txTrembling::saveTrembling(std::string& stream)
{
	stream += "-i " + mInfo + "\r\n";
	stream += "-f " + mFormat + "\r\n";
	int nodeCount = mTremblingNodeList.size();
	FOR_STL(mTremblingNodeList, int i = 0; i < nodeCount; ++i)
	{
		mTremblingNodeList[i]->saveTrembingNode(stream);
	}
	END_FOR_STL(mTremblingNodeList);
}

bool txTrembling::readFile(const std::string& filePath)
{
	// 将文件的每一行处理后放入lineList中
	txVector<std::string> lineList;
	txStringUtility::split(txFileUtility::openTxtFile(filePath), "\r\n", lineList);

	// 将数据字符串拆分出来,放入valueList中
	txVector<std::pair<std::string, txMap<float, float> > > tremblingNodeData;
	int curNodeCount = 0;
	int lineCount = lineList.size();
	FOR_STL(lineList, int i = 0; i < lineCount; ++i)
	{
		const std::string& lineString = lineList[i];
		// 首先去掉所有的空格和制表符
		char* newStringBuffer = TRACE_NEW_ARRAY(char, lineString.length() + 1, newStringBuffer);
		memset(newStringBuffer, 0, lineString.length() + 1);
		int curLen = 0;
		int strLength = lineString.length();
		for (int j = 0; j < strLength; ++j)
		{
			if (lineString[j] != ' ' && lineString[j] != '\t')
			{
				newStringBuffer[curLen] = lineString[j];
				++curLen;
			}
		}
		std::string newString(newStringBuffer);
		TRACE_DELETE_ARRAY(newStringBuffer);

		// 如果该行是空的,或者是注释,则不进行处理
		if (newString.length() > 0 && newString.substr(0, 2) != std::string("//"))
		{
			// *为维度起始符标志,*代表一个维度的开始
			if (newString[0] == '*')
			{
				++curNodeCount;
				tremblingNodeData.push_back(std::pair<std::string, txMap<float, float> >(newString, txMap<float, float>()));
			}
			// -表示特殊信息的开始
			else if (newString[0] == '-')
			{
				if (newString.size() < 2)
				{
					break;
				}
				// 关键帧描述
				if (newString[1] == 'i')
				{
					mInfo = newString.substr(2, newString.length() - 2);
				}
				// 关键帧格式
				else if (newString[1] == 'f')
				{
					mFormat = newString.substr(2, newString.length() - 2);
				}
			}
			else
			{
				// 起始符之前不能有关键帧数据
				if (curNodeCount == 0)
				{
					break;
				}
				txVector<std::string> valueVector;
				txStringUtility::split(newString, ",", valueVector);
				if (valueVector.size() == 2)
				{
					float keyTime = txStringUtility::stringToFloat(valueVector[0]);
					float keyOffset = txStringUtility::stringToFloat(valueVector[1]);
					tremblingNodeData[curNodeCount - 1].second.insert(keyTime, keyOffset);
				}
			}
		}
	}
	END_FOR_STL(lineList);

	// 解析valueList中的数据字符串
	bool ret = true;
	txVector<std::string> valueVector;
	int nodeCount = tremblingNodeData.size();
	FOR_STL(tremblingNodeData, int i = 0; i < nodeCount; ++i)
	{
		valueVector.clear();
		txStringUtility::split(tremblingNodeData[i].first, "|", valueVector);
		if (valueVector.size() == 3)
		{
			addNode(valueVector[1], valueVector[2], tremblingNodeData[i].second);
		}
		else
		{
			ret = false;
			break;
		}
	}
	END_FOR_STL(tremblingNodeData);
	return ret;
}

txTremblingNode* txTrembling::addNode(const std::string& name, const std::string& info, txMap<float, float>& keyFrameList)
{
	std::string newName = name;
	if (newName == EMPTY_STRING)
	{
		newName = "Dimension" + txStringUtility::intToString(mDimensionNameSeed++);
	}
	txTremblingNode* node = TRACE_NEW(txTremblingNode, node, newName);
	node->setKeyFrameList(keyFrameList);
	node->setInfo(info);
	mTremblingNodeList.push_back(node);
	mTremblingMap.insert(newName, mTremblingNodeList.size() - 1);
	refreshLength();
	return node;
}

void txTrembling::deleteNode(const std::string& name)
{
	auto iterName = mTremblingMap.find(name);
	if (iterName == mTremblingMap.end())
	{
		return;
	}
	TRACE_DELETE(mTremblingNodeList[iterName->second]);
	mTremblingNodeList.erase(mTremblingNodeList.begin() + iterName->second);
	mTremblingMap.erase(iterName);
	refreshLength();
}

txTremblingNode* txTrembling::getNode(const std::string& name)
{
	auto iterTrembling = mTremblingMap.find(name);
	if (iterTrembling != mTremblingMap.end())
	{
		return mTremblingNodeList[iterTrembling->second];
	}
	return NULL;
}

void txTrembling::refreshLength()
{
	mLength = 0.0f;
	int nodeCount = mTremblingNodeList.size();
	FOR_STL(mTremblingNodeList, int i = 0; i < nodeCount; ++i)
	{
		mLength = txMath::getMax(mLength, mTremblingNodeList[i]->getLength());
	}
	END_FOR_STL(mTremblingNodeList);
}

void txTrembling::queryValue(const float& time, txVector<float>& valueList, const float& amplitude)
{
	valueList.clear();
	int count = mTremblingNodeList.size();
	FOR_STL(mTremblingNodeList, int i = 0; i < count; ++i)
	{
		float offset = mTremblingNodeList[i]->queryKeyFrame(time) * amplitude;
		valueList.push_back(offset);
	}
	END_FOR_STL(mTremblingNodeList);
}

txVector<float> txTrembling::queryValue(const float& time, const float& amplitude)
{
	txVector<float> valueList;
	queryValue(time, valueList, amplitude);
	return valueList;
}