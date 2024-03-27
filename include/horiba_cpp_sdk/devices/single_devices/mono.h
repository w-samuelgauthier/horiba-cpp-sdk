#ifndef MONO_H
#define MONO_H

#include <horiba_cpp_sdk/communication/communicator.h>
#include <horiba_cpp_sdk/devices/single_devices/device.h>

#include <string>

namespace horiba::devices::single_devices {

/**
 * @brief Represents a Monochromator
 * This class should not be created directly by the end user. Instead a
 * DeviceManager should be used to access the detected Monochromators on the
 * system.
 *
 */
class Monochromator final : public Device {
 public:
  Monochromator(int id,
                std::shared_ptr<communication::Communicator> communicator);
  ~Monochromator() override = default;

  /**
   * @brief Position of the shutter
   */
  enum class ShutterPosition : int { CLOSED, OPENED };

  /**
   * @brief Gratings installed in the monochromator
   */
  enum class Grating : int { FIRST, SECOND, THIRD };

  /**
   * @brief Positions of the filter wheel installed in the monochromator.
   * TODO : clarify naming of filter wheel positions
   */
  enum class FilterWheelPosition : int { RED, GREEN, BLUE, YELLOW };

  /**
   * @brief Mirrors installed in the monochromator
   * TODO : clarify how the mirrors are called
   */
  enum class Mirror : int { FIRST, SECOND };

  /**
   * @brief Possible positions of the mirrors
   * TODO : clarify what possible position there are
   */
  enum class MirrorPosition : int { A, B };

  /**
   * @brief Slits available on the monochromator.
   * TODO : clarify how the slits are called
   */
  enum class Slit : int { A, B, C, D };

  /**
   * @brief Slits steps available on the monochromator.
   * TODO : clarify how the slits are called
   */
  enum class SlitStepPosition : int { A, B, C, D };

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
   * @brief Checks if the connection to the monochromator is open.
   *
   * @return True if the connection is open, false otherwise
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  bool is_open() noexcept(false);

  /**
   * @brief Checks if the monochromator is busy.
   *
   * @return True if busy, false otherwise
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  bool is_busy() noexcept(false);

  /**
   * @brief Starts the monochromator initialization process called "homing".
   *
   * Use is_busy() to know if the operation is still taking place.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void home() noexcept(false);

  /**
   * @brief Returns the configuration of the monochromator.
   *
   * @return Configuration of the monochromator
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  std::string configuration() noexcept(false);

  /**
   * @brief Current wavelength of the monochromator's position in nm.
   *
   * @return The current wavelength in nm
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  float get_current_wavelength() noexcept(false);

  /**
   * @brief This command sets the wavelength value of the current grating
   * position of the monochromator.
   *
   * Warning! This could potentially un-calibrate the monochromator and report
   * an incorrect wavelength compared to the actual output wavelength.
   *
   * @param wavelength Wavelength in nm
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void calibrate_wavelength(float wavelength) noexcept(false);

  /**
   * @brief Orders the monochromator to move to the requested wavelength.
   *
   * Use is_busy() to know if the operation is still taking place.
   *
   * @param wavelength Wavelength in nm
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void move_to_target_wavelength(float wavelength) noexcept(false);

  /**
   * @brief Current grating of the turret
   *
   * @return Current grating of turret, see Monochromator::Grating enum for
   * possible values
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  Grating get_turret_grating() noexcept(false);

  /**
   * @brief Select turret grating
   *
   * @param grating New grating of the turret. See Monochromator::Grating enum
   * for possible values.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_turret_grating(Grating grating) noexcept(false);

  /**
   * @brief Current position of the filter wheel.
   *
   * TODO: refactor in case there can be more than one filter wheel. What should
   * be done if no filter wheel is installed ?
   *
   * @return current position of the filter wheel. See
   * Monochromator::FilterWheelPosition enum for possible values.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  FilterWheelPosition get_filter_wheel_position() noexcept(false);

  /**
   * @brief Sets the current position of the filter wheel.
   *
   * @param position New position of the filter wheel. See
   * Monochromator::FilterWheelPosition enum for possible values.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_filter_wheel_position(FilterWheelPosition position) noexcept(false);

  /**
   * @brief Position of the selected mirror
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param mirror Desired mirror to get the position from. See
   * Monochromator::Mirror for possible values.
   *
   * @return Current mirror position. See Monochromator::MirrorPosition for
   * possible values
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  MirrorPosition get_mirror_position(Mirror mirror) noexcept(false);

  /**
   * @brief Sets the position of the selected mirror
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param mirror Desired mirror to set the position. See Monochromator::Mirror
   * @param position Position to set. See Monochromator::MirrorPosition
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_mirror_position(Mirror mirror,
                           MirrorPosition position) noexcept(false);

  /**
   * @brief Returns the position in millimeters[mm] of the selected slit.
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param slit Desired slit to get the position from. See Monochromator::Slit
   * for possible values
   *
   * @return Position in mm
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  float get_slit_position_in_mm(Slit slit) noexcept(false);

  /**
   * @brief Sets the position of the selected slit.
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param slit Desired slit to set the position. See Monochromator::Slit for
   * possible values. position Position to set in millimeters[mm].
   * @param position_in_mm Position in mm
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_slit_position(Slit slit, float position_in_mm) noexcept(false);

  /**
   * @brief Returns the step position of the selected slit.
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param slit Desired slit to get the position from. See Monochromator::Slit
   * for possible values
   *
   * @return Returns SlitStepPosition step position. See
   * Monochromator::SlitStepPosition for possible values
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  SlitStepPosition get_slit_step_position(Slit slit) noexcept(false);

  /**
   * @brief Sets the step position of the selected slit.
   *
   * TODO: Get more information about possible values and explain elements
   * contained in monochromator at top of this class.
   *
   * @param slit Desired slit to set the step position. See Monochromator::Slit
   * for possible values.
   * @param step_position The step position. See Monochromator::SlitStepPosition
   * for possible values
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void set_slit_step_position(Slit slit,
                              SlitStepPosition step_position) noexcept(false);

  /**
   * @brief Opens the shutter.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void open_shutter() noexcept(false);

  /**
   * @brief Closes the shutter.
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  void close_shutter() noexcept(false);

  /**
   * @brief Returns the shutter position.
   *
   * @return ShutterPosition::OPEN or ShutterPosition::CLOSED
   *
   * @throw std::runtime_error when an error occurred on the device side
   */
  ShutterPosition get_shutter_position() noexcept(false);
};
}  // namespace horiba::devices::single_devices
#endif /* ifndef MONO_H */
