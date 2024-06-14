#ifndef START_FAKE_ICL_SERVER_LISTENER
#define START_FAKE_ICL_SERVER_LISTENER
#include <catch2/catch_test_case_info.hpp>
#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <memory>
#include <string>

#include "fake_icl_server.h"

namespace horiba::test {

template <typename T>
constexpr void UNUSED_PARAMETER(const T&) noexcept {}

std::shared_ptr<FakeICLServer> fake_icl_fixture = nullptr;

class StartFakeICLServerListener : public Catch::EventListenerBase {
 public:
  using Catch::EventListenerBase::EventListenerBase;

  void testRunStarting(Catch::TestRunInfo const& testRunInfo) override {
    [[maybe_unused]] auto unused = testRunInfo;
    fake_icl_fixture =
        std::make_shared<FakeICLServer>(fake_responses_folder_path);
  }

  void testRunEnded(Catch::TestRunStats const& testRunStats) override {
    [[maybe_unused]] auto unused = testRunStats;
    fake_icl_fixture.reset();
  }

  static std::string fake_responses_folder_path;
};
std::string StartFakeICLServerListener::fake_responses_folder_path =
    "./fake_icl_responses/";
} /* namespace horiba::test */
#endif /* ifndef START_FAKE_ICL_SERVER_LISTENER */
