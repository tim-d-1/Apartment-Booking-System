#include "MainMenu.h"
#include "AppService.h"
#include "ApartmentMenu.h"
#include "AuthMenu.h"
#include "AdminMenu.h"
#include "BookingMenu.h"
#include "Input.h"
#include <iostream>

void UI::MainMenu::Run(AppService& service)
{
    while (true)
    {
        if (!service.IsAuthenticated())
            ShowGuestMenu(service);
        else if (service.IsAdmin())
            ShowAdminMenu(service);
        else
            ShowUserMenu(service);
    }
}

void UI::MainMenu::ShowGuestMenu(AppService& service)
{
    std::cout << "\n--- Гість меню ---\n "
              << "1. Вхід\n "
              << "2. Зареєструватися\n "
              << "0. Вийти\n ";

    switch (Input::GetInt("> "))
    {
    case 1:
        UI::AuthMenu::Login(service);
        break;
    case 2:
        UI::AuthMenu::Register(service);
        break;
    case 0:
        exit(0);
    }
}

void UI::MainMenu::ShowUserMenu(AppService& service)
{
    std::cout << "\n--- Меню користувача ---\n "
              << "1. Квартири\n "
              << "2. Меню бронювання\n "
              << "3. Вийти з акаунта\n "
              << "0. Вийти\n ";

    switch (Input::GetInt("> "))
    {
    case 1:
        UI::ApartmentMenu::Show(service);
        break;

    case 2:
        std::cout << "\n--- Дії бронювання ---\n "
                  << "1. Здійснити бронювання\n "
                  << "2. Перерахувати мої бронювання\n "
                  << "3. Оплатити\n "
                  << "4. Скасувати\n "
                  << "5. Експортна квитанція\n ";

        switch (Input::GetInt("> "))
        {
        case 1:
            UI::BookingMenu::BookApartment(service);
            break;
        case 2:
            UI::BookingMenu::ListMyBookings(service);
            break;
        case 3:
            UI::BookingMenu::PayForBooking(service);
            break;
        case 4:
            UI::BookingMenu::CancelBooking(service);
            break;
        case 5:
            UI::BookingMenu::ExportReceipt(service);
            break;
        }
        break;

    case 3:
        service.Logout();
        break;
    case 0:
        exit(0);
    }
}

void UI::MainMenu::ShowAdminMenu(AppService& service)
{
    std::cout << "\n=== ГОЛОВНЕ МЕНЮ (Адміністратор) ===\n "
              << "1. Панель адміністратора\n "
              << "2. Вийти з акаунта\n "
              << "0. Вийти\n ";

    int choice = Input::GetInt("Виберіть: ");

    switch (choice)
    {
    case 1:
        UI::AdminMenu::Show(service);
        break;

    case 2:
        service.Logout();
        break;

    case 0:
        exit(0);

    default:
        std::cout << "Недійсний.\n ";
    }
}
