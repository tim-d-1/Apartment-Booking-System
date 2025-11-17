#pragma once
#include "User.h"

class Admin : public User
{
  public:
    Admin() : User()
    {
    }

    Admin(int id, const std::string& username, const std::string& password)
        : User(id, username, password)
    {
    }

    virtual ~Admin() override = default;
};
