#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <memory>
#include <vector>

namespace horiba::devices::single_devices {
class Monochromator;
class ChargeCoupledDevice;
} /* namespace horiba::devices::single_devices */

namespace horiba::devices {

/**
 * @brief Responsible to detect and and manage Horiba devices connected to the
 * computer
 */
class DeviceManager {
 public:
  virtual ~DeviceManager() = default;

  /**
   * @brief Starts the device manager
   */
  virtual void start() = 0;

  /**
   * @brief Stops the device manager
   */
  virtual void stop() = 0;

  /**
   * @brief Discovers devices connected to the computer
   *
   * @param error_on_no_device whether to throw or not an exception if no
   * devices are found
   */
  virtual void discover_devices(bool error_on_no_device = false) = 0;

  /**
   * @brief The connected monochromators
   *
   * @return connected monochromators
   */
  /* virtual
   * std::vector<std::shared_ptr<horiba::devices::single_devices::Monochromator>>
   * monochromators() const = 0; */

  /**
   * @brief The connected charge coupled devices.
   *
   * @return connected ccds
   */
  virtual std::vector<
      std::shared_ptr<horiba::devices::single_devices::ChargeCoupledDevice>>
  charge_coupled_devices() const = 0;
};
} /* namespace horiba::devices */

#endif /* ifndef DEVICE_MANAGER_H */
