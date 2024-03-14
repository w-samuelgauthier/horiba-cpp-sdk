#include "horiba_cpp_sdk/communication/command.h"

namespace horiba::communication {
Command::Command(std::string command, nlohmann::json parameters)
    : id{this->next_id++},
      command{std::move(command)},
      parameters{std::move(parameters)} {}

Command::~Command() {}

nlohmann::json Command::json() {
  return {{"id", this->id},
          {"command", this->command},
          {"parameters", {this->parameters}}};
}
} /* namespace horiba::communication */
