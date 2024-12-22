#include "inverted_index.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void InvertedIndex::addDocument(const Document& document) {
    std::lock_guard<std::mutex> lock(index_mutex);
    std::cout << "DEBUG: Adding document '" << document.filename << "' to index." << std::endl;
    std::stringstream ss(document.content);
    std::string word;
    while (ss >> word) {
        index[word].insert(document.filename);
    }
    std::cout << "DEBUG: Document '" << document.filename << "' added to index." << std::endl;
}

void InvertedIndex::removeDocument(const std::string& filename) {
    std::lock_guard<std::mutex> lock(index_mutex);
    std::cout << "DEBUG: Removing document '" << filename << "' from index." << std::endl;
    for (auto& pair : index) {
        pair.second.erase(filename);
    }
    for (auto it = index.begin(); it != index.end(); ) {
        if (it->second.empty()) {
            it = index.erase(it);
        } else {
            ++it;
        }
    }
    std::cout << "DEBUG: Document '" << filename << "' removed from index." << std::endl;
}

std::set<std::string> InvertedIndex::search(const std::string& query) {
    std::lock_guard<std::mutex> lock(index_mutex);
    std::cout << "INFO: Searching index for query: '" << query << "'" << std::endl;
    std::stringstream ss(query);
    std::string word;
    std::set<std::string> results;
    bool first_word = true;

    while (ss >> word) {
        auto it = index.find(word);
        if (it != index.end()) {
            if (first_word) {
                results = it->second;
                first_word = false;
            } else {
                std::set<std::string> intersection;
                std::set_intersection(
                        results.begin(), results.end(),
                        it->second.begin(), it->second.end(),
                        std::inserter(intersection, intersection.begin()));
                results = intersection;
            }
        } else {
            std::cout << "INFO: Query term '" << word << "' not found in index." << std::endl;
            return {};
        }
    }
    std::cout << "INFO: Search completed, found " << results.size() << " files." << std::endl;
    return results;
}

void InvertedIndex::clear() {
    std::lock_guard<std::mutex> lock(index_mutex);
    std::cout << "INFO: Clearing the inverted index." << std::endl;
    index.clear();
    std::cout << "INFO: Inverted index cleared." << std::endl;
}

void InvertedIndex::printIndexes() {
    std::lock_guard<std::mutex> lock(index_mutex);
    if (!index.empty()) {
        std::cout << "--- Inverted Index ---" << std::endl;
        for (const auto& pair : index) {
            std::cout << pair.first << ": ";
            for (const auto& filename : pair.second) {
                std::cout << filename << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "----------------------" << std::endl;
    }
}