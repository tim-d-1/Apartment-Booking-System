#include "AuthMenu.h"
#include "Input.h"
#include "AppService.h"
#include <iostream>

using namespace UI;

void AuthMenu::Login(AppService& service)
{
    std::string u = Input::GetLine("Username: ");
    std::string p = Input::GetLine("Password: ");

    try
    {
        if (service.Login(u, p))
            std::cout << "Successfully logged in.\n";
        else
            std::cout << "Invalid credentials.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

void AuthMenu::Register(AppService& service)
{
    std::string u = Input::GetLine("New username: ");
    std::string p = Input::GetLine("New password: ");

    try
    {
        service.RegisterUser(u, p);
        std::cout << "User created.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

void AuthMenu::Logout(AppService& service)
{
    if (!service.IsAuthenticated())
    {
        std::cout << "Not logged in.\n";
        return;
    }

    service.Logout();
    std::cout << "Logged out.\n";
}
