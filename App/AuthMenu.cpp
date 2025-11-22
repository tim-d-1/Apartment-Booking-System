#include "AuthMenu.h"
#include "Input.h"
#include "AppService.h"
#include <iostream>

using namespace UI;

void AuthMenu::Login(AppService& service)
{
    std::string u = Input::GetLine("Ім'я користувача: ");
    std::string p = Input::GetLine("Пароль: ");

    try
    {
        if (service.Login(u, p))
            std::cout << "Успішно ввійшли.\n ";
        else
            std::cout << "Недійсні облікові дані.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}

void AuthMenu::Register(AppService& service)
{
    std::string u = Input::GetLine("Нове ім'я користувача: ");
    std::string p = Input::GetLine("Новий пароль: ");

    try
    {
        service.RegisterUser(u, p);
        std::cout << "Користувача створено.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}

void AuthMenu::Logout(AppService& service)
{
    if (!service.IsAuthenticated())
    {
        std::cout << "Не ввійшли в систему.\n ";
        return;
    }

    service.Logout();
    std::cout << "Вийшов із системи.\n ";
}
