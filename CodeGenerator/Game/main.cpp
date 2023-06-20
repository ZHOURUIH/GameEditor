#include "Utility.h"
#include "CodeCmd.h"
#include "CodeSkill.h"
#include "CodeState.h"
#include "CodeMySQL.h"
#include "CodeSQLite.h"
#include "CodeNetPacket.h"
#include "CodeAchivement.h"
#include "CodeDTNode.h"
#include "CodeUnityBuild.h"
#include "CodeFrameSystem.h"
#include "CodeClassDeclare.h"
#include "CodeObjectItem.h"
#include "CodeStateGroup.h"
#include "CodeComponent.h"
#include "CodeSceneTrigger.h"
#include "CodeCondition.h"
#include "CodeFunctionParam.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		system("pause");
		return;
	}

	cout << "0.全部生成(除了网络通信协议代码)" << endl;
	cout << "1.全部生成(包括网络通信协议代码)" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeCmd::generate();
		CodeSkill::generate();
		CodeState::generate();
		CodeAchivement::generate();
		CodeDTNode::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeObjectItem::generate();
		CodeStateGroup::generate();
		CodeComponent::generate();
		CodeSceneTrigger::generate();
		CodeFunctionParam::generate();
		CodeCondition::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeCmd::generate();
		CodeSkill::generate();
		CodeState::generate();
		CodeAchivement::generate();
		CodeDTNode::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeObjectItem::generate();
		CodeStateGroup::generate();
		CodeComponent::generate();
		CodeSceneTrigger::generate();
		CodeFunctionParam::generate();
		CodeCondition::generate();
	}
	system("pause");
}