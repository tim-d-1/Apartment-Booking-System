#include "BookingMenu.h"
#include "Input.h"
#include "AppService.h"

void UI::BookingMenu::BookApartment(AppService& service)
{
    int id = Input::GetInt("Enter apartment ID: ");
    
    std::string fromStr = Input::GetLine("Enter start date (YYYY-MM-DD): "),
                toStr = Input::GetLine("Enter end date (YYYY-MM-DD): ");

    try
    {
        Date from = Date::FromString(fromStr);
        Date to = Date::FromString(toStr);

        auto breakdown = service.CalculateBookingBreakdown(id, from, to);

        std::cout << "\n=== PRICE BREAKDOWN ===\n";

        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;

            std::cout << HelperFuncs::SeasonName(static_cast<Season>(s)) << ": "
                      << breakdown.daysPerSeason[s] << " days × "
                      << breakdown.dailyPrice[s]
                      << "/day = " << breakdown.pricePerSeason[s] << " UAH\n";
        }

        int totalDays = Date::DaysBetween(from, to);
        float avgDaily = breakdown.total / totalDays;
        float weeklyEquivalent = avgDaily * 7;

        std::cout << "\nTotal: " << breakdown.total << " UAH"
                  << " (" << avgDaily << "/day, " << weeklyEquivalent
                  << "/week)\n\n";

        char confirm;
        std::cin >> confirm;


        if (confirm == 'y')
        {
            auto booking = service.CreateBooking(id, from, to);
            std::cout << "Booking created. ID: " << booking->GetId() << "\n";
        }
        else
        {
            std::cout << "Booking cancelled.\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

void UI::BookingMenu::ListMyBookings(AppService& service)
{
    auto bookings = service.GetUserBookings();
    if (bookings.empty())
    {
        std::cout << "No bookings found.\n";
        return;
    }

    for (auto b : bookings)
    {
        std::cout << "\n=== BOOKING " << b->GetId() << " ===\n";
        std::cout << "Apartment: " << b->GetApartmentId() << "\n"
                  << "Period: " << b->GetFrom().ToString() << " -> "
                  << b->GetTo().ToString() << "\n"
                  << "Paid: " << (b->IsPaid() ? "yes" : "no") << "\n"
                  << "Refunded: " << (b->IsRefunded() ? "yes" : "no") << "\n";

        auto breakdown = service.CalculateBookingBreakdown(
                b->GetApartmentId(), b->GetFrom(), b->GetTo());

        std::cout << "--- Breakdown ---\n";
        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;
            std::cout << HelperFuncs::SeasonName((Season)s) << ": "
                      << breakdown.daysPerSeason[s] << " days @ "
                      << breakdown.dailyPrice[s] << " = "
                      << breakdown.pricePerSeason[s] << "\n";
        }

        std::cout << "Total: " << breakdown.total << "\n";
    }
}

void UI::BookingMenu::PayForBooking(AppService& service)
{
    if (!service.IsAuthenticated())
    {
        std::cout << "You must be logged in.\n";
        return;
    }

    int id = Input::GetInt("Enter Booking ID to pay: ");

    try
    {
        auto booking = service.GetBookingById(id);
        if (!booking)
        {
            std::cout << "Booking not found.\n";
            return;
        }

        if (booking->IsPaid())
        {
            std::cout << "This booking is already paid.\n";
            return;
        }

        service.PayBooking(id);

        std::cout << "\nPayment successful.\n"
                  << "Booking " << booking->GetId()
                  << " | Total: " << booking->GetTotalPayment()
                  << " | Status: PAID\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Payment failed: " << ex.what() << "\n";
    }
}


void UI::BookingMenu::CancelBooking(AppService& service)
{
    int id = Input::GetInt("Booking ID: "); 

    try
    {
        service.CancelBooking(id);
        std::cout << "Booking cancelled.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Error: " << ex.what() << "\n";
    }
}

void UI::BookingMenu::ExportReceipt(AppService& service)
{
    int id = Input::GetInt("Booking ID: ");
    std::string file = Input::GetLine("Output file name: ");

    try
    {
        auto booking = service.GetBookingById(id);
        if (!booking)
            throw std::runtime_error("Booking not found");

        auto breakdown = service.CalculateBookingBreakdown(
                booking->GetApartmentId(), booking->GetFrom(),
                booking->GetTo());

        std::ofstream f(file);
        if (!f.is_open())
            throw std::runtime_error("Failed to open output file");

        f << "===== BOOKING RECEIPT =====\n";
        f << "Booking ID: " << booking->GetId() << "\n";
        f << "Apartment ID: " << booking->GetApartmentId() << "\n";
        f << "User ID: " << booking->GetUserId() << "\n";
        f << "From: " << booking->GetFrom().ToString() << "\n";
        f << "To: " << booking->GetTo().ToString() << "\n";
        f << "\n--- Breakdown by season ---\n";

        for (int s = 0; s < 4; ++s)
        {
            if (breakdown.daysPerSeason[s] == 0)
                continue;

            f << HelperFuncs::SeasonName(static_cast<Season>(s)) << ": "
              << breakdown.daysPerSeason[s] << " days * "
              << breakdown.dailyPrice[s] << " = " << breakdown.pricePerSeason[s]
              << " UAH\n";
        }

        int totalDays = Date::DaysBetween(booking->GetFrom(), booking->GetTo());
        float avgDaily = breakdown.total / totalDays;
        float weeklyEquivalent = avgDaily * 7;

        f << "\nTotal: " << breakdown.total << " UAH\n";
        f << "Avg daily: " << avgDaily << " UAH/day\n";
        f << "Weekly equivalent: " << weeklyEquivalent << " UAH/week\n";
        f << "Paid: " << (booking->IsPaid() ? "yes" : "no") << "\n";
        f << "Refunded: " << (booking->IsRefunded() ? "yes" : "no") << "\n";

        std::cout << "Receipt saved.\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Failed: " << ex.what() << "\n";
    }
}
