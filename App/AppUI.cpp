#include "AppUI.h"
#include "AppService.h"
#include <iostream>
#include <cstdlib>

void AppUI::ShowLoginMenu()
{
    std::cout << "\n=== LOGIN MENU ===\n";
    std::cout << "1. Login\n";
    std::cout << "2. Exit\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    if (choice == 1)
    {
        std::string username, password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;

        if (service.Login(username, password))
            std::cout << "Login successful!\n";
        else
            std::cout << "Invalid credentials.\n";
    }
    else if (choice == 2)
    {
        exit(0);
    }
}

void AppUI::ShowUserMenu()
{
    std::cout << "\n=== USER MENU ===\n";
    std::cout << "1. List My Apartments\n";
    std::cout << "2. Add Apartment\n";
    std::cout << "3. Remove Apartment\n";
    std::cout << "4. Change Password\n";
    std::cout << "5. Logout\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
    {
        auto apartments = service.GetUserApartments();
        for (auto& a : apartments)
            std::cout << "Apartment ID: " << a->GetId()
                      << ", City: " << /* TODO: getter for city */ "\n";
        break;
    }
    case 2:
    {
        std::string city;
        int capacity;
        std::cout << "City: ";
        std::cin >> city;
        std::cout << "Capacity: ";
        std::cin >> capacity;

        service.AddApartment(city, capacity, {}, {}, {}, {});
        std::cout << "Apartment added.\n";
        break;
    }
    case 3:
    {
        int id;
        std::cout << "Enter Apartment ID: ";
        std::cin >> id;
        service.RemoveApartment(id);
        break;
    }
    case 4:
    {
        std::string oldPass, newPass;
        std::cout << "Old password: ";
        std::cin >> oldPass;
        std::cout << "New password: ";
        std::cin >> newPass;
        service.ChangePassword(oldPass, newPass);
        break;
    }
    case 5:
        service.Logout();
        break;
    }
}

void AppUI::ShowAdminMenu()
{
    std::cout << "\n=== ADMIN MENU ===\n";
    std::cout << "1. List Users\n";
    std::cout << "2. Add User\n";
    std::cout << "3. View All Apartments\n";
    std::cout << "4. Logout\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        service.ListUsers();
        break;
    case 2:
    {
        std::string username, password;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;
        service.RegisterUser(username, password);
        break;
    }
    case 3:
    {
        auto apartments = service.GetAllApartments();
        for (auto& a : apartments)
            std::cout << "Apartment ID: " << a->GetId() << "\n";
        break;
    }
    case 4:
        service.Logout();
        break;
    }
}
