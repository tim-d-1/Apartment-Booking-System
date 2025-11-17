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
        id = newId;
    }

  protected:
    int id;
};
