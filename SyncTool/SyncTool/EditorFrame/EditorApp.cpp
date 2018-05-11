#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorBusyInfo.h"
#include "resource.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "txUtility.h"
#include "EditorCore.h"

IMPLEMENT_APP(EditorApp)

BEGIN_EVENT_TABLE(EditorApp, wxApp)
EVT_KEY_DOWN(OnKeyDown)
EVT_KEY_UP(OnKeyUp)
EVT_MOUSEWHEEL(OnMouseWheel)

END_EVENT_TABLE()


EditorApp::EditorApp()
:
mBusyInfo(NULL),
mMainFrame(NULL)
{
	HWND hwnd = GetForegroundWindow();//直接获得前景窗口的句柄         
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1)));
}

EditorApp::~EditorApp()
{
	mMainFrame = NULL;
}

void EditorApp::destroyBusyInfo()
{
	TRACE_DELETE(mBusyInfo);
}

void EditorApp::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnKeyUp(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();
}

bool EditorApp::OnInit()
{
	std::string gymID = txUtility::getRegistryValue("Software\\北京踏行天际科技发展有限公司\\电能动力4.0", "REGISTE_CODE");
	if (gymID == "")
	{
		wxMessageBox("找不到设备的注册信息!");
		return false;
	}
	// 通过命令运行程序
	if (argc == 3)
	{
		// 使用批处理文件主动上传
		if (std::string(argv[1]) == "upload")
		{
			mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
			mBusyInfo->hide();
			wxSize size(600, 400);
			wxPoint pos((1920 - size.x) / 2, (1080 - size.x) / 2);
			mMainFrame = new EditorFrame(this, "上传文件", pos, size);
			mMainFrame->init();
			mMainFrame->Show(true);
			mMainFrame->setAutoStartGame(false);
			mMainFrame->setAutoCloseUploaded(true);
			txVector<std::pair<std::string, std::string>> fileList;
			std::string str = gymID + "/" + txUtility::getDateTimeString();
			fileList.push_back(std::make_pair(std::string(argv[2]), str));
			mMainFrame->start(fileList);
		}
		return true;
	}
	else
	{
		// 判断当前是否已经运行了该游戏,如果已经运行则直接退出
		txSet<std::string> processList;
		txUtility::getAllProcess(processList);  
		if (processList.find(GAME_NAME) != processList.end())
		{
			MessageBoxA(NULL, "游戏已经运行!", "提示", MB_OK);
			return false;
		}

		mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
		mBusyInfo->hide();
		wxSize size(600, 400);
		wxPoint pos((1920 - size.x) / 2, (1080 - size.x) / 2);
		mMainFrame = new EditorFrame(this, "版本更新", pos, size);
		mMainFrame->init();
		mMainFrame->Show(false);

		// 判断当前目录下是否有需要上传的文件
		// 只上传崩溃产生的文件,文件夹名格式20XX-XX-XX_XXXXX
		txVector<std::string> folders;
		FileUtility::findFolders("./", folders);
		FOR_STL(folders, int i = 0; i < folders.size(); )
		{
			// 移除./
			folders[i] = folders[i].substr(strlen("./"), folders[i].length() - strlen("./"));
			// 符合条件的文件夹保留,其余的移除
			if (folders[i].length() > 11 && StringUtility::startWith(folders[i], "20") && folders[i][4] == '-' && folders[i][7] == '-' && folders[i][10] == '_')
			{
				++i;
			}
			else
			{
				folders.erase(folders.begin() + i, false);
			}
		}
		END_FOR_STL(folders);
		// 查找所有需要上传的文件
		mMainFrame->setAutoCloseUploaded(false);
		mMainFrame->setAutoStartGame(true);
		txVector<std::string> files;
		int count = folders.size();
		FOR_STL(folders, int i = 0; i < count; ++i)
		{
			FileUtility::findFiles(folders[i], files);
		}
		END_FOR_STL(folders);
		txVector<std::pair<std::string, std::string>> uploadFiles;
		int uploadCount = files.size();
		FOR_STL(files, int i = 0; i < uploadCount; ++i)
		{
			std::string path = gymID + "/" + StringUtility::getParentDir(files[i]);
			uploadFiles.push_back(std::make_pair(files[i], path));
		}
		END_FOR_STL(files);
		mMainFrame->start(uploadFiles);
		return true;
	}
}