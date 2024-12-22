#include "file_storage.h"
#include <iostream>
#include <fstream>

FileStorage::FileStorage(const std::string& path) : storage_path(path) {
    std::experimental::filesystem::create_directory(storage_path);
    std::cout << "INFO: File storage initialized at '" << storage_path << "'" << std::endl;
}

bool FileStorage::saveFile(const std::string& filename, const std::string& content) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::cout << "INFO: Saving file '" << filename << "' to storage." << std::endl;
    std::ofstream file(storage_path + filename);
    if (file.is_open()) {
        file << content;
        std::cout << "INFO: File '" << filename << "' saved successfully." << std::endl;
        return true;
    } else {
        std::cerr << "ERROR: Could not save file: " << filename << std::endl;
        return false;
    }
}

std::string FileStorage::getFileContent(const std::string& filename) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::cout << "INFO: Getting content of file '" << filename << "' from storage." << std::endl;
    std::ifstream file(storage_path + filename);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::cout << "INFO: Content of file '" << filename << "' retrieved." << std::endl;
        return buffer.str();
    } else {
        std::cerr << "ERROR: Could not read file: " << filename << std::endl;
        return "";
    }
}

std::vector<std::string> FileStorage::getAllFiles() {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::cout << "INFO: Getting list of all files in storage." << std::endl;
    std::vector<std::string> files;
    for (const auto& entry : std::experimental::filesystem::directory_iterator(storage_path)) {
        if (std::experimental::filesystem::is_regular_file(entry)) {
            files.push_back(entry.path().filename().string());
        }
    }
    std::cout << "INFO: Found " << files.size() << " files in storage." << std::endl;
    return files;
}

bool FileStorage::deleteFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::cout << "INFO: Deleting file '" << filename << "' from storage." << std::endl;
    std::error_code ec;
    if (std::experimental::filesystem::remove(storage_path + filename, ec)) {
        std::cout << "INFO: File '" << filename << "' deleted successfully." << std::endl;
        return true;
    } else {
        std::cerr << "ERROR: Could not delete file: " << filename << ". " << ec.message() << std::endl;
        return false;
    }
}