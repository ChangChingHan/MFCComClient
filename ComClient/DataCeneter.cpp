#include "StdAfx.h"
#include "DataCeneter.h"

#define DEVICE_INI				_T(".\\device.ini")
#define STRING_INI_BUFFER		128
#define SECTION_URL				_T("URL")
#define PTZ_ACTION_KEY			_T("PTZAction")
#define PTZ_PRESET_KEY			_T("PTZPreset")
#define EMAIL_CONTENT_KEY		_T("EmailContent")
#define EXACUTE_FILE_KEY		_T("ExacuteFile")

CDataCeneter::CDataCeneter(void)
{
	InitialDeviceIni();
}

CDataCeneter::~CDataCeneter(void)
{
	map<int, CString>::iterator it = m_mapDeviceURL.begin();
	while (it != m_mapDeviceURL.end())
	{
		WritetDeviceURL(it->first,it->second);
		it++;
	}
}

void CDataCeneter::InitialDeviceIni()
{
	CSimpleArray<camera> Array;
	m_dataMgr.QueryFromDC(DATABASE,GET_CAM,(VARIANT*)&Array);

	CString strKey(_T(""));
	CString strURL;
	LPTSTR pstr = strURL.GetBuffer(STRING_INI_BUFFER);


	int nIdx = 0, nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		if (Array[nIdx].isURL)
		{
			GetDeviceURL(Array[nIdx].camera_idx, pstr);
			m_mapDeviceURL[Array[nIdx].camera_idx] = CString(pstr);
		}
	}
	strURL.ReleaseBuffer();
}

void CDataCeneter::WritetDeviceURL(int nDeviceId, CString& strURL)
{
	CString strKey(_T(""));
	strKey.Format(_T("cam%d"),nDeviceId);
	WritePrivateProfileString( SECTION_URL , strKey , strURL , DEVICE_INI );
}

void CDataCeneter::GetDeviceURL(int nDeviceId, LPTSTR pstr)
{
	CString strKey(_T(""));

	strKey.Format(_T("cam%d"),nDeviceId);
	GetPrivateProfileString( SECTION_URL , strKey , _T("") , pstr , STRING_INI_BUFFER , DEVICE_INI );
}

void CDataCeneter::QueryFromDC(unsigned char bDataType, unsigned char bOperation, LPVOID VarData)
{
	switch(bDataType)
	{
	case DATABASE:
		{
			DatabaseOperation(bOperation, VarData);
		}
		break;

	case REG_TBL:
		{
			RegtblOperation(bOperation, VarData);
		}
		break;
	default:
		break;
	}
}

void CDataCeneter::DatabaseOperation(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_GROUP && bOperation <= GET_EVENT_ACTION)
	{
		QueryDatabase(bOperation, VarData);
	}
	else if (bOperation >= INSERT_GROUP && bOperation <= INSERT_EVENT_ACTION)
	{
		InsertDatabase(bOperation, VarData);
	}
	else if(bOperation >= DELETE_GROUP_BY_DEVICE && bOperation <= DELETE_EVENT_ACTION)
	{
		DeleteDatabase(bOperation, VarData);
	}
	else if(bOperation >= UPDATE_GROUP_BY_ID && bOperation <= UPDATE_EVENT_ACTION)
	{
		UpdateDatabase(bOperation, VarData);
	}
}

void CDataCeneter::UpdateDatabase(BYTE bOperation, void* VarData)
{
	if (bOperation >= UPDATE_GROUP_BY_ID && bOperation <= UPDATE_GROUP_BY_TYPE)
	{
		UpdateGroupTbl( bOperation, VarData);
	}
	else if (bOperation >= UPDATE_GROUP_CAM_BY_CAMID && bOperation <= UPDATE_GROUP_CAM)
	{
		UpdateGroupCamTbl( bOperation, VarData);
	}
	else if (bOperation >= UPDATE_CAM && bOperation <= UPDATE_CAM)
	{
		UpdateCamTbl(bOperation, VarData);
	}
	else if (bOperation >= UPDATE_EVENT_ACTION && bOperation <= UPDATE_EVENT_ACTION)
	{
		UpdateEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::UpdateEventActionTbl(BYTE bOperation, void* VarData)
{

}

void CDataCeneter::UpdateGroupTbl(BYTE bOperation, void* VarData)
{
	InsertGroupTbl(bOperation, VarData);
}

void CDataCeneter::UpdateGroupCamTbl(BYTE bOperation, void* VarData)
{
	InsertGroupCamTbl(bOperation, VarData);
}

void CDataCeneter::UpdateCamTbl(BYTE bOperation, void* VarData)
{
	InsertCamTbl(bOperation, VarData, FALSE);
}

void CDataCeneter::DeleteDatabase(BYTE bOperation, void* VarData)
{
	if (bOperation >= DELETE_GROUP_BY_DEVICE && bOperation <= DELETE_GROUP_BY_ID)
	{
		DeleteGroupTbl( bOperation, VarData);
	}
	else if (bOperation >= DELETE_GROUP_CAM_BY_GRPID && bOperation <= DELETE_GROUP_CAM_BY_PRIM)
	{
		DeleteGroupCamTbl( bOperation, VarData);
	}
	else if (bOperation >= DELETE_CAM && bOperation <= DELETE_CAM)
	{
		DeleteCamTbl(bOperation, VarData);
	}
	else if (bOperation >= DELETE_STREAM && bOperation <= DELETE_STREAM)
	{
		DeleteStreamTbl(bOperation, VarData);
	}
	else if (bOperation >= DELETE_RECORD_BY_ID && bOperation <= DELETE_RECORD_TIME_BY_ID)
	{
		DeleteRecordTbl(bOperation, VarData);
	}
	else if (bOperation >= DELETE_EVENT_ACTION && bOperation <= DELETE_EVENT_ACTION)
	{
		DeleteEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::DeleteEventActionTbl(BYTE bOperation, LPVOID VarData)
{

}

void CDataCeneter::DeleteCamTbl(BYTE bOperation, LPVOID VarData)
{
	InsertCamTbl(bOperation, VarData, FALSE);
	
	map<int, CString>::iterator it;
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	int nIdx = 0, nCount = pArray->size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		if ((*pArray)[nIdx].isURL)
		{
			it = m_mapDeviceURL.find((*pArray)[nIdx].camera_idx);
			if (it != m_mapDeviceURL.end())
			{
				m_mapDeviceURL.erase(it);
			}
		}
	}
}

void CDataCeneter::DeleteStreamTbl(BYTE bOperation, LPVOID VarData)
{
	InsertStreamTbl(bOperation, VarData);
}

void CDataCeneter::DeleteRecordTbl(BYTE bOperation, LPVOID VarData)
{
	InsertRecordTbl(bOperation, VarData);
}

void CDataCeneter::DeleteGroupCamTbl(BYTE bOperation, LPVOID VarData)
{
	InsertGroupCamTbl(bOperation, VarData);
}

void CDataCeneter::DeleteGroupTbl(BYTE bOperation, LPVOID VarData)
{
	InsertGroupTbl(bOperation, VarData);
}

void CDataCeneter::InsertDatabase(BYTE bOperation, void* VarData)
{
	if (bOperation == INSERT_GROUP)
	{
		InsertGroupTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_CAM)
	{
		InsertCamTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_RECORD)
	{
		InsertRecordTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_GROUP_CAM)
	{
		InsertGroupCamTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_STREAM)
	{
		InsertStreamTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_EVENT_ACTION)
	{
		InsertEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::InsertEventActionTbl(BYTE bOperation, LPVOID VarData)
{
}

void CDataCeneter::InsertGroupCamTbl(BYTE bOperation, LPVOID VarData)
{
	group_camera data;
	vector<Cam_Group_Cam> *pArray = (vector<Cam_Group_Cam>*)VarData;
	CSimpleArray<group_camera> Array;
	int nIdx = 0, nCount = pArray->size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.cameragroupcameraid = (*pArray)[nIdx].cameragroupcameraid;
		data.cameragroupid = (*pArray)[nIdx].cameragroupid;
		data.cameraid = (*pArray)[nIdx].cameraid;
		data.camera_idx = (*pArray)[nIdx].camera_idx;
		data.streamid = (*pArray)[nIdx].streamid;
		data.numb_ = (*pArray)[nIdx].numb_;
		data.emap_x = (*pArray)[nIdx].emap_x;
		data.emap_y = (*pArray)[nIdx].emap_y;
		data.acmap_x = (*pArray)[nIdx].acmap_x;
		data.acmap_y = (*pArray)[nIdx].acmap_y;

		Array.Add(data);
	}

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertRecordTbl(BYTE bOperation, LPVOID VarData)
{
	video_record data;
	vector<ec_Camera_Rec> *pArray = (vector<ec_Camera_Rec>*)VarData;
	CSimpleArray<video_record> Array;
	int nIdx = 0, nCount = pArray->size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.camera_idx = (*pArray)[nIdx].camera_idx;
		data.recording_type = (*pArray)[nIdx].recording_type;
		data.recording_time = (*pArray)[nIdx].recording_time;
		Array.Add(data);
	}

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertCamTbl(BYTE bOperation, LPVOID VarData, BOOL bInserStream)
{
	camera data;
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	CSimpleArray<camera> Array;
	int nIdx = 0, nCount = pArray->size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.cameraid = (*pArray)[nIdx].cameraid;
		data.camera_idx = (*pArray)[nIdx].camera_idx;
		data.cameraname = (*pArray)[nIdx].cameraname;
		data.connect_type_code = (*pArray)[nIdx].connect_type_code;
		data.ipaddress = (*pArray)[nIdx].ipaddress;
		data.httpport = (*pArray)[nIdx].httpport;
		data.gateway = (*pArray)[nIdx].gateway;
		data.name_server = (*pArray)[nIdx].name_server;
		data.mac_address = (*pArray)[nIdx].mac_address;
		data.username = (*pArray)[nIdx].username;
		data.password = (*pArray)[nIdx].password;
		data.brand_code = (*pArray)[nIdx].brand_code;
		data.model_code = (*pArray)[nIdx].model_code;
		data.ptz_support = (*pArray)[nIdx].ptz_support;
		data.ptz_protocol = (*pArray)[nIdx].ptz_protocol;
		data.digital_in1 = (*pArray)[nIdx].digital_in1;
		data.digital_in2 = (*pArray)[nIdx].digital_in2;
		data.digital_out = (*pArray)[nIdx].digital_out;
		data.video_format = (*pArray)[nIdx].video_format;
		data.speaker_support = (*pArray)[nIdx].speaker_support;
		data.mic_support = (*pArray)[nIdx].mic_support;
		data.subnet_mask1 = (*pArray)[nIdx].subnet_mask1;
		data.subnet_mask2 = (*pArray)[nIdx].subnet_mask2;
		data.subnet_mask3 = (*pArray)[nIdx].subnet_mask3;
		data.subnet_mask4 = (*pArray)[nIdx].subnet_mask4;
		data.active_ = (*pArray)[nIdx].active_;
		data.stream_url = (*pArray)[nIdx].stream_url;

		if (bOperation == INSERT_CAM && IsURLAddress((*pArray)[nIdx].ipaddress))
		{
			data.isURL = true;
			data.ipaddress.clear();
			m_mapDeviceURL[(*pArray)[nIdx].camera_idx] = (*pArray)[nIdx].ipaddress;
		}

		Array.Add(data);
		m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
		Array.RemoveAll();

		if(bInserStream)
			PrepareInsertStreamTbl((*pArray)[nIdx].vcStream);
	}
}

void CDataCeneter::PrepareInsertStreamTbl(vector<ec_Stream>& vcStream)
{
	int nIdx = 0, nCount = vcStream.size();
	int nCameraid = 0;
	CSimpleArray<camera> Array;
	m_dataMgr.QueryFromDC(DATABASE,GET_CAM,(VARIANT*)&Array);
	if(Array.GetSize())
		nCameraid = Array[Array.GetSize()-1].cameraid;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		vcStream[nIdx].cameraid = nCameraid;
	}

	InsertStreamTbl(INSERT_STREAM,(LPVOID)&vcStream);
}

void CDataCeneter::InsertStreamTbl(BYTE bOperation, LPVOID VarData)
{
	video_stream data;
	vector<ec_Stream> *pArray = (vector<ec_Stream>*)VarData;
	CSimpleArray<video_stream> Array;
	int nIdx = 0, nCount = pArray->size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.stream_name = (*pArray)[nIdx].stream_name;
		data.stream_tag = (*pArray)[nIdx].stream_tag;
		data.cameraid = (*pArray)[nIdx].cameraid;
		data.stream_type = (*pArray)[nIdx].stream_type;
		data.stream_port = (*pArray)[nIdx].stream_port;
		data.stream_protocol = (*pArray)[nIdx].stream_protocol;
		data.video_cmpr_type = (*pArray)[nIdx].video_cmpr_type;
		data.video_res = (*pArray)[nIdx].video_res;
		Array.Add(data);
	}

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertGroupTbl(BYTE bOperation, LPVOID VarData)
{
	group data;
	vector<Cam_Group> *pArray = (vector<Cam_Group>*)VarData;
	CSimpleArray<group> Array;
	int nIdx = 0, nCount = pArray->size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.cameragroupid = (*pArray)[nIdx].cameragroupid;
		data.camera_group_desc = (*pArray)[nIdx].camera_group_desc;
		data.default_ = (*pArray)[nIdx].default_;
		data.category_code = (*pArray)[nIdx].category_code;
		data.module_code = (*pArray)[nIdx].module_code;
		data.map_file = (*pArray)[nIdx].map_file;
		data.acmap_file = (*pArray)[nIdx].acmap_file;
		Array.Add(data);
	}

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::QueryDatabase(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_GROUP && bOperation <= GET_GROUP_BY_LAYOUT)
	{
		QueryGroupTbl( bOperation, VarData);
	}
	else if (bOperation >= GET_GROUP_CAM && bOperation <= GET_GROUP_CAM_BY_STATION)
	{
		QueryGroupCamTbl( bOperation, VarData);
	}
	else if (bOperation >= GET_CAM && bOperation <= GET_CAM_BY_INDEX)
	{
		QueryCamTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_STREAM && bOperation <= GET_STREAM_BY_ID)
	{
		QueryStreamTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_RECORD && bOperation <= GET_RECORD)
	{
		QueryRecordTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_STORAGE && bOperation <= GET_STORAGE)
	{
		QueryStorageTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_PARAM && bOperation <= GET_PARAM)
	{
		QueryParamTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_EVENT_LOG && bOperation <= GET_EVENT_LOG)
	{
		QueryEventlogTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_EVENT_ACTION && bOperation <= GET_EVENT_ACTION)
	{
		QueryEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::QueryGroupTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<group> Array;
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	Cam_Group data;
	vector<Cam_Group> *pArray = (vector<Cam_Group>*)VarData;
	nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryGroupCamTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<group_camera> Array;
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);


	Cam_Group_Cam data;
	vector<Cam_Group_Cam> *pArray = (vector<Cam_Group_Cam>*)VarData;
	nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryCamTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<camera> Array;
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	ec_Camera data;
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	nCount = Array.GetSize();
	map<int,CString>::iterator it;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		if (data.isURL)
		{
			it = m_mapDeviceURL.find(data.camera_idx);
			if (it != m_mapDeviceURL.end())
			{
				data.ipaddress = it->second;
			}
		}
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryStreamTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<video_stream> Array;
	vector<ec_Stream> *pArray = NULL;

	if (bOperation == GET_STREAM_BY_ID)
	{
		pArray = (vector<ec_Stream>*)VarData;
		if (pArray && pArray->size())
		{
			video_stream data;
			data.cameraid = (*pArray)[0].cameraid;
			Array.Add(data);
		}
	}
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	ec_Stream data;
	pArray = (vector<ec_Stream>*)VarData;
	pArray->clear();
	nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryRecordTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<video_record> Array;
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	ec_Camera_Rec data;
	vector<ec_Camera_Rec> *pArray = (vector<ec_Camera_Rec>*)VarData;
	nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryStorageTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<storage> Array;
	vector<ec_Storage> *pArray = NULL;

	pArray = (vector<ec_Storage>*)VarData;
	if (pArray && pArray->size())
	{
		storage data;
		data.storage_type = (*pArray)[0].storage_type;
		Array.Add(data);
	}
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	ec_Storage data;
	pArray = (vector<ec_Storage>*)VarData;
	nCount = Array.GetSize();
	pArray->clear();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryEventActionTbl(BYTE bOperation, LPVOID VarData)
{
	CSimpleArray<eventaction> Array;
	vector<ec_Event_Action> *pArray = NULL;
	pArray = (vector<ec_Event_Action>*)VarData;

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
	int nIdx = 0, nCount = Array.GetSize();
	ec_Event_Action data;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		GetCamByMac(data.source_device, Array[nIdx].source_mac);
		GetCamByMac(data.target_device, Array[nIdx].target_mac);

		pArray->push_back(data);
	}
}

void CDataCeneter::GetEventActionDetail(vector<ec_Event_Action> *pArray)
{
	int nIdx = 0, nCount = pArray->size();
	 map<int,wstring> mapValue;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		GetEventActionDetailFromFile((*pArray)[nIdx]);
	}
}

void CDataCeneter::GetEventActionDetailFromFile(ec_Event_Action& eventAction)
{
	CString strKey(_T(""));
	strKey.Format(_T("action_%d"),eventAction.actionid);

	switch(eventAction.action_type)
	{
	case ACTION_PTZ:
		{
			eventAction.ptz_action = (PTZ_ACTION)GetPrivateProfileInt( strKey , PTZ_ACTION_KEY , PTZ_AUTOSCAN , DEVICE_INI );
			if (eventAction.ptz_action == PTZ_PRESET)
			{
				eventAction.ptz_preset = GetPrivateProfileInt( strKey , PTZ_PRESET_KEY , 0 , DEVICE_INI );
			}
		}
		break;
	case ACTION_EMAIL:
		{
			CString strValue;
			LPTSTR pstr = strValue.GetBuffer(STRING_INI_BUFFER);
			GetPrivateProfileString( strKey , EMAIL_CONTENT_KEY , _T("") , pstr , STRING_INI_BUFFER , DEVICE_INI );
			eventAction.email_content = pstr;
			strValue.ReleaseBuffer();
		}
		break;
	case ACTION_CUSTOM:
		{
			CString strValue;
			LPTSTR pstr = strValue.GetBuffer(STRING_INI_BUFFER);
			GetPrivateProfileString( strKey , EXACUTE_FILE_KEY , _T("") , pstr , STRING_INI_BUFFER , DEVICE_INI );
			eventAction.custom_path = pstr;
			strValue.ReleaseBuffer();
		}
		break;
	default:
		break;
	}
}

void CDataCeneter::QueryEventlogTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<eventlog> Array;
	vector<ec_Event_Log> *pArray = NULL;

	pArray = (vector<ec_Event_Log>*)VarData;
	if (pArray && pArray->size())
	{
		eventlog data;
		data.logcount = (*pArray)[0].logcount;
		data.event_type = (*pArray)[0].event_type;
		data.device_mac = (*pArray)[0].device.mac_address;
		data.start_time = (*pArray)[0].start_time;
		data.end_time = (*pArray)[0].end_time;
		Array.Add(data);
	}

	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
	ec_Event_Log data;
	nCount = Array.GetSize();
	pArray->clear();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		GetCamByMac(data.device, Array[nIdx].device_mac);
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryParamTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<parameter> Array;
	vector<ec_Param> *pArray = NULL;

	pArray = (vector<ec_Param>*)VarData;
	if (pArray && pArray->size())
	{
		parameter data;
		data.parm_name = (*pArray)[0].parm_name;
		Array.Add(data);
	}
	m_dataMgr.QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	ec_Param data;
	pArray = (vector<ec_Param>*)VarData;
	nCount = Array.GetSize();
	pArray->clear();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

bool CDataCeneter::IsURLAddress(CString& strAddress)
{
	bool bURLAddress = true;
	USES_CONVERSION;
	string stringAddress = W2A(strAddress);

	char *buffer = new char[stringAddress.length()+1];
	strcpy_s(buffer, stringAddress.length()+1, stringAddress.c_str());
	char *pch = NULL, *pbuffer = NULL;
	vector<string> vcString;

	pch = strtok_s(buffer,".",&pbuffer);
	while(pch != NULL)
	{
		vcString.push_back(pch);
		pch = strtok_s(NULL,".",&pbuffer);
	}
	delete [] buffer;

	if (vcString.size() == 4)
	{
		int a = atoi(vcString[0].c_str());
		int b = atoi(vcString[1].c_str());
		int c = atoi(vcString[2].c_str());
		int d = atoi(vcString[3].c_str());

		if (a && b && c && d && 
			a<=255 && b<=255 && c<=255 && d<=255)
		{
			bURLAddress = false;
		}
	}
	return bURLAddress;
}

void CDataCeneter::GetCamByMac(ec_Camera& cam, const wstring& strMac)
{
	CSimpleArray<camera> Array;
	m_dataMgr.QueryFromDC(DATABASE,GET_CAM,(VARIANT*)&Array);

	int nIdx = 0, nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		if (Array[nIdx].mac_address.compare(strMac)  == 0)
		{
			cam = Array[0];
			break;
		}
	}
}