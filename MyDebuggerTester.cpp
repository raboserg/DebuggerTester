// DebuggerTester.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "MyList.h"

#define AUTOBUF_IMPL
#include "AutoBuf.h"

#include "MyDebugger.h"
/*
__try {
// Is exception is an access violation in the data buffer's region?
if (per->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
__leave;

if (!chINRANGE(m_pszBuffer, (PVOID) per->ExceptionInformation[1],
((PBYTE) m_pszBuffer) + m_nMaxSizeInBytes - 1)) {
__leave;
}

// Attempt to commit storage to the region
if (VirtualAlloc((PVOID) pep->ExceptionRecord->ExceptionInformation[1],
1, MEM_COMMIT, PAGE_READWRITE) == NULL) {
__leave;
}

lDisposition = EXCEPTION_CONTINUE_EXECUTION;
}
__finally {
}
*/

void tetsThreadContext() {
	/*
	List::Node *element = threadList.head;
	while (element) {
	DWORD dwThreadId = element->value;
	element = element->next;
	CONTEXT contextThread = getThreadContext(dwThreadId);
	printf("[*] Dumping registers for thread ID: Ox%lx \r\n", dwThreadId);
	printf("[**] RIP: 0x%I64x \r\n", contextThread.Rip);
	printf("[*] END DUMP \r\n");
	}
	threadList.ClearList();
	*/
}

void testIsWow64(Debugger debugger, DWORD pid) {
	BOOL isWow64 = false;
	BOOL resultKnown = debugger.IsWow64(pid, isWow64);
	if (resultKnown == false)
	printf("Process type is unknown.");
	else
	printf("Pprocess type is %s", (isWow64 ? "x86 (wow64)." : "x64."));
}

void testAutoBuf() {
	//CAutoBuf<TCHAR, sizeof(TCHAR)> autoBuf;
	//CAutoBuf<DWORD, sizeof(DWORD)> autoBuf1;
	//autoBuf = threadEntry.th32ThreadID;
	//for (int i = 0; i < autoBuf.; i ++)		printf("x0%08x", autoBuf[i]);
}

void testGetProcessThreads(Debugger debugger, const DWORD pid) {
	std::list<DWORD> threads = debugger.GetProcessThreads(pid);
	for (DWORD dwThreadId : threads) {
		CONTEXT contextThread = debugger.GetThreadContext(dwThreadId);
		printf("[*] Dumping registers for thread ID: Ox%lx \r\n", dwThreadId);
		printf("[**] EIP: 0x%I64x \r\n", contextThread.Eip);
		printf("[**] ESP: 0x%I64x \r\n", contextThread.Esp);
		printf("[**] EBP: 0x%I64x \r\n", contextThread.Ebp);
		printf("[**] EAX: 0x%I64x \r\n", contextThread.Eax);
		printf("[**] EBX: 0x%I64x \r\n", contextThread.Ebx);
		printf("[**] ECX: 0x%I64x \r\n", contextThread.Ecx);
		printf("[**] EDX: 0x%I64x \r\n", contextThread.Edx);
		printf("[*] END DUMP \r\n");
	}
}

void testLoadProcess(Debugger debugger) {
	const DWORD dwCreationFlags = DEBUG_PROCESS;
	LPCWSTR lpApplicationName = L"c:\\windows\\system32\\calc.exe";
	TCHAR szCommandLine[] = TEXT("c:\\windows\\system32\\calc.exe");

	debugger.LoadProcess(lpApplicationName, dwCreationFlags);
}

void testAttachProcess(Debugger debugger, const DWORD dwProcessId) {

	if(!debugger.AttachProcess(dwProcessId))
		printf("[*] Unable to attach the process [%d]", dwProcessId);
}

void testDetachProcess(Debugger debugger, const DWORD dwProcessId) {
	if (!debugger.DetachProcess())
		printf("[*] Unable to detach the process [%d]", dwProcessId);
}

int main() {
	
	printf("Hello, my ProcessID is %d \r\n", _getpid());
	DWORD pid;
	scanf_s("Input Process ID %d", &pid);
	
	Debugger debugger(pid);

	testLoadProcess(debugger);

	testIsWow64(debugger, pid);
	
	testGetProcessThreads(debugger, debugger.getProcessId());
	//testGetProcessThreads(debugger, pid);
	testAttachProcess(debugger, pid);
	testDetachProcess(debugger, pid);

	_getch();
	return 0;
}