#ifndef RESPONSE_H
#define RESPONSE_H

#include <any>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

namespace horiba::communication {
class Response {
 public:
  Response(unsigned long long int id, std::string command,
           nlohmann::json results, std::vector<std::string> errors);
  nlohmann::json json_results() const;
  ~Response();

 private:
  unsigned long long int id;
  std::string command;
  nlohmann::json results;
  std::vector<std::string> errors;
};
} /* namespace horiba::communication */
#endif /* ifndef RESPONSE_H */
