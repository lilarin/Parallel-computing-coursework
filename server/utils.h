#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <cstdint>

struct Document {
    std::string filename;
    std::string content;
};

enum class RequestType : std::uint8_t {
    UPLOAD_FILES,
    SEARCH,
    DELETE_FILE
};

struct Request {
    RequestType type;
    std::string data;
};

struct Response {
    int status_code;
    std::string data;
};

#endif // TYPES_H