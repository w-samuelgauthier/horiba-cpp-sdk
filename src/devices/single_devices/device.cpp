#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/devices/single_devices/device.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

namespace horiba::devices::single_devices {

Device::Device(int id,
               std::shared_ptr<communication::Communicator> communicator)
    : id{id}, communicator{std::move(communicator)} {}

void Device::open() {
  if (!this->communicator->is_open()) {
    spdlog::debug("[Device] communicator is closed, opening it.");
    this->communicator->open();
    spdlog::debug("[Device] done");
  }
}

communication::Response Device::execute_command(
    const communication::Command& command) {
  if (!this->communicator->is_open()) {
    throw std::runtime_error("communicator is not open");
  }

  auto response = this->communicator->request_with_response(command);

  if (!response.errors().empty()) {
    this->handle_errors(response.errors());
  }
  return response;
}

int Device::device_id() const { return this->id; }

void Device::handle_errors(const std::vector<std::string>& errors) {
  for (const auto& error : errors) {
    spdlog::error(error);
  }
}

}  // namespace horiba::devices::single_devices
