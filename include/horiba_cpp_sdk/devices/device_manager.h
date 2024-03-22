#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <memory>
#include <vector>

namespace horiba::devices::single_devices {
class Monochromator;
class ChargeCoupledDevice;
} /* namespace horiba::devices::single_devices */

namespace horiba::devices {

class DeviceManager {
 public:
  virtual ~DeviceManager() = default;

  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void discover_devices(bool error_on_no_device = false) = 0;
  virtual std::vector<
      std::shared_ptr<horiba::devices::single_devices::Monochromator>>
  monochromators() const = 0;
  virtual std::vector<
      std::shared_ptr<horiba::devices::single_devices::ChargeCoupledDevice>>
  charge_coupled_devices() const = 0;
};
} /* namespace horiba::devices */

#endif /* ifndef DEVICE_MANAGER_H */
