#include "SQLiteTableBase.h"
#include "Serializer.h"

string ENCRYPT_KEY = "ASLDIHQWILDjadiuahrfiqwdo!@##*^%ishduhasf#*$^(][][dajfgsdfsdgbweghwoeeghwoegh*(%&#$";

int main()
{
	string dataBasePath;
	string destPath;
	Vector<string> lines;
	FileUtility::openTxtFileLines("./Config.txt", lines, true);
	FOR_VECTOR(lines)
	{
		Vector<string> params;
		StringUtility::split(lines[i], "=", params);
		if (params.size() != 2)
		{
			continue;
		}
		const string& paramName = params[0];
		if (paramName == "DataBasePath")
		{
			dataBasePath = params[1];
		}
		else if (paramName == "DestPath")
		{
			destPath = params[1];
		}
	}
	END(lines);

	Vector<string> files;
	FileUtility::findFiles(dataBasePath, files, ".db");
	FOR_VECTOR(files)
	{
		Serializer serializer;
		SQLiteTableBase* table = new SQLiteTableBase();
		table->setTableName(StringUtility::getFileNameNoSuffix(files[i], true));
		table->init(files[i]);

		SQLiteDataReader* reader = table->doSelect();
		while (reader->read())
		{
			int culumnCount = reader->getColumnCount();
			FOR_J(culumnCount)
			{
				SQLITE_DATATYPE type = reader->getDataType(j);
				if (type == SQLITE_DATATYPE::SQLITE_DATATYPE_INTEGER)
				{
					serializer.write(reader->getLLong(j));
				}
				else if (type == SQLITE_DATATYPE::SQLITE_DATATYPE_FLOAT)
				{
					serializer.write(reader->getFloat(j));
				}
				else if (type == SQLITE_DATATYPE::SQLITE_DATATYPE_TEXT)
				{
					string str;
					reader->getString(j, str);
					serializer.writeString(str.c_str());
				}
			}
		}
		table->releaseReader(reader);
		// 重新计算密钥
		string key = ENCRYPT_KEY + table->getTableName();
		key = FileUtility::generateFileMD5(key.c_str(), key.length());
		char* buffer = serializer.getWriteableBuffer();
		uint bufferSize = serializer.getBufferSize();
		FOR_J(bufferSize)
		{
			buffer[j] = (buffer[j] - ((2 * j) & 0xFF)) ^ key[j % key.length()];
		}
		string fullPath = destPath + "/" + table->getTableName() + ".bytes";
		serializer.writeToFile(fullPath);
		cout << "生成文件:" << fullPath << endl;
	}
	END(files);
	return 0;
}