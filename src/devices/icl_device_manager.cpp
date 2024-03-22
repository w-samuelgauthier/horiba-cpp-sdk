#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/ccds_discovery.h>
#include <horiba_cpp_sdk/devices/icl_device_manager.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

namespace horiba::devices {
ICLDeviceManager::ICLDeviceManager(std::string websocket_ip,
                                   std::string websocket_port, bool start_icl,
                                   bool enable_binary_messages)
    : websocket_ip{std::move(websocket_ip)},
      websocket_port{std::move(websocket_port)},
      start_icl{start_icl},
      enable_binary_messages{enable_binary_messages},
      communicator{
          std::make_shared<horiba::communication::WebSocketCommunicator>(
              websocket_ip, websocket_port)} {}

void ICLDeviceManager::start() {
  if (this->start_icl) {
    this->start_icl_process();
  }

  this->communicator->open();

  const communication::Response response =
      this->communicator->request_with_response(
          communication::Command("icl_info", {}));
  spdlog::debug("[ICLDeviceManager] ICL info: {}", response.json_results());

  if (this->enable_binary_messages) {
    this->enable_binary_messages_on_icl();
  }

  this->discover_devices();
}

void ICLDeviceManager::stop() {
  if (!this->communicator->is_open()) {
    this->communicator->open();
  }

  const auto _ignored_response = this->communicator->request_with_response(
      communication::Command("icl_shutdown", {}));

  if (this->communicator->is_open()) {
    this->communicator->close();
  }

  spdlog::debug("[ICLDeviceManager] ICL stopped");
}

void ICLDeviceManager::discover_devices(bool error_on_no_device) {
  ChargeCoupledDevicesDiscovery ccds_discovery =
      ChargeCoupledDevicesDiscovery(this->communicator);

  ccds_discovery.execute(error_on_no_device);
  this->ccds = ccds_discovery.charge_coupled_devices();

  /* MonochromatorsDiscovery monochromators_discovery = */
  /*     MonochromatorsDiscovery(this->communicator, this->icl_error_db); */

  /* monochromators_discovery.execute(error_on_no_device); */
  /* this->monos = monochromators_discovery.monochromators(); */
}

std::vector<std::shared_ptr<horiba::devices::single_devices::Monochromator>>
ICLDeviceManager::monochromators() const {
  return this->monos;
}

std::vector<
    std::shared_ptr<horiba::devices::single_devices::ChargeCoupledDevice>>
ICLDeviceManager::charge_coupled_devices() const {
  return this->ccds;
}

void ICLDeviceManager::start_icl_process() {
#ifndef _WIN32
  spdlog::debug(
      "[ICLDeviceManager] Only Windows is supported for the ICL software. "
      "Skipping starting of the icl process");
  return;
#endif

  spdlog::debug("[ICLDeviceManager] starting icl process");

  if (this->is_process_running("icl.exe")) {
    spdlog::info("[ICLDeviceManager] icl.exe is running, not starting it.");
    return;
  }

  const std::string icl_path =
      R"(C:\Program Files\HORIBA Scientific\SDK\icl.exe)";
  this->start_process(icl_path);
}

void ICLDeviceManager::enable_binary_messages_on_icl() {
  spdlog::debug("[ICLDeviceManager] enable binary messages on the ICL");

  const communication::Response response =
      this->communicator->request_with_response(
          communication::Command("icl_binMode", {"mode", "all"}));

  if (!response.errors().empty()) {
    for (auto& error : response.errors()) {
    }
    // TODO: add DB Error
  }
}

void ICLDeviceManager::start_process(const std::string& path) {
#ifdef _WIN32
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  const wchar_t* converted_path converter.from_bytes(path);
  SECURITY_ATTRIBUTES security_attributes;
  security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  security_attributes.bInheritHandle = TRUE;
  security_attributes.lpSecurityDescriptor = NULL;

  PROCESS_INFORMATION process_info;
  ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));

  if (!CreateProcess(NULL,                  // No module name (use command line)
                     converted_path,        // Command line
                     NULL,                  // Process handle not inheritable
                     NULL,                  // Thread handle not inheritable
                     TRUE,                  // Set handle inheritance to TRUE
                     0,                     // No creation flags
                     NULL,                  // Use parent's environment block
                     NULL,                  // Use parent's starting directory
                     &security_attributes,  // Pointer to security attributes
                     &process_info  // Pointer to PROCESS_INFORMATION structure
                     )) {
    spdlog::error("[ICLDeviceManager] Failed to start process. Error code: {}",
                  GetLastError());
    throw std::runtime_error("failed to start icl process");
  }

  // Close process and thread handles
  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);
#else
  throw std::runtime_error(
      "start_process unimplemented function under UNIX systems");
#endif
}

bool ICLDeviceManager::is_process_running(const std::string& process_name) {
#ifdef _WIN32
  HANDLE help_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (help_snapshot == INVALID_HANDLE_VALUE) {
    spdlog::error("[ICLDeviceManager] Failed to create snapshot");
    return false;
  }

  PROCESSENTRY32W process_entry;
  process_entry.dwSize = sizeof(PROCESSENTRY32W);

  if (!Process32FirstW(help_snapshot, &process_entry)) {
    CloseHandle(help_snapshot);
    logger::error("Failed to retrieve process information.");
    return false;
  }

  bool process_found = false;
  do {
    if (process_name == process_entry.szExeFile) {
      process_found = true;
      break;
    }
  } while (Process32NextW(help_snapshot, &process_entry));

  CloseHandle(help_snapshot);

  return process_found;
#else
  throw std::runtime_error(
      "is_process_running unimplemented function under UNIX systems");
  return false;
#endif
}

} /* namespace horiba::devices */
