#ifndef WEBSOCKET_COMMUNICATOR_H
#define WEBSOCKET_COMMUNICATOR_H

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>

#include "horiba_cpp_sdk/communication/communicator.h"

namespace horiba::communication {

class Command;
class Response;

class WebSocketCommunicator : public Communicator {
 public:
  WebSocketCommunicator(std::string host, std::string port);
  ~WebSocketCommunicator() override;

  void open() override;
  void close() override;
  bool is_open() override;

  Response request_with_response(Command command) override;

 private:
  std::string host;
  std::string port;
  boost::asio::io_context context;
  boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket{
      context};
};
} /* namespace horiba::communication */

#endif /* ifndef WEBSOCKET_COMMUNICATOR_H */
