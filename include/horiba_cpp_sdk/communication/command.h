#ifndef COMMAND_H
#define COMMAND_H

#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

namespace horiba::communication {
class Command {
 public:
  Command(std::string command, nlohmann::json parameters);
  ~Command();
  nlohmann::json json();

 private:
  static std::atomic<unsigned long long int> next_id;
  unsigned long long int id;
  std::string command;
  nlohmann::json parameters;
};
} /* namespace horiba::communication */
#endif /* ifndef COMMAND_H */
