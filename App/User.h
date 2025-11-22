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
    User() : Model(), username("гість "), password("гість ")
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
        std::cout << "Об'єкт користувача " << id << "знищено.\n ";
    }

    void SetUsername(const std::string& u)
    {
        if (u.empty())
            throw std::invalid_argument("Ім'я користувача не може бути пустим. ");
        username = u;
    }

    void SetPassword(const std::string& p)
    {
        if (!HelperFuncs::isValidPassword(p))
            throw std::invalid_argument("Пароль занадто слабкий. ");
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
            throw std::runtime_error("Старий пароль неправильний. ");
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
            throw std::runtime_error("User::Deserialize очікувані 3 поля. ");

        id = std::stoi(params[0]);
        SetUsername(params[1]);
        password = params[2];
    }
};
