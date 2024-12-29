#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <string>
#include "indexer.h"
#include "thread_pool.h"

class InvertedIndex;
class FileStorage;
class Indexer;
class ThreadPool;
struct Request;
struct Response;

class WebServer {
private:
    int port;
    InvertedIndex& inverted_index;
    FileStorage& file_storage;
    Indexer indexer;
    ThreadPool thread_pool;

    Response handleFileUpload(const Request& request);
    Response handleDeleteFileRequest(const Request& request);
    Response handleSearchRequest(const Request& request);
    void handleClient(int client_socket);

public:
    WebServer(int port, InvertedIndex& index, FileStorage& storage, int num_threads);
    void start();
};

#endif // WEB_SERVER_H