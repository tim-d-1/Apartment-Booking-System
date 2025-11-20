#pragma once
#include "IStorageProvider.h"
#include <filesystem>
#include <fstream>
#include "Config.h"

class CsvFileStorage : public IStorageProvider
{
  public:
    std::vector<std::string> ReadAll(const std::string& path) override
    {
        std::ifstream file(path);
        std::vector<std::string> lines;
        std::string line;

        if (!file.is_open())
            return lines;

        while (std::getline(file, line))
            lines.push_back(line);

        return lines;
    }

    void WriteAll(const std::string& path,
                  const std::vector<std::string>& lines) override
    {
        std::ofstream file(path, std::ios::trunc);
        if (!file.is_open())
        {
            #ifdef LOGGING
                std::cerr << "[Error] File " << path << " wasn't open\n";
            #endif
            return;
        }

        for (const auto& l : lines)
            file << l << '\n';
    }

    bool Exists(const std::string& path) const override
    {
        return std::filesystem::exists(path);
    }
};
