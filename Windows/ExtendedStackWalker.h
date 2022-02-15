#ifndef WINDOWS_EXTENDEDSTACKWALKER_H_
#define WINDOWS_EXTENDEDSTACKWALKER_H_

#include <string>
#include <sstream>

#include "third_party/stackwalker/Main/StackWalker/StackWalker.h"
#include "common.h"

class ExtendedStackWalker : public StackWalker
{
 public:
  ExtendedStackWalker(DWORD dwProcessId, HANDLE hProcess)
    : StackWalker(StackWalker::StackWalkOptions::OptionsAll, NULL, dwProcessId, hProcess) { }

  char* GetCallstack(DWORD threadId);
 protected:
  void OnOutput(LPCSTR szText) override;
  void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;
  void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override;
 private:
  std::string collected_stack;
};

#endif //WINDOWS_EXTENDEDSTACKWALKER_H_
