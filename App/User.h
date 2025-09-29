#pragma once
#include "Model.h"
#include "HelperFuncs.h"

class User;

class User :
    public Model
{
protected:
    std::string username;
    std::string password;
public:
    User(int id, const std::string& username, const std::string& password)
        : Model(id), username{ username }, password{ password }
    {
    }

    void ChangePassword(const std::string& oldPass, const std::string& newPass)
    {
        if (oldPass != password) // FAIL
            return;

        if (isValidPassword(newPass))
        {
            password = newPass;
        }
    }
};  