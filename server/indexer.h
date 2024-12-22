#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <vector>

class InvertedIndex;
class FileStorage;

class Indexer {
private:
    InvertedIndex& inverted_index;
    FileStorage& file_storage;

public:
    Indexer(InvertedIndex& index, FileStorage& storage);
    void buildIndex(const std::vector<std::string>& filenames);
    void addFileToIndex(const std::string& filename);
    void removeFileFromIndex(const std::string& filename);
};

#endif // INDEXER_H