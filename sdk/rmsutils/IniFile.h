
#ifndef RMSUTILS_INIFILE_H
#define RMSUTILS_INIFILE_H

/**
* QSetting: rms depends on QSettings::IniFormat
*/

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cassert>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>

namespace rmsutils {

class IniFile
{
  std::string fname;
  bool updated = false;
  std::map<std::string, std::string> kv_map;
  std::string current_group;
public:
  IniFile(const std::string& file_name): fname(file_name) {
    load();
  }

  ~IniFile() {
    if (updated) {
      save();
    }
  }

  void beginGroup(const std::string& group) {
    current_group = decodeUserPath(group);
  }

  void endGroup() {
    current_group = "";
  }

  template<class T>
  void setValue(const std::string& path, const T& val) {
    std::stringstream str;
    str << val;
    setValue(path, str.str());
  }

  void setValue(const std::string& key, const std::string& val) {
    kv_map[decodeUserPath(key)] = val;  
    updated = true;
  }

  std::string getValue(const std::string& key, const std::string& default_value = "") const {
    const std::string& path = decodeUserPath(key);
    const auto& it = kv_map.find(path);
    return it != kv_map.end() ? it->second : default_value;
  }

  bool getBool(const std::string& key, bool default_value = false) const {
    const std::string& val = getValue(key);
    if (val.empty()) {
      return default_value;
    }

    char* endptr = 0;
    auto lval = strtol(val.c_str(), &endptr, 10);
    if (*endptr == 0) {
      return lval != 0;
    }

    return strcasecmp(val.c_str(), "false") != 0;
  }

  int getInt(const std::string& key, int default_value = 0) const {
    const std::string& val = getValue(key);
    if (val.empty()) {
      return default_value;
    }
    char* endptr = 0;
    auto lval = strtol(val.c_str(), &endptr, 10);
    if (*endptr == 0) {
      return lval;
    }
    return default_value;
  }
private:
  class NameCoder {
    char first = 0;
    int state = 0;
    std::string out;
  public:
    void decode(char c) {
      if (c == '/') {
        c = '\\';
      }
      if (state == 0) {
        if (c != '%') {
          out.push_back(c);
        } else {
          state = 1;
        }
      } else if (state == 1) {
        if (!isxdigit(c)) {
          out.push_back('%');
          out.push_back(c);
          state = 0; 
        } else {
          first = c;
          state = 2;
        }
      } else {
        state = 0;
        if (!isxdigit(c)) {
          out.push_back('%');
          out.push_back(first);
          out.push_back(c);
        } else {
          char in[] = { first, c, 0 };
          unsigned int val = 0;
          sscanf(in, "%x", &val);
          out.push_back((char)val);
        }
      }
    }

    std::string decode_finish() {
      if (state == 1) {
        out.push_back('%');
      } else if (state == 2) {
        out.push_back('%');
        out.push_back(first);
      }
      state = 0;
      return std::move(out);
    }

    static void encode(char c, std::string& out) {
      if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-' || c == '_')) {
        out.push_back(c);
      } else {
        char val[4];
        snprintf(val, sizeof(val), "%02x", (int)c);
        out.push_back('%');
        out.push_back(val[0]);
        out.push_back(val[1]);
      }
    }
  };

  // from file to memory
  static std::string decodePath(const std::string& name) {
    NameCoder coder;
    for (char c: name) {
      coder.decode(c);
    }    
    return coder.decode_finish();
  }

  // from memory to file
  static std::string encodePath(const std::string& name) {
    std::string path;
    for (char c: name) {
      NameCoder::encode(c, path);
    }
    return path;
  }

  std::string decodeUserPath(const std::string& name) const {
    std::string path;
    bool ingroup = false;
    path.reserve(name.size());
    for (char c: name) {
      if (c == '/') {
        c = '\\';
      }
      if (c == '\\') {
        ingroup = true;
      }
      path.push_back(c);
    }
    if (!ingroup) {
      path = std::string("General\\") + path;
    }
    return path;
  }

  void save() const {
    FILE* file = fopen(fname.c_str(), "w");
    if (!file) {
      return;
    }

    std::string current_group = "";
    for ( auto const& [key, val]: kv_map) {
      auto pos = key.find('\\');
      assert(pos != std::string::npos);
      const auto& group = encodePath(key.substr(0, pos));
      const auto& subpath = encodePath(key.substr(pos + 1)); 
      if (current_group != group) {
        if (!current_group.empty()) {
          fwrite("\n", 1, 1, file);
        }
        current_group = group;
        fwrite("[", 1, 1, file);
        fwrite(group.c_str(), 1, group.size(), file);
        fwrite("]\n", 1, 2, file);
      } 

      fwrite(subpath.c_str(), 1, subpath.size(), file);
      fwrite(" = ", 1, 3, file);
      fwrite(val.c_str(), 1, val.size(), file);
      fwrite("\n", 1, 1, file);
    }
    fflush(file);
    fclose(file);
  }

  struct Parser {
    std::string group = "General";
    std::string trim(const std::string& str)
    {
      auto notspace = [](char c) { return !isspace(c); };
      auto beg = std::distance(str.begin(), std::find_if(str.begin(), str.end(), notspace));
      auto end = std::distance(std::find_if(str.rbegin(), str.rend(), notspace), str.rend());
      return str.substr(beg, end - beg);
    }

    void parse(const std::string& input, std::function<void(const std::string& path, const std::string value)> setter) {
      auto line = trim(input);
      if (line.empty()) {
        return;
      }

      if (line[0] == '[') {
        auto end = line.find(']', 1);
        if (end == std::string::npos) {
          return;
        }
        group = trim(line.substr(1, end - 1));     
        if (group.empty()) {
          group = "General";
        }
      } else {
        auto end = line.find('=');
        if (end == std::string::npos) {
          return;
        }
        auto key = trim(line.substr(0, end));
        auto value = trim(line.substr(end + 1));

        setter(group + "\\" + key, value);
      }
    }
  };
  void load() {
    FILE* file = fopen(fname.c_str(), "r");    
    if (!file) {
      return;
    }
    struct Parser parser;
    std::string line;
    std::string buff(1024, '\0');
    auto setter = [=](const std::string& path, const std::string& value) {
      this->kv_map[IniFile::decodePath(path)] = value;
    };
    size_t len;
    while((len = fread(&buff[0], 1, buff.size(), file)) > 0) {
      size_t beg = 0;
      for(;;) { 
        auto end = std::find(&buff[beg], &buff[len], '\n');
        line += buff.substr(beg, end - &buff[beg]);
        if (end == &buff[len]) {
          break;
        }
        parser.parse(line, setter);
        beg = end - &buff[0] + 1;
        line = "";
      }
    }
    fclose(file);
  }
};

} // namespace rmsutils

#endif


