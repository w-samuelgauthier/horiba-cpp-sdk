#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include "../../fake_icl_server.h"

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;


TEST_CASE_METHOD(FakeICLServer, "CCD test with fake ICL", "[ccd_no_hw]") {
  // arrange
  auto websocket_communicator = std::make_shared<WebSocketCommunicator>(FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT));
  auto ccd = ChargeCoupledDevice(0, websocket_communicator);

  SECTION("CCD can be opened") {
    // act
    ccd.open();
    auto ccd_open = ccd.is_open();

    // assert
    REQUIRE(ccd_open == true);
  }

  SECTION("CCD can be closed") {
    // arrange
    ccd.open();
    auto ccd_open = ccd.is_open();

    // act
    REQUIRE_NOTHROW(ccd.close());
    // we do not check if it is closed as the fake answer from the ICL always returns true

    // assert
    REQUIRE(ccd_open == true);
  }

  SECTION("CCD can be restarted") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.restart());
  }

  SECTION("CCD configuration can be accessed") {
    // arrange
    ccd.open();

    // act
    auto configuration = ccd.get_configuration();

    // assert
    // TODO: At the moment we do not know how the configuration looks like, hence it is empty
    REQUIRE(configuration.empty() == true);
  }

  SECTION("CCD number of averages") {
    // arrange
    ccd.open();

    // act
    auto number_of_averages = ccd.get_number_of_averages();

    // assert
    // TODO: At the moment this function is not supported by the ICL
    REQUIRE(number_of_averages == 0);
  }

  SECTION("CCD get gain") {
    // arrange
    ccd.open();

    // act
    auto gain = ccd.get_gain();

    // assert
    REQUIRE(gain == ChargeCoupledDevice::Gain::HIGH_LIGHT);
  }

  SECTION("CCD gain can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_gain(ChargeCoupledDevice::Gain::HIGH_LIGHT));
    // we do not check if the new gain is set, as the fake answer from the ICL always returns the same value
  }

  SECTION("CCD get speed") {
    // arrange
    ccd.open();

    // act
    auto speed = ccd.get_speed();

    // assert
    REQUIRE(speed == ChargeCoupledDevice::Speed::SLOW_45_kHz);
  }

  SECTION("CCD speed can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_speed(ChargeCoupledDevice::Speed::SLOW_45_kHz));
    // we do not check if the new speed is set, as the fake answer from the ICL always returns the same value
  }

  SECTION("CCD get fit params") {
    // arrange
    ccd.open();

    // act
    auto fit_params = ccd.get_fit_params();

    // assert
    // TODO: at the moment we do not know what is returned, so it stays a string
    REQUIRE(fit_params == "0,1,0,0,0");
  }

  SECTION("CCD fit params can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_fit_params("0,1,0,0,0"));
    // we do not check if the new fit params are set, as the fake answer from the ICL always returns the same value
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}
}
