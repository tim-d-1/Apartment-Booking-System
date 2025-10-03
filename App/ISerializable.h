#pragma once
#include <sstream>

class ISerializable {
public:
    virtual ~ISerializable() = default;

    /**
     * @brief Serializes an object into a stringstream and returns it
     */
    virtual std::stringstream Serialize() const = 0;
};
