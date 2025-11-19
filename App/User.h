#pragma once
#include "HelperFuncs.h"
#include "Model.h"
#include <stdexcept>

class User : public Model
{
  protected:
    std::string username;
    std::string password;

  public:
    User() : Model(), username("guest"), password("guest")
    {
    }

    User(int id, const std::string& username, const std::string& password)
        : Model(id)
    {
        SetUsername(username);
        SetPassword(password);
    }

    User(const User& other)
        : Model(other.id), username(other.username), password(other.password)
    {
    }

    User(User&& other) noexcept
        : Model(other.id), username(std::move(other.username)),
          password(std::move(other.password))
    {
        other.id = 0;
    }

    User& operator=(const User& other)
    {
        if (this != &other)
        {
            id = other.id;
            username = other.username;
            password = other.password;
        }
        return *this;
    }

    User& operator=(User&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            username = std::move(other.username);
            password = std::move(other.password);
            other.id = 0;
        }
        return *this;
    }

    virtual ~User() override
    {
        std::cout << "User object " << id << " destroyed.\n";
    }

    void SetUsername(const std::string& u)
    {
        if (u.empty())
            throw std::invalid_argument("Username cannot be empty.");
        username = u;
    }

    void SetPassword(const std::string& p)
    {
        if (!HelperFuncs::isValidPassword(p))
            throw std::invalid_argument("Password is too weak.");
        password = p;
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
            throw std::runtime_error("Old password incorrect.");
        SetPassword(newPass);
    }

    virtual std::stringstream Serialize() const override
    {
        std::stringstream ss;
        ss << id << "," << username << "," << password;
        return ss;
    }

    virtual void Deserialize(std::vector<std::string> params) override
    {
        if (params.size() != 3)
            throw std::runtime_error("User::Deserialize expected 3 fields.");

        id = std::stoi(params[0]);
        SetUsername(params[1]);
        password = params[2];
    }
};
