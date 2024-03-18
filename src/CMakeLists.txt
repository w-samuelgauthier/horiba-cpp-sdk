include(GenerateExportHeader)

set(HORIBA_CPP_LIB_SOURCES
  communication/command.cpp
  communication/response.cpp
  communication/websocket_communicator.cpp
  )

add_library(horiba_cpp_sdk ${HORIBA_CPP_LIB_SOURCES})

add_library(horiba_cpp_sdk::horiba_cpp_sdk ALIAS horiba_cpp_sdk)

target_link_libraries(horiba_cpp_sdk PRIVATE
  horiba_cpp_sdk_options
  horiba_cpp_sdk_warnings
  Boost::beast
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  )

target_include_directories(horiba_cpp_sdk ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(horiba_cpp_sdk PUBLIC cxx_std_20)

set_target_properties(
  horiba_cpp_sdk
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(horiba_cpp_sdk EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/horiba_cpp_sdk/horiba_cpp_sdk_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(horiba_cpp_sdk PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()


add_subdirectory(sample_library)
add_subdirectory(examples)