
struct Cam_Group
{
	short cameragroupid;
	CString camera_group_desc;
	CString default_;
	CString category_code;
	CString module_code;
	CString map_file;
	CString acmap_file;

	Cam_Group()
		:cameragroupid(0),camera_group_desc(_T("")),default_(_T("")),category_code(_T("")),
		module_code(_T("")),map_file(_T("")),acmap_file(_T("")){}

	void operator = (const Cam_Group& data)
	{
		cameragroupid = data.cameragroupid;
		camera_group_desc = data.camera_group_desc;
		default_ = data.default_;
		category_code = data.camera_group_desc;
		module_code = data.module_code;
		map_file = data.map_file;
		acmap_file = data.acmap_file;
	}

	void operator = (const group& data)
	{
		//USES_CONVERSION;
		cameragroupid = data.cameragroupid;

		camera_group_desc.Format(_T("%s"),data.camera_group_desc.c_str());
		default_.Format(_T("%s"),data.camera_group_desc.c_str());
		category_code.Format(_T("%s"),data.category_code.c_str());
		module_code.Format(_T("%s"),data.module_code.c_str());
		map_file.Format(_T("%s"),data.map_file.c_str());
		acmap_file.Format(_T("%s"),data.acmap_file.c_str());
	}

	void clear()
	{
		cameragroupid=0;
		camera_group_desc = _T("");
		default_ = _T("");
		category_code = _T("");
		module_code = _T("");
		map_file = _T("");
		acmap_file = _T("");
	}

	bool isempty()
	{
		if (cameragroupid == 0 || camera_group_desc.IsEmpty())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

struct Cam_Group_Cam
{
	int cameragroupcameraid;
	short cameragroupid;
	short cameraid;
	short camera_idx;
	int streamid;
	short numb_;	//Check for device type. If a object is a device(like NVR), the value will be 1.

	int emap_x;
	int emap_y;
	int acmap_x;
	int acmap_y;

	Cam_Group_Cam()
		:cameragroupid(0),cameraid(0),camera_idx(0),numb_(0),
		emap_x(0),emap_y(0),acmap_x(0),acmap_y(0),cameragroupcameraid(0),streamid(0){}

	void operator = (const group_camera& data)
	{
		cameragroupcameraid = data.cameragroupcameraid;
		cameragroupid = data.cameragroupid;
		cameraid = data.cameraid;
		camera_idx = data.camera_idx;
		streamid = data.streamid;
		numb_ = data.numb_;
		emap_x = data.emap_x;
		emap_y = data.emap_y;
		acmap_x = data.acmap_x;
		acmap_y = data.acmap_y;
	}

	void operator = (const Cam_Group_Cam& data)
	{
		cameragroupcameraid = data.cameragroupcameraid;
		cameragroupid = data.cameragroupid;
		cameraid = data.cameraid;
		camera_idx = data.camera_idx;
		streamid = data.streamid;
		numb_ = data.numb_;
		emap_x = data.emap_x;
		emap_y = data.emap_y;
		acmap_x = data.acmap_x;
		acmap_y = data.acmap_y;
	}

	void clear()
	{
		cameragroupcameraid = 0;
		cameragroupid = 0;
		cameraid = 0;
		camera_idx = 0;
		numb_ = 0;
		emap_x = 0;
		emap_y = 0;
		acmap_x = 0;
		acmap_y = 0;
		streamid = 0;
	}

	bool isempty()
	{
		if (cameragroupid == 0 || cameraid == 0 || cameragroupcameraid == 0)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}
};

struct ec_Stream
{
	CString stream_name;
	CString stream_tag;
	int cameraid;
	CString stream_type;
	short stream_port;
	CString stream_protocol;
	CString video_cmpr_type;
	CString video_res;

	ec_Stream()
		:stream_name(_T("")),stream_protocol(_T("")),stream_port(0),cameraid(0),stream_type(_T("")),
		stream_tag(_T("")),video_cmpr_type(_T("")),video_res(_T("")){}

	void operator = (const video_stream& data)
	{
		stream_port = data.stream_port;
		cameraid = data.cameraid;

		stream_name.Format(_T("%s"),data.stream_name.c_str());
		stream_tag.Format(_T("%s"),data.stream_tag.c_str());
		stream_type.Format(_T("%s"),data.stream_type.c_str());
		stream_protocol.Format(_T("%s"),data.stream_protocol.c_str());
		video_cmpr_type.Format(_T("%s"),data.video_cmpr_type.c_str());
		video_res.Format(_T("%s"),data.video_res.c_str());
	}

	void operator = (const ec_Stream& data)
	{
		stream_port = data.stream_port;
		cameraid = data.cameraid;

		stream_name.Format(_T("%s"),data.stream_name);
		stream_tag.Format(_T("%s"),data.stream_tag);
		stream_type.Format(_T("%s"),data.stream_type);
		stream_protocol.Format(_T("%s"),data.stream_protocol);
		video_cmpr_type.Format(_T("%s"),data.video_cmpr_type);
		video_res.Format(_T("%s"),data.video_res);
	}

	void clear()
	{
		stream_name = _T("");
		stream_protocol = _T("");
		stream_port = 0;
		cameraid = 0;
		stream_type = _T("");
		video_cmpr_type = _T("");
		video_res = _T("");
	}

};
struct ec_Camera
{
	short cameraid;
	short camera_idx;
	CString cameraname;
	CString connect_type_code; 
	CString ipaddress;
	short httpport;
	CString gateway;
	CString name_server;
	CString mac_address;
	CString username;
	CString password;
	CString brand_code;
	CString model_code;
	CString ptz_support;
	CString ptz_protocol;
	CString digital_in1;
	CString digital_in2;
	CString digital_out;
	CString video_format;
	CString speaker_support;
	CString mic_support;
	CString subnet_mask1;
	CString subnet_mask2;
	CString subnet_mask3;
	CString subnet_mask4;
	CString active_;
	CString stream_url;

	vector<ec_Stream> vcStream;

	ec_Camera()
		:stream_url(_T("")),cameraname(_T("")),ipaddress(_T("")),mac_address(_T("")),
		username(_T("")),password(_T("")),model_code(_T("")),video_format(_T("")),
		cameraid(0),camera_idx(0),httpport(0),vcStream(),ptz_support(_T("")),active_(_T("")),
		connect_type_code(_T("")),gateway(_T("")),name_server(_T("")),ptz_protocol(_T("")),
		digital_in1(_T("")),digital_in2(_T("")),digital_out(_T("")),subnet_mask1(_T("")),
		subnet_mask2(_T("")),subnet_mask3(_T("")),subnet_mask4(_T("")),brand_code(_T("")),
		speaker_support(_T("")),mic_support(_T("")){}

	void operator = (const camera& data)
	{
		cameraid = data.cameraid;
		camera_idx = data.camera_idx;
		httpport = data.httpport;

		cameraname.Format(_T("%s"),data.cameraname.c_str());
		connect_type_code.Format(_T("%s"),data.connect_type_code.c_str());
		ipaddress.Format(_T("%s"),data.ipaddress.c_str());
		gateway.Format(_T("%s"),data.gateway.c_str());
		name_server.Format(_T("%s"),data.name_server.c_str());
		mac_address.Format(_T("%s"),data.mac_address.c_str());
		username.Format(_T("%s"),data.username.c_str());
		password.Format(_T("%s"),data.password.c_str());
		brand_code.Format(_T("%s"),data.brand_code.c_str());
		model_code.Format(_T("%s"),data.model_code.c_str());
		ptz_support.Format(_T("%s"),data.ptz_support.c_str());
		ptz_protocol.Format(_T("%s"),data.ptz_protocol.c_str());
		digital_in1.Format(_T("%s"),data.digital_in1.c_str());
		digital_in2.Format(_T("%s"),data.digital_in2.c_str());
		digital_out.Format(_T("%s"),data.digital_out.c_str());
		video_format.Format(_T("%s"),data.video_format.c_str());
		speaker_support.Format(_T("%s"),data.speaker_support.c_str());
		mic_support.Format(_T("%s"),data.mic_support.c_str());
		subnet_mask1.Format(_T("%s"),data.subnet_mask1.c_str());
		subnet_mask2.Format(_T("%s"),data.subnet_mask2.c_str());
		subnet_mask3.Format(_T("%s"),data.subnet_mask3.c_str());
		subnet_mask4.Format(_T("%s"),data.subnet_mask4.c_str());
		active_.Format(_T("%s"),data.active_.c_str());
		stream_url.Format(_T("%s"),data.stream_url.c_str());
	}

	void operator = (const ec_Camera& data)
	{
		cameraid = data.cameraid;
		camera_idx = data.camera_idx;
		httpport = data.httpport;

		cameraname.Format(_T("%s"),data.cameraname);
		connect_type_code.Format(_T("%s"),data.connect_type_code);
		ipaddress.Format(_T("%s"),data.ipaddress);
		gateway.Format(_T("%s"),data.gateway);
		name_server.Format(_T("%s"),data.name_server);
		mac_address.Format(_T("%s"),data.mac_address);
		username.Format(_T("%s"),data.username);
		password.Format(_T("%s"),data.password);
		brand_code.Format(_T("%s"),data.brand_code);
		model_code.Format(_T("%s"),data.model_code);
		ptz_support.Format(_T("%s"),data.ptz_support);
		ptz_protocol.Format(_T("%s"),data.ptz_protocol);
		digital_in1.Format(_T("%s"),data.digital_in1);
		digital_in2.Format(_T("%s"),data.digital_in2);
		digital_out.Format(_T("%s"),data.digital_out);
		video_format.Format(_T("%s"),data.video_format);
		speaker_support.Format(_T("%s"),data.speaker_support);
		mic_support.Format(_T("%s"),data.mic_support);
		subnet_mask1.Format(_T("%s"),data.subnet_mask1);
		subnet_mask2.Format(_T("%s"),data.subnet_mask2);
		subnet_mask3.Format(_T("%s"),data.subnet_mask3);
		subnet_mask4.Format(_T("%s"),data.subnet_mask4);
		active_.Format(_T("%s"),data.active_);
		stream_url.Format(_T("%s"),data.stream_url);
	}

	void clear()
	{
		stream_url = _T("");
		cameraname = _T("");
		ipaddress = _T("");
		mac_address = _T("");
		username = _T("");
		password = _T("");
		model_code = _T("");
		video_format = _T("");
		cameraid = 0;
		camera_idx = 0;
		httpport = 0;
		vcStream.clear();

		ptz_support = _T("");
		active_ = _T("");
		connect_type_code = _T("");;
		gateway = _T("");
		name_server = _T("");
		ptz_protocol = _T("");
		digital_in1 = _T("");
		digital_in2 = _T("");
		digital_out = _T("");
		subnet_mask1 = _T("");
		subnet_mask2 = _T("");
		subnet_mask3 = _T("");
		subnet_mask4 = _T("");
		brand_code = _T("");
		speaker_support = _T("");
		mic_support = _T("");
	}

	bool isempty()
	{
		if (mac_address.GetLength())
			return false;
		else
			return true;
	}
};
struct ec_Camera_Rec
{
	int camera_idx;
	CString recording_type;
	CString recording_time;

	ec_Camera_Rec()
		:camera_idx(0),recording_type(_T("")),recording_time(_T(""))
	{}

	void operator = (const video_record& data)
	{
		camera_idx = data.camera_idx;
		recording_type.Format(_T("%s"),data.recording_type.c_str());
		recording_time.Format(_T("%s"),data.recording_time.c_str());
	}

	void operator = (const ec_Camera_Rec& data)
	{
		camera_idx = data.camera_idx;
		recording_type.Format(_T("%s"),data.recording_type);
		recording_time.Format(_T("%s"),data.recording_time);
	}

	void clear()
	{
		recording_time = _T("");
		recording_type = _T("");
		camera_idx = 0;
	}

	bool isempty()
	{
		if (!camera_idx)
			return false;
		else
			return true;
	}
};
struct ec_Storage
{
	CString storage_type;
	CString store_location;
	CString buffer_size;
	CString server_user;
	CString server_password;

	void operator = (const storage& data)
	{
		storage_type.Format(_T("%s"),data.storage_type.c_str());
		store_location.Format(_T("%s"),data.store_location.c_str());
		buffer_size.Format(_T("%s"),data.buffer_size.c_str());
		server_user.Format(_T("%s"),data.server_user.c_str());
		server_password.Format(_T("%s"),data.server_password.c_str());
	}

	void operator = (const ec_Storage& data)
	{
		storage_type.Format(_T("%s"),data.storage_type);
		store_location.Format(_T("%s"),data.store_location);
		buffer_size.Format(_T("%s"),data.buffer_size);
		server_user.Format(_T("%s"),data.server_user);
		server_password.Format(_T("%s"),data.server_password);
	}

	ec_Storage()
		:storage_type(_T("")),store_location(_T("")),buffer_size(_T("")),
		server_user(_T("")),server_password(_T("")){}

	void clear()
	{
		storage_type = _T("");
		store_location = _T("");
		buffer_size = _T("");
		server_user = _T("");
		server_password = _T("");
	}
};

struct ec_Param
{
	CString parm_name;
	CString parm_value;

	void operator = (const parameter& data)
	{
		parm_name.Format(_T("%s"),data.parm_name.c_str());
		parm_value.Format(_T("%s"),data.parm_value.c_str());
	}

	void operator = (const ec_Param& data)
	{
		parm_name.Format(_T("%s"),data.parm_name);
		parm_value.Format(_T("%s"),data.parm_value);
	}

	ec_Param()
		:parm_name(_T("")),parm_value(_T("")){}

	void clear()
	{
		parm_name = _T("");
		parm_value = _T("");
	}
};