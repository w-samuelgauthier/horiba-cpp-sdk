#ifndef WINDOWS_PROCESS_H
#define WINDOWS_PROCESS_H

#include <horiba_cpp_sdk/os/process.h>

#include <string>

namespace horiba::os {
/**
 * @brief Represents a Windows process that can be started and stopped
 */
class WindowsProcess : public Process {
 public:
  /**
   * @brief Builds a Windows process
   *
   * @param process_path Folder where the executable is located
   * @param process_name Name of the executable
   */
  explicit WindowsProcess(std::string process_path, std::string process_name);

  /**
   * @brief Starts the process if not already started
   */
  void start() override;

  /**
   * @brief Returns whether the process is running or not.
   *
   * @return True if the process is running, false otherwise.
   */
  bool running() override;

  /**
   * @brief Stops the process if currently running
   */
  void stop() override;

 private:
  std::string process_path;
  std::string process_name;
};
} /* namespace horiba::os */

#endif /* ifndef WINDOWS_PROCESS_H */
