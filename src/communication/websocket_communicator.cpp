#include "horiba_cpp_sdk/communication/websocket_communicator.h"

#include <spdlog/spdlog.h>

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/make_printable.hpp>
#include <exception>
#include <memory>
#include <stdexcept>

#include "horiba_cpp_sdk/communication/command.h"
#include "horiba_cpp_sdk/communication/response.h"

namespace horiba::communication {

WebSocketCommunicator::WebSocketCommunicator(std::string host, std::string port)
    : host{std::move(host)}, port{std::move(port)}, websocket_open{false} {}

WebSocketCommunicator::~WebSocketCommunicator() {}

void WebSocketCommunicator::open() {
  spdlog::debug("Opening WebSocket on {}:{}", this->host, this->port);
  boost::asio::ip::tcp::resolver resolver{this->context};

  auto const results = resolver.resolve(this->host, this->port);
  boost::asio::connect(this->websocket.next_layer(), results.begin(),
                       results.end());

  this->websocket.set_option(boost::beast::websocket::stream_base::decorator(
      [](boost::beast::websocket::request_type &req) {
        req.set(
            boost::beast::http::field::user_agent,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
      }));

  this->websocket.handshake(host, "/");

  spdlog::debug("WebSocket opened");
  this->websocket_open = true;
}

void WebSocketCommunicator::close() {
  if (!this->websocket_open) {
    spdlog::error("Failed to close WebSocket: not opened");
    throw new std::runtime_error("websocket is not open");
  }

  this->websocket.close(boost::beast::websocket::close_code::normal);
  this->websocket_open = false;
  spdlog::debug("WebSocket closed");
}

Response WebSocketCommunicator::request_with_response(Command command) {
  std::string json_command = command.json();
  spdlog::debug("Sending request: {}", json_command);
  this->websocket.write(boost::asio::buffer(json_command));

  boost::beast::flat_buffer buffer;
  this->websocket.read(buffer);

  nlohmann::json json_response =
      nlohmann::json::parse(boost::beast::buffers_to_string(buffer.data()));
  spdlog::debug("Received response: {}", json_response);
  /* unsigned long long int id = json_response["id"]; */
  /* std::string command_string = json_response["command"]; */
  /* nlohmann::json results = json_response["reslts"]; */
  /* std::vector<std::string> errors = json_response["errors"]; */

  return Response{json_response["id"], json_response["command"],
                  json_response["reslts"], json_response["errors"]};
}
} /* namespace horiba::communication */
