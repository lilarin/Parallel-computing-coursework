#include <iostream>
#include <sstream>
#include <algorithm>
#include <mutex>
#include "inverted_index.h"

void InvertedIndex::addDocument(const Document& document) {
    std::cout << "DEBUG: Adding document '" << document.filename << "' to index." << std::endl;
    std::stringstream ss(document.content);
    std::string word;
    while (ss >> word) {
        size_t bucket_index = getBucketIndex(word);
        std::lock_guard<std::mutex> lock(bucket_mutexes[bucket_index]);
        buckets[bucket_index][word].insert(document.filename);
    }
    std::cout << "DEBUG: Document '" << document.filename << "' added to index." << std::endl;
}

void InvertedIndex::removeDocument(const std::string& filename) {
    std::cout << "DEBUG: Removing document '" << filename << "' from index." << std::endl;
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        std::lock_guard<std::mutex> lock(bucket_mutexes[i]);
        for (auto& pair : buckets[i]) {
            pair.second.erase(filename);
        }
        for (auto it = buckets[i].begin(); it != buckets[i].end(); ) {
            if (it->second.empty()) {
                it = buckets[i].erase(it);
            } else {
                ++it;
            }
        }
    }
    std::cout << "DEBUG: Document '" << filename << "' removed from index." << std::endl;
}

std::set<std::string> InvertedIndex::search(const std::string& query) {
    std::cout << "INFO: Searching index for query: '" << query << "'" << std::endl;
    std::stringstream ss(query);
    std::string word;
    std::set<std::string> results;
    bool first_word = true;

    while (ss >> word) {
        size_t bucket_index = getBucketIndex(word);
        std::lock_guard<std::mutex> lock(bucket_mutexes[bucket_index]);
        auto it = buckets[bucket_index].find(word);
        if (it != buckets[bucket_index].end()) {
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
    std::cout << "INFO: Clearing the inverted index." << std::endl;
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        std::lock_guard<std::mutex> lock(bucket_mutexes[i]);
        buckets[i].clear();
    }
    std::cout << "INFO: Inverted index cleared." << std::endl;
}

void InvertedIndex::printIndexes() {
    if (!buckets.empty()) {
        std::cout << "--- Inverted Index ---" << std::endl;
        for (int i = 0; i < NUM_BUCKETS; ++i) {
            std::lock_guard<std::mutex> lock(bucket_mutexes[i]);
            for (const auto& pair : buckets[i]) {
                std::cout << pair.first << ": ";
                for (const auto& filename : pair.second) {
                    std::cout << filename << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "----------------------" << std::endl;
    }
}