#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/mono.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../fake_icl_server.h"

namespace horiba::test {

using Catch::Matchers::Equals;
using Catch::Matchers::WithinAbs;

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE("Mono test with fake ICL", "[mono_no_hw]") {
  // arrange
  auto websocket_communicator = std::make_shared<WebSocketCommunicator>(
      FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT));
  auto mono = Monochromator(0, websocket_communicator);

  SECTION("Mono can be opened") {
    // act
    mono.open();
    auto mono_open = mono.is_open();

    // assert
    REQUIRE(mono_open);
  }

  SECTION("Mono can be closed") {
    // arrange
    mono.open();
    auto mono_open = mono.is_open();

    // act
    REQUIRE_NOTHROW(mono.close());
    // we do not check if it is closed as the fake answer from the ICL always
    // returns true

    // assert
    REQUIRE(mono_open);
  }

  SECTION("Mono is busy") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_FALSE(mono.is_busy());
  }

  SECTION("Mono can be homed") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.home());
  }

  SECTION("Mono config") {
    // arrange
    mono.open();

    // act
    auto config = mono.configuration();

    // assert
    REQUIRE_FALSE(config.empty());
  }

  SECTION("Mono get current wavelength") {
    // arrange
    mono.open();

    // act
    auto wavelength = mono.get_current_wavelength();

    // assert
    REQUIRE_THAT(wavelength, WithinAbs(320.0, 0.001));
  }

  SECTION("Mono wavelenght can be set") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.calibrate_wavelength(120.0));
    // we do not check if the new wavelength is set, as the fake answer from the
    // ICL always returns the same value
  }

  SECTION("Mono move to target wavelength") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.move_to_target_wavelength(125.0));
    // we do not check if the new wavelength is set, as the fake answer from the
    // ICL always returns the same value
  }

  SECTION("Mono get turret grating") {
    // arrange
    mono.open();

    // act
    auto turret_grating = mono.get_turret_grating();

    // assert
    REQUIRE(turret_grating == Monochromator::Grating::SECOND);
  }

  SECTION("Mono can set turret grating") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_turret_grating(Monochromator::Grating::FIRST));
    // we do not check if the new grating params are set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("Mono get filter wheel position") {
    // arrange
    mono.open();

    // act
    auto filter_wheel_position = mono.get_filter_wheel_position();

    // assert
    REQUIRE(filter_wheel_position == Monochromator::FilterWheelPosition::RED);
  }

  SECTION("Mono set filter wheel position") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_filter_wheel_position(
        Monochromator::FilterWheelPosition::BLUE));
    // we do not check if the new filter wheel position is set, as the fake
    // answer from the ICL always returns the same value
  }

  SECTION("Mono get mirror position") {
    // arrange
    mono.open();

    // act
    auto mirror_position =
        mono.get_mirror_position(Monochromator::Mirror::FIRST);

    // assert
    REQUIRE(mirror_position == Monochromator::MirrorPosition::A);
  }

  SECTION("Mono mirror position can be set") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_mirror_position(Monochromator::Mirror::FIRST,
                                             Monochromator::MirrorPosition::B));
    // we do not check if the new position is set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("Mono get slit position") {
    // arrange
    mono.open();

    // act
    auto slit_position = mono.get_slit_position_in_mm(Monochromator::Slit::A);

    // assert
    REQUIRE_THAT(slit_position, WithinAbs(1.5, 0.001));
  }

  SECTION("Mono slit position can be set") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_slit_position(Monochromator::Slit::A, 2.2));
    // we do not check if the new slit position is set, as the fake
    // answer from the ICL always returns the same value
  }

  SECTION("Mono get slit step position") {
    // arrange
    mono.open();

    // act
    auto slit_step_position =
        mono.get_slit_step_position(Monochromator::Slit::A);

    // assert
    REQUIRE(slit_step_position == Monochromator::SlitStepPosition::A);
  }

  SECTION("Mono slit step position can be set") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_slit_step_position(
        Monochromator::Slit::A, Monochromator::SlitStepPosition::C));
    // we do not check if the new slit step position is set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("Mono can open shutter") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.open_shutter());
    // we do not check if the shutter is open, as the fake answer from the ICL
    // always returns the same value
  }

  SECTION("Mono can close shutter") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.close_shutter());
    // we do not check if the shutter is open, as the fake answer from the ICL
    // always returns the same value
  }

  SECTION("Mono get shutter position") {
    // arrange
    mono.open();

    // act
    auto shutter_position = mono.get_shutter_position();

    // assert
    REQUIRE(shutter_position == Monochromator::ShutterPosition::CLOSED);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}
}  // namespace horiba::test
