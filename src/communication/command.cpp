#include "horiba_cpp_sdk/communication/command.h"

#include <utility>

namespace horiba::communication {

std::atomic<unsigned long long int> Command::next_id{0};

Command::Command(std::string command, nlohmann::json parameters)
    : id{this->next_id++},
      command{std::move(command)},
      parameters{std::move(parameters)} {}

nlohmann::json Command::json() {
  return {{"id", this->id},
          {"command", this->command},
          {"parameters", {this->parameters}}};
}
} /* namespace horiba::communication */
