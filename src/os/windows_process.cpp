#include <Windows.h>
#include <errhandlingapi.h>
#include <handleapi.h>
#include <horiba_cpp_sdk/os/windows_process.h>
#include <processthreadsapi.h>
#include <spdlog/spdlog.h>
#include <stringapiset.h>
#include <tlhelp32.h>

#include <cwchar>
#include <stdexcept>

#include "spdlog/common.h"

namespace horiba::os {
WindowsProcess::WindowsProcess(std::string process_path,
                               std::string process_name)
    : process_path{std::move(process_path)},
      process_name{std::move(process_name)} {}

void WindowsProcess::start() {
  if (this->running()) {
    spdlog::info("[WindowsProcess] '{}' is running, not starting it.",
                 this->process_name);
    return;
  }
  spdlog::info("[WindowsProcess] '{}' is not running, starting it.",
               this->process_name);
  auto full_path = this->process_path + this->process_name;
  auto converted_path = const_cast<LPSTR>(full_path.c_str());

  STARTUPINFO startup_info;
  PROCESS_INFORMATION process_info;

  ZeroMemory(&startup_info, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);
  ZeroMemory(&process_info, sizeof(process_info));

  spdlog::debug("[WindowsProcess] Starting process: {}", full_path);
  if (!CreateProcess(nullptr,         // No module name (use command line)
                     converted_path,  // Command line
                     nullptr,         // Process handle not inheritable
                     nullptr,         // Thread handle not inheritable
                     1,               // Set handle inheritance to TRUE
                     0,               // No creation flags
                     nullptr,         // Use parent's environment block
                     nullptr,         // Use parent's starting directory
                     &startup_info,   // Pointer to STARTUPINFO structure
                     &process_info  // Pointer to PROCESS_INFORMATION structure
                     )) {
    spdlog::error("[WindowsProcess] Failed to start process. Error code: {}",
                  GetLastError());
    throw std::runtime_error("failed to start '" + full_path + "' process");
  }
  spdlog::debug("[WindowsProcess] Process started: {}", full_path);

  CloseHandle(process_info.hProcess);
  CloseHandle(process_info.hThread);
}

bool WindowsProcess::running() {
  HANDLE help_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (help_snapshot == INVALID_HANDLE_VALUE) {
    spdlog::error("[WindowsProcess] Failed to create snapshot: {}",
                  GetLastError());
    throw std::runtime_error("failed to create snapshot");
  }

  PROCESSENTRY32W process_entry;
  process_entry.dwSize = sizeof(PROCESSENTRY32W);

  auto result = Process32FirstW(help_snapshot, &process_entry);
  if (result != 1) {
    CloseHandle(help_snapshot);
    spdlog::error(
        "[WindowsProcess] Failed to retrieve process information: {}, {}",
        GetLastError(), result);
    throw std::runtime_error("failed to retrieve process information");
  }

  std::wstring wide_process_name =
      std::wstring(this->process_name.begin(), this->process_name.end());
  const wchar_t* converted_path = wide_process_name.c_str();

  spdlog::debug("[WindowsProcess] searching for: {}", this->process_name);
  bool process_found = false;
  do {
    if (std::wcscmp(converted_path, process_entry.szExeFile) == 0) {
      process_found = true;
      break;
    }
  } while (Process32NextW(help_snapshot, &process_entry) == 1);

  CloseHandle(help_snapshot);

  spdlog::debug("[WindowsProcess] '{}' is running: {}", this->process_name,
                process_found);

  return process_found;
}

void WindowsProcess::stop() {
  if (!this->running()) {
    spdlog::info("[WindowsProcess] '{}' is not running, not stopping it.",
                 this->process_name);
    return;
  }

  spdlog::info("[WindowsProcess] '{}' is running, stopping it.",
               this->process_name);

  HANDLE help_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (help_snapshot == INVALID_HANDLE_VALUE) {
    spdlog::error("[WindowsProcess] Failed to create snapshot");
    throw std::runtime_error("failed to create snapshot");
  }

  PROCESSENTRY32W process_entry;
  process_entry.dwSize = sizeof(PROCESSENTRY32W);

  auto result = Process32FirstW(help_snapshot, &process_entry);
  if (result != 1) {
    CloseHandle(help_snapshot);
    spdlog::error(
        "[WindowsProcess] Failed to retrieve process information: {}, {}",
        GetLastError(), result);
    throw std::runtime_error("failed to retrieve process information");
  }

  std::wstring wide_process_name =
      std::wstring(this->process_name.begin(), this->process_name.end());
  const wchar_t* converted_path = wide_process_name.c_str();

  bool process_found = false;
  DWORD process_id = 0;
  do {
    if (std::wcscmp(converted_path, process_entry.szExeFile) == 0) {
      process_found = true;
      process_id = process_entry.th32ProcessID;
      break;
    }
  } while (Process32NextW(help_snapshot, &process_entry) == 1);

  CloseHandle(help_snapshot);

  if (!process_found) {
    spdlog::error("[WindowsProcess] Process is not running");
    throw std::runtime_error("process is not running");
  }

  const auto icl_exe = OpenProcess(PROCESS_TERMINATE, false, process_id);
  TerminateProcess(icl_exe, 1);
  CloseHandle(icl_exe);
}

} /* namespace horiba::os */
