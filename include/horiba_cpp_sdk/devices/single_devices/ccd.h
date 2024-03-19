#include <horiba_cpp_sdk/devices/single_devices/device.h>

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include "horiba_cpp_sdk/communication/communicator.h"

namespace horiba::devices::single_devices {
class ChargeCoupledDevice : public Device {
 public:
  enum class AcquisitionFormat {
    SPECTRA,
    IMAGE,
    CROP,
    FAST_KINETICS,
  };

  enum class Gain {
    HIGH_LIGHT,
    BEST_DYNAMIC_RANGE,
    HIGH_SENSITIVITY,
  };

  enum class Speed {
    SLOW_45_kHz,
    MEDIUM_1_MHz,
    FAST_1_MHz_Ultra,
  };

  enum class XAxisConversionType {
    NONE,
    FROM_CCD_FIRMWARE,
    FROM_ICL_SETTINGS_INI
  };

  enum class CleanCountMode { MODE_1 };

  ChargeCoupledDevice(
      int id, std::shared_ptr<communication::Communicator> communicator);
  ~ChargeCoupledDevice() override = default;

  bool is_open();

  void restart();

  std::unordered_map<std::string, std::any> get_configuration();

  int get_number_of_averages();
  void set_number_of_averages(int number_of_averages);

  Gain get_gain();
  void set_gain(Gain gain);

  Speed get_speed();
  void set_speed(Speed speed);

  std::string get_fit_params();
  void set_fit_params(std::string fit_params);

  int get_timer_resolution();
  void set_timer_resolution(int timer_resolution);

  void set_acquisition_format(int number_of_rois,
                              AcquisitionFormat acquisition_format);

  XAxisConversionType get_x_axis_conversion_type();
  void set_x_axis_conversion_type(XAxisConversionType conversion_type);

  int get_acquisition_count();
  void set_acquisition_count(int count);

  CleanCountMode get_clean_count();
  void set_clean_count(CleanCountMode mode);

  int get_data_size();
  double get_temperature();
  std::pair<int, int> get_chip_size();

  int get_exposure_time();
  void set_exposure_time(int exposure_time_ms);

  bool get_acquisition_ready();
  bool set_acquisition_start(bool open_shutter);

  void set_region_of_interest(int roi_index = 1, int x_origin = 0,
                              int y_origin = 0, int x_size = 1024,
                              int y_size = 256, int x_bin = 1, int y_bin = 256);

  std::string get_acquisition_data();
  bool get_acquisition_busy();
  void abort_acquisition();

 private:
  int id;
  std::shared_ptr<communication::Communicator> communicator;
};
} /* namespace horiba::devices::single_devices */
