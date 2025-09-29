#pragma once
#include <sstream>

class ISerializable
{
    /**
     * @brief Serializes an object into a stringstream and returns it
     */
    virtual std::stringstream Serialize() const = 0;
};

