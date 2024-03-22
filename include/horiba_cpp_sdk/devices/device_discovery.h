#ifndef DEVICE_DISCOVERY_H
#define DEVICE_DISCOVERY_H

namespace horiba::devices {

class DeviceDiscovery {
 public:
  virtual ~DeviceDiscovery() = default;

  virtual void execute(bool error_on_no_devices) = 0;
};
} /* namespace horiba::devices */

#endif /* ifndef DEVICE_DISCOVERY_H */
