#pragma once
class Debugger
{
public:
	Debugger();
	Debugger(DWORD dwProcessId);
	~Debugger();
	void LoadProcess(PCTSTR pszPathToExe, const DWORD dwCreationFlags);
	HANDLE OpenProcess(const DWORD dwProcessId);
	bool AttachProcess(const DWORD dwProcessId);
	bool DetachProcess();
	HANDLE OpenThread(const DWORD threadId);
	std::list<DWORD> GetProcessThreads(const DWORD dwProcessId);
	CONTEXT GetThreadContext(const DWORD threadId);
	bool IsWow64(DWORD dwProcessID, BOOL &isWow64);
private:
	HANDLE hProcess;
	DWORD dwProcessId;
	bool bDebuggerActive;
	bool bLoadedProcess;
};

