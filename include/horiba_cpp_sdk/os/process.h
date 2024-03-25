#ifndef PROCESS_H
#define PROCESS_H

namespace horiba::os {

/**
 * @brief Represents a platform agnostic interface for a process
 */
class Process {
 public:
  virtual ~Process() = default;

  /**
   * @brief Starts the process if not already started
   */
  virtual void start() = 0;

  /**
   * @brief Returns whether the process is running or not.
   *
   * @return True if the process is running, false otherwise.
   */
  virtual bool running() = 0;

  /**
   * @brief Stops the process if currently running
   */
  virtual void stop() = 0;
};

} /* namespace horiba::os */
#endif /* ifndef PROCESS_H */
