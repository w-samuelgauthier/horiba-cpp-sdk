#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/devices/ccds_discovery.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "spdlog/common.h"

namespace horiba::devices {

ChargeCoupledDevicesDiscovery::ChargeCoupledDevicesDiscovery(
    std::shared_ptr<horiba::communication::Communicator> communicator)
    : communicator{std::move(communicator)} {}

void ChargeCoupledDevicesDiscovery::execute(bool error_on_no_devices) {
  if (!this->communicator->is_open()) {
    this->communicator->open();
  }

  const auto _ignored_response = this->communicator->request_with_response(
      communication::Command("ccd_discover", {}));

  const auto response = this->communicator->request_with_response(
      communication::Command("ccd_list", {}));
  if (response.json_results().empty() && error_on_no_devices) {
    throw new std::runtime_error("No CCDs connected");
  }

  auto raw_cdds = response.json_results();
  this->ccds = this->parse_ccds(raw_cdds);
}

std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>>
ChargeCoupledDevicesDiscovery::charge_coupled_devices() const {
  return this->ccds;
}

std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>>
ChargeCoupledDevicesDiscovery::parse_ccds(nlohmann::json raw_ccds) {
  spdlog::info("[ChargeCoupledDevicesDiscovery] detected #{} CCDS",
               raw_ccds.size());
  std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>>
      detected_ccds;
  for (auto it = raw_ccds.begin(); it != raw_ccds.end(); ++it) {
    const std::string& key = it.key();

    // we need to extract the <number> from the string 'index<number>:
    // %<other_number>'
    size_t start = key.find('x') + 1;
    size_t stop = key.find(':');
    const std::string index = key.substr(start, stop - start);
    detected_ccds.push_back(
        std::make_shared<single_devices::ChargeCoupledDevice>(
            std::stoi(index), this->communicator));
  }
  return detected_ccds;
}

} /* namespace horiba::devices */
