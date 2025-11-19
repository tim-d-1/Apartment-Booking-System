#include "AdminMenu.h"
#include "AppService.h"
#include "Input.h"
#include <iostream>

using namespace UI;

void AdminMenu::Show(AppService& service)
{
    if (!service.IsAdmin())
    {
        std::cout << "Access denied.\n";
        return;
    }

    while (true)
    {
        std::cout << "\n=== ADMIN PANEL ===\n"
                  << "1. List all apartments\n"
                  << "2. Remove apartment\n"
                  << "3. List all bookings\n"
                  << "4. Cancel booking\n"
                  << "5. List users\n"
                  << "0. Back\n";

        int choice = Input::GetInt("Choose: ");

        switch (choice)
        {
        case 1:
            ListAllApartments(service);
            break;
        case 2:
            RemoveApartment(service);
            break;
        case 3:
            ListAllBookings(service);
            break;
        case 4:
            CancelBooking(service);
            break;
        case 5:
            ListUsers(service);
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid option.\n";
            break;
        }
    }
}

void AdminMenu::ListAllApartments(AppService& service)
{
    auto apts = service.GetAllApartments();

    if (apts.empty())
    {
        std::cout << "No apartments found.\n";
        return;
    }

    for (auto& ap : apts)
    {
        std::cout << "ID: " << ap->GetId() << " | City: " << ap->GetCity()
                  << " | Owner: " << ap->GetSellerId() << "\n";
    }
}

void AdminMenu::RemoveApartment(AppService& service)
{
    int id = Input::GetInt("Enter apartment ID to remove: ");

    if (service.RemoveApartmentAdmin(id))
        std::cout << "Removed.\n";
    else
        std::cout << "Not found.\n";
}

void AdminMenu::ListAllBookings(AppService& service)
{
    auto bookings = service.GetAllBookings();

    if (bookings.empty())
    {
        std::cout << "No bookings found.\n";
        return;
    }

    for (auto& b : bookings)
    {
        std::cout << "Booking " << b->GetId()
                  << " | Apt: " << b->GetApartmentId()
                  << " | User: " << b->GetUserId() << " | "
                  << b->GetFrom().ToString() << " -> " << b->GetTo().ToString()
                  << " | Paid: " << b->IsPaid()
                  << " | Refunded: " << b->IsRefunded() << "\n";
    }
}

void AdminMenu::CancelBooking(AppService& service)
{
    int id = Input::GetInt("Booking ID: ");

    try
    {
        service.CancelBooking(id);
        std::cout << "Cancelled & refunded.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

void AdminMenu::ListUsers(AppService& service)
{
    auto users = service.GetAllUsers();

    for (auto& u : users)
    {
        std::cout << "User: " << u->GetUsername() << " | ID: " << u->GetId()
                  << "\n";
    }
}
