#include <horiba_cpp_sdk/devices/icl_device_manager.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>
#include <horiba_cpp_sdk/os/process.h>

#include <iostream>
#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <horiba_cpp_sdk/os/windows_process.h>
#endif

namespace horiba::os {
class FakeProcess : public Process {
 public:
  void start() { this->is_running = true; }
  bool running() { return this->is_running; }
  void stop() { this->is_running = false; }

 private:
  bool is_running = false;
};
} /* namespace horiba::os */

auto main(int argc, char *argv[]) -> int {
  using namespace nlohmann;
  using namespace horiba::devices;
  using namespace horiba::os;
  using namespace horiba::devices::single_devices;
  using namespace horiba::communication;
  using namespace std;

#ifdef _WIN32
  auto icl_process = std::make_shared<WindowsProcess>(
      R"(C:\Program Files\HORIBA Scientific\SDK\)", R"(icl.exe)");
#else
  auto icl_process = std::make_shared<FakeProcess>();
#endif
  auto icl_device_manager = ICLDeviceManager(icl_process);

  icl_device_manager.start();
  icl_device_manager.discover_devices();

  const auto ccds = icl_device_manager.charge_coupled_devices();
  if (ccds.empty()) {
    std::cerr << "No CCDs found" << std::endl;
    return 1;
  }

  const auto ccd = ccds[0];

  try {
    ccd->open();

    auto config = ccd->get_configuration();

    cout << "------ Configuration ------" << endl;
    cout << "Gains: " << config["gains"].dump() << endl;
    cout << "Speeds: " << config["speeds"].dump() << endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    ccd->close();
  }

  ccd->close();
  icl_device_manager.stop();

  return 0;
}
