#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/ccds_discovery.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include <catch2/catch_test_macros.hpp>
#include <cstdlib>

#include "../fake_icl_server.h"
#include "../icl_exe.h"

// Warning about getenv being unsafe, we don't care about it here
#pragma warning(disable : 4996)

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE_METHOD(FakeICLServer, "CCDs Discovery with fake ICL",
                 "[ccd_discovery]") {
  // arrange
  auto websocket_communicator = std::make_shared<WebSocketCommunicator>(
      FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT));
  auto ccds_discovery =
      devices::ChargeCoupledDevicesDiscovery(websocket_communicator);

  SECTION("CCDs can be discovered") {
    // act
    REQUIRE_NOTHROW(ccds_discovery.execute(true));
    auto ccds = ccds_discovery.charge_coupled_devices();

    // assert
    REQUIRE(ccds.empty() == false);
    REQUIRE(ccds.size() == 1);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}

TEST_CASE_METHOD(ICLExe, "CCDs Discovery with ICL", "[ccd_discovery_hw]") {
  const char* has_hardware = std::getenv("HAS_HARDWARE");
  if (has_hardware == nullptr || std::string(has_hardware) == "0" ||
      std::string(has_hardware) == "false") {
    SUCCEED("Skipped: HAS_HARDWARE is not set");
    return;
  }

  start();

  // arrange
  auto websocket_communicator =
      std::make_shared<WebSocketCommunicator>("127.0.0.1", "25010");
  websocket_communicator->open();
  auto ccds_discovery =
      devices::ChargeCoupledDevicesDiscovery(websocket_communicator);

  SECTION("CCDs can be discovered") {
    // act
    REQUIRE_NOTHROW(ccds_discovery.execute(true));
    auto ccds = ccds_discovery.charge_coupled_devices();

    // assert
    REQUIRE(ccds.empty() == false);
    REQUIRE(ccds.size() == 1);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }

  stop();
}

}  // namespace horiba::test
