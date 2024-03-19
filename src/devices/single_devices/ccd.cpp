#include <horiba_cpp_sdk/devices/single_devices/ccd.h>

namespace horiba::devices::single_devices {

ChargeCoupledDevice::ChargeCoupledDevice(
    int id, std::shared_ptr<communication::Communicator> communicator)
    : id{id}, communicator{communicator} {}

bool ChargeCoupledDevice::is_open() {}

void ChargeCoupledDevice::restart() {}

std::unordered_map<std::string, std::any>
ChargeCoupledDevice::get_configuration() {
  return {};
}

int ChargeCoupledDevice::get_number_of_averages() { return 0; }
void ChargeCoupledDevice::set_number_of_averages(int number_of_averages) {}

ChargeCoupledDevice::Gain ChargeCoupledDevice::get_gain() {
  return ChargeCoupledDevice::Gain::HIGH_LIGHT;
}
void ChargeCoupledDevice::set_gain(ChargeCoupledDevice::Gain gain) {}

ChargeCoupledDevice::Speed ChargeCoupledDevice::get_speed() {
  return ChargeCoupledDevice::Speed::SLOW_45_kHz;
}
void ChargeCoupledDevice::set_speed(ChargeCoupledDevice::Speed speed) {}

std::string ChargeCoupledDevice::get_fit_params() { return ""; }
void ChargeCoupledDevice::set_fit_params(std::string fit_params) {}

int ChargeCoupledDevice::get_timer_resolution() { return 0; }
void ChargeCoupledDevice::set_timer_resolution(int timer_resolution) {}

void ChargeCoupledDevice::set_acquisition_format(
    int number_of_rois, AcquisitionFormat acquisition_format) {}

ChargeCoupledDevice::XAxisConversionType
ChargeCoupledDevice::get_x_axis_conversion_type() {
  return ChargeCoupledDevice::XAxisConversionType::NONE;
}
void ChargeCoupledDevice::set_x_axis_conversion_type(
    ChargeCoupledDevice::XAxisConversionType conversion_type) {}

int ChargeCoupledDevice::get_acquisition_count() { return 0; }
void ChargeCoupledDevice::set_acquisition_count(int count) {}

ChargeCoupledDevice::CleanCountMode ChargeCoupledDevice::get_clean_count() {
  return ChargeCoupledDevice::CleanCountMode::MODE_1;
}
void ChargeCoupledDevice::set_clean_count(CleanCountMode mode) {}

int ChargeCoupledDevice::get_data_size() { return 0; }
double ChargeCoupledDevice::get_temperature() { return 0.0; }
std::pair<int, int> ChargeCoupledDevice::get_chip_size() { return {}; }

int ChargeCoupledDevice::get_exposure_time() { return 0; }

void ChargeCoupledDevice::set_exposure_time(int exposure_time_ms) {}

bool ChargeCoupledDevice::get_acquisition_ready() { return false; }

bool ChargeCoupledDevice::set_acquisition_start(bool open_shutter) {
  return false;
}

void ChargeCoupledDevice::set_region_of_interest(int roi_index, int x_origin,
                                                 int y_origin, int x_size,
                                                 int y_size, int x_bin,
                                                 int y_bin) {}

std::string ChargeCoupledDevice::get_acquisition_data() { return ""; }

bool ChargeCoupledDevice::get_acquisition_busy() { return false; }

void ChargeCoupledDevice::abort_acquisition() {}
} /* namespace horiba::devices::single_devices */
