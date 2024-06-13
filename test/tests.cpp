#define CATCH_CONFIG_RUNNER  // This allows you to write your own main()
#include <spdlog/spdlog.h>

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <iostream>

#include "start_fake_icl_server_listener.h"

CATCH_REGISTER_LISTENER(horiba::test::StartFakeICLServerListener)

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::debug);
  using namespace Catch::Clara;
  Catch::Session session;

  auto cli =
      session.cli() |
      Opt(horiba::test::StartFakeICLServerListener::fake_responses_folder_path,
          "data")["-z"]["--fake_responses_folder_path"]("Data for the test");
  session.cli(cli);

  int return_code = session.applyCommandLine(argc, argv);
  if (return_code != 0) {
    return return_code;
  }

  return session.run();
}
