#ifndef DEVICE_DISCOVERY_H
#define DEVICE_DISCOVERY_H

namespace horiba::devices {

/**
 * @brief Interface for device discoveries on the ICL
 */
class DeviceDiscovery {
 public:
  virtual ~DeviceDiscovery() = default;

  /**
   * @brief Executes the device discovery
   *
   * @param error_on_no_devices whether to throw an exception or not if no
   * devices are detected
   */
  virtual void execute(bool error_on_no_devices) = 0;
};
} /* namespace horiba::devices */

#endif /* ifndef DEVICE_DISCOVERY_H */
