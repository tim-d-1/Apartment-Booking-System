#include "BookingMenu.h"
#include "Input.h"
#include "AppService.h"

void UI::BookingMenu::BookApartment(AppService& service)
{
    int id = Input::GetInt("Введіть ID квартири: ");
    
    std::string fromStr = Input::GetLine("Введіть дату початку (РРРР-ММ-ДД): "),
                toStr = Input::GetLine("Введіть кінцеву дату (РРРР-ММ-ДД): ");

    try
    {
        Date from = Date::FromString(fromStr);
        Date to = Date::FromString(toStr);

        auto breakdown = service.CalculateBookingBreakdown(id, from, to);

        std::cout << "\n=== РОЗПОДІЛ ЦІН ===\n ";

        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;

            std::cout << HelperFuncs::SeasonName(static_cast<Season>(s)) << ": "
                      << breakdown.daysPerSeason[s] << "днів × "
                      << breakdown.dailyPrice[s]
                      << "/день = " << breakdown.pricePerSeason[s] << "грн\n ";
        }

        int totalDays = Date::DaysBetween(from, to);
        float avgDaily = breakdown.total / totalDays;
        float weeklyEquivalent = avgDaily * 7;

        std::cout << "\nВсього: " << breakdown.total << "грн "
                  << " (" << avgDaily << "/день, " << weeklyEquivalent
                  << "/тиждень)\n\n ";

        char confirm;
        std::cin >> confirm;


        if (confirm == 'y')
        {
            auto booking = service.CreateBooking(id, from, to);
            std::cout << "Бронювання створено. ID: " << booking->GetId() << "\n ";
        }
        else
        {
            std::cout << "Бронювання скасовано.\n ";
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}

void UI::BookingMenu::ListMyBookings(AppService& service)
{
    auto bookings = service.GetUserBookings();
    if (bookings.empty())
    {
        std::cout << "Замовлень не знайдено.\n ";
        return;
    }

    for (auto b : bookings)
    {
        std::cout << "\n=== БРОНЮВАННЯ " << b->GetId() << "===\n ";
        std::cout << "Квартира: " << b->GetApartmentId() << "\n "
                  << "Період: " << b->GetFrom().ToString() << " -> "
                  << b->GetTo().ToString() << "\n "
                  << "Оплачено: " << (b->IsPaid() ? "yes" : "немає ") << "\n "
                  << "Повернено: " << (b->IsRefunded() ? "yes" : "немає ") << "\n ";

        auto breakdown = service.CalculateBookingBreakdown(
                b->GetApartmentId(), b->GetFrom(), b->GetTo());

        std::cout << "--- Деталі ---\n ";
        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;
            std::cout << HelperFuncs::SeasonName((Season)s) << ": "
                      << breakdown.daysPerSeason[s] << "днів @ "
                      << breakdown.dailyPrice[s] << " = "
                      << breakdown.pricePerSeason[s] << "\n ";
        }

        std::cout << "Всього: " << breakdown.total << "\n ";
    }
}

void UI::BookingMenu::PayForBooking(AppService& service)
{
    if (!service.IsAuthenticated())
    {
        std::cout << "Ви повинні увійти в систему.\n ";
        return;
    }

    int id = Input::GetInt("Введіть ідентифікатор бронювання для оплати: ");

    try
    {
        auto booking = service.GetBookingById(id);
        if (!booking)
        {
            std::cout << "Бронювання не знайдено.\n ";
            return;
        }

        if (booking->IsPaid())
        {
            std::cout << "Це бронювання вже оплачено.\n ";
            return;
        }

        service.PayBooking(id);

        std::cout << "\nПлатіж успішно.\n "
                  << "Бронювання " << booking->GetId()
                  << "| Всього: " << booking->GetTotalPayment()
                  << "| Статус: ОПЛАЧЕНО\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка платежу: " << ex.what() << "\n ";
    }
}


void UI::BookingMenu::CancelBooking(AppService& service)
{
    int id = Input::GetInt("Ідентифікатор бронювання: "); 

    try
    {
        service.CancelBooking(id);
        std::cout << "Бронювання скасовано.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}

void UI::BookingMenu::ExportReceipt(AppService& service)
{
    int id = Input::GetInt("Ідентифікатор бронювання: ");
    std::string file = Input::GetLine("Ім'я вихідного файлу: ");

    try
    {
        auto booking = service.GetBookingById(id);
        if (!booking)
            throw std::runtime_error("Бронювання не знайдено ");

        auto breakdown = service.CalculateBookingBreakdown(
                booking->GetApartmentId(), booking->GetFrom(),
                booking->GetTo());

        std::ofstream f(file);
        if (!f.is_open())
            throw std::runtime_error("Не вдалося відкрити вихідний файл ");

        f << "===== КВИТАНЦІЯ ПРО БРОНЮВАННЯ =====\n ";
        f << "Ідентифікатор бронювання: " << booking->GetId() << "\n ";
        f << "ID квартири: " << booking->GetApartmentId() << "\n ";
        f << "ID користувача: " << booking->GetUserId() << "\n ";
        f << "Від: " << booking->GetFrom().ToString() << "\n ";
        f << "до: " << booking->GetTo().ToString() << "\n ";
        f << "\n--- Розбивка по сезонах ---\n ";

        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;

            f << HelperFuncs::SeasonName(static_cast<Season>(s)) << ": "
              << breakdown.daysPerSeason[s] << "днів * "
              << breakdown.dailyPrice[s] << " = " << breakdown.pricePerSeason[s]
              << "грн\n ";
        }

        int totalDays = Date::DaysBetween(booking->GetFrom(), booking->GetTo());
        float avgDaily = breakdown.total / totalDays;
        float weeklyEquivalent = avgDaily * 7;

        f << "\nВсього: " << breakdown.total << "грн\n ";
        f << "Середнє щоденне: " << avgDaily << "грн/день\n ";
        f << "Тижневий еквівалент: " << weeklyEquivalent << "грн/тиждень\n ";
        f << "Оплачено: " << (booking->IsPaid() ? "yes" : "немає ") << "\n ";
        f << "Повернено: " << (booking->IsRefunded() ? "yes" : "немає ") << "\n ";

        std::cout << "Квитанцію збережено.\n ";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Помилка: " << ex.what() << "\n ";
    }
}
