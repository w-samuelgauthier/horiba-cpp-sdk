#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/devices/monos_discovery.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "spdlog/common.h"

namespace horiba::devices {

MonochromatorsDiscovery::MonochromatorsDiscovery(
    std::shared_ptr<horiba::communication::Communicator> communicator)
    : communicator{std::move(communicator)} {}

void MonochromatorsDiscovery::execute(bool error_on_no_devices) {
  if (!this->communicator->is_open()) {
    this->communicator->open();
  }

  const auto _ignored_response = this->communicator->request_with_response(
      communication::Command("mono_discover", {}));

  const auto response = this->communicator->request_with_response(
      communication::Command("mono_list", {}));
  if (response.json_results().empty() && error_on_no_devices) {
    throw std::runtime_error("No Monochromators connected");
  }

  auto raw_monos_list = response.json_results()["devices"];
  this->monos = this->parse_monos(raw_monos_list);
}

std::vector<std::shared_ptr<single_devices::Monochromator>>
MonochromatorsDiscovery::monochromators() const {
  return this->monos;
}

std::vector<std::shared_ptr<single_devices::Monochromator>>
MonochromatorsDiscovery::parse_monos(const nlohmann::json& raw_monos_list) {
  spdlog::info("[MonochromatorsDiscovery] detected #{} monos",
               raw_monos_list.size());
  std::vector<std::shared_ptr<single_devices::Monochromator>> detected_monos;
  for (const auto& raw_mono : raw_monos_list) {
    const int index = raw_mono["index"].get<int>();
    detected_monos.push_back(std::make_shared<single_devices::Monochromator>(
        index, this->communicator));
  }
  return detected_monos;
}

} /* namespace horiba::devices */
