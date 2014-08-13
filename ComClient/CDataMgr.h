// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "D:\\Lynn-Test\\ComObject\\ComServer\\ComServer\\Debug\\ComServer.tlb" no_namespace named_guids raw_interfaces_only
// CDataMgr wrapper class

class CDataMgr : public COleDispatchDriver
{
public:
	CDataMgr(){
		COleException ErrorOle;
		CreateDispatch(CLSID_DataMgr,&ErrorOle);
	} // Calls COleDispatchDriver default constructor
	CDataMgr(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDataMgr(const CDataMgr& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IDataMgr methods
public:
	void QueryFromDC(unsigned char bDataType, unsigned char bOperation, VARIANT * VarData)
	{
		static BYTE parms[] = VTS_UI1 VTS_UI1 VTS_PVARIANT ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bDataType, bOperation, VarData);
	}

	// IDataMgr properties
public:

};
