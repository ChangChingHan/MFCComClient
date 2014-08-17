
enum DATATYPE{DATABASE=0,REG_TBL};

enum REGTABL_OPERATION
{
	
}

enum DATABASE_OPERATION
{
	/*Query*/
	GET_GROUP = 0,
	GET_GROUP_BY_DEVICE,
	GET_GROUP_BY_LAYOUT,
	GET_GROUP_CAM,
	GET_LAST_GROUP_CAM,
	GET_GROUP_CAM_BY_STATION,
	GET_CAM,
	GET_CAM_BY_INDEX,
	GET_STREAM,
	GET_STREAM_BY_ID,
	GET_RECORD,
	GET_STORAGE,
	GET_PARAM,

	/*Insert*/
	INSERT_GROUP,
	INSERT_CAM,
	INSERT_RECORD,
	INSERT_GROUP_CAM,
	INSERT_STREAM,

	/*Delete*/
	DELETE_GROUP_BY_DEVICE,
	DELETE_GROUP_BY_LAYOUT,
	DELETE_GROUP_BY_ID,
	DELETE_GROUP_CAM_BY_GRPID,
	DELETE_GROUP_CAM_BY_CAMID,
	DELETE_GROUP_CAM_BY_CAMID_GRPID,
	DELETE_GROUP_CAM_BY_PRIM,
	DELETE_CAM,
	DELETE_STREAM,
	DELETE_RECORD_BY_ID,
	DELETE_RECORD_TIME_BY_ID,

	/*Update*/
	UPDATE_GROUP_BY_ID,
	UPDATE_GROUP_BY_NAME,
	UPDATE_DEFAULT_GROUP,
	UPDATE_GROUP_BY_TYPE,
	UPDATE_GROUP_CAM_BY_CAMID,
	UPDATE_GROUP_CAM,
	UPDATE_CAM,
	UPDATE_STREAM
};

struct camera
{
	int			cameraid;
	int			camera_idx;
	wstring		cameraname;
	wstring		connect_type_code; 
	wstring		ipaddress;
	int			httpport;
	wstring		gateway;
	wstring		name_server;
	wstring		mac_address;
	wstring		username;
	wstring		password;
	wstring		brand_code;
	wstring		model_code;
	wstring		ptz_support;
	wstring		ptz_protocol;
	wstring		digital_in1;
	wstring		digital_in2;
	wstring		digital_out;
	wstring		video_format;
	wstring		speaker_support;
	wstring		mic_support;
	wstring		subnet_mask1;
	wstring		subnet_mask2;
	wstring		subnet_mask3;
	wstring		subnet_mask4;
	wstring		active_;
	wstring		stream_url;
};


struct group
{
	int		cameragroupid;
	wstring camera_group_desc;
	wstring default_;
	wstring category_code;
	wstring module_code;
	wstring map_file;
	wstring acmap_file;
};

struct group_camera
{
	int cameragroupcameraid;
	int cameragroupid;
	int cameraid;
	int camera_idx;
	int streamid;
	int numb_;	//Check for device type. If a object is a device(like NVR), the value will be 1.

	int emap_x;
	int emap_y;
	int acmap_x;
	int acmap_y;
};

struct video_stream
{
	wstring stream_name;
	wstring stream_tag;
	int cameraid;
	wstring stream_type;
	int stream_port;
	wstring stream_protocol;
	wstring video_cmpr_type;
	wstring video_res;
};

struct video_record
{
	int camera_idx;
	wstring recording_type;
	wstring recording_time;
};

struct storage
{
	wstring storage_type;
	wstring store_location;
	wstring buffer_size;
	wstring server_user;
	wstring server_password;
};

struct parameter
{
	wstring parm_name;
	wstring parm_value;
};
