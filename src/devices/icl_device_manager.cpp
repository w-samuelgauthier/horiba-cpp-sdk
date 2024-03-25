#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>
#include <horiba_cpp_sdk/devices/ccds_discovery.h>
#include <horiba_cpp_sdk/devices/icl_device_manager.h>
#include <horiba_cpp_sdk/devices/single_devices/ccd.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <errhandlingapi.h>
#include <handleapi.h>
#include <processthreadsapi.h>
#include <stringapiset.h>
#include <tlhelp32.h>

#include <cwchar>

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

/* std::vector<std::shared_ptr<horiba::devices::single_devices::Monochromator>>
 */
/* ICLDeviceManager::monochromators() const { */
/*   return this->monos; */
/* } */

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
      spdlog::error(error);
    }
    // TODO: add DB Error
  }
}

void ICLDeviceManager::start_process(const std::string& path) {
#ifdef _WIN32
  std::wstring converted_path = this->convert_to_wstring(path);

  STARTUPINFO startup_info;
  PROCESS_INFORMATION process_info;

  ZeroMemory(&startup_info, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);
  ZeroMemory(&process_info, sizeof(process_info));

  if (!CreateProcess(nullptr,  // No module name (use command line)
                     converted_path.c_str(),  // Command line
                     nullptr,                 // Process handle not inheritable
                     nullptr,                 // Thread handle not inheritable
                     TRUE,                    // Set handle inheritance to TRUE
                     0,                       // No creation flags
                     nullptr,                 // Use parent's environment block
                     nullptr,                 // Use parent's starting directory
                     &startup_info,  // Pointer to STARTUPINFO structure
                     &process_info   // Pointer to PROCESS_INFORMATION structure
                     )) {
    spdlog::error("[ICLDeviceManager] Failed to start process. Error code: {}",
                  GetLastError());
    throw std::runtime_error("failed to start icl process");
  }

  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);
#else
  throw std::runtime_error("start_process (" + path +
                           ") function unimplemented under UNIX systems");
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
    spdlog::error("Failed to retrieve process information.");
    return false;
  }

  std::wstring converted_path = this->convert_to_wstring(process_name);
  bool process_found = false;
  do {
    if (std::wcscmp(converted_path.data(), process_entry.szExeFile) == 0) {
      process_found = true;
      break;
    }
  } while (Process32NextW(help_snapshot, &process_entry) != 0);

  CloseHandle(help_snapshot);

  return process_found;
#else
  throw std::runtime_error("is_process_running( " + process_name +
                           " ) function unimplemented under UNIX systems");
#endif
}

std::wstring ICLDeviceManager::convert_to_wstring(const std::string& s) {
#ifdef _WIN32
  const int size_needed = MultiByteToWideChar(
      CP_UTF8, 0, s.c_str(), static_cast<int>(s.length()), nullptr, 0);
  if (size_needed == 0) {
    spdlog::error(
        "[ICLDeviceManager] Failed to get needed size for string conversion."
        "Error code: {}",
        GetLastError());
    throw std::runtime_error(
        "failed to get needed size for string conversion.");
  }

  std::wstring converted_string(size_needed, 0);
  const int chars_written =
      MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.length()),
                          &converted_string[0], size_needed);
  if (chars_written == 0) {
    spdlog::error("[ICLDeviceManager] Failed to convert string. Error code: {}",
                  GetLastError());
    throw std::runtime_error("failed to convert path.");
  }
  return converted_string;
#else
  throw std::runtime_error("convert_to_string( " + s +
                           " ) function unimplemented under UNIX systems");
#endif
}

} /* namespace horiba::devices */
