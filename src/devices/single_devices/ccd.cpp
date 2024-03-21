#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

#include <sstream>
#include <unordered_map>

#include "horiba_cpp_sdk/communication/command.h"

namespace horiba::devices::single_devices {

ChargeCoupledDevice::ChargeCoupledDevice(
    int id, std::shared_ptr<communication::Communicator> communicator)
    : Device(id, communicator) {}

void ChargeCoupledDevice::open() {
  Device::open();
  Device::execute_command(
      communication::Command("ccd_open", {{"index", Device::device_id()}}));
}

void ChargeCoupledDevice::close() { Device::close(); }

bool ChargeCoupledDevice::is_open() {
  auto response = Device::execute_command(
      communication::Command("ccd_isOpen", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  return json_results.at("open").get<bool>();
}

void ChargeCoupledDevice::restart() {}

std::unordered_map<std::string, std::any>
ChargeCoupledDevice::get_configuration() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getConfig", {{"index", Device::device_id()}}));
  auto results = response.json_results();

  // TODO: chek how to properly return configuration. New type CCDConfiguration
  // needed?
  std::unordered_map<std::string, std::any> configuration;
  for (const auto& [key, value] : results.items()) {
    configuration[std::string(key)] = value;
  }
  return configuration;
}

int ChargeCoupledDevice::get_number_of_averages() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getNumberOfAvgs", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  return json_results.at("count").get<int>();
}

void ChargeCoupledDevice::set_number_of_averages(int number_of_averages) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setNumberOfAvgs",
      {{"index", Device::device_id()}, {"count", number_of_averages}}));
}

ChargeCoupledDevice::Gain ChargeCoupledDevice::get_gain() {
  auto response = Device::execute_command(
      communication::Command("ccd_getGain", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto gain = json_results.at("token").get<int>();

  return static_cast<ChargeCoupledDevice::Gain>(gain);
}

void ChargeCoupledDevice::set_gain(ChargeCoupledDevice::Gain gain) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setGain",
      {{"index", Device::device_id()}, {"token", static_cast<int>(gain)}}));
}

ChargeCoupledDevice::Speed ChargeCoupledDevice::get_speed() {
  auto response = Device::execute_command(
      communication::Command("ccd_getSeed", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto speed = json_results.at("info").get<int>();

  return static_cast<ChargeCoupledDevice::Speed>(speed);
}

void ChargeCoupledDevice::set_speed(ChargeCoupledDevice::Speed speed) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setSpeed",
      {{"index", Device::device_id()}, {"token", static_cast<int>(speed)}}));
}

std::string ChargeCoupledDevice::get_fit_params() {
  auto response = Device::execute_command(communication::Command(
      "ccd_setFitParams", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto fit_params = json_results.at("params").get<std::string>();
  return fit_params;
}

void ChargeCoupledDevice::set_fit_params(std::string fit_params) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setFitParams",
      {{"index", Device::device_id()}, {"params", fit_params}}));
}

int ChargeCoupledDevice::get_timer_resolution() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getTimerResolution", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto timer_resolution = json_results.at("resolution").get<int>();
  return timer_resolution;
}

void ChargeCoupledDevice::set_timer_resolution(int timer_resolution) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setTimerResolution",
      {{"index", Device::device_id()}, {"resolution", timer_resolution}}));
}

void ChargeCoupledDevice::set_acquisition_format(
    int number_of_rois, AcquisitionFormat acquisition_format) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setAcqFormat", {{"index", Device::device_id()},
                           {"format", static_cast<int>(acquisition_format)},
                           {"numberOfRois", number_of_rois}}));
}

ChargeCoupledDevice::XAxisConversionType
ChargeCoupledDevice::get_x_axis_conversion_type() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getXAxisConversionType", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto x_axis_conversion_type = json_results.at("type").get<int>();

  return static_cast<ChargeCoupledDevice::XAxisConversionType>(
      x_axis_conversion_type);
}

void ChargeCoupledDevice::set_x_axis_conversion_type(
    ChargeCoupledDevice::XAxisConversionType conversion_type) {
  auto _ignored_response = Device::execute_command(
      communication::Command("ccd_setXAxisConversionType",
                             {{"index", Device::device_id()},
                              {"type", static_cast<int>(conversion_type)}}));
}

int ChargeCoupledDevice::get_acquisition_count() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getAcqCount", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto acquisition_count = json_results.at("count").get<int>();

  return acquisition_count;
}

void ChargeCoupledDevice::set_acquisition_count(int count) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setAcqCount", {{"index", Device::device_id()}, {"count", count}}));
}

std::string ChargeCoupledDevice::get_clean_count() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getAcqCount", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto acquisition_count = json_results.at("count").get<int>();
  auto acquisition_mode = json_results.at("mode").get<int>();

  std::stringstream stream;
  stream << "count: " << acquisition_count << " mode:" << acquisition_mode;

  return stream.str();
}
void ChargeCoupledDevice::set_clean_count(int count, CleanCountMode mode) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setCleanCount", {{"index", Device::device_id()},
                            {"count", count},
                            {"mode", static_cast<int>(mode)}}));
}

int ChargeCoupledDevice::get_data_size() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getDataSize", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto data_size = json_results.at("size").get<int>();

  return data_size;
}

double ChargeCoupledDevice::get_temperature() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getChipTemperature", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto temperature = json_results.at("temperature").get<double>();

  return temperature;
}

std::pair<int, int> ChargeCoupledDevice::get_chip_size() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getChipSize", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto x = json_results.at("x").get<int>();
  auto y = json_results.at("y").get<int>();

  return {x, y};
}

int ChargeCoupledDevice::get_exposure_time() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getExposureTime", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto time = json_results.at("time").get<int>();

  return time;
}

void ChargeCoupledDevice::set_exposure_time(int exposure_time_ms) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setExposureTime",
      {{"index", Device::device_id()}, {"time", exposure_time_ms}}));
}

bool ChargeCoupledDevice::get_acquisition_ready() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getAcquisitionReady", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto ready = json_results.at("time").get<bool>();

  return ready;
}

void ChargeCoupledDevice::set_acquisition_start(bool open_shutter) {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setAcquisitionStart",
      {{"index", Device::device_id()}, {"openShutter", open_shutter}}));
}

void ChargeCoupledDevice::set_region_of_interest(int roi_index, int x_origin,
                                                 int y_origin, int x_size,
                                                 int y_size, int x_bin,
                                                 int y_bin) {
  auto _ignored_response = Device::execute_command(
      communication::Command("ccd_setRoi", {{"index", Device::device_id()},
                                            {"roiIndex", roi_index},
                                            {"xOrigin", x_origin},
                                            {"yOrigin", y_origin},
                                            {"xSize", x_size},
                                            {"ySize", y_size},
                                            {"xBin", x_bin},
                                            {"yBin", y_bin}}));
}

std::vector<std::vector<int>> ChargeCoupledDevice::get_acquisition_data() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getAcquisitionData", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();

  std::vector<std::vector<int>> xy_data;
  for (const auto& elem : json_results["data"]["xyData"]) {
    std::vector<int> line;
    for (const auto& value : elem) {
      line.push_back(value);
    }
    xy_data.push_back(line);
  }

  return xy_data;
}

bool ChargeCoupledDevice::get_acquisition_busy() {
  auto response = Device::execute_command(communication::Command(
      "ccd_getAcquisitionBusy", {{"index", Device::device_id()}}));
  auto json_results = response.json_results();
  auto ready = json_results.at("isBusy").get<bool>();

  return ready;
}

void ChargeCoupledDevice::abort_acquisition() {
  auto _ignored_response = Device::execute_command(communication::Command(
      "ccd_setAcquisitionAbort", {{"index", Device::device_id()}}));
}
} /* namespace horiba::devices::single_devices */
