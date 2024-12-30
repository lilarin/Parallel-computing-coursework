#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <set>
#include <string>
#include <mutex>
#include <vector>
#include <functional>
#include "utils.h"

class InvertedIndex {
private:

    static const int NUM_BUCKETS = 16;
    std::vector<std::unordered_map<std::string, std::set<std::string>>> buckets;
    std::vector<std::mutex> bucket_mutexes;

    static size_t getBucketIndex(const std::string& word) {
        std::hash<std::string> hasher;
        return hasher(word) % NUM_BUCKETS;
    }

public:
    InvertedIndex() : buckets(NUM_BUCKETS), bucket_mutexes(NUM_BUCKETS) {}

    void addDocument(const Document& document);
    void removeDocument(const std::string& filename);
    std::set<std::string> search(const std::string& query);
    void clear();
    void printIndexes();
};

#endif // INVERTED_INDEX_H