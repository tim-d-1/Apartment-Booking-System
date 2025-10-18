#pragma once
#include <sstream>
#include <string>
#include <vector>

class ISerializable {
public:
    virtual ~ISerializable() = default;

    /**
     * @brief Serializes an object into a stringstream and returns it
     */
    virtual std::stringstream Serialize() const = 0;

    virtual void Deserialize(std::vector<std::string> params) = 0;
};
