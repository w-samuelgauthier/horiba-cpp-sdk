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
    : host{std::move(host)}, port{std::move(port)} {}

void WebSocketCommunicator::open() {
  if (this->is_open()) {
    spdlog::error("Failed to open WebSocket: already opened");
    throw std::runtime_error("websocket is already open");
  }

  spdlog::debug("Opening WebSocket on {}:{}", this->host, this->port);
  boost::asio::ip::tcp::resolver resolver{this->context};

  auto const results = resolver.resolve(this->host, this->port);
  auto endpoint = boost::asio::connect(this->websocket.next_layer(), results);

  this->websocket.set_option(boost::beast::websocket::stream_base::decorator(
      [](boost::beast::websocket::request_type& req) {
        req.set(
            boost::beast::http::field::user_agent,
            std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
      }));

  this->websocket.handshake(host + ':' + std::to_string(endpoint.port()), "/");

  spdlog::debug("WebSocket opened");
}

void WebSocketCommunicator::close() {
  if (!this->is_open()) {
    spdlog::error("Failed to close WebSocket: not opened");
    throw std::runtime_error("websocket is not open");
  }

  this->websocket.close(boost::beast::websocket::close_code::normal);
  spdlog::debug("WebSocket closed");
}

bool WebSocketCommunicator::is_open() { return this->websocket.is_open(); }

Response WebSocketCommunicator::request_with_response(const Command& command) {
  std::string json_command = command.json().dump();
  spdlog::debug("Sending request: {}", json_command);
  this->websocket.write(boost::asio::buffer(json_command));

  boost::beast::flat_buffer buffer;
  this->websocket.read(buffer);

  nlohmann::json json_response =
      nlohmann::json::parse(boost::beast::buffers_to_string(buffer.data()));
  spdlog::debug("Received response: {}", json_response.dump());

  return Response{json_response["id"], json_response["command"],
                  json_response["results"], json_response["errors"]};
}
} /* namespace horiba::communication */
