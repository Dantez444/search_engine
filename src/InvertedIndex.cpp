#include "InvertedIndex.h"
#include <sstream>
#include <algorithm>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    
    freq_dictionary.clear();
    
    std::vector<std::thread> threads;
    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        threads.emplace_back(&InvertedIndex::IndexDocument, this, doc_id, std::ref(docs[doc_id]));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    
    return {};
}

void InvertedIndex::IndexDocument(size_t doc_id, const std::string& content) {
    std::istringstream stream(content);
    std::string word;
    
    std::map<std::string, size_t> word_count;
    
    while (stream >> word) {
        std::transform(word.begin(), word.end(), word.begin(), 
                     [](unsigned char c){ return std::tolower(c); });
        
        ++word_count[word];
    }
    
    std::lock_guard<std::mutex> lock(freq_dict_mutex);
    
    for (const auto& [word, count] : word_count) {
        auto& entries = freq_dictionary[word];
        
        entries.push_back({doc_id, count});
    }
} 