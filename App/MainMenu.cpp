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
    std::cout << "\n--- Guest Menu ---\n"
              << "1. Login\n"
              << "2. Register\n"
              << "0. Exit\n";

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
    std::cout << "\n--- User Menu ---\n"
              << "1. List apartments\n"
              << "2. Search / Filters\n"
              << "3. Booking menu\n"
              << "4. Logout\n"
              << "0. Exit\n";

    switch (Input::GetInt("> "))
    {
    case 1:
        UI::ApartmentMenu::ListAll(service);
        break;
    case 2:
        UI::ApartmentMenu::Search(service);
        break;
    case 3:
        std::cout << "\n--- Booking Actions ---\n"
                  << "1. Make booking\n"
                  << "2. List my bookings\n"
                  << "3. Pay\n"
                  << "4. Cancel\n"
                  << "5. Export receipt\n";

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

    case 4:
        service.Logout();
        break;
    case 0:
        exit(0);
    }
}

void UI::MainMenu::ShowAdminMenu(AppService& service)
{
    std::cout << "\n=== MAIN MENU (Admin) ===\n"
              << "1. Admin panel\n"
              << "2. Logout\n"
              << "0. Exit\n";

    int choice = Input::GetInt("Choose: ");

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
        std::cout << "Invalid.\n";
    }
}
