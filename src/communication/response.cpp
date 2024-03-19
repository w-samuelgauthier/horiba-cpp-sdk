#include "horiba_cpp_sdk/communication/response.h"

#include <utility>

namespace horiba::communication {
Response::Response(unsigned long long int id, std::string command,
                   nlohmann::json results, std::vector<std::string> errors)
    : id{id},
      command{std::move(command)},
      results{std::move(results)},
      errors{std::move(errors)} {}

nlohmann::json Response::json_results() const { return this->results; }
} /* namespace horiba::communication */
