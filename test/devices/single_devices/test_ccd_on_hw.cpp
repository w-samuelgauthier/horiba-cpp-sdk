
#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdlib>
#include <thread>

#include "../../icl_exe.h"

// Warning about getenv being unsafe, we don't care about it here
#pragma warning(disable : 4996)

namespace horiba::test {

using namespace horiba::devices::single_devices;
using namespace horiba::communication;

TEST_CASE_METHOD(ICLExe, "CCD test on HW", "[ccd_hw]") {
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
      websocket_communicator->request_with_response(Command("ccd_discover"));
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
    REQUIRE(configuration.empty() == false);
  }

  SECTION("CCD get gain") {
    // arrange
    auto expected_gain = 0;
    ccd.open();

    // act
    auto gain = ccd.get_gain_token();

    // assert
    REQUIRE(gain == expected_gain);
  }

  SECTION("CCD gain can be set") {
    // arrange
    auto expected_gain_before = 1;
    auto expected_gain_after = 0;
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_gain(expected_gain_before));
    auto actual_gain_before = ccd.get_gain_token();

    // act
    REQUIRE_NOTHROW(ccd.set_gain(expected_gain_after));
    auto actual_gain_after = ccd.get_gain_token();

    // assert
    REQUIRE(actual_gain_before == expected_gain_before);
    REQUIRE(actual_gain_after == expected_gain_after);
  }

  SECTION("CCD get speed") {
    // arrange
    ccd.open();
    auto expected_speed = 0;

    // act
    auto speed = ccd.get_speed_token();

    // assert
    REQUIRE(speed == expected_speed);
  }

  SECTION("CCD speed can be set") {
    // arrange
    auto expected_speed_before = 1;
    auto expected_speed_after = 0;
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_speed(expected_speed_before));
    auto actual_speed_before = ccd.get_speed_token();

    // act
    REQUIRE_NOTHROW(ccd.set_speed(expected_speed_after));
    auto actual_speed_after = ccd.get_speed_token();

    // assert
    REQUIRE(actual_speed_before == expected_speed_before);
    REQUIRE(actual_speed_after == expected_speed_after);
  }

  SECTION("CCD get fit parameters") {
    // arrange
    ccd.open();

    // act
    auto fit_parameters = ccd.get_fit_parameters();

    // assert
    std::vector<int> expected_fit_parameters = {0, 1, 0, 0, 0};
    REQUIRE(fit_parameters == expected_fit_parameters);
  }

  SECTION("CCD fit parameters can be set") {
    // arrange
    ccd.open();
    std::vector<int> fit_parameters = {0, 1, 0, 0, 0};
    REQUIRE_NOTHROW(ccd.set_fit_parameters(fit_parameters));
    auto fit_parameters_before = ccd.get_fit_parameters();

    // act
    fit_parameters = {0, 1, 0, 0, 1};
    REQUIRE_NOTHROW(ccd.set_fit_parameters(fit_parameters));
    auto fit_parameters_after = ccd.get_fit_parameters();

    // assert
    REQUIRE(fit_parameters_before != fit_parameters_after);
  }

  SECTION("CCD get timer resolution") {
    // arrange
    ccd.open();

    // act
    auto timer_resolution = ccd.get_timer_resolution();

    // assert
    REQUIRE(timer_resolution ==
            ChargeCoupledDevice::TimerResolution::THOUSAND_MICROSECONDS);
  }

  SECTION("CCD timer resolution can be set") {
    // arrange
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_timer_resolution(
        ChargeCoupledDevice::TimerResolution::THOUSAND_MICROSECONDS));
    auto timer_resolution_before = ccd.get_timer_resolution();

    // act
    REQUIRE_NOTHROW(ccd.set_timer_resolution(
        ChargeCoupledDevice::TimerResolution::ONE_MICROSECOND));
    auto timer_resolution_after = ccd.get_timer_resolution();

    // assert
    REQUIRE(timer_resolution_before != timer_resolution_after);
    REQUIRE(timer_resolution_before ==
            ChargeCoupledDevice::TimerResolution::THOUSAND_MICROSECONDS);
    REQUIRE(timer_resolution_after ==
            ChargeCoupledDevice::TimerResolution::ONE_MICROSECOND);
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
    REQUIRE(acquisition_count == 1);
  }

  SECTION("CCD acquisition count can be set") {
    // arrange
    ccd.open();
    REQUIRE_NOTHROW(ccd.set_acquisition_count(1));
    auto acquisition_count_before = ccd.get_acquisition_count();

    // act
    REQUIRE_NOTHROW(ccd.set_acquisition_count(2));
    auto acquisition_count_after = ccd.get_acquisition_count();

    // assert
    REQUIRE(acquisition_count_before != acquisition_count_after);
    REQUIRE(acquisition_count_before == 1);
    REQUIRE(acquisition_count_after == 2);
  }

  SECTION("CCD get clean count") {
    // arrange
    ccd.open();

    // act
    auto clean_count = ccd.get_clean_count();

    // assert
    std::pair<int, ChargeCoupledDevice::CleanCountMode> expected_clean_count = {
        1, ChargeCoupledDevice::CleanCountMode::MODE_1};
    REQUIRE(clean_count == expected_clean_count);
  }

  // TODO: doesn't seem to work yet
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

    std::pair<int, ChargeCoupledDevice::CleanCountMode>
        expected_clean_count_before = {
            0, ChargeCoupledDevice::CleanCountMode::MODE_1};
    std::pair<int, ChargeCoupledDevice::CleanCountMode>
        expected_clean_count_after = {
            2, ChargeCoupledDevice::CleanCountMode::MODE_1};
    // assert
    REQUIRE(clean_count_before != clean_count_after);
    REQUIRE(clean_count_before == expected_clean_count_before);
    REQUIRE(clean_count_after == expected_clean_count_after);
  }

  SECTION("CCD get data size") {
    // arrange
    ccd.open();

    // act
    auto data_size = ccd.get_acquisition_data_size();

    // assert
    REQUIRE(data_size == 1024);
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
    REQUIRE(acquisition_data.has_value() == false);
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
    REQUIRE_NOTHROW(ccd.set_exposure_time(100000));
    REQUIRE_NOTHROW(ccd.set_region_of_interest());
    REQUIRE_NOTHROW(ccd.set_x_axis_conversion_type(
        ChargeCoupledDevice::XAxisConversionType::NONE));
    REQUIRE_NOTHROW(ccd.set_acquisition_start(true));

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // act
    auto acquisition_busy_before_abort = ccd.get_acquisition_busy();
    REQUIRE_NOTHROW(ccd.abort_acquisition(true));

    int wait_ms = 500;
    int total_waited_time_ms = 0;
    while (ccd.get_acquisition_busy() && total_waited_time_ms < 8000) {
      std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
      total_waited_time_ms += wait_ms;
    }
    auto acquisition_busy_after_abort = ccd.get_acquisition_busy();

    // assert
    REQUIRE(acquisition_busy_before_abort == true);
    REQUIRE(acquisition_busy_after_abort == false);
  }

  if (websocket_communicator->is_open()) {
    websocket_communicator->close();
  }

  stop();
}
}  // namespace horiba::test
