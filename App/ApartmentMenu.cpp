#include "ApartmentMenu.h"
#include "Input.h"
#include "Apartment.h"
#include <iostream>

using namespace UI;


void ApartmentMenu::Show(AppService& service)
{
    while (true)
    {
        std::cout << "\n=== КВАРТИРИ ===\n "
                  << "1. Перерахувати всі\n "
                  << "2. Фільтр\n "
                  << "3. Сортувати\n "
                  << "4. Переглянути деталі\n "
                  << "5. Додати квартиру\n "
                  << "6. Редагувати квартиру\n "
                  << "0. Назад\n ";

        int choice = Input::GetInt("Виберіть: ");

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
        case 6:
            EditApartment(service);
            break;
        case 0:
            return;
        default:
            std::cout << "Недійсний.\n ";
        }
    }
}

void ApartmentMenu::AddApartment(AppService& service)
{
    std::cout << "\n--- Додати нову квартиру ---\n ";

    std::string city = Input::GetLine("Місто: ");
    int capacity = Input::GetInt("Ємність: ");

    auto livingCond = Input::GetListInput("Умови проживання ");
    auto bookingCond = Input::GetListInput("Умови бронювання ");
    auto amenities = Input::GetListInput("Зручності (Wi-Fi, басейн тощо) ");

    std::array<float, 4> prices;
    std::cout << "Введіть щоденну ціну для кожного сезону:\n ";
    prices[0] = Input::GetFloat("Весна: ");
    prices[1] = Input::GetFloat("Літо:");
    prices[2] = Input::GetFloat("Осінь: ");
    prices[3] = Input::GetFloat("Зима: ");

    try
    {
        service.AddApartment(city, capacity, livingCond, bookingCond, amenities,
                             prices);
        std::cout << "Квартира успішно додана!\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка додавання квартири: " << ex.what() << "\n ";
    }
}

void ApartmentMenu::EditApartment(AppService& service)
{
    int id = Input::GetInt("Введіть ID квартири для редагування: ");

    auto apt = service.GetApartmentById(id);
    if (!apt)
    {
        std::cout << "Квартира не знайдена.\n ";
        return;
    }

    std::cout << "Редагування квартири " << id << ". Виберіть поле:\n "
              << "1. Місто\n "
              << "2. Ємність\n "
              << "3. Ціна (Зима)\n "
              << "0. Скасувати\n ";

    int choice = Input::GetInt("> ");

    try
    {
        switch (choice)
        {
        case 1:
        {
            std::string newCity = Input::GetLine("Нове місто: ");
            service.UpdateApartmentCity(id, newCity);
            break;
        }
        case 2:
        {
            int newCap = Input::GetInt("Нова ємність: ");
            service.UpdateApartmentCapacity(id, newCap);
            break;
        }
        case 3:
        {
            int season = Input::GetInt("Сезон:\n0. Весна\n1. Літо\n2. "
                                       "Падіння\n3. Зима\n ");
            float newPrice = Input::GetFloat("Нова ціна: ");
            service.UpdateSeasonPrice(id, Season(season), newPrice);
            break;
        }
        case 0:
            return;
        }
        std::cout << "Оновлення успішне.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка оновлення: " << ex.what() << "\n ";
    }
}

void ApartmentMenu::ListAll(AppService& service)
{
    auto list = service.GetAllApartments();

    if (list.empty())
    {
        std::cout << "Квартир не знайдено.\n ";
        return;
    }

    for (auto ap : list)
    {
        std::cout << ap->GetId() << " | " << ap->GetCity()
                  << "| Ємність: " << ap->GetCapacity()
                  << "| Ціна (зима): "
                  << ap->GetDailyPrice((int)Season::Winter) << "\n ";
    }
}

void ApartmentMenu::Filter(AppService& service)
{
    std::cout << "\nФільтр:\n "
              << "1. За містом\n "
              << "2. За мінімальною потужністю\n "
              << "3. За максимальною ціною (сезон)\n "
              << "0. Назад\n ";

    int ch = Input::GetInt("Виберіть: ");

    switch (ch)
    {
    case 1:
    {
        std::string city = Input::GetLine("Місто: ");
        auto res = service.SearchByCity(city);
        if (res.empty())
            std::cout << "Немає результатів.\n ";
        else
            for (auto ap : res)
                std::cout << ap->GetId() << " | " << ap->GetCity() << "\n ";
        break;
    }

    case 2:
    {
        int cap = Input::GetInt("Мінімальна потужність: ");
        auto res = service.SearchByCapacity(cap);
        if (res.empty())
            std::cout << "Немає результатів.\n ";
        else
            for (auto ap : res)
                std::cout << ap->GetId() << "| шапка: " << ap->GetCapacity()
                          << "\n ";
        break;
    }

    case 3:
    {
        int season = Input::GetInt("Сезон (0-3): ");
        float maxPrice = Input::GetFloat("Максимальна ціна за сезон: ");
        auto res = service.SearchByMaxPrice(maxPrice, season);
        if (res.empty())
            std::cout << "Немає результатів.\n ";
        else
            for (auto ap : res)
                std::cout << ap->GetId()
                          << "| ціна: " << ap->GetDailyPrice(season) << "\n ";
        break;
    }

    default:
        return;
    }
}

void ApartmentMenu::Sort(AppService& service)
{
    std::cout << "\nСортувати:\n "
              << "1. За зростанням ціни\n "
              << "2. За спадною ціною\n "
              << "3. За зростанням ємності\n "
              << "4. За ємністю спадання\n "
              << "0. Назад\n ";

    int ch = Input::GetInt("Виберіть: ");

    switch (ch)
    {
    case 1:
    {
        auto res = service.SortByPrice(true);
        for (auto ap : res)
            std::cout << ap->GetId() << " | " << ap->GetDailyPrice(0) << "\n ";
        break;
    }
    case 2:
    {
        auto res = service.SortByPrice(false);
        for (auto ap : res)
            std::cout << ap->GetId() << " | " << ap->GetDailyPrice(0) << "\n ";
        break;
    }
    case 3:
    {
        auto res = service.SortByCapacity(true);
        for (auto ap : res)
            std::cout << ap->GetId() << "| шапка: " << ap->GetCapacity() << "\n ";
        break;
    }
    case 4:
    {
        auto res = service.SortByCapacity(false);
        for (auto ap : res)
            std::cout << ap->GetId() << "| шапка: " << ap->GetCapacity() << "\n ";
        break;
    }
    default:
        return;
    }
}

void UI::ApartmentMenu::ViewDetails(AppService& service)
{
    int id = Input::GetInt("Введіть ID квартири: ");

    auto ap = service.GetApartmentById(id);
    if (!ap)
    {
        std::cout << "Не знайдено.\n ";
        return;
    }

    std::cout << "\n=== КВАРТИРА " << id << "===\n ";
    std::cout << "Місто: " << ap->GetCity() << "\n ";
    std::cout << "Ємність: " << ap->GetCapacity() << "\n ";
    std::cout << "ID власника: " << ap->GetSellerId() << "\n ";

    std::cout << "\n--- Умови життя ---\n ";
    for (auto& s : ap->GetLivingConditions())
        std::cout << "- " << s << "\n ";

    std::cout << "\n--- Умови бронювання ---\n ";
    for (auto& s : ap->GetBookingConditions())
        std::cout << "- " << s << "\n ";

    std::cout << "\n--- Зручності ---\n ";
    for (auto& s : ap->GetAmenities())
        std::cout << "- " << s << "\n ";

    std::cout << "\n--- Ціни (щодня / щотижня) ---\n ";
    for (int s = 0; s < 4; ++s)
    {
        float daily = ap->GetDailyPrice(s);
        float weekly = daily * 7;
        std::cout << HelperFuncs::SeasonName((Season)s) << ": " << daily
                  << " / " << weekly << "грн\n ";
    }
}
