#ifndef RESPONSE_H
#define RESPONSE_H

#include <any>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace horiba::communication {
/**
 * @brief Represents a response of the ICL
 */
class Response {
 public:
  /**
   * @brief Builds a response of the ICL.
   *
   * @param id The id of the sent command
   * @param command The sent command
   * @param results The results, if any, of the command
   * @param errors The errors, if any, of the command
   */
  Response(unsigned long long int id, std::string command,
           nlohmann::json::object_t results, std::vector<std::string> errors);

  /**
   * @brief JSON representation of the "results" field of the response.
   *
   * @return JSON of response["results"]
   */
  [[nodiscard]] nlohmann::json json_results() const;

  /**
   * @brief Errors, if any, from the ICL.
   *
   * @return Errors happened during the call to the ICL
   */
  [[nodiscard]] std::vector<std::string> errors() const;

 private:
  unsigned long long int id;
  std::string command;
  nlohmann::json::object_t results;
  std::vector<std::string> icl_errors;
};
} /* namespace horiba::communication */
#endif /* ifndef RESPONSE_H */
