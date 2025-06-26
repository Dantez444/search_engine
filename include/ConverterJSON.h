#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>


class ConverterJSON {
public:
    ConverterJSON() = default;
    

    std::vector<std::string> GetTextDocuments();
    

    int GetResponsesLimit();
    

    std::vector<std::string> GetRequests();
    

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
    const std::string CONFIG_PATH = "../config/config.json";
    const std::string REQUESTS_PATH = "../config/requests.json";
    const std::string ANSWERS_PATH = "../config/answers.json";
    
    nlohmann::json readJSON(const std::string& filePath);
    void writeJSON(const std::string& filePath, const nlohmann::json& jsonData);
}; 