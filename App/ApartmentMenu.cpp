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
                  << "5. Add Apartment\n"
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
        case 5:
            AddApartment(service);
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid.\n";
        }
    }
}

void ApartmentMenu::AddApartment(AppService& service)
{
    std::cout << "\n--- Add New Apartment ---\n";

    std::string city = Input::GetLine("City: ");
    int capacity = Input::GetInt("Capacity: ");

    auto livingCond = Input::GetListInput("Living Conditions");
    auto bookingCond = Input::GetListInput("Booking Conditions");
    auto amenities = Input::GetListInput("Amenities (WiFi, Pool, etc.)");

    std::array<float, 4> prices;
    std::cout << "Enter daily price for each season:\n";
    prices[0] = Input::GetFloat("Spring: ");
    prices[1] = Input::GetFloat("Summer: ");
    prices[2] = Input::GetFloat("Autumn: ");
    prices[3] = Input::GetFloat("Winter: ");

    try
    {
        service.AddApartment(city, capacity, livingCond, bookingCond, amenities,
                             prices);
        std::cout << "Apartment added successfully!\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error adding apartment: " << ex.what() << "\n";
    }
}

void ApartmentMenu::EditApartment(AppService& service)
{
    int id = Input::GetInt("Enter Apartment ID to edit: ");

    auto apt = service.GetApartmentById(id);
    if (!apt)
    {
        std::cout << "Apartment not found.\n";
        return;
    }

    std::cout << "Editing Apartment " << id << ". Choose field:\n"
              << "1. City\n"
              << "2. Capacity\n"
              << "3. Price (Winter)\n"
              << "0. Cancel\n";

    int choice = Input::GetInt("> ");

    try
    {
        switch (choice)
        {
        case 1:
        {
            std::string newCity = Input::GetLine("New City: ");
            service.UpdateApartmentCity(id, newCity);
            break;
        }
        case 2:
        {
            int newCap = Input::GetInt("New Capacity: ");
            service.UpdateApartmentCapacity(id, newCap);
            break;
        }
        case 3:
        {
            int season = Input::GetInt("Season:\n0. Spring\n1. Summer\n2. "
                                       "Fall\n3. Winter\n");
            float newPrice = Input::GetFloat("New Price: ");
            service.UpdateSeasonPrice(id, Season(season), newPrice);
            break;
        }
        case 0:
            return;
        }
        std::cout << "Update successful.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Update failed: " << ex.what() << "\n";
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
