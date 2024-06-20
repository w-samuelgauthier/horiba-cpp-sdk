add_executable(center_scan main.cpp)

target_link_libraries(center_scan PRIVATE
  horiba_cpp_sdk::horiba_cpp_sdk_options
  horiba_cpp_sdk::horiba_cpp_sdk_warnings
  horiba_cpp_sdk::horiba_cpp_sdk
  nlohmann_json::nlohmann_json
  Matplot++::matplot
  Boost::beast
)

target_include_directories(center_scan PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

