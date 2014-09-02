#include "StdAfx.h"
#include "DataCeneter.h"
#include "atlbase.h"

#define REGISTSUBFOLDER		_T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")
#define REGIST_EVENT_LEVEL	_T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\EVENT\\")

void CDataCeneter::RegtblOperation(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_DDNS_INFO && bOperation <= GET_ALL_EVENT)
	{
		GetRegTbl(bOperation, VarData);
	}
	else if (bOperation >= SET_DDNS_INFO && bOperation <= SET_EVENT_TYPE)
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
	else if (bOperation >= GET_INTER_EVENT && bOperation <= GET_ALL_EVENT) 
	{
		GetEventType(bOperation, VarData);
	}
}

void CDataCeneter::SetRegTbl(BYTE bOperation, LPVOID VarData)
{
	if (bOperation == SET_DDNS_INFO)
	{
		SetDDNSInfo(bOperation, VarData);
	}
	else if (bOperation == SET_EVENT_TYPE)
	{
		SetEventType(bOperation, VarData);
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
void CDataCeneter::GetEventType(BYTE bOperation, LPVOID VarData)
{
	CRegKey regKey;
	CString str;
	DWORD dValue = 0;
	strucEventSeverity EventSeverity;
	vector<strucEventSeverity> *pArray = (vector<strucEventSeverity>*)VarData;
	vector<pair<EVENTTYPE,wstring> > vcEventType;

	if (bOperation == GET_INTER_EVENT || bOperation == GET_ALL_EVENT)
	{
		vcEventType.clear();
		GetInterEventType(vcEventType);
		if (regKey.Open(HKEY_LOCAL_MACHINE, REGIST_EVENT_LEVEL) == ERROR_SUCCESS)
		{
			int nIdx = 0, nCount = vcEventType.size();
			for (nIdx=0 ; nIdx<nCount ; nIdx++)
			{
				str.Format(_T("%d"),vcEventType[nIdx]);
				if (regKey.QueryDWORDValue(str, (DWORD&)EventSeverity.event_level) == ERROR_SUCCESS)
				{
					EventSeverity.event_type = vcEventType[nIdx].first;
					EventSeverity.event_name = vcEventType[nIdx].second;
					EventSeverity.event_color = RGB(0,255,0);
					pArray->push_back(EventSeverity);
				}
			}
			regKey.Close();
		}
		else	//default
		{
			int nIdx = 0, nCount = vcEventType.size();
			for (nIdx=0 ; nIdx<nCount ; nIdx++)
			{
				EventSeverity.event_level = EVENT_CRITICAL;
				EventSeverity.event_type = vcEventType[nIdx].first;
				EventSeverity.event_name = vcEventType[nIdx].second;
				EventSeverity.event_color = RGB(0,255,0);
				pArray->push_back(EventSeverity);
			}
		}
	}

	if (bOperation == GET_INTRA_EVENT || bOperation == GET_ALL_EVENT)
	{
		vcEventType.clear();
		GetIntraEventType(vcEventType);
		int nIdx = 0, nCount = vcEventType.size();
		for (nIdx=0 ; nIdx<nCount ; nIdx++)
		{
			EventSeverity.event_level = EVENT_SYSTEM;
			EventSeverity.event_type = vcEventType[nIdx].first;
			EventSeverity.event_name = vcEventType[nIdx].second;
			EventSeverity.event_color = RGB(255,0,0);
			pArray->push_back(EventSeverity);
		}
	}
}

void CDataCeneter::SetEventType(BYTE bOperation, LPVOID VarData)
{
	vector<strucEventSeverity> *pArray = (vector<strucEventSeverity>*)VarData;

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

void CDataCeneter::GetInterEventType(vector< pair<EVENTTYPE,wstring> >& vcEventType)
{
	pair<EVENTTYPE,wstring> pairEvent;

	pairEvent.first = MOTION_DETECTION;
	pairEvent.second = _T("Motion Detect");
	vcEventType.push_back(pairEvent);

	pairEvent.first = DIGITAL_INPUT;
	pairEvent.second = _T("Digital Input");
	vcEventType.push_back(pairEvent);

	pairEvent.first = VIDEO_LOST;
	pairEvent.second = _T("Video Lost");
	vcEventType.push_back(pairEvent);

	pairEvent.first = VIDEO_OPEN;
	pairEvent.second = _T("Video Open");
	vcEventType.push_back(pairEvent);

	pairEvent.first = REBOOT;
	pairEvent.second = _T("REBOOT");
	vcEventType.push_back(pairEvent);

	pairEvent.first = DIGITAL_OUTPUT;
	pairEvent.second = _T("Digital Output");
	vcEventType.push_back(pairEvent);

	pairEvent.first = MISSING_OBJECT;
	pairEvent.second = _T("Missing Object");
	vcEventType.push_back(pairEvent);

	pairEvent.first = EVENT_TRIGGER;
	pairEvent.second = _T("Event Trigger");
	vcEventType.push_back(pairEvent);

	pairEvent.first = TRANSACTION;
	pairEvent.second = _T("Transaction");
	vcEventType.push_back(pairEvent);
}

void CDataCeneter::GetIntraEventType(vector< pair<EVENTTYPE,wstring> >& vcEventType)
{
	pair<EVENTTYPE,wstring> pairEvent;

	pairEvent.first = DISK_FULL;
	pairEvent.second = _T("Disk Full");
	vcEventType.push_back(pairEvent);

	pairEvent.first = DISCONNECT;
	pairEvent.second = _T("Disconnect");
	vcEventType.push_back(pairEvent);

	pairEvent.first = RECORDING_FAIL;
	pairEvent.second = _T("Recording Status Change");
	vcEventType.push_back(pairEvent);
}

