#ifndef CCD_DEVICES_DISCOVERY_H
#define CCD_DEVICES_DISCOVERY_H

#include <horiba_cpp_sdk/communication/communicator.h>
#include <horiba_cpp_sdk/devices/device_discovery.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include <memory>
#include <nlohmann/json.hpp>

namespace horiba::devices {
class ChargeCoupledDevicesDiscovery : public DeviceDiscovery {
 public:
  explicit ChargeCoupledDevicesDiscovery(
      std::shared_ptr<horiba::communication::Communicator> communicator);
  ~ChargeCoupledDevicesDiscovery() override = default;

  void execute(bool error_on_no_devices) noexcept(false) override;

  std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>>
  charge_coupled_devices() const;

 private:
  std::shared_ptr<horiba::communication::Communicator> communicator;
  std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>> ccds;

  std::vector<std::shared_ptr<single_devices::ChargeCoupledDevice>> parse_ccds(
      nlohmann::json raw_ccds);
};
} /* namespace horiba::devices */
#endif /* ifndef CCD_DEVICES_DISCOVERY_H */
