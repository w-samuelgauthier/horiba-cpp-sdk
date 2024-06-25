// Note: on Windows if you use scaling, add the environment variable GNUTERM="qt"
// to avoid strange rendering artifacts
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/icl_device_manager.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>
#include <horiba_cpp_sdk/devices/single_devices/mono.h>
#include <horiba_cpp_sdk/os/process.h>
#include <matplot/matplot.h>

#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <cmath>

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
  using namespace matplot;
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
  const auto ccd = ccds[0];

  const auto monos = icl_device_manager.monochromators();
  const auto mono = monos[0];
  const auto timeout = std::chrono::seconds(180);

  try {
    ccd->open();
    mono->open();
    mono->wait_until_ready(timeout);

    mono->home();
    mono->wait_until_ready(timeout);

    auto target_wavelength = 123.0;
    mono->move_to_target_wavelength(target_wavelength);
    mono->wait_until_ready(timeout);

    ccd->set_acquisition_format(
        1, ChargeCoupledDevice::AcquisitionFormat::SPECTRA);
    ccd->set_acquisition_count(1);
    ccd->set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::FROM_ICL_SETTINGS_INI);
    ccd->set_timer_resolution(
        ChargeCoupledDevice::TimerResolution::THOUSAND_MICROSECONDS);
    ccd->set_exposure_time(2);
    ccd->set_region_of_interest();

    if (ccd->get_acquisition_ready()) {
      auto open_shutter = true;
      ccd->set_acquisition_start(open_shutter);
      // wait a short time for the acquisition to start
      this_thread::sleep_for(chrono::milliseconds(200));

      while (ccd->get_acquisition_busy()) {
        this_thread::sleep_for(chrono::milliseconds(500));
      }

      auto raw_data = any_cast<json>(ccd->get_acquisition_data());
      auto xy_data = raw_data[0]["roi"][0]["xyData"];
      cout << xy_data << endl;

      auto x_data =
          xy_data | views::transform([](const auto &pair) { return pair[0]; });
      auto y_data =
          xy_data | views::transform([](const auto &pair) { return pair[1]; });

      vector<double> x_values;
      vector<int> y_values;

      ranges::copy(x_data, back_inserter(x_values));
      ranges::copy(y_data, back_inserter(y_values));

      plot(x_values, y_values);
      title("Center Scan At Wavelength " + to_string(target_wavelength) + "nm");
      xlabel("Wavelength [nm]");
      ylabel("Intensity");
      show();
    }

  } catch (const exception &e) {
    cout << e.what() << endl;
    ccd->close();
    mono->close();
    icl_device_manager.stop();
    return 1;
  }

  try {
    ccd->close();
    mono->close();
    icl_device_manager.stop();
  } catch (const exception &e) {
    cout << e.what() << endl;
    // we expect an exception when the socket gets closed by the remote
  }

  return 0;
}
