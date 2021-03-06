#include "StdAfx.h"
#include "DataCeneter.h"
#include "CDataMgr.h"

CString g_DeviceIni = _T("");

#define DEVICE_INI				_T("\\device.ini")
#define STRING_INI_BUFFER		128
#define SECTION_URL				_T("URL")
#define PTZ_ACTION_KEY			_T("PTZAction")
#define PTZ_PRESET_KEY			_T("PTZPreset")
#define EMAIL_CONTENT_KEY		_T("EmailContent")
#define EXACUTE_FILE_KEY		_T("ExacuteFile")

CDataCeneter::CDataCeneter(void):m_pdataMgr(NULL)
{
	system("regsvr32 /s .\\ComServer.dll");

	USES_CONVERSION;
	wchar_t buffer[125];
	GetModuleFileName(NULL,buffer,125);
	int pos = wstring( buffer ).find_last_of( _T("\\/") );
	wstring str = wstring( buffer ).substr( 0, pos);
	g_DeviceIni.Format(_T("%s\\%s"),str.c_str(),DEVICE_INI);

	m_pdataMgr = new CDataMgr;
	//InitialDeviceIni();
}

CDataCeneter::~CDataCeneter(void)
{
	map<int, CString>::iterator it = m_mapDeviceURL.begin();
	while (it != m_mapDeviceURL.end())
	{
		WritetDeviceURL(it->first,it->second);
		it++;
	}

	if (m_pdataMgr)
	{
		delete m_pdataMgr;
	}
}

void CDataCeneter::InitialDeviceIni()
{
	CSimpleArray<camera> Array;
	m_pdataMgr->QueryFromDC(DATABASE,GET_CAM,(VARIANT*)&Array);

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
	WritePrivateProfileString( SECTION_URL , strKey , strURL , g_DeviceIni );
}

void CDataCeneter::GetDeviceURL(int nDeviceId, LPTSTR pstr)
{
	CString strKey(_T(""));

	strKey.Format(_T("cam%d"),nDeviceId);
	GetPrivateProfileString( SECTION_URL , strKey , _T("") , pstr , STRING_INI_BUFFER , g_DeviceIni );
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
	case FLUSH_DATA:
		{
			FlushData();
		}
		break;
	default:
		break;
	}
}

void CDataCeneter::DatabaseOperation(BYTE bOperation, LPVOID VarData)
{
	if (bOperation >= GET_GROUP && bOperation <= GET_EVENT_ACTION_BY_MAC)
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
	else if (bOperation >= UPDATE_STREAM && bOperation <= UPDATE_STREAM)
	{
		UpdateStreamTbl(bOperation, VarData);
	}
	else if (bOperation >= UPDATE_EVENT_ACTION && bOperation <= UPDATE_EVENT_ACTION)
	{
		UpdateEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::UpdateStreamTbl(BYTE bOperation, void* VarData)
{
	InsertStreamTbl(bOperation, VarData);
}

void CDataCeneter::UpdateEventActionTbl(BYTE bOperation, void* VarData)
{
	DeleteEventActionTbl(bOperation, VarData);
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
	CSimpleArray<eventaction> Array;
	vector<ec_Event_Action> *pArray = NULL;
	pArray = (vector<ec_Event_Action>*)VarData;

	int nLastActionId = 0;
	int nIdx = 0, nCount = (int)pArray->size();
	eventaction data;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.actionid = (*pArray)[nIdx].actionid;
		data.event_type = (*pArray)[nIdx].event_type;
		data.action_type = (*pArray)[nIdx].action_type;
		data.source_mac = (*pArray)[nIdx].source_device.mac_address.MakeUpper();
		data.target_mac = (*pArray)[nIdx].target_device.mac_address.MakeUpper();
		Array.Add(data);
		SetEventActionDetailIntoFile(data.actionid, (*pArray)[nIdx]);
	}
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::DeleteCamTbl(BYTE bOperation, LPVOID VarData)
{
	InsertCamTbl(bOperation, VarData, FALSE);
	
	map<int, CString>::iterator it;
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	int nIdx = 0, nCount = (int)pArray->size();

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
	else if (bOperation == INSERT_EVENT_LOG)
	{
		InsertEventLogTbl(bOperation, VarData);
	}
	else if (bOperation == INSERT_EVENT_ACTION)
	{
		InsertEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::InsertEventLogTbl(BYTE bOperation, LPVOID VarData)
{
	CSimpleArray<eventlog> Array;
	vector<ec_Event_Log> *pArray = NULL;
	pArray = (vector<ec_Event_Log>*)VarData;

	int nIdx = 0, nCount = (int)pArray->size();
	eventlog data;
	
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.event_type = (*pArray)[nIdx].event_type;
		data.device_mac = (*pArray)[nIdx].device.mac_address.MakeUpper();
		data.start_time = (*pArray)[nIdx].start_time;
		data.end_time = (*pArray)[nIdx].end_time;
		data.ipaddress = (*pArray)[nIdx].device.ipaddress;
		Array.Add(data);
	}
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertEventActionTbl(BYTE bOperation, LPVOID VarData)
{
	CSimpleArray<eventaction> Array;
	vector<ec_Event_Action> *pArray = NULL;
	pArray = (vector<ec_Event_Action>*)VarData;

	int nLastActionId = 0;
	int nIdx = 0, nCount = (int)pArray->size();
	eventaction data;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.actionid = (*pArray)[nIdx].actionid;
		data.event_type = (*pArray)[nIdx].event_type;
		data.action_type = (*pArray)[nIdx].action_type;
		data.source_mac = (*pArray)[nIdx].source_device.mac_address.MakeUpper();
		data.target_mac = (*pArray)[nIdx].target_device.mac_address.MakeUpper();
		Array.Add(data);
	}
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	CSimpleArray<eventaction> LastActionId;
	m_pdataMgr->QueryFromDC(DATABASE,GET_LAST_EVENT_ACTION,(VARIANT*)&LastActionId);
	
	if (LastActionId.GetSize())
		nLastActionId = LastActionId[0].actionid;

	SetEventActionDetail(nLastActionId-(((int)pArray->size())-1), pArray);
}

void CDataCeneter::InsertGroupCamTbl(BYTE bOperation, LPVOID VarData)
{
	group_camera data;
	vector<Cam_Group_Cam> *pArray = (vector<Cam_Group_Cam>*)VarData;
	CSimpleArray<group_camera> Array;
	int nIdx = 0, nCount = (int)pArray->size();
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

	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertRecordTbl(BYTE bOperation, LPVOID VarData)
{
	video_record data;
	vector<ec_Camera_Rec> *pArray = (vector<ec_Camera_Rec>*)VarData;
	CSimpleArray<video_record> Array;
	int nIdx = 0, nCount = (int)pArray->size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data.camera_idx = (*pArray)[nIdx].camera_idx;
		data.recording_type = (*pArray)[nIdx].recording_type;
		data.recording_time = (*pArray)[nIdx].recording_time;
		Array.Add(data);
	}

	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertCamTbl(BYTE bOperation, LPVOID VarData, BOOL bInserStream)
{
	camera data;
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	CSimpleArray<camera> Array;
	int nIdx = 0, nCount = (int)pArray->size();

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
		data.mac_address = (*pArray)[nIdx].mac_address.MakeUpper();
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
		data.total_stream_count = (*pArray)[nIdx].vcStream.size();

		if (bOperation == INSERT_CAM && IsURLAddress((*pArray)[nIdx].ipaddress))
		{
			data.isURL = true;
			data.ipaddress.clear();
			m_mapDeviceURL[(*pArray)[nIdx].camera_idx] = (*pArray)[nIdx].ipaddress;
		}

		Array.Add(data);
		m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
		Array.RemoveAll();

		if(bInserStream)
			PrepareInsertStreamTbl((*pArray)[nIdx].vcStream);
	}
}

void CDataCeneter::PrepareInsertStreamTbl(vector<ec_Stream>& vcStream)
{
	int nIdx = 0, nCount = (int)vcStream.size();
	int nCameraid = 0;
	CSimpleArray<camera> Array;
	m_pdataMgr->QueryFromDC(DATABASE,GET_LAST_CAM,(VARIANT*)&Array);
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
	int nIdx = 0, nCount = (int)pArray->size();
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

	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
}

void CDataCeneter::InsertGroupTbl(BYTE bOperation, LPVOID VarData)
{
	group data;
	vector<Cam_Group> *pArray = (vector<Cam_Group>*)VarData;
	CSimpleArray<group> Array;
	int nIdx = 0, nCount = (int)pArray->size();
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

	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
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
	else if (bOperation >= GET_CAM && bOperation <= GET_CAM_BY_GROUP)
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
	else if (bOperation >= GET_EVENT_LOG && bOperation <= GET_EVENT_LOG_BY_INDEX)
	{
		QueryEventlogTbl(bOperation, VarData);
	}
	else if (bOperation >= GET_EVENT_ACTION && bOperation <= GET_EVENT_ACTION_BY_MAC)
	{
		QueryEventActionTbl(bOperation, VarData);
	}
}

void CDataCeneter::QueryGroupTbl(BYTE bOperation, LPVOID VarData)
{
	int nIdx = 0, nCount = 0;
	CSimpleArray<group> Array;
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);


	Cam_Group_Cam data;
	vector<Cam_Group_Cam> *pArray = (vector<Cam_Group_Cam>*)VarData;
	nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		pArray->push_back(data);
	}
}

void CDataCeneter::QueryGroupCamTblByGroupId(int nGroupId, vector<Cam_Group_Cam>& vcGroupCam)
{
	vector<Cam_Group_Cam> vcArray;
	QueryGroupCamTbl(GET_GROUP_CAM,(LPVOID)&vcArray);

	int nIdx = 0, nCount = (int) vcArray.size();
	if (nCount)
	{
		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			if (vcArray[nIdx].cameragroupid == nGroupId)
			{
				vcGroupCam.push_back(vcArray[nIdx]);
			}
		}
	}
}

void CDataCeneter::QueryCamTblByGroupId(BYTE bOperation, LPVOID VarData)
{
	vector<ec_Camera> *pArray = (vector<ec_Camera>*)VarData;
	ec_Stream ecstream;
	if (pArray->size())
	{
		vector<Cam_Group_Cam> vcGroupCam;
		QueryGroupCamTblByGroupId((*pArray)[0].getgroupid(), vcGroupCam);
	
		vector<ec_Camera> vcCam;
		QueryCamTbl(GET_CAM,(LPVOID)&vcCam);
		pArray->clear();

		int nIdx = 0, nCount = (int)vcGroupCam.size();
		int nIdxCam = 0, nCountCam = (int)vcCam.size();
		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			for (nIdxCam = 0; nIdxCam < nCountCam; nIdxCam++)
			{
				if (vcCam[nIdxCam].cameraid == vcGroupCam[nIdx].cameraid)
				{
					ecstream.cameraid = vcCam[nIdxCam].cameraid;
					vcCam[nIdxCam].vcStream.push_back(ecstream);
					QueryFromDC(DATABASE,GET_STREAM_BY_ID,(LPVOID)&vcCam[nIdxCam].vcStream);
					pArray->push_back(vcCam[nIdxCam]);
					break;
				}
			}
		}
	}
}

void CDataCeneter::QueryCamTbl(BYTE bOperation, LPVOID VarData)
{
	if (bOperation == GET_CAM_BY_GROUP)
	{
		QueryCamTblByGroupId(bOperation,VarData);
	}
	else
	{
		int nIdx = 0, nCount = 0;
		CSimpleArray<camera> Array;
		m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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

	if (bOperation == GET_EVENT_ACTION_BY_MAC && pArray->size())
	{
		eventaction data;
		data.actionid = (*pArray)[0].actionid;
		data.event_type = (*pArray)[0].event_type;
		data.action_type = (*pArray)[0].action_type;
		data.source_mac = (*pArray)[0].source_device.mac_address.MakeUpper();
		data.target_mac = (*pArray)[0].target_device.mac_address.MakeUpper();
		Array.Add(data);
	}
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

	int nIdx = 0, nCount = Array.GetSize();
	ec_Event_Action data;
	pArray->clear();

	map<CString, vector<camera>> mapCamArray;
	PrepareCamByMac(mapCamArray);

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		data = Array[nIdx];
		if (bOperation == GET_EVENT_ACTION)
		{
			if (!GetCamByMac(mapCamArray, data.source_device, Array[nIdx].source_mac, IPCAMERA, true))
				continue;
			
			if (!GetCamByMac(mapCamArray, data.target_device, Array[nIdx].target_mac, IPCAMERA, true) && 
				(data.action_type == ACTION_RECORDING || data.action_type == ACTION_PTZ || data.action_type == ACTION_SNAPSHOT))
				continue;
		}
		pArray->push_back(data);
	}

	if (bOperation == GET_EVENT_ACTION)
		GetEventActionDetail(pArray);
}

bool CDataCeneter::CheckDeviceIni()
{
	bool bResult = true;
	CFileFind findFile;
	if (!findFile.FindFile(g_DeviceIni))
	{
		CFile cfile;
		CFileException ex;
		cfile.Open(g_DeviceIni,CFile::modeCreate, &ex);
		cfile.Close();
		bResult = false;
	}
	return bResult;
}

void CDataCeneter::SetEventActionDetail(int nBeginActionId, vector<ec_Event_Action> *pArray)
{
	CheckDeviceIni();
	int nIdx = 0, nCount = (int)pArray->size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		SetEventActionDetailIntoFile(nBeginActionId+nIdx, (*pArray)[nIdx]);
	}
}

void CDataCeneter::WritePrivateProfileInt(const CString strSection, const CString strKey, int nValue, const CString strPath)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);
	WritePrivateProfileString( strSection , strKey , strValue , strPath );
}

void CDataCeneter::SetEventActionDetailIntoFile(int nActionId ,ec_Event_Action& eventAction)
{
	CString strKey(_T(""));
	strKey.Format(_T("action_%d"),nActionId);

	switch(eventAction.action_type)
	{
	case ACTION_PTZ:
		{
			WritePrivateProfileInt( strKey , PTZ_ACTION_KEY , eventAction.ptz_action , g_DeviceIni );
			if (eventAction.ptz_action == PTZ_PRESET)
			{
				WritePrivateProfileInt( strKey , PTZ_PRESET_KEY , eventAction.ptz_preset , g_DeviceIni );
			}
		}
		break;
	case ACTION_EMAIL:
		{
			WritePrivateProfileString(strKey, EMAIL_CONTENT_KEY, eventAction.email_content.c_str(), g_DeviceIni);
		}
		break;
	case ACTION_CUSTOM:
		{
			WritePrivateProfileString(strKey, EXACUTE_FILE_KEY, eventAction.custom_path.c_str(), g_DeviceIni);	
		}
		break;
	default:
		break;
	}
}

void CDataCeneter::GetEventActionDetail(vector<ec_Event_Action> *pArray)
{
	int nIdx = 0, nCount = (int)pArray->size();
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
			eventAction.ptz_action = (PTZ_ACTION)GetPrivateProfileInt( strKey , PTZ_ACTION_KEY , PTZ_AUTOSCAN , g_DeviceIni );
			if (eventAction.ptz_action == PTZ_PRESET)
			{
				eventAction.ptz_preset = GetPrivateProfileInt( strKey , PTZ_PRESET_KEY , 0 , g_DeviceIni );
			}
		}
		break;
	case ACTION_EMAIL:
		{
			CString strValue;
			LPTSTR pstr = strValue.GetBuffer(STRING_INI_BUFFER);
			GetPrivateProfileString( strKey , EMAIL_CONTENT_KEY , _T("") , pstr , STRING_INI_BUFFER , g_DeviceIni );
			eventAction.email_content = pstr;
			strValue.ReleaseBuffer();
		}
		break;
	case ACTION_CUSTOM:
		{
			CString strValue;
			LPTSTR pstr = strValue.GetBuffer(STRING_INI_BUFFER);
			GetPrivateProfileString( strKey , EXACUTE_FILE_KEY , _T("") , pstr , STRING_INI_BUFFER , g_DeviceIni );
			eventAction.custom_path = pstr;
			strValue.ReleaseBuffer();
		}
		break;
	default:
		break;
	}
}

bool CDataCeneter::PackEventlogCondition(BYTE bOperation, vector<ec_Event_Log> *pArray, CSimpleArray<eventlog>& Array)
{
	bool bReverse = false;
	if (bOperation == GET_EVENT_LOG_BY_INDEX)
	{
		if (pArray->size() >= 2)
		{
			if ((*pArray)[0].logcount > (*pArray)[1].logcount)
			{
				bReverse = true;
				eventlog data;
				data.logcount = (*pArray)[1].logcount;
				Array.Add(data);
				data.logcount = (*pArray)[0].logcount;
				Array.Add(data);
			}
			else
			{
				eventlog data;
				data.logcount = (*pArray)[0].logcount;
				Array.Add(data);
				data.logcount = (*pArray)[1].logcount;
				Array.Add(data);
			}
		}
	}
	else
	{
		eventlog data;
		data.logcount = (*pArray)[0].logcount;
		data.event_type = (*pArray)[0].event_type;
		data.device_mac = (*pArray)[0].device.mac_address.MakeUpper();
		data.start_time = (*pArray)[0].start_time;
		data.end_time = (*pArray)[0].end_time;
		Array.Add(data);
	}

	return bReverse;
}

void CDataCeneter::QueryEventlogTbl(BYTE bOperation, LPVOID VarData)
{
	if (bOperation == GET_EVENT_LOG_COUNT)
	{
		CSimpleArray<eventlog> Array;
		vector<ec_Event_Log> *pArray = NULL;
		pArray = (vector<ec_Event_Log>*)VarData;
		m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

		if (Array.GetSize())
		{
			ec_Event_Log data;
			data.logcount = Array[0].logcount;
			pArray->push_back(data);
		}
	}
	else
	{
		int nIdx = 0, nCount = 0;
		CSimpleArray<eventlog> Array;
		vector<ec_Event_Log> *pArray = NULL;
		bool bReverse = false;

		pArray = (vector<ec_Event_Log>*)VarData;
		if (pArray && pArray->size())
		{
			bReverse = PackEventlogCondition(bOperation, pArray, Array);
		}

		m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);
		ec_Event_Log data;
		nCount = Array.GetSize();
		pArray->clear();

		map<CString, vector<camera>> mapCamArray;
		PrepareCamByMac(mapCamArray);

		int n = GetTickCount();
		if (bReverse)
		{
			for (nIdx=(nCount-1); nIdx >= 0; nIdx--)
			{
				data = Array[nIdx];
				if (!GetCamByMac(mapCamArray, data.device, Array[nIdx].device_mac,IPCAMERA,false))
				{
					data.device.ipaddress.Format(_T("%s"),Array[nIdx].ipaddress.c_str());
				}
				pArray->push_back(data);
			}
		}
		else
		{
			for (nIdx = 0; nIdx < nCount; nIdx++)
			{
				data = Array[nIdx];
				if (!GetCamByMac(mapCamArray, data.device, Array[nIdx].device_mac,IPCAMERA,false))
				{
					data.device.ipaddress.Format(_T("%s"),Array[nIdx].ipaddress.c_str());
				}
				pArray->push_back(data);
			}
		}
		CString str;
		str.Format(_T("Lynn tickcount = %d"), GetTickCount()-n);
		OutputDebugString(str);
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
	m_pdataMgr->QueryFromDC(DATABASE,bOperation,(VARIANT*)&Array);

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
	return false;

	//Lynn if URL project start to implement please open this function
	/*bool bURLAddress = true;
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
		byte a = atoi(vcString[0].c_str());
		byte b = atoi(vcString[1].c_str());
		byte c = atoi(vcString[2].c_str());
		byte d = atoi(vcString[3].c_str());

		if (a == 127 && b == 0 && c == 0 && d == 1)
		{
			bURLAddress = false;
		}
		else
		{
			if (a && b && c && d && 
				a<=255 && b<=255 && c<=255 && d<=255)
			{
				bURLAddress = false;
			}
		}
	}
	return bURLAddress;*/
}

void CDataCeneter::PrepareCamByMac(map<CString, vector<camera>>& mapCam)
{
	CSimpleArray<camera> Array;
	map<CString, vector<camera> >::iterator it;
	vector<camera> vcCam;
	m_pdataMgr->QueryFromDC(DATABASE,GET_CAM,(VARIANT*)&Array);
	CString str;

	int nIdx = 0, nCount = Array.GetSize();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		vcCam.clear();
		str.Format(_T("%s"), Array[nIdx].mac_address.c_str());

		it = mapCam.find(str.MakeUpper());
		if (it == mapCam.end())
		{
			vcCam.push_back(Array[nIdx]);
			mapCam[str.MakeUpper()] = vcCam;
		} 
		else
		{
			it->second.push_back(Array[nIdx]);
		}
	}
}

bool CDataCeneter::GetCamByMac(map<CString, vector<camera>>& mapCamArray, ec_Camera& cam, const wstring& strMac, DEVICETYPE deviceType, bool isNeedStream)
{
	bool isDeviceExist = false;
	ec_Stream ecstream;

	cam.clear();
	if (_wcsnicmp(_T("000000000000"),strMac.c_str(),16) == 0)
	{
		cam.mac_address = _T("000000000000");
		cam.cameraname = _T("Local machine");
		cam.ipaddress = _T("127.0.0.1");
		isDeviceExist = true;
	}
	else
	{
		if (_wcsnicmp(_T(""),strMac.c_str(),16) != 0)
		{
			CString str;
			str.Format(_T("%s"), strMac.c_str());
			map<CString, vector<camera>>::iterator it;
			it = mapCamArray.find(str.MakeUpper());
			if (it != mapCamArray.end())
			{
				int nIdx = 0, nCount = (int)it->second.size();
				for (nIdx = 0 ; nIdx < nCount; nIdx++)
				{
					cam = it->second[nIdx];

					ecstream.cameraid = cam.cameraid;
					cam.vcStream.push_back(ecstream);

					if(isNeedStream)
						QueryStreamTbl(GET_STREAM_BY_ID, (LPVOID)&cam.vcStream);

					isDeviceExist = true;

					if (deviceType == IPCAMERA && cam.ischannel())
						continue;
					else
						break;
				}
			}
		}
	}
	return isDeviceExist;
}

void CDataCeneter::FlushData()
{
	m_pdataMgr->QueryFromDC(FLUSH_DATA,NULL,NULL);
}