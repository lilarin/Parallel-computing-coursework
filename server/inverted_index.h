#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <set>
#include <string>
#include <mutex>
#include "utils.h"

class InvertedIndex {
private:
    std::unordered_map<std::string, std::set<std::string>> index;
    std::mutex index_mutex;

public:
    void addDocument(const Document& document);
    void removeDocument(const std::string& filename);
    std::set<std::string> search(const std::string& query);
    void clear();
    void printIndexes();
};

#endif // INVERTED_INDEX_H