#pragma once
#include <sstream>
#include <memory>

class IModel
{
    int id;
public:
    IModel(int id) 
    {
        this->id = id;
    }

    int GetId()
    {
        return id;
    }
    /**
     * @brief Serializes an object into a stringstream and returns it
     */
    virtual std::stringstream Serialize() = 0;
};