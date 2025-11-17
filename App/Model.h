#pragma once
#include "ISerializable.h"
#include <iostream>

class Model : public ISerializable
{
  public:
    Model() : id(0)
    {
    }

    explicit Model(int id) : id(id)
    {
    }

    Model(const Model& other) : id(other.id)
    {
    }

    Model(Model&& other) noexcept : id(other.id)
    {
        other.id = 0;
    }

    virtual ~Model()
    {
    }

    int GetId() const
    {
        return id;
    }

    void SetId(int newId)
    {
        if (newId < 0)
            throw std::invalid_argument("ID must be non-negative");

        id = newId;
    }


    Model& operator=(const Model& other)
    {
        if (this != &other)
        {
            id = other.id;
        }
        return *this;
    }

    Model& operator=(Model&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            other.id = 0;
        }
        return *this;
    }


  protected:
    int id;
};
