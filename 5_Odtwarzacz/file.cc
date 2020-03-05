#include <iostream>
#include <string>
#include "file.h"
#include "exceptions.h"

File::File(std::string file) {
    std::string delimiter = "|";

    size_t pos = 0;
    std::string token;

    while ((pos = file.find(delimiter)) != std::string::npos) {
        token = file.substr(0, pos);
        data.push_back(token);
        file.erase(0, pos + delimiter.length());
    }

    data.push_back(file);
}

std::vector<std::string>& File::getData() {
    return data;
}
