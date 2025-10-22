#pragma once
#include <vector>
#include <string>

class IStorage {
public:
    virtual std::vector<std::string> ReadAll(const std::string& path) = 0;
    virtual void WriteAll(const std::string& path, const std::vector<std::string>& lines) = 0;
    virtual bool Exists(const std::string& path) const = 0;
    virtual ~IStorage() = default;
};


