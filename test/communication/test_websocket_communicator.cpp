/* #include <horiba_cpp_sdk/communication/command.h> */
/* #include <horiba_cpp_sdk/communication/communicator.h> */
#include <horiba_cpp_sdk/communication/websocket_communicator.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <catch2/catch_test_macros.hpp>
#include <thread>

/* namespace horiba::test { */

const int FAKE_ICL_PORT = 8765;

// Define a test fixture for WebSocket server
class FakeICLServer {
 public:
  FakeICLServer() {
    // Initialize the I/O context and acceptor
    m_ioContext = std::make_shared<boost::asio::io_context>();
    m_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*m_ioContext);

    // Set up the WebSocket server
    m_webSocketServer = std::make_shared<
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>(
        *m_ioContext);
    m_acceptor->open(boost::asio::ip::tcp::v4());
    m_acceptor->bind({boost::asio::ip::address_v4::loopback(), FAKE_ICL_PORT});
    m_acceptor->listen();
    m_acceptor->async_accept(
        m_webSocketServer->next_layer(),
        [&](const boost::system::error_code& ec) {
          if (!ec) {
            m_webSocketServer->async_accept(
                [this](const boost::system::error_code& ec) {
                  if (!ec) {
                    this->handleWebSocket();
                  }
                });
          }
        });

    // Run the I/O context asynchronously in a separate thread
    m_ioThread = std::thread([this]() { m_ioContext->run(); });
  }

  ~FakeICLServer() {
    // Stop the I/O context and join the thread
    m_ioContext->stop();
    m_ioThread.join();
  }

  void handleWebSocket() {
    // Handle WebSocket messages
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket>& ws =
        *m_webSocketServer;
    ws.async_read(m_buffer, [&](const boost::system::error_code& ec,
                                size_t bytes_transferred) {
      if (!ec) {
        ws.text(ws.got_text());
        ws.async_write(m_buffer.data(), [&](const boost::system::error_code& ec,
                                            size_t bytes_transferred) {
          if (!ec) {
            handleWebSocket();  // Continue reading WebSocket messages
          }
        });
      }
    });
  }

 private:
  std::shared_ptr<boost::asio::io_context> m_ioContext;
  std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
  std::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>
      m_webSocketServer;
  std::thread m_ioThread;
  boost::beast::multi_buffer m_buffer;
};

TEST_CASE_METHOD(FakeICLServer, "WebSocket communicator test",
                 "[websocket_communicator]") {
  horiba::communication::WebSocketCommunicator websocket_communicator(
      "localhost", "8765");
  /* horiba::communication::Command c("test_command", {}); */
}
/* }  // namespace horiba::test */
