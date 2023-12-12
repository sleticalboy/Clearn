//
// Created by binlee on 2020/8/31.
//

#ifndef BASIC_STRINGS_H
#define BASIC_STRINGS_H

bool startsWith(const std::string &s, const std::string &prefix) {
    return s.find(prefix) == 0;
}

bool endsWith(const std::string &s, const std::string &suffix) {
    auto pos = s.rfind(suffix);
    return pos != std::string::npos && s.substr(pos) == suffix;
}

std::string getName(const std::string &path) {
    auto pos = path.rfind('/');
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    }
    return path;
}

std::string getExt(const std::string &path) {
    auto name = getName(path);
    auto pos = name.rfind('.');
    if (pos != std::string::npos) {
        return name.substr(pos + 1);
    }
    return "";
}

void cpp_string();

#endif //BASIC_STRINGS_H
