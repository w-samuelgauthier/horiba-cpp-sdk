#include <catch2/catch_test_macros.hpp>

TEST_CASE("Constexpr tests", "[constexpr]") {
  STATIC_REQUIRE(1 == 1);
  /* STATIC_REQUIRE(factorial_constexpr(0) == 1); */
  /* STATIC_REQUIRE(factorial_constexpr(1) == 1); */
  /* STATIC_REQUIRE(factorial_constexpr(2) == 2); */
  /* STATIC_REQUIRE(factorial_constexpr(3) == 6); */
  /* STATIC_REQUIRE(factorial_constexpr(10) == 3628800); */
}
