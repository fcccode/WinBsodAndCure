#define STDCALL __stdcall
#define _cdecl
#define __cdecl


//__declspec(dllimport) SDE KeServiceDescriptorTable;
//#include <WinNt.h>
// IOCTLs to control driver
#define IOCTL_TO_ALLOCATE_PAGED_POOL				(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,2050,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_IF_POOL_WAS_PAGED						(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,2051,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_RAISE_IRQL							(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,2052,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_GET_PAGE								(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,2053,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define DEVICE_NAME      L"\\Device\\Pooldevice"
#define DOS_DEVICE_NAME     L"\\DosDevices\\Pooldosdevice"
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef	unsigned long*		PDWORD;
typedef unsigned char*      PBYTE;
PDEVICE_OBJECT fakeDevice = NULL;
	UINT64 Param1;
	UINT64 Param2;
	UINT64 Param3;
	UINT64 Param4;
	UINT64 Param5;
	UINT64 Switcher;
	CONTEXT                       GlobalContext;
	PVOID PtrAddress;
	SIZE_T PtrAddressSize;
	HANDLE   CurrprocHandle;
//Structures from WinNt

#define UNWIND_HISTORY_TABLE_SIZE 12
#define RUNTIME_FUNCTION_INDIRECT 0x1


typedef struct _RUNTIME_FUNCTION {
    DWORD BeginAddress;
    DWORD EndAddress;
    DWORD UnwindData;
} RUNTIME_FUNCTION, *PRUNTIME_FUNCTION;
typedef struct _UNWIND_HISTORY_TABLE_ENTRY {
    DWORD64 ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
} UNWIND_HISTORY_TABLE_ENTRY, *PUNWIND_HISTORY_TABLE_ENTRY;

typedef struct _UNWIND_HISTORY_TABLE {
    DWORD Count;
    BYTE  LocalHint;
    BYTE  GlobalHint;
    BYTE  Search;
    BYTE  Once;
    DWORD64 LowAddress;
    DWORD64 HighAddress;
    UNWIND_HISTORY_TABLE_ENTRY Entry[UNWIND_HISTORY_TABLE_SIZE];
} UNWIND_HISTORY_TABLE, *PUNWIND_HISTORY_TABLE;


NTSYSAPI
PRUNTIME_FUNCTION
NTAPI
RtlLookupFunctionEntry (
    __in DWORD64 ControlPc,
    __out PDWORD64 ImageBase,
    __inout_opt PUNWIND_HISTORY_TABLE HistoryTable
    );
	




typedef struct _KNONVOLATILE_CONTEXT_POINTERS {
    union {
        PM128A FloatingContext[16];
        struct {
            PM128A Xmm0;
            PM128A Xmm1;
            PM128A Xmm2;
            PM128A Xmm3;
            PM128A Xmm4;
            PM128A Xmm5;
            PM128A Xmm6;
            PM128A Xmm7;
            PM128A Xmm8;
            PM128A Xmm9;
            PM128A Xmm10;
            PM128A Xmm11;
            PM128A Xmm12;
            PM128A Xmm13;
            PM128A Xmm14;
            PM128A Xmm15;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

    union {
        PDWORD64 IntegerContext[16];
        struct {
            PDWORD64 Rax;
            PDWORD64 Rcx;
            PDWORD64 Rdx;
            PDWORD64 Rbx;
            PDWORD64 Rsp;
            PDWORD64 Rbp;
            PDWORD64 Rsi;
            PDWORD64 Rdi;
            PDWORD64 R8;
            PDWORD64 R9;
            PDWORD64 R10;
            PDWORD64 R11;
            PDWORD64 R12;
            PDWORD64 R13;
            PDWORD64 R14;
            PDWORD64 R15;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME2;

} KNONVOLATILE_CONTEXT_POINTERS, *PKNONVOLATILE_CONTEXT_POINTERS;

NTSYSAPI
PEXCEPTION_ROUTINE
NTAPI
RtlVirtualUnwind (
    __in DWORD HandlerType,
    __in DWORD64 ImageBase,
    __in DWORD64 ControlPc,
    __in PRUNTIME_FUNCTION FunctionEntry,
    __inout PCONTEXT ContextRecord,
    __out PVOID *HandlerData,
    __out PDWORD64 EstablisherFrame,
    __inout_opt PKNONVOLATILE_CONTEXT_POINTERS ContextPointers
    );

VOID  RtlRestoreContext(
    PCONTEXT ContextRecord,
    PEXCEPTION_RECORD ExceptionRecord
);

///////////////////////////////////////////////////////////////////////////////////////////////////////ffffffffffffff
NTSTATUS STDCALL ZwCreateEvent(
	OUT PHANDLE  EventHandle,
	IN ACCESS_MASK  DesiredAccess, 
	IN POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
	IN EVENT_TYPE  EventType,
	IN BOOLEAN  InitialState);

NTSTATUS ZwAllocateVirtualMemory(
		HANDLE ProcessHandle,
		PVOID *BaseAddress,
		ULONG_PTR ZeroBits,
		PSIZE_T RegionSize,
		ULONG AllocationType,
		ULONG Protect
);
NTSYSAPI NTSTATUS NTAPI ZwProtectVirtualMemory(
    IN HANDLE ProcessHandle,
    IN PVOID *  BaseAddress,
    IN SIZE_T *     NumberOfBytesToProtect,
    IN ULONG    NewAccessProtection,
    OUT PULONG  OldAccessProtection 
);
NTSYSAPI 
NTSTATUS
NTAPI
NtProtectVirtualMemory(


  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN SIZE_T*           NumberOfBytesToProtect,
  IN ULONG                NewAccessProtection,
  OUT PULONG              OldAccessProtection );
	
ULONG  ZwGetCurrentProcessorNumber(VOID);	
VOID NTAPI HalDisplayString(PVOID szAnsiStr);
VOID RtlCaptureContext(  PCONTEXT ContextRecord
);

//Forward declarations of variables
//static PVOID PoolPtr;
PMDL	PoolPtr;
PVOID	AddressPoolptr;
PVOID AdressKeBugCheckEx;

// Forward declarations of all funcrions 
VOID MyBugCheckFunc();	

VOID EventFunc();

PUNWIND_HISTORY_TABLE
 MyStackTrace64();

PKBUGCHECK_CALLBACK_RECORD BugCheckDataPool;

VOID PoolPtrBugCheckCallback(__in  PVOID Buffer,__in  ULONG Length);

NTSTATUS STDCALL DriverEntry(__in PDRIVER_OBJECT pDriverObject, __in PUNICODE_STRING path);

VOID DriverUnload( __in PDRIVER_OBJECT pDriverObject);

NTSTATUS DriverControl(__in PDEVICE_OBJECT pDeviceObject, __in PIRP Irp);

VOID PoolAlloc();

VOID CallbackRegistration();

NTSTATUS GetPage(PVOID PoolPtr);

BOOLEAN TestForSwap(PVOID PoolPtr);

VOID RaiseIrql();

VOID KBSOD();

NTSTATUS  AddDevice(  __in PDRIVER_OBJECT DriverObject, __in PDEVICE_OBJECT PhysicalDeviceObject );
//asm functions
extern void KeBugPatch(UINT64 a, UINT64 b);
extern void	GetArgs();
extern void	SpoilContext();


