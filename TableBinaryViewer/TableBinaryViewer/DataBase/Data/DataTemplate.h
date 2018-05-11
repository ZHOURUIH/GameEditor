#ifndef _DATA_TEMPLATE_H_
#define _DATA_TEMPLATE_H_

#include "Data.h"

class DataTemplate : public Data
{
public:
	DataTemplate(const std::string& type)
		:
		Data(type)
	{}
	virtual ~DataTemplate(){ destroy(); }
	void destroy();
	bool configParam(const std::vector<std::vector<std::string> >& paramList);
	bool getConfigParam(std::vector<std::string>& paramList);
	Data* createData() const;
	void destroyData(Data* data) const;
	void addTemplateParam(const std::string& type, const std::string& describe, int paramPos, std::vector<Data*> dataList);
	void addTemplateArrayParam(const std::string& type, int count, const std::string& describe, int paramPos, std::vector<Data*> dataList);
	void deleteTemplateParam(int paramPos, std::vector<Data*> dataList);
	void moveTemplateParam(int paramPos, int destPos, std::vector<Data*> dataList);
protected:
};

#endif