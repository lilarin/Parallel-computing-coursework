#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "utils.h"

class Client {
private:
    std::string host;
    int port;

    std::pair<int, std::string> sendRequest(RequestType request_type, const std::string& data = "") {
        try {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("socket creation failed");
                return {0, ""};
            }

            struct sockaddr_in serv_addr{};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(port);

            if (inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
                perror("Invalid address/ Address not supported");
                close(sock);
                return {0, ""};
            }

            if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
                perror("Connection Failed");
                close(sock);
                return {0, ""};
            }

            std::string request = std::string(1, static_cast<char>(request_type)) + data;
            send(sock, request.c_str(), request.length(), 0);

            char buffer[1024] = {0};
            int valread = read(sock, buffer, 1024);
            close(sock);

            if (valread <= 0) {
                std::cerr << "No response received from server." << std::endl;
                return {0, ""};
            }

            std::string response_str(buffer, valread);
            size_t newline_pos = response_str.find('\n');
            if (newline_pos == std::string::npos) {
                std::cerr << "Invalid response format." << std::endl;
                return {0, ""};
            }

            int status_code = std::stoi(response_str.substr(0, newline_pos));
            std::string response_text = response_str.substr(newline_pos + 1);

            return {status_code, response_text};



        } catch (const std::exception& e) {
            std::cerr << "An error occurred during communication: " << e.what() << std::endl;
            return {0, ""};
        }
    }

public:
    Client(const std::string& h, int p) : host(h), port(p) {}

    std::pair<int, std::string> uploadFile(const std::string& filename, const std::string& content) {
        std::string data = filename + "\n" + content;
        return sendRequest(RequestType::UPLOAD_FILES, data);
    }

    std::pair<int, std::string> searchIndex(const std::string& query) {
        return sendRequest(RequestType::SEARCH, query);
    }

    std::pair<int, std::string> deleteFile(const std::string& filename) {
        return sendRequest(RequestType::DELETE_FILE, filename);
    }
};

int main() {
    Client client("127.0.0.1", 8080);

    int status;
    std::string data;

    auto upload_file1_result = client.uploadFile("test1.txt", "This is the content of test file 1.");
    status = upload_file1_result.first;
    data = upload_file1_result.second;
    std::cout << "Upload file1: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto upload_file2_result = client.uploadFile("test2.txt", "This is the content of test file 2.");
    status = upload_file2_result.first;
    data = upload_file2_result.second;
    std::cout << "Upload file2: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto search_content_result = client.searchIndex("content");
    status = search_content_result.first;
    data = search_content_result.second;
    std::cout << "Search the data that in both files: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto search_non_existing_result = client.searchIndex("12333333qsadadasdad");
    status = search_non_existing_result.first;
    data = search_non_existing_result.second;
    std::cout << "Search with non-existing query: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto delete_file1_result = client.deleteFile("test1.txt");
    status = delete_file1_result.first;
    data = delete_file1_result.second;
    std::cout << "Delete the file1: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto search_content_again_result = client.searchIndex("content");
    status = search_content_again_result.first;
    data = search_content_again_result.second;
    std::cout << "Search the data that now only in the file2: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto delete_file2_result = client.deleteFile("test2.txt");
    status = delete_file2_result.first;
    data = delete_file2_result.second;
    std::cout << "Delete the file2: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto search_non_existing_content_result = client.searchIndex("content");
    status = search_non_existing_content_result.first;
    data = search_non_existing_content_result.second;
    std::cout << "Search the data that now not exists: " << status << " " << data << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    auto delete_non_existing_file_result = client.deleteFile("nonexistent.txt");
    status = delete_non_existing_file_result.first;
    data = delete_non_existing_file_result.second;
    std::cout << "Delete non-existing file: " << status << " " << data << std::endl;

    return 0;
}