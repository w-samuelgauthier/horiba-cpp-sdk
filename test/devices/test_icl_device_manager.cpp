#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/icl_device_manager.h>
#include <horiba_cpp_sdk/os/process.h>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <string>

#include "../fake_icl_server.h"
#include "../icl_exe.h"
#include "../os/fake_process.h"

// Warning about getenv being unsafe, we don't care about it here
#pragma warning(disable : 4996)

namespace horiba::test {

using json = nlohmann::json;

TEST_CASE("ICL Device Manager test with fake ICL", "[icl_device_manager]") {
  spdlog::set_level(spdlog::level::debug);

  // arrange
  std::shared_ptr<horiba::os::Process> fake_icl_process =
      std::make_shared<horiba::os::FakeProcess>();
  horiba::devices::ICLDeviceManager device_manager(
      fake_icl_process, FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT), true, true);

  SECTION("Devices can be discovered") {
    // act
    device_manager.discover_devices();

    const auto ccds = device_manager.charge_coupled_devices();
    const auto monos = device_manager.monochromators();

    // assert
    REQUIRE(ccds.size() == 1);
    REQUIRE(monos.size() == 1);
  }
}

TEST_CASE("ICL Device Manager test on hardware", "[icl_device_manager_hw]") {
  const char* has_hardware = std::getenv("HAS_HARDWARE");
  if (has_hardware == nullptr || std::string(has_hardware) == "0" ||
      std::string(has_hardware) == "false") {
    SUCCEED("Skipped: HAS_HARDWARE is not set");
    return;
  }
#if _WIN32
  const auto icl_path = R"(C:\Program Files\HORIBA Scientific\SDK\)";
  const auto icl_exe = R"(icl.exe)";

  std::shared_ptr<horiba::os::Process> icl_process =
      std::make_shared<horiba::os::WindowsProcess>(icl_path, icl_exe);

  horiba::devices::ICLDeviceManager device_manager(icl_process);

  device_manager.start();
  SECTION("Devices can be discovered") {
    // act
    device_manager.discover_devices();

    const auto ccds = device_manager.charge_coupled_devices();
    const auto monos = device_manager.monochromators();

    // assert
    REQUIRE(ccds.size() == 1);
    REQUIRE(monos.size() == 1);
  }
  device_manager.stop();
#else
  SUCCEED("ICL Device Manager tests on HW are not run on Unix systems");
#endif
}
}  // namespace horiba::test
