#ifndef RMS_DIR_H_
#define RMS_DIR_H_

#include<string>
#include<filesystem>
#include<vector>

namespace rmscore {
namespace common {

class RMSDir {
public:
    RMSDir() {}

public:
    static bool mkpath(const std::string& path) {
        std::filesystem::path dir(path);
        if (std::filesystem::create_directories(dir)) {
            return true;
        } else {
            return false;
        }
    }

    static std::vector<std::string> entryList(const std::string& path, const std::vector<std::string>& filters) {
        std::vector<std::string> files;
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_regular_file(file) 
            && std::find_if(filters.begin(), filters.end(), [&](const std::string& filter) {
                return std::filesystem::path(file.path()).extension() == filter;
            }) != filters.end()) {
                files.push_back(file.path().string());
            }
        }

        return files;
    }

    static std::vector<std::filesystem::path> entryInfoList(const std::string& path, const std::vector<std::string>& filters) {
        std::vector<std::filesystem::path> files;
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_regular_file(file) 
            && std::find_if(filters.begin(), filters.end(), [&](const std::string& filter) {
                return std::filesystem::path(file.path()).extension() == filter;
            }) != filters.end()) {
                files.push_back(file.path());
            }
        }

        return files;
    }
};

}
}

#endif