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

    ~User() override {}

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
        out << id << ','
            << username << ','
            << password;
    }

    virtual void Deserialize(std::vector<std::string> params) override {
        if (params.size() != 3) {
            // FAIL
            return;
        }

        id = std::stoi(params[0]);
        username = params[1];
        password = params[2];
    }
};  