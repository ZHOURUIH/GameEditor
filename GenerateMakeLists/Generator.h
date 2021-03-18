#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "FrameUtility.h"

class Generator : public FrameUtility
{
public:
	static myMap<string, string> readConfig(const string& configPath);
	static myVector<string> generateDirectory(const string& path, const myVector<string>& exceptList);
	static myVector<string> generateSourceFile(const string& path, const myVector<string>& exceptDirList, const myVector<string>& exceptFileList);
protected:
	static void removeIgnorePath(myVector<string>& pathList, const myVector<string>& ignoreList);
	static void removeIgnoreFile(myVector<string>& fileList, const myVector<string>& ignoreList);
};

#endif