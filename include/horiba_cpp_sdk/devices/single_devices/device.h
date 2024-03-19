namespace horiba::devices::single_devices {
/**
 * @brief Interface for devices connected to the ICL
 */
class Device {
 public:
  virtual ~Device() = default;

  /**
   * @brief Opens the device.
   */
  virtual void open() = 0;

  /**
   * @brief Closes the device.
   */
  virtual void close() = 0;
};
} /* namespace horiba::devices::single_devices */
