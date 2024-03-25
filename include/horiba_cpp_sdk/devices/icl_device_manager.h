#ifndef ICL_DEVICE_MANAGER_H
#define ICL_DEVICE_MANAGER_H

#include <horiba_cpp_sdk/communication/communicator.h>
#include <horiba_cpp_sdk/devices/device_manager.h>

namespace horiba::communication {
class Communicator;
} /* namespace horiba::communication */

namespace horiba::devices {
class ICLDeviceManager : public DeviceManager {
 public:
  explicit ICLDeviceManager(std::string websocket_ip = "127.0.0.1",
                            std::string websocket_port = "25010",
                            bool start_icl = true,
                            bool enable_binary_messages = true);

  void start() override;
  void stop() override;

  void discover_devices(bool error_on_no_device = false) override;
  /* std::vector<std::shared_ptr<horiba::devices::single_devices::Monochromator>>
   */
  /* monochromators() const override; */
  std::vector<
      std::shared_ptr<horiba::devices::single_devices::ChargeCoupledDevice>>
  charge_coupled_devices() const override;

 private:
  std::string websocket_ip;
  std::string websocket_port;
  bool start_icl;
  bool enable_binary_messages;
  std::shared_ptr<horiba::communication::Communicator> communicator;
  /* std::vector<std::shared_ptr<horiba::devices::single_devices::Monochromator>>
   */
  /*     monos; */
  std::vector<
      std::shared_ptr<horiba::devices::single_devices::ChargeCoupledDevice>>
      ccds;

  void start_icl_process();
  void enable_binary_messages_on_icl();
  void start_process(const std::string& path);
  bool is_process_running(const std::string& process_name);
};
} /* namespace horiba::devices */

#endif /* ifndef ICL_DEVICE_MANAGER_H */
