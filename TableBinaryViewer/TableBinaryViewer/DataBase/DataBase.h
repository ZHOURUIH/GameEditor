#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_

#include "DataDefine.h"

class Data;
class DataTemplate;
class DataFactoryBase;

class DataBase
{
public:
	DataBase(){}
	virtual ~DataBase(){ destroy(); }
	void setErrorCallback(errorMessage errorCallback) { mErrorCallback = errorCallback; }
	// 初始化所有数据
	void init(const bool& loadAllData);
	void loadAllDataFromFile();
	void destroyAllData();
	void destroyData(const std::string& type);
	// forceCover是否强制覆盖当前已加载的该表格的数据, reloadTemplate是否重新加载该类型的表格数据模板,如果在之前可能修改过模板,但是未保存,则应该重新加载
	void loadData(const std::string& filePath, const bool& forceCover, const bool& reloadTemplate);
	bool writeBinaryFile(const std::string& type);
	void destroy()
	{
		destroyAllData();
		// 再销毁工厂
		destroyDataTemplate();
	}
	// 创建一个数据类型
	DataTemplate* createDataTemplate(const std::string& type, const std::vector<std::string>& paramList);
	DataTemplate* createDataTemplate(const std::string& type, const std::vector<std::vector<std::string> >& paramList);
	void writeTemplateConfig(const std::string& type);
	// 创建一个数据结构
	Data* createData(const std::string& type);
	// 销毁一个数据,外部使用createData,但是没有加入到DataBase时,需要使用destroyData将数据销毁掉
	void destroyData(Data* data);
	// 得到数据数量
	int getDataCount(const std::string& type);
	// 查询数据
	Data* queryData(const std::string& type, int index);
	// 添加一行数据
	bool addData(const std::string& type, Data* data, int pos = -1);
	// 创建空的数据列表
	bool newData(const std::string& type);
	bool deleteData(const std::string& type, const int& index);
	bool txtToBinary(const std::string& txtFileName);
	const std::map<std::string, DataTemplate*>& getDataTemplateList() { return mDataTemplateList; }
	const DataTemplate* getDataTemplate(const std::string& type)
	{
		std::map<std::string, DataTemplate*>::iterator itrFind = mDataTemplateList.find(type);
		if (itrFind != mDataTemplateList.end())
		{
			return itrFind->second;
		}
		return NULL;
	}
	void addTemplateParam(const std::string& templateName, const std::string& type, const std::string& describe, int paramPos);
	void addTemplateArrayParam(const std::string& templateName, const std::string& type, int count, const std::string& describe, int paramPos);
	void deleteTemplateParam(const std::string& templateName, int paramPos);
	void moveTemplateParam(const std::string& templateName, int paramPos, int destPos);
	static errorMessage mErrorCallback;
protected:
	void initDataTemplate();
	void destroyDataTemplate();
	void readTemplateConfig(const std::string& fileName);
	void destroyTemplate(const std::string& );
protected:
	std::map<std::string, std::vector<Data*> > mDataStructList;
	std::map<std::string, DataTemplate*> mDataTemplateList;
};

#endif