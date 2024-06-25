#include <horiba_cpp_sdk/communication/command.h>

#include <catch2/catch_test_macros.hpp>
#include <unordered_set>
#include <vector>

namespace horiba::test {
using namespace horiba::communication;
TEST_CASE("A Command id is always incremented", "[command]") {
  SECTION("Single threaded") {
    std::unordered_set<int> ids;

    for (int i = 0; i < 100; i++) {
      Command c1("icl_info", {});
      auto c1_json = c1.json();
      auto id = c1_json.at("id").get<int>();
      REQUIRE(!ids.contains(id));
      ids.insert(id);
    }
  }
}

TEST_CASE("Command parameters are correctly parsed", "[command]") {
  SECTION("From raw string") {
    // arrange
    std::string expect_raw_json =
        R"({"command":"test","id":100,"parameters":{"key1":1}})";

    // act
    Command command("test", {{"key1", 1}});
    auto command_json = command.json();

    // assert
    REQUIRE(command_json.dump() == expect_raw_json);
  }
}
}  // namespace horiba::test
