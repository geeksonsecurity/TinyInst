#ifndef WINDOWS_EXTENDEDSTACKWALKER_H_
#define WINDOWS_EXTENDEDSTACKWALKER_H_

#include <string>
#include <sstream>

#include "tinyinst.h"
#include "common.h"
#include "third_party/stackwalker/Main/StackWalker/StackWalker.h"

class ExtendedStackWalker : public StackWalker
{
 public:
  ExtendedStackWalker(TinyInst *tinyInst, DWORD dwProcessId, HANDLE hProcess)
    : StackWalker(StackWalker::StackWalkOptions::OptionsAll, NULL, dwProcessId, hProcess) {
    _tinyInst = tinyInst;
  }
  char* GetCallstack(DWORD threadId);
 protected:
  void OnOutput(LPCSTR szText) override;
  void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) override;
  void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) override;
 private:
  TinyInst *_tinyInst;
  std::string _collected_stack;
};

#endif //WINDOWS_EXTENDEDSTACKWALKER_H_
