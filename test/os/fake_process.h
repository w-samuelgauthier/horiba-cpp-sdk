#ifndef FAKE_PROCESS_H
#define FAKE_PROCESS_H

#include <horiba_cpp_sdk/os/process.h>

#include <string>

namespace horiba::os {
/**
 * @brief Represents a fake process that can be started and stopped
 */
class FakeProcess : public Process {
 public:
  /**
   * @brief Starts the process if not already started
   */
  void start() { this->is_running = true; }

  /**
   * @brief Returns whether the process is running or not.
   *
   * @return True if the process is running, false otherwise.
   */
  bool running() { return this->is_running; }

  /**
   * @brief Stops the process if currently running
   */
  void stop() { this->is_running = false; }

 private:
  bool is_running = false;
};
} /* namespace horiba::os */

#endif /* ifndef FAKE_PROCESS_H */
