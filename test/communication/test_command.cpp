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
}  // namespace horiba::test
