#ifndef FAKE_ICL_SERVER_H
#define FAKE_ICL_SERVER_H

#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <thread>

namespace horiba::test {

class FakeICLServer {
 public:
  static const int FAKE_ICL_PORT = 8765;
  static const std::string FAKE_ICL_ADDRESS;

  FakeICLServer() {
    spdlog::debug("FakeICLServer");

    spdlog::debug("load fake responses");
    this->load_fake_responses();

    server_thread = std::thread([this] {
      try {
        boost::asio::io_context ioc{1};

        boost::asio::ip::tcp::acceptor acceptor{
            ioc,
            {boost::asio::ip::make_address(FAKE_ICL_ADDRESS), FAKE_ICL_PORT}};

        boost::asio::ip::tcp::socket socket{ioc};

        spdlog::debug("blocking to accept new connection");
        acceptor.accept(socket);
        spdlog::debug("got new connection");

        std::thread([this, socket = std::move(socket)]() mutable {
          do_session(std::move(socket));
        }).detach();
        while (run_server) {
          std::this_thread::sleep_for(
              std::chrono::milliseconds(SERVER_SLEEP_TIME_MS));
        }
        spdlog::debug("server thread ending...");
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    });
  }

  ~FakeICLServer() {
    spdlog::debug("~FakeICLServer");
    if (server_thread.joinable()) {
      server_thread.join();
    }
  }

 private:
  void load_fake_responses() {
    std::string icl_json_file_path{
        std::filesystem::absolute("./test/fake_icl_responses/icl.json")
            .string()};
    if (!std::filesystem::exists(icl_json_file_path)) {
      spdlog::error("File '{}' does not exist", icl_json_file_path);
    }
    spdlog::debug("ICL json file path: {}", icl_json_file_path);
    std::ifstream icl_json_file(icl_json_file_path);
    this->icl_data = nlohmann::json::parse(icl_json_file);

    std::string ccd_json_file_path{
        std::filesystem::absolute("./test/fake_icl_responses/ccd.json")
            .string()};
    if (!std::filesystem::exists(ccd_json_file_path)) {
      spdlog::error("File '{}' does not exist", ccd_json_file_path);
    }
    spdlog::debug("CCD json file path: {}", ccd_json_file_path);
    std::ifstream ccd_json_file(ccd_json_file_path);
    this->ccd_data = nlohmann::json::parse(ccd_json_file);

    std::string mono_json_file_path{
        std::filesystem::absolute(
            "./test/fake_icl_responses/monochromator.json")
            .string()};
    if (!std::filesystem::exists(mono_json_file_path)) {
      spdlog::error("File '{}' does not exist", mono_json_file_path);
    }
    spdlog::debug("Monochromator json file path: {}", mono_json_file_path);
    std::ifstream mono_json_file(mono_json_file_path);
    this->mono_data = nlohmann::json::parse(mono_json_file);
  }

  void do_session(boost::asio::ip::tcp::socket socket) {
    try {
      spdlog::debug("do_session");
      boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket{
          std::move(socket)};

      websocket.set_option(boost::beast::websocket::stream_base::decorator(
          [](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-server-sync");
          }));

      websocket.accept();

      for (;;) {
        boost::beast::flat_buffer buffer;

        websocket.read(buffer);

        const nlohmann::json json_command_request = nlohmann::json::parse(
            boost::beast::buffers_to_string(buffer.data()));

        const std::string command =
            json_command_request["command"].get<std::string>();
        std::string response;
        if (command.compare(0, 4, "icl_") == 0) {
          response = this->icl_data[command].dump();
        } else if (command.compare(0, 4, "ccd_") == 0) {
          response = this->ccd_data[command].dump();
        } else if (command.compare(0, 4, "mono_") == 0) {
          response = this->mono_data[command].dump();
        } else {
          nlohmann::json generic_response = GENERIC_RESPONSE;
          generic_response["command"] = json_command_request["command"];
          generic_response["id"] = json_command_request["id"];
          response = generic_response.dump();
        }

        websocket.text(websocket.got_text());
        websocket.write(boost::asio::buffer(response));
      }
    } catch (boost::beast::system_error const& se) {
      if (se.code() != boost::beast::websocket::error::closed) {
        spdlog::error("Error: {}", se.code().message());
      }
      spdlog::debug("end of do_session");
      this->run_server = false;
    } catch (std::exception const& e) {
      spdlog::error("Error: {}", e.what());
      spdlog::debug("end of do_session");
    }
  }

  const nlohmann::json GENERIC_RESPONSE =
      R"({"command": "icl_info","errors": [],"id": 1234, "results":{}})"_json;
  const int SERVER_SLEEP_TIME_MS = 100;
  std::thread server_thread;
  std::atomic<bool> run_server{true};
  nlohmann::json icl_data;
  nlohmann::json ccd_data;
  nlohmann::json mono_data;
};

inline const std::string FakeICLServer::FAKE_ICL_ADDRESS = "127.0.0.1";

}  // namespace horiba::test
#endif /* ifndef FAKE_ICL_SERVER_H */
