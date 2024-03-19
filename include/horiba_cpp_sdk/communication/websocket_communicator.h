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

/**
 * @brief Represents a communication channel with the ICL using a websocket.
 */
class WebSocketCommunicator : public Communicator {
 public:
  /**
   * @brief Constructs a communication channel with the ICL based on a host and
   * port.
   *
   * @param host The host to connect to
   * @param port The port to connect to
   */
  WebSocketCommunicator(std::string host, std::string port);

  ~WebSocketCommunicator() override;

  /**
   * @brief Opens the communication channel with the ICL
   */
  void open() override;

  /**
   * @brief Closes the communication channel with the ICL
   */
  void close() override;
  /**
   * @brief Checks if the communication channel is open or not.
   *
   * @return True if the communication channel is open, false otherwise
   */
  bool is_open() override;

  /**
   * @brief Sends a command to the ICL and returns the response
   *
   * @param command The command for the ICL
   *
   * @return The response from the ICL
   */
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
