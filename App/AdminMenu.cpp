#include "AdminMenu.h"
#include "AppService.h"
#include "Input.h"
#include <iostream>

using namespace UI;

void AdminMenu::Show(AppService& service)
{
    if (!service.IsAdmin())
    {
        std::cout << "Доступ заборонено.\n ";
        return;
    }

    while (true)
    {
        std::cout << "\n=== ПАНЕЛЬ АДМІНІСТРАЦІЇ ===\n "
                  << "1. Перерахувати всі квартири\n "
                  << "2. Зняти квартиру\n "
                  << "3. Перерахуйте всі бронювання\n "
                  << "4. Скасувати бронювання\n "
                  << "5. Список користувачів\n "
                  << "0. Назад\n ";

        int choice = Input::GetInt("Виберіть: ");

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
            std::cout << "Недійсний параметр.\n ";
            break;
        }
    }
}

void AdminMenu::ListAllApartments(AppService& service)
{
    auto apts = service.GetAllApartments();

    if (apts.empty())
    {
        std::cout << "Квартир не знайдено.\n ";
        return;
    }

    for (auto& ap : apts)
    {
        std::cout << "ID: " << ap->GetId() << "| Місто: " << ap->GetCity()
                  << "| Власник: " << ap->GetSellerId() << "\n ";
    }
}

void AdminMenu::RemoveApartment(AppService& service)
{
    int id = Input::GetInt("Введіть ID квартири для видалення: ");

    if (service.RemoveApartmentAdmin(id))
        std::cout << "Видалено.\n ";
    else
        std::cout << "Не знайдено.\n ";
}

void AdminMenu::ListAllBookings(AppService& service)
{
    auto bookings = service.GetAllBookings();

    if (bookings.empty())
    {
        std::cout << "Замовлень не знайдено.\n ";
        return;
    }

    for (auto& b : bookings)
    {
        std::cout << "Бронювання id: " << b->GetId()
                  << "| Квартира: " << b->GetApartmentId()
                  << "| Користувач: " << b->GetUserId() << " | "
                  << b->GetFrom().ToString() << " -> " << b->GetTo().ToString()
                  << "| Оплачено: " << (b->IsPaid() ? "так" : "ні")
                  << "| Повернено: " << (b->IsRefunded() ? "так" : "ні") << "\n ";
    }
}

void AdminMenu::CancelBooking(AppService& service)
{
    int id = Input::GetInt("Ідентифікатор бронювання: ");

    try
    {
        service.CancelBooking(id);
        std::cout << "Скасовано та повернуто кошти.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}

void AdminMenu::ListUsers(AppService& service)
{
    auto users = service.GetAllUsers();

    for (auto& u : users)
    {
        std::cout << "Користувач: " << u->GetUsername()
                  << "| ID: " << u->GetId() << "\n ";
    }
}
