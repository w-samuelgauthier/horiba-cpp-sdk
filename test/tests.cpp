#include <spdlog/spdlog.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Enable spdlog", "[global]") {
  spdlog::set_level(spdlog::level::debug);
}
