#pragma once

#ifdef IMPL_STOCKCHART
#define LIBAPI_STOCKCHART __declspec(dllexport)
#else
#define LIBAPI_STOCKCHART __declspec(dllimport)
#endif 

#include "CDataMgr.h"

class LIBAPI_STOCKCHART CDataCeneter
{
public:
	CDataCeneter(void);
	~CDataCeneter(void);

private:
	CDataMgr m_dataMgr;

private:
	void DatabaseOperation(BYTE bOperation, LPVOID VarData);

	void QueryDatabase(BYTE bOperation, LPVOID VarData);
	void QueryGroupTbl(BYTE bOperation, LPVOID VarData);
	void QueryGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void QueryCamTbl(BYTE bOperation, LPVOID VarData);
	void QueryStreamTbl(BYTE bOperation, LPVOID VarData);
	void QueryRecordTbl(BYTE bOperation, LPVOID VarData);
	void QueryStorageTbl(BYTE bOperation, LPVOID VarData);
	void QueryParamTbl(BYTE bOperation, LPVOID VarData);

	void InsertDatabase(BYTE bOperation, void* VarData);
	void InsertGroupTbl(BYTE bOperation, LPVOID VarData);
	void InsertCamTbl(BYTE bOperation, LPVOID VarData, BOOL bInserStream = TRUE);
	void InsertRecordTbl(BYTE bOperation, LPVOID VarData);
	void InsertGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void InsertStreamTbl(BYTE bOperation, LPVOID VarData);
	void PrepareInsertStreamTbl(vector<ec_Stream>& vcStream);

	void DeleteDatabase(BYTE bOperation, void* VarData);
	void DeleteGroupTbl(BYTE bOperation, LPVOID VarData);
	void DeleteGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void DeleteRecordTbl(BYTE bOperation, LPVOID VarData);
	void DeleteCamTbl(BYTE bOperation, LPVOID VarData);
	void DeleteStreamTbl(BYTE bOperation, LPVOID VarData);

	void UpdateDatabase(BYTE bOperation, void* VarData);
	void UpdateGroupTbl(BYTE bOperation, void* VarData);
	void UpdateGroupCamTbl(BYTE bOperation, void* VarData);
	void UpdateCamTbl(BYTE bOperation, void* VarData);
	
public:
	void QueryFromDC(unsigned char bDataType, unsigned char bOperation, LPVOID varData);
};
