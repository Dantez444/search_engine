#pragma once

#include "InvertedIndex.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <set>

struct RelativeIndex {
    size_t doc_id;
    float rank;
    
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex& idx) : _index(idx) {};
    
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;
    
    std::vector<std::string> parseQuery(const std::string& query);
    
    std::vector<RelativeIndex> calculateRelevance(const std::vector<std::string>& query_words);
}; 