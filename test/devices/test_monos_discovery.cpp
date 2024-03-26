#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/monos_discovery.h>
#include <horiba_cpp_sdk/devices/single_devices/mono.h>

#include <catch2/catch_test_macros.hpp>

#include "../fake_icl_server.h"
#include "../icl_exe.h"

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE_METHOD(FakeICLServer, "Monochromators Discovery with fake ICL",
                 "[mono_discovery]") {
  // arrange
  auto websocket_communicator = std::make_shared<WebSocketCommunicator>(
      FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT));
  auto monos_discovery =
      devices::MonochromatorsDiscovery(websocket_communicator);

  SECTION("Monochromators can be discovered") {
    // act
    REQUIRE_NOTHROW(monos_discovery.execute(true));
    auto monos = monos_discovery.monochromators();

    // assert
    REQUIRE(monos.empty() == false);
    REQUIRE(monos.size() == 2);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}

TEST_CASE_METHOD(ICLExe, "Monochromators Discovery with ICL",
                 "[mono_discovery_hw]") {
  if (std::getenv("HAS_HARDWARE") == nullptr) {
    SUCCEED("Skipped: HAS_HARDWARE is not set");
    return;
  }

  // arrange
  auto websocket_communicator =
      std::make_shared<WebSocketCommunicator>("127.0.0.1", "25010");
  auto monos_discovery =
      devices::MonochromatorsDiscovery(websocket_communicator);

  SECTION("Monochromators can be discovered") {
    // act
    REQUIRE_NOTHROW(monos_discovery.execute(true));
    auto monos = monos_discovery.monochromators();

    // assert
    REQUIRE(monos.empty() == false);
    REQUIRE(monos.size() == 1);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}

}  // namespace horiba::test
