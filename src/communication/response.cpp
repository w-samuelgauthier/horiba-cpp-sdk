#include "horiba_cpp_sdk/communication/response.h"

namespace horiba::communication {
Response::Response(unsigned long long int id, std::string command,
                   nlohmann::json results, std::vector<std::string> errors)
    : id{id}, command{command}, results{results}, errors{errors} {}

nlohmann::json Response::json_results() const { return this->results; }
} /* namespace horiba::communication */
