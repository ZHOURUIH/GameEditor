#ifndef _FILE_LIST_H_
#define _FILE_LIST_H_

#include "SystemUtility.h"

class FileInfo;
class FileList : public SystemUtility
{
public:
	static void generateFileList();
protected:
	static void destroyMapElement(myMap<string, FileInfo*>& list, const string& element);
protected:
	static string VERSION;
	static string FILELIST;
	static string GAME_PDB;
};

#endif