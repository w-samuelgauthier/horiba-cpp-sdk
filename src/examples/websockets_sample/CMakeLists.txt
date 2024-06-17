add_executable(websockets main.cpp)

target_link_libraries(websockets PRIVATE horiba_cpp_sdk::horiba_cpp_sdk_options horiba_cpp_sdk::horiba_cpp_sdk_warnings
                                         Boost::beast)

target_link_system_libraries(
  websockets
  PRIVATE
  CLI11::CLI11
  spdlog::spdlog
  )

message("${CMAKE_BINARY_DIR}")
target_include_directories(websockets PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
