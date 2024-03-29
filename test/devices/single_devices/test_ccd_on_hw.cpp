
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include <catch2/catch_test_macros.hpp>

#include "../../fake_icl_server.h"

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE("CCD test on HW", "[ccd_hw]") {
  if (std::getenv("HAS_HARDWARE") == nullptr) {
    SUCCEED("Skipped: HAS_HARDWARE is not set");
    return;
  }

  // arrange
  auto websocket_communicator =
      std::make_shared<WebSocketCommunicator>("127.0.0.1", "25010");
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
    auto ccd_open_before_close = ccd.is_open();

    // act
    REQUIRE_NOTHROW(ccd.close());

    auto ccd_open_after_close = ccd.is_open();

    // assert
    REQUIRE(ccd_open_before_close == true);
    REQUIRE(ccd_open_after_close == false);
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
    REQUIRE_NOTHROW(ccd.set_gain(ChargeCoupledDevice::Gain::HIGH_LIGHT));
    auto gain_before = ccd.get_gain();

    // act
    REQUIRE_NOTHROW(
        ccd.set_gain(ChargeCoupledDevice::Gain::BEST_DYNAMIC_RANGE));
    auto gain_after = ccd.get_gain();

    // assert
    REQUIRE(gain_before != gain_after);
    REQUIRE(gain_before == ChargeCoupledDevice::Gain::HIGH_LIGHT);
    REQUIRE(gain_after == ChargeCoupledDevice::Gain::BEST_DYNAMIC_RANGE);
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
    REQUIRE_NOTHROW(ccd.set_speed(ChargeCoupledDevice::Speed::SLOW_45_kHz));
    auto speed_before = ccd.get_speed();

    // act
    REQUIRE_NOTHROW(ccd.set_speed(ChargeCoupledDevice::Speed::MEDIUM_1_MHz));
    auto speed_after = ccd.get_speed();

    // assert
    REQUIRE(speed_before != speed_after);
    REQUIRE(speed_before == ChargeCoupledDevice::Speed::SLOW_45_kHz);
    REQUIRE(speed_after == ChargeCoupledDevice::Speed::MEDIUM_1_MHz);
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
    REQUIRE_NOTHROW(ccd.set_fit_params("0,1,0,0,0"));
    auto fit_params_before = ccd.get_fit_params();

    // act
    REQUIRE_NOTHROW(ccd.set_fit_params("0,1,0,0,1"));
    auto fit_params_after = ccd.get_fit_params();

    // assert
    REQUIRE(fit_params_before != fit_params_after);
    REQUIRE(fit_params_before != "0,1,0,0,0");
    REQUIRE(fit_params_after != "0,1,0,0,1");
  }

  SECTION("CCD get timer resolution") {
    // arrange
    ccd.open();

    // act
    auto timer_resolution = ccd.get_timer_resolution();

    // assert
    // TODO: check if this is the case
    REQUIRE(timer_resolution == 1000);
  }

  SECTION("CCD timer resolution can be set") {
    // arrange
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_timer_resolution(1000));
    auto timer_resolution_before = ccd.get_timer_resolution();

    // act
    REQUIRE_NOTHROW(ccd.set_timer_resolution(2000));
    auto timer_resolution_after = ccd.get_timer_resolution();

    // assert
    REQUIRE(timer_resolution_before != timer_resolution_after);
    REQUIRE(timer_resolution_before == 1000);
    REQUIRE(timer_resolution_after == 2000);
  }

  SECTION("CCD acquisition format can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_acquisition_format(
        1, ChargeCoupledDevice::AcquisitionFormat::IMAGE));
    // we cannot check those values
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
    REQUIRE_NOTHROW(ccd.set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::NONE));
    auto x_axis_conversion_type_before = ccd.get_x_axis_conversion_type();

    // act
    REQUIRE_NOTHROW(ccd.set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::FROM_CCD_FIRMWARE));
    auto x_axis_conversion_type_after = ccd.get_x_axis_conversion_type();

    // assert
    REQUIRE(x_axis_conversion_type_before != x_axis_conversion_type_after);
    REQUIRE(x_axis_conversion_type_before ==
            ChargeCoupledDevice::XAxisConversionType::NONE);
    REQUIRE(x_axis_conversion_type_after ==
            ChargeCoupledDevice::XAxisConversionType::FROM_CCD_FIRMWARE);
  }

  SECTION("CCD get acquisition count") {
    // arrange
    ccd.open();

    // act
    auto acquisition_count = ccd.get_acquisition_count();

    // assert
    REQUIRE(acquisition_count == 0);
  }

  SECTION("CCD acquisition count can be set") {
    // arrange
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_acquisition_count(0));
    auto acquisition_count_before = ccd.get_acquisition_count();

    // act
    REQUIRE_NOTHROW(ccd.set_acquisition_count(1));
    auto acquisition_count_after = ccd.get_acquisition_count();

    // assert
    REQUIRE(acquisition_count_before != acquisition_count_after);
    REQUIRE(acquisition_count_before == 0);
    REQUIRE(acquisition_count_after == 1);
  }

  SECTION("CCD get clean count") {
    // arrange
    ccd.open();

    // act
    auto clean_count = ccd.get_clean_count();

    // assert
    REQUIRE(clean_count == "count: 0 mode: 238");
  }

  SECTION("CCD clean count can be set") {
    // arrange
    ccd.open();
    REQUIRE_NOTHROW(
        ccd.set_clean_count(0, ChargeCoupledDevice::CleanCountMode::MODE_1));
    auto clean_count_before = ccd.get_clean_count();

    // act
    REQUIRE_NOTHROW(
        ccd.set_clean_count(2, ChargeCoupledDevice::CleanCountMode::MODE_1));
    auto clean_count_after = ccd.get_clean_count();

    // assert
    REQUIRE(clean_count_before != clean_count_after);
    REQUIRE(clean_count_before == "count: 0 mode: 238");
    REQUIRE(clean_count_after == "count: 2 mode: 238");
  }

  SECTION("CCD get data size") {
    // arrange
    ccd.open();

    // act
    auto data_size = ccd.get_data_size();

    // assert
    REQUIRE(data_size == 0);
  }

  SECTION("CCD get temperature") {
    // arrange
    ccd.open();

    // act
    auto temperature = ccd.get_temperature();

    // assert
    REQUIRE(temperature < 0.0);
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
    REQUIRE_NOTHROW(ccd.set_exposure_time(0));
    auto exposure_time_before = ccd.get_exposure_time();

    // act
    REQUIRE_NOTHROW(ccd.set_exposure_time(1000));
    auto exposure_time_after = ccd.get_exposure_time();

    // assert
    REQUIRE(exposure_time_before != exposure_time_after);
    REQUIRE(exposure_time_before == 0);
    REQUIRE(exposure_time_after == 1000);
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
  }

  SECTION("CCD ROI can be set") {
    // arrange
    ccd.open();

    // act
    // assert
    REQUIRE_NOTHROW(ccd.set_region_of_interest(1, 1, 1, 1023, 255, 1, 256));
    // there is no way to check what ROI is set
  }

  SECTION("CCD acquisition data can be obtained") {
    // arrange
    ccd.open();

    // act
    auto acquisition_data = ccd.get_acquisition_data();

    // assert
    REQUIRE(acquisition_data.empty() == true);
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

    REQUIRE_NOTHROW(ccd.set_acquisition_count(1));
    REQUIRE_NOTHROW(ccd.set_exposure_time(4000));
    REQUIRE_NOTHROW(ccd.set_region_of_interest());
    REQUIRE_NOTHROW(ccd.set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::NONE));
    REQUIRE_NOTHROW(ccd.set_acquisition_start(true));

    // act
    auto acquisition_busy_before_abort = ccd.get_acquisition_busy();
    REQUIRE_NOTHROW(ccd.abort_acquisition());
    auto acquisition_busy_after_abort = ccd.get_acquisition_busy();

    // assert
    REQUIRE(acquisition_busy_before_abort == true);
    REQUIRE(acquisition_busy_after_abort == false);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }
}
}  // namespace horiba::test
