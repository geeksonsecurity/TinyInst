#ifndef WINDOWS_EXTENDEDSTACKWALKER_H_
#define WINDOWS_EXTENDEDSTACKWALKER_H_

#include <string>
#include <sstream>

#include "tinyinst.h"
#include "common.h"
#include "third_party/stackwalker/Main/StackWalker/StackWalker.h"
#include "unwind.h"
#include <DbgHelp.h>

class ExtendedStackWalker : public StackWalker
{
 public:
  ExtendedStackWalker(TinyInst* tinyInst, DWORD dwProcessId, HANDLE hProcess)
    : tinyInst_(tinyInst), StackWalker(StackWalker::StackWalkOptions::OptionsAll, NULL, dwProcessId, hProcess) {}
  char* GetCallstack(DWORD threadId);
 protected:
  void OnOutput(LPCSTR szText) override;
  void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;
  void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override;
  static PVOID MySymFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase, LPVOID  pUserData);
 private:
  TinyInst* tinyInst_;
  std::string _collected_stack;
};

#endif //WINDOWS_EXTENDEDSTACKWALKER_H_
