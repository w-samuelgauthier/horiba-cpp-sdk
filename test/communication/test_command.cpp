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
  SECTION("From raw string with zero parameters") {
    // arrange
    // act
    Command command("test", {});
    auto command_json = command.json();
    auto parameters = command_json.at("parameters");

    // assert
    REQUIRE(parameters.is_null());
  }

  SECTION("From raw string with one parameter") {
    // arrange
    std::string expect_raw_json = R"({"key1":1})";

    // act
    Command command("test", {{"key1", 1}});
    auto command_json = command.json();
    auto parameters = command_json.at("parameters");

    // assert
    REQUIRE(parameters.dump() == expect_raw_json);
  }

  SECTION("From raw string with multiple parameters") {
    // arrange
    std::string expect_raw_json = R"({"key1":1,"key2":2})";

    // act
    Command command("test", {{"key1", 1}, {"key2", 2}});
    auto command_json = command.json();
    auto parameters = command_json.at("parameters");

    // assert
    REQUIRE(parameters.dump() == expect_raw_json);
  }
}
}  // namespace horiba::test
