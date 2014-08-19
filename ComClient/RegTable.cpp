#include "StdAfx.h"
#include "DataCeneter.h"
#include "atlbase.h"

#define REGISTSUBFOLDER _T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")

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
	if (bOperation == SET_EVENT_COLOR)
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
	vector<strucEventColor> *pArray = (vector<strucEventColor>*)VarData;
}

void CDataCeneter::SetEventColor(LPVOID VarData)
{
	vector<strucEventColor> *pArray = (vector<strucEventColor>*)VarData;
}