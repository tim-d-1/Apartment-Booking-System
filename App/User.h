#pragma once
#include "Model.h"
#include "HelperFuncs.h"

class Model;

class User :
    public Model
{
protected:
    std::string username;
    std::string password;
public:
    User() : Model(), username("guest"), password("guest") {}
    User(int id, std::string username, std::string password)
        : Model(id), username(std::move(username)), password(std::move(password)) {
    }

    ~User() override {
        std::cout << "User destroyed: " << username << "\n"; // REMOVE
    }

    bool Authenticate(const std::string& pass) const { return pass == password; }

    std::string GetUsername() const {
        return username;
    }

    virtual void ChangePassword(const std::string& oldPass, const std::string& newPass)
    {
        if (oldPass != password) // FAIL
            return;

        if (isValidPassword(newPass))
        {
            password = newPass;
        }
    }

    virtual std::stringstream Serialize() const override {
        std::stringstream out;
        out << username << ','
            << password;
    }
};  