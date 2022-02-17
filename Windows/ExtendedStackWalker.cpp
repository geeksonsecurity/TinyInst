#include "ExtendedStackWalker.h"

void ExtendedStackWalker::OnOutput(LPCSTR szText) {
  _collected_stack += szText;
}

char *ExtendedStackWalker::GetCallstack(DWORD threadId) {
  _collected_stack = "";
  HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME, false, threadId);
  if(!hThread){
    WARN("Unable to open handle to thread id %d in exception\n", threadId);
  } else {
    ShowCallstack(hThread, NULL, NULL, NULL, MySymFunctionTableAccess64, (LPVOID) tinyInst_);
    CloseHandle(hThread);
  }
  char *new_string = new char[_collected_stack.length() + 1];
  strcpy_s(new_string, _collected_stack.length() + 1, _collected_stack.c_str());
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

    DWORD64 translated = tinyInst_->GetBaseAddress(entry.offset);

    if(translated != entry.offset){
      ss << (LPVOID)translated << " ";
      ss << "(ORIG: " << (LPVOID)entry.offset << ") ";
    } else {
      ss << (LPVOID)entry.offset << " ";
    }

    if (entry.lineFileName[0] == 0)
    {
      ss << entry.moduleName << " " << entry.lineFileName << " " << entryName.str();
    }
    else
      ss << entry.lineFileName << " " << entry.lineNumber << " " << entryName.str();

    ss << std::endl;
  }
  _collected_stack += ss.str();
}
void ExtendedStackWalker::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) {
  // mute all the symbol resolution failures
}

PVOID ExtendedStackWalker::MySymFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase, PVOID data) {
  if(data == NULL) return NULL;
  TinyInst* tinyInst = reinterpret_cast<TinyInst*>(data);
  PVOID addr = SymFunctionTableAccess64(hProcess, AddrBase);
  if(addr == NULL){
    size_t found = tinyInst->unwind_generator->LookupUnwindInfoForTranslatedAddress(AddrBase);
    PVOID target = reinterpret_cast<PVOID>(found);
    return target;
  }
  return addr;
}
