//#ifndef _TX_SERIALIZED_DATA_H_
//#define _TX_SERIALIZED_DATA_H_
//
//#include <string.h>
//#include <vector>
//#include <stdlib.h>
//
//struct DataParameter
//{
//	char* mDataPtr;
//	int mDataSize;
//	std::string mDataType;
//	std::string mDescribe;
//};
//
//class txSerializedData
//{
//public:
//	txSerializedData(){}
//	virtual ~txSerializedData(){}
//
//	bool read(char* pBuffer, int bufferSize);
//	bool write(char* pBuffer, int bufferSize);
//	bool writeData(std::string dataString, int paramIndex);
//	bool writeData(char* buffer, int bufferSize, int paramIndex);
//	std::string getValueString(int paramIndex);
//	bool readStringList(std::vector<std::string>& dataList);
//	int getSize();
//	bool readByte(char* dest, char* source, int& curSourceOffset, int sourceSize, int readSize)
//	{
//		if (curSourceOffset + readSize > sourceSize)
//		{
//			return false;
//		}
//		memcpy(dest, source + curSourceOffset, readSize);
//		curSourceOffset += readSize;
//		return true;
//	}
//	bool writeByte(char* dest, char* source, int& destOffset, int destSize, int writeSize)
//	{
//		if (destOffset + writeSize > destSize)
//		{
//			return false;
//		}
//		memcpy(dest + destOffset, source, writeSize);
//		destOffset += writeSize;
//		return true;
//	}
//	virtual void fillParams() = 0;
//	void zeroParams();
//	template<typename T>
//	void pushParam(T& param, std::string describe)
//	{
//		DataParameter parameter;
//		parameter.mDataPtr = (char*)&param;
//		parameter.mDataSize = sizeof(param);
//		parameter.mDataType = typeid(T).name();
//		parameter.mDescribe = describe;
//		mDataParameterList.push_back(parameter);
//	}
//	template<typename T>
//	void pushArrayParam(T* param, int count, std::string describe)
//	{
//		DataParameter parameter;
//		parameter.mDataPtr = (char*)param;
//		parameter.mDataSize = sizeof(param[0]) * count;
//		parameter.mDataType = typeid(T*).name();
//		parameter.mDescribe = describe;
//		mDataParameterList.push_back(parameter);
//	}
//protected:
//	std::vector<DataParameter> mDataParameterList;
//};
//
//#endif