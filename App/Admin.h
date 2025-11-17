#pragma once
#include "User.h"

class User;

class Admin : public User
{
  public:
    Admin(int id, const std::string& username, const std::string& password)
        : User(id, username, password)
    {
    }

    Admin() : User()
    {
    }
};
