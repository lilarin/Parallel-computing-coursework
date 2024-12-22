#include "indexer.h"
#include "inverted_index.h"
#include "file_storage.h"
#include <iostream>

Indexer::Indexer(InvertedIndex& index, FileStorage& storage) : inverted_index(index), file_storage(storage) {}

void Indexer::buildIndex(const std::vector<std::string>& filenames) {
    std::cout << "INFO: Index rebuild started for " << filenames.size() << " files." << std::endl;
    inverted_index.clear();
    for (const auto& filename : filenames) {
        std::string content = file_storage.getFileContent(filename);
        if (!content.empty()) {
            inverted_index.addDocument({filename, content});
        }
    }
    std::cout << "INFO: Index rebuild finished. Indexed " << filenames.size() << " files." << std::endl;
    inverted_index.printIndexes();
}

void Indexer::addFileToIndex(const std::string& filename) {
    std::cout << "INFO: Adding file '" << filename << "' to index." << std::endl;
    std::string content = file_storage.getFileContent(filename);
    if (!content.empty()) {
        inverted_index.addDocument({filename, content});
        std::cout << "DEBUG: Added file '" << filename << "' to index." << std::endl;
    } else {
        std::cerr << "WARNING: Could not add file '" << filename << "' to index as content is empty or file not found." << std::endl;
    }
    inverted_index.printIndexes();
}

void Indexer::removeFileFromIndex(const std::string& filename) {
    std::cout << "INFO: Removing file '" << filename << "' from index." << std::endl;
    inverted_index.removeDocument(filename);
    std::cout << "DEBUG: Removed file '" << filename << "' from index." << std::endl;
    inverted_index.printIndexes();
}