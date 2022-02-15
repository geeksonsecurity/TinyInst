#include "ExtendedStackWalker.h"

void ExtendedStackWalker::OnOutput(LPCSTR szText) {
  collected_stack += szText;
}

char *ExtendedStackWalker::GetCallstack(DWORD threadId) {
  collected_stack = "";
  HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, false, threadId);
  if(!hThread){
    WARN("Unable to open handle to thread id %d in exception\n", threadId);
  } else {
    ShowCallstack(hThread);
    CloseHandle(hThread);
  }
  char *new_string = new char[collected_stack.length() + 1];
  strcpy_s(new_string, collected_stack.length() + 1, collected_stack.c_str());
  return new_string;
}

void ExtendedStackWalker::OnCallstackEntry(StackWalker::CallstackEntryType eType, StackWalker::CallstackEntry &entry) {
  std::ostringstream ss;
  if ((eType != lastEntry) && (entry.offset != 0))
  {
    std::ostringstream entryName;
    if (entry.undName[0] != 0)
      entryName << entry.undName << " ";
    if (entry.undFullName[0] != 0)
      entryName << entry.undFullName << " ";

    if (entry.lineFileName[0] == 0)
    {
      ss << (LPVOID)entry.offset << " " << entry.moduleName << " " << entry.lineFileName << " " << entryName.str();
    }
    else
      ss << (LPVOID)entry.offset << " " << entry.lineFileName << " " << entry.lineNumber << " " << entryName.str();

    ss << std::endl;
  }
  collected_stack += ss.str();
}
void ExtendedStackWalker::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) {
  // mute all the symbol resolution failures
}
