#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

namespace horiba::communication {

class Command;
class Response;

/**
 * @brief Interface representing a communication channel with the ICL.
 */
class Communicator {
 public:
  virtual ~Communicator() = default;

  /**
   * @brief Opens the communication channel with the ICL
   */
  virtual void open() = 0;

  /**
   * @brief Closes the communication channel with the ICL
   */
  virtual void close() = 0;

  /**
   * @brief Checks if the communication channel is open or not.
   *
   * @return True if the communication channel is open, false otherwise
   */
  virtual bool is_open() = 0;

  /**
   * @brief Sends a command to the ICL and returns the response
   *
   * @param command The command for the ICL
   *
   * @return The response from the ICL
   */
  virtual Response request_with_response(const Command& command) = 0;
};

}  // namespace horiba::communication
#endif /* ifndef COMMUNICATOR */
