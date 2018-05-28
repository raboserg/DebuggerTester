#include "stdafx.h"
#include "MyDebugger.h"


Debugger::Debugger() : hProcess(NULL), bDebuggerActive(false)
, bLoadedProcess(false)
{
}

Debugger::Debugger(DWORD dwProcessId) : hProcess(NULL), bDebuggerActive(false) {
	this->dwProcessId = dwProcessId;
}


Debugger::~Debugger(){
	if (this->hProcess != NULL)
		CloseHandle(this->hProcess);
}

DWORD Debugger::getProcessId() {
	return this->dwProcessId;
}

void Debugger::LoadProcess(PCTSTR pszPathToExe, const DWORD dwCreationFlags){
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	PROCESS_INFORMATION pi;
	si.dwFlags = 0x1;
	si.wShowWindow = 0x0;
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (::CreateProcess(pszPathToExe, NULL, NULL, NULL, FALSE, dwCreationFlags, NULL, NULL, &si, &pi)) {
		this->dwProcessId = pi.dwProcessId;
		this->hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, pi.dwProcessId);
		this->bLoadedProcess = true;
		printf("[*] We have successfully launched the process: %d \n\r", pi.dwProcessId);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else {
		printf("[*] We have error to launched the process: %d \n\r", GetLastError());
	}
}


HANDLE Debugger::OpenProcess(const DWORD dwProcessId){
	return ::OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId);
}


bool Debugger::AttachProcess(const DWORD dwProcessId){
	// TODO use dwProcessId in constructor ???
	if(this->hProcess != NULL)
		this->hProcess = OpenProcess(dwProcessId); //??? 
	if (::DebugActiveProcess(dwProcessId)) {
		this->bDebuggerActive = true;
		this->dwProcessId = dwProcessId; // ???
	}
	return this->bDebuggerActive;
}


bool Debugger::DetachProcess(){
	if (::DebugActiveProcessStop(this->dwProcessId))
		this->bDebuggerActive = false;
	return this->bDebuggerActive;
}


HANDLE Debugger::OpenThread(const DWORD dwThreadId){
	return ::OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadId);
}


std::list<DWORD> Debugger::GetProcessThreads(const DWORD dwProcessId){
	std::list<DWORD> threads;
	CToolhelp thThreads(TH32CS_SNAPTHREAD, dwProcessId);
	THREADENTRY32 te = { sizeof(te) };
	thThreads.ThreadFirst(&te);
	do {
		if (te.th32OwnerProcessID == dwProcessId) {
			threads.push_back(te.th32ThreadID);
		}
	} while (thThreads.ThreadNext(&te));
	return threads;
}


CONTEXT Debugger::GetThreadContext(const DWORD dwThreadId){
	CONTEXT context;
	context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
	HANDLE hThread = OpenThread(dwThreadId);
	if (hThread != NULL)
		::GetThreadContext(hThread, &context);
	::CloseHandle(hThread);
	return context;
}


bool Debugger::IsWow64(DWORD dwProcessID, BOOL &isWow64) {
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
	if (hProcess == NULL)
		return false;
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	bool result = fnIsWow64Process != NULL && fnIsWow64Process(hProcess, &isWow64);
	CloseHandle(hProcess);
	return result;
}
