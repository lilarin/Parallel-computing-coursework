#include "file_storage.h"
#include "inverted_index.h"
#include "web_server.h"
#include <vector>

int main() {
    FileStorage file_storage;
    InvertedIndex inverted_index;
    Indexer indexer(inverted_index, file_storage);
    std::vector<std::string> initial_files = file_storage.getAllFiles();
    indexer.buildIndex(initial_files);
}