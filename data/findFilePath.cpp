#include "findFilePath.hpp"
    std::string findFilePath(const std::string &dir)
    {
        // 通配符 vault*.bin  转正则  ^vault.*\.bin$
        std::regex pat(R"(^vault.*\.bin$)", std::regex_constants::icase);

        for (const auto &e : std::filesystem::directory_iterator(dir))
        {
            if (!e.is_regular_file())
                continue;
            std::string name = e.path().filename().string();
            if (std::regex_match(name, pat))
                return e.path().string(); // 找到立即返回
        }
        return {}; // 没找到
    }