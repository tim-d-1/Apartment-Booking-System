#pragma once
#include "Config.h"
#include "HelperFuncs.h"
#include "Model.h"

class Model;

class User : public Model
{
  protected:
    std::string username;
    std::string password;

  public:
    User() : Model(), username("guest"), password("guest")
    {
    }
    User(int id, std::string username, std::string password)
        : Model(id), username(std::move(username)),
          password(std::move(password))
    {
    }

    ~User() override
    {
    }

    bool Authenticate(const std::string& pass) const
    {
        return pass == password;
    }

    std::string GetUsername() const
    {
        return username;
    }

    virtual void ChangePassword(const std::string& oldPass,
                                const std::string& newPass)
    {
        if (oldPass != password)
            return;

        if (HelperFuncs::isValidPassword(newPass))
        {
            password = newPass;
        }
    }

    virtual std::stringstream Serialize() const override
    {
        std::stringstream out;
        out << id << ',' << username << ',' << password;
        return out;
    }

    virtual void Deserialize(std::vector<std::string> params) override
    {
        if (params.size() != 3)
        {
#ifdef LOGGING
            std::cerr << "[Error] Not enough params for Deserialize, params: '"
                      << HelperFuncs::vectorToString(params, ' ')
                      << "'\nRequired: 3\n";
#endif
            return;
        }

        id = std::stoi(params[0]);
        username = params[1];
        password = params[2];
    }
};
