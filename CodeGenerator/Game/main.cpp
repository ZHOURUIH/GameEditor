#include "Utility.h"
#include "CodeCmd.h"
#include "CodeSkill.h"
#include "CodeState.h"
#include "CodeMySQL.h"
#include "CodeSQLite.h"
#include "CodeNetPacket.h"
#include "CodeAchivement.h"
#include "CodeEvent.h"
#include "CodeDTNode.h"
#include "CodeUnityBuild.h"
#include "CodeFrameSystem.h"
#include "CodeClassDeclare.h"
#include "CodeObjectItem.h"
#include "CodeStateGroup.h"
#include "CodeComponent.h"
#include "CodeSceneTrigger.h"

void main()
{
	if (!CodeUtility::initPath())
	{
		cout << "未找到环境变量SERVER_PROJECT_PATH或GAME_PROJECT_PATH" << endl;
		system("pause");
		return;
	}

	cout << "0.全部生成" << endl;
	cout << "1.网络通信协议代码" << endl;
	cout << "2.SQLite数据代码" << endl;
	cout << "3.MySQL数据代码" << endl;
	cout << "4.命令代码" << endl;
	cout << "5.技能代码" << endl;
	cout << "6.状态代码" << endl;
	cout << "7.成就代码" << endl;
	cout << "8.事件代码" << endl;
	cout << "9.行为树节点代码" << endl;
	cout << "10.UnityBuild文件" << endl;
	cout << "11.FrameSystem文件" << endl;
	cout << "12.类声明文件" << endl;
	cout << "13.场景触发器代码" << endl;
	int input;
	cin >> input;
	if (input == 0)
	{
		CodeNetPacket::generate();
		CodeSQLite::generate();
		CodeMySQL::generate();
		CodeCmd::generate();
		CodeSkill::generate();
		CodeState::generate();
		CodeAchivement::generate();
		CodeEvent::generate();
		CodeDTNode::generate();
		CodeUnityBuild::generate();
		CodeFrameSystem::generate();
		CodeClassDeclare::generate();
		CodeObjectItem::generate();
		CodeStateGroup::generate();
		CodeComponent::generate();
		CodeSceneTrigger::generate();
	}
	else if (input == 1)
	{
		CodeNetPacket::generate();
	}
	else if (input == 2)
	{
		CodeSQLite::generate();
	}
	else if (input == 3)
	{
		CodeMySQL::generate();
	}
	else if (input == 4)
	{
		CodeCmd::generate();
	}
	else if (input == 5)
	{
		CodeSkill::generate();
	}
	else if (input == 6)
	{
		CodeState::generate();
	}
	else if (input == 7)
	{
		CodeAchivement::generate();
	}
	else if (input == 8)
	{
		CodeEvent::generate();
	}
	else if (input == 9)
	{
		CodeDTNode::generate();
	}
	else if (input == 10)
	{
		CodeUnityBuild::generate();
	}
	else if (input == 11)
	{
		CodeFrameSystem::generate();
	}
	else if (input == 12)
	{
		CodeClassDeclare::generate();
	}
	else if (input == 13)
	{
		CodeSceneTrigger::generate();
	}
	system("pause");
}