#include <spdlog/spdlog.h>

#include <catch2/catch_test_macros.hpp>
#include <horiba_cpp_sdk/sample_library.hpp>

TEST_CASE("Enable spdlog", "[global]") {
  spdlog::set_level(spdlog::level::debug);
}

TEST_CASE("Factorials are computed", "[factorial]") {
  REQUIRE(factorial(0) == 1);
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3628800);
}
