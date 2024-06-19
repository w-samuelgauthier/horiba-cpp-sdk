include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)

macro(horiba_cpp_sdk_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(horiba_cpp_sdk_setup_options)
  option(horiba_cpp_sdk_ENABLE_HARDENING "Enable hardening" ON)
  option(horiba_cpp_sdk_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    horiba_cpp_sdk_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    horiba_cpp_sdk_ENABLE_HARDENING
    OFF)

  horiba_cpp_sdk_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR horiba_cpp_sdk_PACKAGING_MAINTAINER_MODE)
    option(horiba_cpp_sdk_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(horiba_cpp_sdk_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(horiba_cpp_sdk_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(horiba_cpp_sdk_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(horiba_cpp_sdk_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(horiba_cpp_sdk_ENABLE_PCH "Enable precompiled headers" OFF)
    option(horiba_cpp_sdk_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(horiba_cpp_sdk_ENABLE_IPO "Enable IPO/LTO" ON)
    option(horiba_cpp_sdk_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(horiba_cpp_sdk_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(horiba_cpp_sdk_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(horiba_cpp_sdk_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(horiba_cpp_sdk_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(horiba_cpp_sdk_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(horiba_cpp_sdk_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(horiba_cpp_sdk_ENABLE_PCH "Enable precompiled headers" OFF)
    option(horiba_cpp_sdk_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      horiba_cpp_sdk_ENABLE_IPO
      horiba_cpp_sdk_WARNINGS_AS_ERRORS
      horiba_cpp_sdk_ENABLE_USER_LINKER
      horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS
      horiba_cpp_sdk_ENABLE_SANITIZER_LEAK
      horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED
      horiba_cpp_sdk_ENABLE_SANITIZER_THREAD
      horiba_cpp_sdk_ENABLE_SANITIZER_MEMORY
      horiba_cpp_sdk_ENABLE_UNITY_BUILD
      horiba_cpp_sdk_ENABLE_CLANG_TIDY
      horiba_cpp_sdk_ENABLE_CPPCHECK
      horiba_cpp_sdk_ENABLE_COVERAGE
      horiba_cpp_sdk_ENABLE_PCH
      horiba_cpp_sdk_ENABLE_CACHE)
  endif()

  horiba_cpp_sdk_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED
     AND (horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS
          OR horiba_cpp_sdk_ENABLE_SANITIZER_THREAD
          OR horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(horiba_cpp_sdk_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(horiba_cpp_sdk_global_options)
  if(horiba_cpp_sdk_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    horiba_cpp_sdk_enable_ipo()
  endif()

  horiba_cpp_sdk_supports_sanitizers()

  if(horiba_cpp_sdk_ENABLE_HARDENING AND horiba_cpp_sdk_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED
       OR horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS
       OR horiba_cpp_sdk_ENABLE_SANITIZER_THREAD
       OR horiba_cpp_sdk_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message(
      "${horiba_cpp_sdk_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED}")
    horiba_cpp_sdk_enable_hardening(horiba_cpp_sdk_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(horiba_cpp_sdk_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(horiba_cpp_sdk_warnings INTERFACE)
  add_library(horiba_cpp_sdk_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  horiba_cpp_sdk_set_project_warnings(
    horiba_cpp_sdk_warnings
    ${horiba_cpp_sdk_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(horiba_cpp_sdk_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(horiba_cpp_sdk_options)
  endif()

  include(cmake/Sanitizers.cmake)
  horiba_cpp_sdk_enable_sanitizers(
    horiba_cpp_sdk_options
    ${horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS}
    ${horiba_cpp_sdk_ENABLE_SANITIZER_LEAK}
    ${horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED}
    ${horiba_cpp_sdk_ENABLE_SANITIZER_THREAD}
    ${horiba_cpp_sdk_ENABLE_SANITIZER_MEMORY})

  set_target_properties(horiba_cpp_sdk_options PROPERTIES UNITY_BUILD ${horiba_cpp_sdk_ENABLE_UNITY_BUILD})

  if(horiba_cpp_sdk_ENABLE_PCH)
    target_precompile_headers(
      horiba_cpp_sdk_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(horiba_cpp_sdk_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    horiba_cpp_sdk_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(horiba_cpp_sdk_ENABLE_CLANG_TIDY)
    horiba_cpp_sdk_enable_clang_tidy(horiba_cpp_sdk_options ${horiba_cpp_sdk_WARNINGS_AS_ERRORS})
  endif()

  if(horiba_cpp_sdk_ENABLE_CPPCHECK)
    horiba_cpp_sdk_enable_cppcheck(${horiba_cpp_sdk_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(horiba_cpp_sdk_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    horiba_cpp_sdk_enable_coverage(horiba_cpp_sdk_options)
  endif()

  if(horiba_cpp_sdk_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(horiba_cpp_sdk_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(horiba_cpp_sdk_ENABLE_HARDENING AND NOT horiba_cpp_sdk_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN
       OR horiba_cpp_sdk_ENABLE_SANITIZER_UNDEFINED
       OR horiba_cpp_sdk_ENABLE_SANITIZER_ADDRESS
       OR horiba_cpp_sdk_ENABLE_SANITIZER_THREAD
       OR horiba_cpp_sdk_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    horiba_cpp_sdk_enable_hardening(horiba_cpp_sdk_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
