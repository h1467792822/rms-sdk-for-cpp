#ifndef RMS_DIR_H_
#define RMS_DIR_H_

#include<string>
#include<filesystem>

namespace rmscore {
namespace common {

class RMSDir {
public:
    static bool mkpath(const std::string& path) {
        std::filesystem::path dir(path);
        if (std::filesystem::create_directory(dir)) {
            return true;
        } else {
            return false;
        }
    }
};

}
}

#endif