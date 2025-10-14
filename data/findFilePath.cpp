#include "findFilePath.hpp"

std::string findFilePath(const std::string &dir)
{
    std::regex pat(R"(^vault.*\.bin$)", std::regex_constants::icase);

    std::vector<std::filesystem::directory_entry> v;
    for (auto &e : std::filesystem::directory_iterator(dir))
        if (e.is_regular_file())
            v.push_back(e);

    // 文件名降序
    std::sort(v.begin(), v.end(),
              [](auto &a, auto &b)
              { return a.path().filename() > b.path().filename(); });

    for (auto &e : v)
        if (std::regex_match(e.path().filename().string(), pat))
            return e.path().string(); // 第一个匹配（即文件名最大）

    return {}; // 未找到
}