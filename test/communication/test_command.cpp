#include <horiba_cpp_sdk/communication/command.h>

#include <catch2/catch_test_macros.hpp>

using namespace horiba_cpp_sdk::communication;

TEST_CASE("A Command id is always incremented", "[command]") {
  SECTION("Single threaded") {
    Command c1("icl_info", {});
    auto c1_json = c1.json();
    Command c2("icl_info", {});
    auto c2_json = c2.json();

    REQUIRE(c1["id"] != c2["id"]);
  }
}
