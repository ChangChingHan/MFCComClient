#include "StdAfx.h"
#include "DataCeneter.h"
#include "atlbase.h"

#define REGISTSUBFOLDER_COMPANY		_T("SOFTWARE\\COMPANY\\CMS\\")
#define REGISTSUBFOLDER_ETRO		_T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")
#define REGIST_EVENT_LEVEL			_T("%sEVENT\\")
#define REGIST_RECORD_PATH			_T("%sEVENT\\")


void CDataCeneter::RegtblOperation(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_DDNS_INFO && bOperation <= GET_RECORD_PATH)
	{
		GetRegTbl(bOperation, VarData);
	}
	else if (bOperation >= SET_DDNS_INFO && bOperation <= SET_MAIL_SERVER)
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
	else if (bOperation >= GET_INTER_EVENT && bOperation <= GET_ALL_EVENT_BLACK) 
	{
		GetEventType(bOperation, VarData);
	}
	else if (bOperation >= GET_MAIL_SERVER && bOperation <= GET_MAIL_SERVER) 
	{
		GetMailServer(bOperation, VarData);
	}
	else if (bOperation >= GET_RECORD_PATH && bOperation <= GET_RECORD_PATH) 
	{
		GetRecordPath(bOperation, VarData);
	}
}

void CDataCeneter::GetRecordPath(BYTE bOperation, LPVOID VarData)
{
	wstring *pRecordPath = (wstring*)VarData;

	TCHAR szData[1024] = {0};
	ULONG ulChars = 1024;
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, GetRegFolder()) == ERROR_SUCCESS)
	{
		LONG nRet = regKey.QueryStringValue(_T("Recording Path"), szData,&ulChars);
		if(nRet != ERROR_SUCCESS)
		{
			vector<ec_Storage> vcStorageObj;
			ec_Storage data;
			data.storage_type = _T("RECORD");
			vcStorageObj.push_back(data);
			QueryFromDC(DATABASE,GET_STORAGE,(LPVOID)&vcStorageObj);
			if (vcStorageObj.size())
			{
				*pRecordPath = vcStorageObj[0].store_location;
			}
		}
		else
		{
			*pRecordPath = szData;
		}
	}
	regKey.Close();
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
	else if (bOperation == SET_MAIL_SERVER)
	{
		SetMailServer(bOperation, VarData);
	}
}

void CDataCeneter::GetDDNSInfo(BYTE bOperation, LPVOID VarData)
{
	struDDNS *pDDNSData = (struDDNS*)VarData;
	
	ULONG ulChars = 1024;
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, GetRegFolder()) == ERROR_SUCCESS)
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
	if( regKey.Open(HKEY_LOCAL_MACHINE, GetRegFolder()) == ERROR_SUCCESS)
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

	if (bOperation == GET_INTER_EVENT || bOperation == GET_INTER_EVENT_BLACK ||
		bOperation == GET_ALL_EVENT || bOperation == GET_ALL_EVENT_BLACK)
	{
		vcEventType.clear();
		GetInterEventType(vcEventType);

		CString str;
		if (regKey.Open(HKEY_LOCAL_MACHINE, GetEventRegFolder()) == ERROR_SUCCESS)
		{
			int nIdx = 0, nCount = (int)vcEventType.size();
			for (nIdx=0 ; nIdx<nCount ; nIdx++)
			{
				str.Format(_T("%d"),(DWORD)vcEventType[nIdx].first);
				if (regKey.QueryDWORDValue(str, (DWORD&)EventSeverity.event_level) == ERROR_SUCCESS)
				{
					EventSeverity.event_type = vcEventType[nIdx].first;
					EventSeverity.event_name = vcEventType[nIdx].second;

					if (bOperation == GET_INTER_EVENT_BLACK || bOperation == GET_ALL_EVENT_BLACK)
					{
						EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level,true);
					} 
					else
					{
						EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level);
					}
					
					pArray->push_back(EventSeverity);
				}
			}
			regKey.Close();
		}
		else	//default
		{
			int nIdx = 0, nCount = (int)vcEventType.size();
			for (nIdx=0 ; nIdx<nCount ; nIdx++)
			{
				EventSeverity.event_level = EVENT_CRITICAL;
				EventSeverity.event_type = vcEventType[nIdx].first;
				EventSeverity.event_name = vcEventType[nIdx].second;

				if (bOperation == GET_INTER_EVENT_BLACK || bOperation == GET_ALL_EVENT_BLACK)
				{
					EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level,true);
				} 
				else
				{
					EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level);
				}
				pArray->push_back(EventSeverity);
			}
		}
	}

	if (bOperation == GET_INTRA_EVENT || bOperation == GET_INTRA_EVENT_BLACK ||
		bOperation == GET_ALL_EVENT	|| bOperation == GET_ALL_EVENT_BLACK)
	{
		vcEventType.clear();
		GetIntraEventType(vcEventType);
		int nIdx = 0, nCount = (int)vcEventType.size();
		for (nIdx=0 ; nIdx<nCount ; nIdx++)
		{
			EventSeverity.event_level = EVENT_SYSTEM;
			EventSeverity.event_type = vcEventType[nIdx].first;
			EventSeverity.event_name = vcEventType[nIdx].second;

			if (bOperation == GET_INTRA_EVENT_BLACK || bOperation == GET_ALL_EVENT_BLACK)
			{
				EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level,true);
			} 
			else
			{
				EventSeverity.event_color = GetEventSeverityColor(EventSeverity.event_level);
			}
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

	lResult = regKey.Open(HKEY_LOCAL_MACHINE, GetEventRegFolder());
	if( lResult != ERROR_SUCCESS)
		lResult = regKey.Create(HKEY_LOCAL_MACHINE, GetEventRegFolder());

	if (lResult == ERROR_SUCCESS)
	{
		int nIdx = 0, nCount = (int)pArray->size();
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
	pairEvent.second = _T("Motion Detection");
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
	pairEvent.second = _T("Overwrite file");
	vcEventType.push_back(pairEvent);

	pairEvent.first = DISCONNECT;
	pairEvent.second = _T("Disconnect");
	vcEventType.push_back(pairEvent);

	pairEvent.first = RECORDING_FAIL;
	pairEvent.second = _T("Recording Status Change");
	vcEventType.push_back(pairEvent);
}

CString CDataCeneter::GetRegFolder()
{
	CRegKey regKey;

	if (regKey.Open(HKEY_LOCAL_MACHINE, REGISTSUBFOLDER_ETRO) == ERROR_SUCCESS)
	{
		return REGISTSUBFOLDER_ETRO;
	}
	else
	{	
		return REGISTSUBFOLDER_COMPANY;
	}
}

CString CDataCeneter::GetEventRegFolder()
{
	CString str;
	str.Format(REGIST_EVENT_LEVEL, GetRegFolder());
	return str;
}

void CDataCeneter::GetMailServer(BYTE bOperation, LPVOID VarData)
{
	struMail *pMailData = (struMail*)VarData;

	TCHAR szData[1024] = {0};
	ULONG ulChars = 1024;
	CRegKey regKey;

	if( regKey.Open(HKEY_LOCAL_MACHINE, GetRegFolder()) == ERROR_SUCCESS)
	{
		regKey.QueryStringValue(_T("SMTPSETTING"), szData,&ulChars);
	}
	regKey.Close();

	CString csSMTPSecure, csSMTPServer, csSMTPPort, csSMTPAccount, csSMTPPassword;
	AfxExtractSubString(csSMTPSecure, szData, 0, ';');
	AfxExtractSubString(csSMTPServer, szData, 1, ';');
	AfxExtractSubString(csSMTPPort, szData, 2, ';');
	AfxExtractSubString(csSMTPAccount, szData, 3, ';');
	AfxExtractSubString(csSMTPPassword, szData, 4, ';');

	pMailData->SMTPSecure = (SMTP_SECURE)_ttoi(csSMTPSecure);
	pMailData->SMTPServer = csSMTPServer;
	pMailData->SMTPPort = csSMTPPort;
	pMailData->SMTPAccount = csSMTPAccount;
	pMailData->SMTPPassword = csSMTPPassword;
}

void CDataCeneter::SetMailServer(BYTE bOperation, LPVOID VarData)
{
	struMail *pMailData = (struMail*)VarData;

	CString csRegValue;
	csRegValue.Format(_T("%d;%s;%s;%s;%s;"), (int)pMailData->SMTPSecure, 
											pMailData->SMTPServer.c_str(), 
											pMailData->SMTPPort.c_str(), 
											pMailData->SMTPAccount.c_str(), 
											pMailData->SMTPPassword.c_str());
	CRegKey regKey;
	if( regKey.Open(HKEY_LOCAL_MACHINE, GetRegFolder()) == ERROR_SUCCESS)
	{
		regKey.SetValue(csRegValue.GetBuffer(), _T("SMTPSETTING"));
		csRegValue.ReleaseBuffer();
	}
	regKey.Close();

}

COLORREF CDataCeneter::GetEventSeverityColor(EVENT_LEVEL event_level, bool bBlack)
{
	COLORREF event_color;

	switch(event_level)
	{
	case EVENT_CRITICAL:
		{
			event_color = bBlack ? CRITICAL_COLOR_BLACK:CRITICAL_COLOR;
		}
		break;
	case EVENT_WARNING:
		{
			event_color = bBlack ? WARNING_COLOR_BLACK:WARNING_COLOR;
		}	
		break;
	case EVENT_INFORMATION:
		{
			event_color = bBlack ? INFORMATION_COLOR_BLACK:INFORMATION_COLOR;
		}
		break;
	case EVENT_SYSTEM:
		{
			event_color = bBlack ? SYSTEM_COLOR_BLACK:SYSTEM_COLOR;
		}
		break;
	}

	return event_color;
}