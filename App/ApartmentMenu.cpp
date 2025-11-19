#include "ApartmentMenu.h"
#include "Input.h"
#include <iostream>

using namespace UI;

void ApartmentMenu::Show(AppService& service)
{
    while (true)
    {
        std::cout << "\n=== APARTMENTS ===\n"
                  << "1. List all\n"
                  << "2. Filter\n"
                  << "3. Sort\n"
                  << "4. View details\n"
                  << "0. Back\n";

        int choice = Input::GetInt("Choose: ");

        switch (choice)
        {
        case 1:
            ListAll(service);
            break;
        case 2:
            Filter(service);
            break;
        case 3:
            Sort(service);
            break;
        case 4:
            ViewDetails(service);
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid.\n";
        }
    }
}

void ApartmentMenu::ListAll(AppService& service)
{
    auto list = service.GetAllApartments();

    if (list.empty())
    {
        std::cout << "No apartments found.\n";
        return;
    }

    for (auto ap : list)
    {
        std::cout << ap->GetId() << " | " << ap->GetCity()
                  << " | Capacity: " << ap->GetCapacity()
                  << " | Price(winter): "
                  << ap->GetDailyPrice((int)Season::Winter) << "\n";
    }
}

void ApartmentMenu::Filter(AppService& service)
{
    std::cout << "\nFilter:\n"
              << "1. By city\n"
              << "2. By min capacity\n"
              << "3. By max price (season)\n"
              << "0. Back\n";

    int ch = Input::GetInt("Choose: ");

    switch (ch)
    {
    case 1:
    {
        std::string city = Input::GetLine("City: ");
        auto res = service.SearchByCity(city);
        if (res.empty())
            std::cout << "No results.\n";
        else
            for (auto ap : res)
                std::cout << ap->GetId() << " | " << ap->GetCity() << "\n";
        break;
    }

    case 2:
    {
        int cap = Input::GetInt("Min capacity: ");
        auto res = service.SearchByCapacity(cap);
        if (res.empty())
            std::cout << "No results.\n";
        else
            for (auto ap : res)
                std::cout << ap->GetId() << " | cap: " << ap->GetCapacity()
                          << "\n";
        break;
    }

    case 3:
    {
        int season = Input::GetInt("Season(0-3): ");
        float maxPrice = Input::GetFloat("Max price per season: ");
        auto res = service.SearchByMaxPrice(maxPrice, season);
        if (res.empty())
            std::cout << "No results.\n";
        else
            for (auto ap : res)
                std::cout << ap->GetId()
                          << " | price: " << ap->GetDailyPrice(season) << "\n";
        break;
    }

    default:
        return;
    }
}

void ApartmentMenu::Sort(AppService& service)
{
    std::cout << "\nSort:\n"
              << "1. By price ascending\n"
              << "2. By price descending\n"
              << "3. By capacity ascending\n"
              << "4. By capacity descending\n"
              << "0. Back\n";

    int ch = Input::GetInt("Choose: ");

    switch (ch)
    {
    case 1:
    {
        auto res = service.SortByPrice(true);
        for (auto ap : res)
            std::cout << ap->GetId() << " | " << ap->GetDailyPrice(0) << "\n";
        break;
    }
    case 2:
    {
        auto res = service.SortByPrice(false);
        for (auto ap : res)
            std::cout << ap->GetId() << " | " << ap->GetDailyPrice(0) << "\n";
        break;
    }
    case 3:
    {
        auto res = service.SortByCapacity(true);
        for (auto ap : res)
            std::cout << ap->GetId() << " | cap: " << ap->GetCapacity() << "\n";
        break;
    }
    case 4:
    {
        auto res = service.SortByCapacity(false);
        for (auto ap : res)
            std::cout << ap->GetId() << " | cap: " << ap->GetCapacity() << "\n";
        break;
    }
    default:
        return;
    }
}
