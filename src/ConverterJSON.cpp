#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <filesystem>

nlohmann::json ConverterJSON::readJSON(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            if (filePath == CONFIG_PATH) {
                throw std::runtime_error("config file is missing");
            }
            throw std::runtime_error("Cannot open file: " + filePath);
        }
        
        nlohmann::json jsonData;
        file >> jsonData;
        
        if (filePath == CONFIG_PATH && jsonData.find("config") == jsonData.end()) {
            throw std::runtime_error("config file is empty");
        }
        
        return jsonData;
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error("JSON parsing error in file " + filePath + ": " + e.what());
    }
}

void ConverterJSON::writeJSON(const std::string& filePath, const nlohmann::json& jsonData) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }
    
    file << jsonData.dump(4);
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> documents;
    auto json = readJSON(CONFIG_PATH);
    
    if (json.find("files") == json.end()) {
        return documents;
    }
    
    for (const auto& filePath : json["files"]) {
        try {
            std::ifstream file(filePath.get<std::string>());
            if (file.is_open()) {
                std::string content;
                std::string line;
                while (std::getline(file, line)) {
                    content += line + "\n";
                }
                documents.push_back(content);
            } else {
                std::cerr << "Failed to open file: " << filePath << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error reading file " << filePath << ": " << e.what() << std::endl;
        }
    }
    
    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    auto json = readJSON(CONFIG_PATH);
    
    if (json["config"].find("max_responses") != json["config"].end()) {
        return json["config"]["max_responses"].get<int>();
    }
    
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> requests;
    try {
        auto json = readJSON(REQUESTS_PATH);
        
        if (json.find("requests") == json.end()) {
            return requests;
        }
        
        for (const auto& request : json["requests"]) {
            requests.push_back(request.get<std::string>());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading requests: " << e.what() << std::endl;
    }
    
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::json answersJson;
    answersJson["answers"] = nlohmann::json::object();
    
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string requestId = "request" + std::string(3 - std::to_string(i + 1).length(), '0') + std::to_string(i + 1);
        
        if (answers[i].empty()) {
            answersJson["answers"][requestId]["result"] = "false";
        } else {
            answersJson["answers"][requestId]["result"] = "true";
            
            if (answers[i].size() > 1) {
                nlohmann::json relevance = nlohmann::json::array();
                
                for (const auto& [docId, rank] : answers[i]) {
                    nlohmann::json docResult;
                    docResult["docid"] = docId;
                    docResult["rank"] = rank;
                    relevance.push_back(docResult);
                }
                
                answersJson["answers"][requestId]["relevance"] = relevance;
            } else {
                answersJson["answers"][requestId]["docid"] = answers[i][0].first;
                answersJson["answers"][requestId]["rank"] = answers[i][0].second;
            }
        }
    }
    
    writeJSON(ANSWERS_PATH, answersJson);
} 