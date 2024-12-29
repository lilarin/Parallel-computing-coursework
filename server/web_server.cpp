#include "web_server.h"
#include "inverted_index.h"
#include "file_storage.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

WebServer::WebServer(int p, InvertedIndex& index, FileStorage& storage, int num_threads)
        : port(p), inverted_index(index), file_storage(storage), indexer(index, storage), thread_pool(num_threads) {
    std::cout << "INFO: WebServer initialized on port " << port
              << " with " << num_threads << " threads." << std::endl;
}

Response WebServer::handleFileUpload(const Request& request) {
    std::cout << "INFO: Handling file upload request." << std::endl;
    std::string data = request.data;
    size_t newline_pos = data.find('\n');
    if (newline_pos == std::string::npos) {
        std::cerr << "ERROR: Invalid file upload format - missing newline." << std::endl;
        return {400, "Invalid file upload format."};
    }

    std::string filename = data.substr(0, newline_pos);
    std::string content = data.substr(newline_pos + 1);

    if (file_storage.saveFile(filename, content)) {
        thread_pool.enqueue([this, filename]() {
            std::cout << "INFO: Adding file to index after upload: " << filename << std::endl;
            indexer.addFileToIndex(filename);
        });
        std::cout << "INFO: File upload handled successfully." << std::endl;
        return {201, "File uploaded successfully."};
    } else {
        std::cout << "INFO: File upload failed." << std::endl;
        return {500, "Error uploading file."};
    }
}

Response WebServer::handleDeleteFileRequest(const Request& request) {
    std::cout << "INFO: Handling delete file request for: '" << request.data << "'" << std::endl;
    const std::string& filename = request.data;
    if (file_storage.deleteFile(filename)) {
        thread_pool.enqueue([this, filename]() {
            std::cout << "INFO: Removing file from index after deletion: " << filename << std::endl;
            indexer.removeFileFromIndex(filename);
        });
        std::cout << "INFO: Delete file request processed successfully." << std::endl;
        return {200, "File deleted successfully."};
    } else {
        std::cout << "INFO: Delete file request failed." << std::endl;
        return {404, "File not found."};
    }
}

Response WebServer::handleSearchRequest(const Request& request) {
    std::cout << "INFO: Handling search request for: '" << request.data << "'" << std::endl;
    auto results = inverted_index.search(request.data);
    std::string result_string;

    if (results.empty()) {
        std::cout << "INFO: No files found for query: '" << request.data << "'" << std::endl;
        return {404, "No files found matching the query."};
    }

    for (const auto& filename : results) {
        result_string += filename + " ";
    }
    std::cout << "INFO: Search request processed, found " << results.size() << " files." << std::endl;
    return {200, result_string};
}

void WebServer::handleClient(int client_socket) {
    std::cout << "INFO: Accepted new client connection." << std::endl;
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        std::cout << "DEBUG: Received raw data: " << std::string(buffer, bytes_received) << std::endl;
        if (bytes_received < 1) {
            std::cerr << "ERROR: Received incomplete request." << std::endl;
            close(client_socket);
            return;
        }

        auto request_type_int = static_cast<std::underlying_type_t<RequestType>>(buffer[0]);
        std::string request_data;
        if (bytes_received > 1) {
            request_data = std::string(buffer + 1, bytes_received - 1);
        }

        Request request;
        request.type = static_cast<RequestType>(request_type_int);
        request.data = request_data;

        std::cout << "INFO: Received request of type " << static_cast<int>(request.type) << " with data: '" << request.data << "'" << std::endl;

        Response response;
        switch (request.type) {
            case RequestType::UPLOAD_FILES:
                response = handleFileUpload(request);
                break;
            case RequestType::SEARCH:
                response = handleSearchRequest(request);
                break;
            case RequestType::DELETE_FILE:
                response = handleDeleteFileRequest(request);
                break;
            default:
                response = {400, "Unknown request type."};
                std::cout << "WARNING: Received unknown request type: " << static_cast<int>(request.type) << std::endl;
                break;
        }

        std::string response_str = std::to_string(response.status_code) + "\n" + response.data;
        send(client_socket, response_str.c_str(), response_str.length(), 0);
        std::cout << "INFO: Sent response to client." << std::endl;
    } else if (bytes_received == 0) {
        std::cout << "INFO: Client disconnected." << std::endl;
    } else {
        perror("recv failed");
    }
    close(client_socket);
}

void WebServer::start() {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Binding the socket to the address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("accept");
            continue;
        }

        // Handle each client connection in a separate thread
        std::thread client_thread(&WebServer::handleClient, this, new_socket);
        client_thread.detach(); // Detach the thread to handle it independently
    }
}