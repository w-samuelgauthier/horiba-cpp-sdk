#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/mono.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <cstdlib>

#include "../../icl_exe.h"

// Warning about getenv being unsafe, we don't care about it here
#pragma warning(disable : 4996)

namespace horiba::test {

using Catch::Matchers::Equals;
using Catch::Matchers::WithinAbs;

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE_METHOD(ICLExe, "Mono test on HW", "[mono_hw]") {
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
  auto _ignored_response =
      websocket_communicator->request_with_response(Command("mono_discover"));
  auto mono = Monochromator(0, websocket_communicator);

  SECTION("Mono can be opened") {
    // act
    mono.open();
    auto mono_open = mono.is_open();

    // assert
    REQUIRE(mono_open == true);
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
    REQUIRE(mono_open == true);
  }

  SECTION("Mono is busy") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE(mono.is_busy() == false);
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
    // TODO: At the moment we do not know how the config looks like.
    REQUIRE_THAT(config, Equals("{}"));
  }

  SECTION("Mono get current wavelength") {
    // arrange
    mono.open();

    // act
    auto wavelength = mono.get_current_wavelength();

    // assert
    REQUIRE_THAT(wavelength, WithinAbs(0.0, 0.001));
  }

  SECTION("Mono wavelenght can be set") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.calibrate_wavelength(120.0));
    // we do not check if the new wavelength is set, as the fake answer from */
    // /*  * the ICL always returns the same value
  }

  SECTION("Mono move to target wavelength") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.move_to_target_wavelength(125.0));
    // we do not check if the new wavelength is set, as the fake answer from */
    // /*  * the ICL always returns the same value
  }

  SECTION("Mono get turret grating") {
    // arrange
    mono.open();
    mono.set_turret_grating(Monochromator::Grating::FIRST);

    // act
    auto turret_grating = mono.get_turret_grating();

    // assert
    REQUIRE(turret_grating == Monochromator::Grating::FIRST);
  }

  SECTION("Mono can set turret grating") {
    // arrange
    mono.open();
    REQUIRE_NOTHROW(mono.set_turret_grating(Monochromator::Grating::FIRST));
    const auto grating_before = mono.get_turret_grating();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_turret_grating(Monochromator::Grating::SECOND));
    const auto grating_after = mono.get_turret_grating();

    REQUIRE(grating_before != grating_after);
    REQUIRE(grating_before == Monochromator::Grating::FIRST);
    REQUIRE(grating_after == Monochromator::Grating::SECOND);
  }

  // TODO: Function is not supported ATM
  SECTION("Mono get filter wheel position") {
    // arrange
    mono.open();

    // act
    auto filter_wheel_position = mono.get_filter_wheel_position();

    // assert
    REQUIRE(filter_wheel_position == Monochromator::FilterWheelPosition::RED);
  }

  // TODO: Function is not supported
  SECTION("Mono set filter wheel position") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.set_filter_wheel_position(
        Monochromator::FilterWheelPosition::BLUE));
    // we do not check if the new filter wheel position is set, as the fake */
    // /* answer from the ICL always returns the same value
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
    REQUIRE_THAT(slit_position, WithinAbs(1.0, 0.001));
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

  // TODO: We need to get more information about slit step position, is it in
  // mm? or fixed values?
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
    // we do not check if the new slit step position is set, as the fake */ /*
    // * answer from the ICL always returns the same value
  }

  SECTION("Mono can open shutter") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.open_shutter());
    // we do not check if the shutter is open, as the fake answer from the */ /*
    // * ICL always returns the same value
  }

  SECTION("Mono can close shutter") {
    // arrange
    mono.open();

    // act
    // assert
    REQUIRE_NOTHROW(mono.close_shutter());
    // we do not check if the shutter is open, as the fake answer from the */ /*
    // * ICL always returns the same value
  }

  // TODO: What should be done when shutter is not configured?
  SECTION("Mono get shutter position") {
    // arrange
    mono.open();

    // act
    auto shutter_position = mono.get_shutter_position();

    // assert
    REQUIRE(shutter_position == Monochromator::ShutterPosition::CLOSED);
  }

  if (mono.is_open()) {
    mono.close();
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }

  stop();
}
}  // namespace horiba::test
