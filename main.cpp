#include <iostream>
#include <exception>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    try {
        ConverterJSON converter;
        

        try {
            int responsesLimit = converter.GetResponsesLimit();
            std::cout << "Max responses: " << responsesLimit << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error getting responses limit: " << e.what() << std::endl;
        }
        

        std::vector<std::string> requests;
        try {
            requests = converter.GetRequests();
            std::cout << "Total requests: " << requests.size() << std::endl;
            for (size_t i = 0; i < requests.size(); ++i) {
                std::cout << "Request " << i + 1 << ": " << requests[i] << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error getting requests: " << e.what() << std::endl;
        }
        

        std::vector<std::string> docs;
        try {
            docs = converter.GetTextDocuments();
            std::cout << "Total documents: " << docs.size() << std::endl;
            for (size_t i = 0; i < docs.size(); ++i) {
                std::cout << "Document " << i << " size: " << docs[i].size() << " bytes" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error getting documents: " << e.what() << std::endl;
        }
        

        InvertedIndex index;
        index.UpdateDocumentBase(docs);
        

        SearchServer server(index);
        auto search_results = server.search(requests);
        

        std::vector<std::vector<std::pair<int, float>>> answers;
        for (const auto& result : search_results) {
            std::vector<std::pair<int, float>> answer;
            for (const auto& item : result) {
                answer.push_back({item.doc_id, item.rank});
            }
            answers.push_back(answer);
        }
        

        try {
            converter.putAnswers(answers);
            std::cout << "Answers successfully written" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error writing answers: " << e.what() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }
    

    return RUN_ALL_TESTS();
}