#include "src/file_storage.h"
#include "src/inverted_index.h"
#include "src/web_server.h"
#include <vector>

int main() {
    FileStorage file_storage("storage/");
    InvertedIndex inverted_index;
    Indexer indexer(inverted_index, file_storage);
    std::vector<std::string> initial_files = file_storage.getAllFiles();
    indexer.buildIndex(initial_files);

    WebServer web_server(8080, inverted_index, file_storage, 12);
    web_server.start();
}
