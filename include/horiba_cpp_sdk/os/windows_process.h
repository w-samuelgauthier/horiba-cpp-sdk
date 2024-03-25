#ifndef WINDOWS_PROCESS_H
#define WINDOWS_PROCESS_H

#include <horiba_cpp_sdk/os/process.h>

#include <string>

namespace horiba::os {
class WindowsProcess : public Process {
 public:
  explicit WindowsProcess(std::string process_path, std::string process_name);

  void start() override;
  bool running() override;
  void stop() override;

 private:
  std::string process_path;
  std::string process_name;
};
} /* namespace horiba::os */

#endif /* ifndef WINDOWS_PROCESS_H */
