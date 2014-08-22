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
	void QueryFromDC(unsigned char bDataType, unsigned char bOperation, LPVOID varData);

public:
	CDataCeneter(void);
	~CDataCeneter(void);

private:
	CDataMgr			m_dataMgr;
	map<int, CString>	m_mapDeviceURL;

private:
	/************************************************************************/
	/* initial + utility function                                                            
	/************************************************************************/
	void InitialDeviceIni();
	void GetDeviceURL(int nDeviceId, LPTSTR pURL);
	void WritetDeviceURL(int nDeviceId, CString& strURL);
	bool IsURLAddress(CString& strAddress);
	void GetAllEventType(vector<EVENTTYPE>& vcEventType);
	void GetCamByMac(ec_Camera& cam, const wstring& strMac);

	/************************************************************************/
	/* define register table function                  
	/************************************************************************/
	void RegtblOperation(BYTE bOperation, LPVOID VarData);

	void GetRegTbl(BYTE bOperation, LPVOID VarData);
	void SetRegTbl(BYTE bOperation, LPVOID VarData);

	void GetDDNSInfo(BYTE bOperation, LPVOID VarData);
	void SetDDNSInfo(BYTE bOperation, LPVOID VarData);

	void GetEventColor(LPVOID VarData);
	void SetEventColor(LPVOID VarData);
	/************************************************************************/
	/* define database function                  
	/************************************************************************/
	void DatabaseOperation(BYTE bOperation, LPVOID VarData);
	
	void QueryDatabase(BYTE bOperation, LPVOID VarData);
	void QueryGroupTbl(BYTE bOperation, LPVOID VarData);
	void QueryGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void QueryCamTbl(BYTE bOperation, LPVOID VarData);
	void QueryStreamTbl(BYTE bOperation, LPVOID VarData);
	void QueryRecordTbl(BYTE bOperation, LPVOID VarData);
	void QueryStorageTbl(BYTE bOperation, LPVOID VarData);
	void QueryParamTbl(BYTE bOperation, LPVOID VarData);
	void QueryEventlogTbl(BYTE bOperation, LPVOID VarData);
	void QueryEventActionTbl(BYTE bOperation, LPVOID VarData);

	void InsertDatabase(BYTE bOperation, void* VarData);
	void InsertGroupTbl(BYTE bOperation, LPVOID VarData);
	void InsertCamTbl(BYTE bOperation, LPVOID VarData, BOOL bInserStream = TRUE);
	void InsertRecordTbl(BYTE bOperation, LPVOID VarData);
	void InsertGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void InsertStreamTbl(BYTE bOperation, LPVOID VarData);
	void InsertEventActionTbl(BYTE bOperation, LPVOID VarData);
	void PrepareInsertStreamTbl(vector<ec_Stream>& vcStream);

	void DeleteDatabase(BYTE bOperation, void* VarData);
	void DeleteGroupTbl(BYTE bOperation, LPVOID VarData);
	void DeleteGroupCamTbl(BYTE bOperation, LPVOID VarData);
	void DeleteRecordTbl(BYTE bOperation, LPVOID VarData);
	void DeleteCamTbl(BYTE bOperation, LPVOID VarData);
	void DeleteStreamTbl(BYTE bOperation, LPVOID VarData);
	void DeleteEventActionTbl(BYTE bOperation, LPVOID VarData);

	void UpdateDatabase(BYTE bOperation, void* VarData);
	void UpdateGroupTbl(BYTE bOperation, void* VarData);
	void UpdateGroupCamTbl(BYTE bOperation, void* VarData);
	void UpdateCamTbl(BYTE bOperation, void* VarData);
	void UpdateEventActionTbl(BYTE bOperation, void* VarData);
};
