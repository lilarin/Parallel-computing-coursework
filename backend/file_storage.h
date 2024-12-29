#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <string>
#include <vector>
#include <shared_mutex>
#include <experimental/filesystem>

class FileStorage {
private:
    std::string storage_path;
    std::shared_mutex file_rw_mutex;

public:
    FileStorage(const std::string& path);
    bool saveFile(const std::string& filename, const std::string& content);
    std::string getFileContent(const std::string& filename);
    std::vector<std::string> getAllFiles();
    bool deleteFile(const std::string& filename);
};

#endif // FILE_STORAGE_H