#pragma once
#include <memory>
#include "ISerializable.h"

class Model
    : ISerializable
{
protected:
    int id;
public:
    Model(int id) 
    {
        this->id = id;
    }

    int GetId() const
    {
        return id;
    }
};