#include <horiba_cpp_sdk/communication/command.h>
#include <horiba_cpp_sdk/communication/response.h>
#include <horiba_cpp_sdk/communication/websocket_communicator.h>

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <nlohmann/json.hpp>

#include "../fake_icl_server.h"
#include "../icl_exe.h"

// Warning about getenv being unsafe, we don't care about it here
#pragma warning(disable : 4996)

namespace horiba::test {

using json = nlohmann::json;

TEST_CASE("WebSocket communicator test with fake ICL",
          "[websocket_communicator]") {
  // arrange
  horiba::communication::WebSocketCommunicator websocket_communicator(
      FakeICLServer::FAKE_ICL_ADDRESS,
      std::to_string(FakeICLServer::FAKE_ICL_PORT));

  SECTION("WebSocketCommunicator can be opened") {
    // act
    websocket_communicator.open();
    auto websocket_open = websocket_communicator.is_open();

    // assert
    REQUIRE(websocket_open == true);
  }

  SECTION("WebSocketCommunicator can be closed") {
    // act
    websocket_communicator.open();
    auto websocket_open = websocket_communicator.is_open();
    websocket_communicator.close();

    // assert
    REQUIRE(websocket_open == true);
    REQUIRE(websocket_communicator.is_open() == false);
  }

  SECTION("WebSocketCommunicator can send message") {
    // arrange
    websocket_communicator.open();
    const horiba::communication::Command command("test_command", {});

    // act
    // assert
    REQUIRE_NOTHROW([&]() {
      auto _response = websocket_communicator.request_with_response(command);
    });
  }

  SECTION("WebSocketCommunicator can receive response") {
    // arrange
    websocket_communicator.open();
    const std::string command_name = "test_command";
    const horiba::communication::Command command(command_name, {});

    // act
    const auto response = websocket_communicator.request_with_response(command);
    websocket_communicator.close();

    // assert
    REQUIRE(response.json_results().empty());
  }

  SECTION("WebSocketCommunicator can send and recieve multiple times") {
    // arrange
    websocket_communicator.open();
    const std::string command_name = "test_command";
    const horiba::communication::Command command(command_name, {});

    // act
    const size_t amount_requests = 10;
    std::vector<communication::Response> responses;
    for (size_t i = 0; i < amount_requests; i++) {
      responses.push_back(
          websocket_communicator.request_with_response(command));
    }
    websocket_communicator.close();

    // assert
    for (size_t i = 0; i < amount_requests; i++) {
      REQUIRE(responses[i].json_results().empty());
    }
  }

  SECTION("Already opened WebSocketCommunicator cannot be opened again") {
    // act
    websocket_communicator.open();

    // assert
    REQUIRE(websocket_communicator.is_open() == true);
    REQUIRE_THROWS(websocket_communicator.open());
    REQUIRE(websocket_communicator.is_open() == true);
  }

  SECTION("Already closed WebSocketCommunicator cannot be closed again") {
    // arrange
    websocket_communicator.open();

    // act
    websocket_communicator.close();

    // assert
    REQUIRE(websocket_communicator.is_open() == false);
    REQUIRE_THROWS(websocket_communicator.close());
    REQUIRE(websocket_communicator.is_open() == false);
  }

  if (websocket_communicator.is_open()) {
    websocket_communicator.close();
  }
}

TEST_CASE("WebSocket communicator test without fake ICL",
          "[websocket_communicator]") {
  horiba::communication::WebSocketCommunicator websocket_communicator(
      "127.0.0.1", "1111");

  REQUIRE_THROWS(websocket_communicator.open());

  REQUIRE_THROWS(websocket_communicator.close());
}

TEST_CASE_METHOD(ICLExe, "WebSocket communicator test with real ICL",
                 "[websocket_communicator]") {
  const char* has_hardware = std::getenv("HAS_HARDWARE");
  if (has_hardware == nullptr || std::string(has_hardware) == "0" ||
      std::string(has_hardware) == "false") {
    SUCCEED("Skipped: HAS_HARDWARE is not set");
    return;
  }

  start();

  // arrange
  horiba::communication::WebSocketCommunicator websocket_communicator(
      "127.0.0.1", std::to_string(25010));

  SECTION("WebSocketCommunicator can be opened") {
    // act
    websocket_communicator.open();
    auto websocket_open = websocket_communicator.is_open();

    // assert
    REQUIRE(websocket_open == true);
  }

  SECTION("WebSocketCommunicator can be closed") {
    // act
    websocket_communicator.open();
    auto websocket_open = websocket_communicator.is_open();
    websocket_communicator.close();

    // assert
    REQUIRE(websocket_open == true);
    REQUIRE(websocket_communicator.is_open() == false);
  }

  SECTION("WebSocketCommunicator can send message") {
    // arrange
    websocket_communicator.open();
    const horiba::communication::Command command("icl_info", {});

    // act
    // assert
    REQUIRE_NOTHROW([&]() {
      auto _response = websocket_communicator.request_with_response(command);
    });
  }

  SECTION("WebSocketCommunicator can receive response") {
    // arrange
    websocket_communicator.open();
    const std::string command_name = "icl_info";
    const horiba::communication::Command command(command_name, {});

    // act
    const auto response = websocket_communicator.request_with_response(command);
    websocket_communicator.close();

    // assert
    REQUIRE(!response.json_results().empty());
  }

  SECTION("WebSocketCommunicator can send and recieve multiple times") {
    // arrange
    websocket_communicator.open();
    const std::string command_name = "icl_info";
    const horiba::communication::Command command(command_name, {});

    // act
    const size_t amount_requests = 10;
    std::vector<communication::Response> responses;
    for (size_t i = 0; i < amount_requests; i++) {
      responses.push_back(
          websocket_communicator.request_with_response(command));
    }
    websocket_communicator.close();

    // assert
    for (size_t i = 0; i < amount_requests; i++) {
      REQUIRE(!responses[i].json_results().empty());
    }
  }

  SECTION("Already opened WebSocketCommunicator cannot be opened again") {
    // act
    websocket_communicator.open();

    // assert
    REQUIRE(websocket_communicator.is_open() == true);
    REQUIRE_THROWS(websocket_communicator.open());
    REQUIRE(websocket_communicator.is_open() == true);
  }

  SECTION("Already closed WebSocketCommunicator cannot be closed again") {
    // arrange
    websocket_communicator.open();

    // act
    websocket_communicator.close();

    // assert
    REQUIRE(websocket_communicator.is_open() == false);
    REQUIRE_THROWS(websocket_communicator.close());
    REQUIRE(websocket_communicator.is_open() == false);
  }

  if (websocket_communicator.is_open()) {
    websocket_communicator.close();
  }

  stop();
}
}  // namespace horiba::test
