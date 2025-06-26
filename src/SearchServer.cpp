#include "SearchServer.h"
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

std::vector<std::string> SearchServer::parseQuery(const std::string& query) {
    std::istringstream iss(query);
    std::string word;
    std::set<std::string> unique_words;
    
    while (iss >> word) {
        std::transform(word.begin(), word.end(), word.begin(), 
                     [](unsigned char c){ return std::tolower(c); });
        
        unique_words.insert(word);
    }
    
    std::vector<std::string> result(unique_words.begin(), unique_words.end());
    
    std::sort(result.begin(), result.end(), [this](const std::string& a, const std::string& b) {
        return _index.GetWordCount(a).size() < _index.GetWordCount(b).size();
    });
    
    return result;
}

std::vector<RelativeIndex> SearchServer::calculateRelevance(const std::vector<std::string>& query_words) {
    if (query_words.empty()) {
        return {};
    }
    
    std::set<size_t> all_docs;
    for (const auto& word : query_words) {
        auto entries = _index.GetWordCount(word);
        for (const auto& entry : entries) {
            all_docs.insert(entry.doc_id);
        }
    }
    
    if (all_docs.empty()) {
        return {};
    }
    
    std::map<size_t, float> absolute_relevance;
    for (const auto& doc_id : all_docs) {
        float relevance = 0.0f;
        
        for (const auto& word : query_words) {
            for (const auto& entry : _index.GetWordCount(word)) {
                if (entry.doc_id == doc_id) {
                    relevance += entry.count;
                    break;
                }
            }
        }
        
        absolute_relevance[doc_id] = relevance;
    }
    
    float max_relevance = 0.0f;
    for (const auto& [doc_id, relevance] : absolute_relevance) {
        max_relevance = std::max(max_relevance, relevance);
    }
    
    std::vector<RelativeIndex> result;
    for (const auto& [doc_id, relevance] : absolute_relevance) {
        if (relevance > 0) {
            result.push_back({doc_id, relevance / max_relevance});
        }
    }
    
    std::sort(result.begin(), result.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
        return a.rank > b.rank || (a.rank == b.rank && a.doc_id < b.doc_id);
    });
    
    if (result.size() > 5) {
        result.resize(5);
    }
    
    return result;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> search_results;
    
    for (const auto& query : queries_input) {
        std::vector<std::string> query_words = parseQuery(query);
        
        std::vector<RelativeIndex> relevance = calculateRelevance(query_words);
        
        search_results.push_back(relevance);
    }
    
    return search_results;
} 