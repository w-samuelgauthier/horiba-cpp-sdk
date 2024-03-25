#ifndef ICL_EXE_H
#define ICL_EXE_H

#include <horiba_cpp_sdk/os/process.h>
#include <spdlog/spdlog.h>

#if WIN32
#include <horiba_cpp_sdk/os/windows_process.h>
#endif

namespace horiba::test {

class ICLExe {
 public:
  ICLExe() : icl_process{nullptr} {
    spdlog::debug("ICLExe");
#if WIN32
    this->icl_process = std::make_shared<horiba::os::WindowsProcess>(
        R"(C:\Program Files\HORIBA Scientific\SDK\icl.exe)", "icl.exe");
    this->icl_process->start();
#else
    spdlog::debug("On a Unix platform, skip starting icl.exe");
#endif
  }

  ~ICLExe() {
    spdlog::debug("~ICLExe");
#if WIN32
    this->icl_process->stop();
#else
    spdlog::debug("On a Unix platform, skip stopping icl.exe");
#endif
  }

 private:
  std::shared_ptr<horiba::os::Process> icl_process;
};

}  // namespace horiba::test
#endif /* ifndef ICLExe */
