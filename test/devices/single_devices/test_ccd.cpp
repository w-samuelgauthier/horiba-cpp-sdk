#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include "../../fake_icl_server.h"

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE_METHOD(FakeICLServer, "CCD test with fake ICL", "[ccd_no_hw]") {
  // arrange
  auto websocket_communicator = std::make_shared<WebSocketCommunicator>(
      FakeICLServer::FAKE_ICL_ADDRESS,
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
    // we do not check if it is closed as the fake answer from the ICL always
    // returns true

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
    // TODO: At the moment we do not know how the configuration looks like,
    // hence it is empty
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
    // we do not check if the new gain is set, as the fake answer from the ICL
    // always returns the same value
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
    // we do not check if the new speed is set, as the fake answer from the ICL
    // always returns the same value
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
    // we do not check if the new fit params are set, as the fake answer from
    // the ICL always returns the same value
  }

  SECTION("CCD get timer resolution") {
    // arrange
    ccd.open();

    // act
    auto timer_resolution = ccd.get_timer_resolution();

    // assert
    REQUIRE(timer_resolution == 1000);
  }

  SECTION("CCD timer resolution can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_timer_resolution(2000));
    // we do not check if the new timer resolution is set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("CCD acquisition format can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_acquisition_format(
        1, ChargeCoupledDevice::AcquisitionFormat::IMAGE));
    // we do not check if the new acquisition format is set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("CCD get x axis conversion type") {
    // arrange
    ccd.open();

    // act
    auto x_axis_conversion_type = ccd.get_x_axis_conversion_type();

    // assert
    REQUIRE(x_axis_conversion_type ==
            ChargeCoupledDevice::XAxisConversionType::NONE);
  }

  SECTION("CCD x axis conversion can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::FROM_CCD_FIRMWARE));
    // we do not check if the new x axis conversion type is set, as the fake
    // answer from the ICL always returns the same value
  }

  SECTION("CCD get acquisition count") {
    // arrange
    ccd.open();

    // act
    auto acquisition_count = ccd.get_acquisition_count();

    // assert
    REQUIRE(acquisition_count == 1);
  }

  SECTION("CCD acquisition count can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_acquisition_count(0));
    // we do not check if the new acquisition count is set, as the fake answer
    // from the ICL always returns the same value
  }

  SECTION("CCD get clean count") {
    // arrange
    ccd.open();

    // act
    auto clean_count = ccd.get_clean_count();

    // assert
    REQUIRE(clean_count == "count: 1 mode: 238");
  }

  SECTION("CCD clean count can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(
        ccd.set_clean_count(0, ChargeCoupledDevice::CleanCountMode::MODE_1));
    // we do not check if the new fit params are set, as the fake answer from
    // the ICL always returns the same value
  }

  SECTION("CCD get data size") {
    // arrange
    ccd.open();

    // act
    auto data_size = ccd.get_data_size();

    // assert
    REQUIRE(data_size == 1024);
  }

  SECTION("CCD get temperature") {
    // arrange
    ccd.open();

    // act
    auto temperature = ccd.get_temperature();

    // assert
    REQUIRE(temperature == -31.219999313354492);
  }

  SECTION("CCD get chip size") {
    // arrange
    ccd.open();

    // act
    auto chip_size = ccd.get_chip_size();

    // assert
    REQUIRE(chip_size.first == 1024);
    REQUIRE(chip_size.second == 256);
  }

  SECTION("CCD get exposure time") {
    // arrange
    ccd.open();

    // act
    auto exposure_time = ccd.get_exposure_time();

    // assert
    REQUIRE(exposure_time == 0);
  }

  SECTION("CCD exposure time can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_exposure_time(1000));
    // we do not check if the new exposure time is set, as the fake answer from
    // the ICL always returns the same value
  }

  SECTION("CCD get acquisition ready") {
    // arrange
    ccd.open();

    // act
    auto acquisition_ready = ccd.get_acquisition_ready();

    // assert
    REQUIRE(acquisition_ready == true);
  }

  SECTION("CCD acquisition start can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_acquisition_start(true));
    // we do not check if the acquisition started, as the fake answer from
    // the ICL always returns the same value
  }

  SECTION("CCD ROI can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_region_of_interest(2, 1, 1, 1023, 255, 1, 256));
    // we do not check if the new ROI is set, as the fake answer from
    // the ICL always returns the same value
  }

  SECTION("CCD acquisition data can be obtained") {
    // arrange
    ccd.open();

    // act
    auto acquisition_data = ccd.get_acquisition_data();

    // assert
    REQUIRE(acquisition_data.empty() == false);
    REQUIRE(acquisition_data.size() == 3);
  }

  SECTION("CCD get acquisition busy") {
    // arrange
    ccd.open();

    // act
    auto acquisition_busy = ccd.get_acquisition_busy();

    // assert
    REQUIRE(acquisition_busy == false);
  }

  SECTION("CCD acquisition can be aborted") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.abort_acquisition());
    // we do not check if the acquisition has truly stopped, as the fake answer
    // from the ICL always returns the same value
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}
}  // namespace horiba::test
