#ifndef COMMAND_H
#define COMMAND_H

#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace horiba::communication {
/**
 * @brief Represents a command sent to the ICL
 */
class Command {
 public:
  /**
   * @brief Builds a command based on the command string and json parameters.
   *
   * @param command The ICL command
   * @param parameters The parameters necessary for the command
   */
  Command(std::string command, nlohmann::json parameters);
  ~Command() = default;

  /**
   * @brief JSON representation of the command.
   *
   * @return JSON construct of the command
   */
  nlohmann::json json();

 private:
  static std::atomic<unsigned long long int> next_id;
  unsigned long long int id;
  std::string command;
  nlohmann::json parameters;
};
} /* namespace horiba::communication */
#endif /* ifndef COMMAND_H */
