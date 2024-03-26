#ifndef MONO_DEVICES_DISCOVERY_H
#define MONO_DEVICES_DISCOVERY_H

#include <horiba_cpp_sdk/communication/communicator.h>
#include <horiba_cpp_sdk/devices/device_discovery.h>
#include <horiba_cpp_sdk/devices/single_devices/mono.h>

#include <memory>
#include <nlohmann/json.hpp>

namespace horiba::devices {
/**
 * @brief Represents a discovery of CCD cameras on the ICL
 */
class MonochromatorsDiscovery : public DeviceDiscovery {
 public:
  /**
   * @brief Builds a Monochromator discovery
   *
   * @param communicator The communicator that will talk to the ICL
   */
  explicit MonochromatorsDiscovery(
      std::shared_ptr<horiba::communication::Communicator> communicator);
  ~MonochromatorsDiscovery() override = default;

  /**
   * @brief Executes the Monochromator discovery
   *
   * @param error_on_no_devices Whether to throw an exception or not if no
   * devices are detected
   */
  void execute(bool error_on_no_devices) noexcept(false) override;

  /**
   * @brief Monochromators that have been discovered after calling the execute()
   * function
   *
   * @return The detected monochromators
   */
  [[nodiscard]] std::vector<std::shared_ptr<single_devices::Monochromator>>
  monochromators() const;

 private:
  std::shared_ptr<horiba::communication::Communicator> communicator;
  std::vector<std::shared_ptr<single_devices::Monochromator>> monos;

  std::vector<std::shared_ptr<single_devices::Monochromator>> parse_monos(
      const nlohmann::json& raw_monos_list);
};
} /* namespace horiba::devices */
#endif /* ifndef MONO_DEVICES_DISCOVERY_H */
