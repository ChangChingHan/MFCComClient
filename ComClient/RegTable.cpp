#include "StdAfx.h"
#include "DataCeneter.h"
#include "atlbase.h"

#define REGISTSUBFOLDER		_T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")
#define REGIST_EVENT_LEVEL	_T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\EVENT\\")

void CDataCeneter::RegtblOperation(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_DDNS_INFO && bOperation <= GET_EVENT_COLOR)
	{
		GetRegTbl(bOperation, VarData);
	}
	else if (bOperation >= SET_DDNS_INFO && bOperation <= SET_EVENT_COLOR)
	{
		SetRegTbl(bOperation, VarData);
	}
}

void CDataCeneter::GetRegTbl(BYTE bOperation, LPVOID VarData)
{
	if (bOperation == GET_DDNS_INFO)
	{
		GetDDNSInfo(bOperation, VarData);
	}
	else if (bOperation == GET_EVENT_COLOR)
	{
		GetEventColor(VarData);
	}
}

void CDataCeneter::SetRegTbl(BYTE bOperation, LPVOID VarData)
{
	if (bOperation == SET_DDNS_INFO)
	{
		SetDDNSInfo(bOperation, VarData);
	}
	else if (bOperation == SET_EVENT_COLOR)
	{
		SetEventColor(VarData);
	}
}

void CDataCeneter::GetDDNSInfo(BYTE bOperation, LPVOID VarData)
{
	struDDNS *pDDNSData = (struDDNS*)VarData;
	
	ULONG ulChars = 1024;
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER) == ERROR_SUCCESS)
	{
		regKey.QueryDWORDValue(_T("EnableDDNS"), (DWORD&)pDDNSData->bEnableDDNS);
		regKey.QueryDWORDValue(_T("Provider"), (DWORD&)pDDNSData->Provider);
		regKey.QueryStringValue(_T("ProviderURL"), pDDNSData->szProviderURL,&ulChars);
		regKey.QueryStringValue(_T("DDNSUser"), pDDNSData->szUser,&ulChars);
		regKey.QueryStringValue(_T("DDNSPass"), pDDNSData->szPass,&ulChars);
		regKey.QueryDWORDValue(_T("IntervalTime"), (DWORD&)pDDNSData->nIntervalTime);
	}
	regKey.Close();
}

void CDataCeneter::SetDDNSInfo(BYTE bOperation, LPVOID VarData)
{
	struDDNS *pDDNSData = (struDDNS*)VarData;

	ULONG ulChars = 0;
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER) == ERROR_SUCCESS)
	{
		regKey.SetDWORDValue(_T("EnableDDNS"), pDDNSData->bEnableDDNS);
		regKey.SetDWORDValue(_T("Provider"), pDDNSData->Provider);
		regKey.SetStringValue(_T("ProviderURL"), pDDNSData->szProviderURL, REG_SZ);
		regKey.SetStringValue(_T("DDNSUser"), pDDNSData->szUser,REG_SZ);
		regKey.SetStringValue(_T("DDNSPass"), pDDNSData->szPass,REG_SZ);
		regKey.SetDWORDValue(_T("IntervalTime"), pDDNSData->nIntervalTime);
	}
	regKey.Close();
}
void CDataCeneter::GetEventColor(LPVOID VarData)
{
	CRegKey regKey;
	CString str;
	DWORD dValue = 0;
	strucEventColor EventColorData;

	vector<strucEventColor> *pArray = (vector<strucEventColor>*)VarData;
	vector<EVENTTYPE> vcEventType;
	GetAllEventType(vcEventType);
	
	if (regKey.Open(HKEY_LOCAL_MACHINE, REGIST_EVENT_LEVEL) == ERROR_SUCCESS)
	{
		int nIdx = 0, nCount = vcEventType.size();
		for (nIdx=0 ; nIdx<nCount ; nIdx++)
		{
			str.Format(_T("%d"),vcEventType[nIdx]);
			if (regKey.QueryDWORDValue(str, (DWORD&)EventColorData.event_level) == ERROR_SUCCESS)
			{
				EventColorData.event_type = vcEventType[nIdx];
				pArray->push_back(EventColorData);
			}
		}
	}

	regKey.Close();
}

void CDataCeneter::SetEventColor(LPVOID VarData)
{
	vector<strucEventColor> *pArray = (vector<strucEventColor>*)VarData;

	CRegKey regKey;
	LONG lResult = ERROR_SUCCESS;
	CString str;

	lResult = regKey.Open(HKEY_LOCAL_MACHINE, REGIST_EVENT_LEVEL);
	if( lResult != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_LOCAL_MACHINE, REGIST_EVENT_LEVEL);

	if (lResult == ERROR_SUCCESS)
	{
		int nIdx = 0, nCount = pArray->size();
		for (nIdx=0 ; nIdx<nCount ; nIdx++)
		{
			str.Format(_T("%d"),(*pArray)[nIdx].event_type);
			regKey.SetDWORDValue(str, (*pArray)[nIdx].event_level);
		}
	}

	regKey.Close();
}

void CDataCeneter::GetAllEventType(vector<EVENTTYPE>& vcEventType)
{
	vcEventType.push_back(MOTION_DETECTION);
	vcEventType.push_back(DIGITAL_INPUT);
	vcEventType.push_back(VIDEO_LOST);
	vcEventType.push_back(VIDEO_OPEN);
	vcEventType.push_back(REBOOT);
	vcEventType.push_back(DIGITAL_OUTPUT);
	vcEventType.push_back(MISSING_OBJECT);
	vcEventType.push_back(EVENT_TRIGGER);
	vcEventType.push_back(TRANSACTION);
	vcEventType.push_back(DISK_FULL);
	vcEventType.push_back(LOG_IN);
	vcEventType.push_back(LOG_OUT);
	vcEventType.push_back(DISCONNECT);
	vcEventType.push_back(RECORDING_FAIL);
}
