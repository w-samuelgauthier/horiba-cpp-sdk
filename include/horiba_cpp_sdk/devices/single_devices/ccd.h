#ifndef CCD_H
#define CCD_H

#include <horiba_cpp_sdk/devices/single_devices/device.h>

#include <any>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "horiba_cpp_sdk/communication/communicator.h"

namespace horiba::devices::single_devices {
/**
 * @brief Represents a CCD camera
 *
 * This class should not be created directly by the end user. Instead a
 * DeviceManager should be used to access the detected CCDs on the system.
 *
 * TODO: write a basic description of the CCD
 *
 * The cells of the CCD can be binned and summed up.
 */
class ChargeCoupledDevice final : public Device {
 public:
  enum class AcquisitionFormat : int {
    SPECTRA,
    IMAGE,
    CROP,
    FAST_KINETICS,
  };

  enum class Gain : int {
    HIGH_LIGHT = 0,
    BEST_DYNAMIC_RANGE,
    HIGH_SENSITIVITY,
  };

  enum class Speed : int {
    SLOW_45_kHz = 0,
    MEDIUM_1_MHz,
    FAST_1_MHz_Ultra,
  };

  /**
   * @brief CCD X axis converstion type
   *
   * - None (default)
   * - CCD FIT parameters contained in the CCD firmware
   * - Mono Wavelength parameters contained in the icl_settings.ini file
   */
  enum class XAxisConversionType : int {
    NONE = 0,
    FROM_CCD_FIRMWARE,
    FROM_ICL_SETTINGS_INI
  };

  enum class CleanCountMode : int { MODE_1 = 0 };

  ChargeCoupledDevice(
      int id, std::shared_ptr<communication::Communicator> communicator);
  ~ChargeCoupledDevice() override = default;

  /**
   * @brief Opens the device.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void open() noexcept(false) override;

  /**
   * @brief Closes the device.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void close() noexcept(false) override;

  /**
   * @brief Checks if the connection to the CCD device is open.
   *
   * @return True if the connection is open, false otherwise.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  bool is_open() noexcept(false);

  /**
   * @brief Restarts the CCD.
   *
   * Note that this function only works if the camera has been opened before.
   * The connection to the camera stays open after the restart.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void restart() noexcept(false);

  /**
   * @brief Returns the configuration of the CCD.
   *
   * @return Configuration of the CCD
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  std::unordered_map<std::string, std::any> get_configuration() noexcept(false);

  /**
   * @brief Returns the number of averages of the CCD.
   *
   * @return Number of averages
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  int get_number_of_averages() noexcept(false);

  /**
   * @brief Sets the number of averages of the CCD.
   *
   * @param number_of_averages The number of averages
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_number_of_averages(int number_of_averages) noexcept(false);

  /**
   * @brief Returns the gain of the CCD.
   *
   * @return Gain of the CCD
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  Gain get_gain() noexcept(false);

  /**
   * @brief Sets the gain of the CCD
   *
   * @param gain Gain
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_gain(Gain gain) noexcept(false);

  /**
   * @brief Returns the speed of the CCD
   *
   * @return Speed
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  Speed get_speed() noexcept(false);

  /**
   * @brief Sets the speed of the CCD
   *
   * @param speed Speed in TODO: units and >details
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_speed(Speed speed) noexcept(false);

  /**
   * @brief Returns the fit parameters of the CCD
   *
   * @return Fit parameters TODO: units and >details
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  std::string get_fit_params() noexcept(false);

  /**
   * @brief Sets the fit parameters of the CCD
   *
   * @param fit_params Fit parameters TODO: units and >details
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_fit_params(std::string fit_params) noexcept(false);

  /**
   * @brief Returns the timer resolution of the CCD.
   *
   * @return Timer resolution TODO: units and >details
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  int get_timer_resolution() noexcept(false);

  /**
   * @brief Sets the timer resolution of the CCD
   *
   * @param timer_resolution Timer resolution TODO: units and >details
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_timer_resolution(int timer_resolution) noexcept(false);

  /**
   * @brief Sets the number of ROIs (Regions of Interest) or areas and the
   * acquisition format.
   *
   * After using this command to set the number of ROIs and format, the
   * ccd_setRoi command should be used to define each ROI.
   *
   * Note: The Crop (2) and Fast Kinetics (3) acquisition formats are not
   * supported by every CCD.
   *
   *
   * @param number_of_rois Number of regions of interest
   * @param acquisition_format Acquisition format
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_acquisition_format(
      int number_of_rois, AcquisitionFormat acquisition_format) noexcept(false);

  /**
   * @brief Gets the conversion type of the x axis.
   *
   * @return Conversion Type of the x axis
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  XAxisConversionType get_x_axis_conversion_type() noexcept(false);

  /**
   * @brief Sets the x-axis pixel conversion type to be used when retrieving
   * the acquisition data with thee.
   *
   * @param
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_x_axis_conversion_type(XAxisConversionType conversion_type) noexcept(
      false);

  /**
   * @brief Gets the number of acquisitions to be performed.
   *
   * The acquisition count is used to perform multiple acquisitions in a row.
   *
   * @return int The acquisition count.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  int get_acquisition_count() noexcept(false);

  /**
   * @brief Sets the number of acquisitions to be performed. The acquisition
   * count is used to perform multiple acquisitions in a row.
   *
   * @param count Number of acquisitions to be performed.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void set_acquisition_count(int count) noexcept(false);

  /**
   * @brief Gets the clean count mode of the CCD and the corresponding mode.
   *
   * @return std::string Clean count information (e.g., "count: 10 mode:
   * 0").
   *
   * @throws std::exception When an error occurs on the device side.
   */
  std::string get_clean_count() noexcept(false);

  /**
   * @brief Sets the clean count mode of the CCD and the corresponding mode.
   *
   * @param count Number of acquisitions to be performed.
   * @param mode Clean count mode
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void set_clean_count(int count, CleanCountMode mode) noexcept(false);

  /**
   * @brief Returns the size of the data from the CCD.
   *
   * @return int Size of the data. TODO: In which unit?
   *
   * @throws std::exception When an error occurs on the device side.
   */
  int get_data_size() noexcept(false);

  /**
   * @brief Returns the chip temperature of the CCD.
   *
   * @return double Chip temperature in degrees Celsius.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  double get_temperature() noexcept(false);

  /**
   * @brief Gets the chip resolution of the CCD.
   *
   * @return std::pair<int, int> Chip resolution (width, height).
   *
   * @throws std::exception When an error occurs on the device side.
   */
  std::pair<int, int> get_chip_size() noexcept(false);

  /**
   * @brief Returns the exposure time in milliseconds.
   *
   * @return int Exposure time in milliseconds.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  int get_exposure_time() noexcept(false);

  /**
   * @brief Sets the exposure time in milliseconds.
   *
   * @param exposure_time_ms Exposure time in milliseconds.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void set_exposure_time(int exposure_time_ms) noexcept(false);

  /**
   * @brief Returns true if the CCD is ready to acquire.
   *
   * @return bool True if the CCD is ready to acquire.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  bool get_acquisition_ready() noexcept(false);

  /**
   * @brief Starts the acquisition of the CCD.
   *
   * @param open_shutter Whether the shutter of the camera should be open.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void set_acquisition_start(bool open_shutter) noexcept(false);

  /**
   * @brief Sets the region of interest (ROI) of the CCD.
   *
   * @param roi_index Index of the region of interest. Defaults to 1.
   * @param x_origin X origin of the region of interest. Defaults to 0.
   * @param y_origin Y origin of the region of interest. Defaults to 0.
   * @param x_size X size of the region of interest. Defaults to 1024.
   * @param y_size Y size of the region of interest. Defaults to 256.
   * @param x_bin X bin of the region of interest. Defaults to 1.
   * @param y_bin Y bin of the region of interest. Defaults to 256.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void set_region_of_interest(int roi_index = 1, int x_origin = 0,
                              int y_origin = 0, int x_size = 1024,
                              int y_size = 256, int x_bin = 1,
                              int y_bin = 256) noexcept(false);

  /**
   * @brief Returns the acquisition data of the CCD.
   *
   * The data comes as a 1D array, where the the index represents the x axis and
   * the value the binned y sum.
   *
   * @return std::vector<int> array containing acquisition data
   * data.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  std::vector<int> get_acquisition_data() noexcept(false);

  /**
   * @brief Returns true if the CCD is busy with the acquisition.
   *
   * @return bool True if the CCD is busy.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  bool get_acquisition_busy() noexcept(false);

  /**
   * @brief Stops the acquisition of the CCD.
   *
   * @throws std::exception When an error occurs on the device side.
   */
  void abort_acquisition() noexcept(false);

 private:
};
} /* namespace horiba::devices::single_devices */
#endif /* ifndef CCD_H */
