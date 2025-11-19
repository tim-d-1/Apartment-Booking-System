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

        float price = service.CalculateBookingPrice(service.GetApartmentById(id),
                                                    from, to);

        std::cout << "Total cost: " << price << " UAH\nConfirm? (y/n): ";

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
        std::cout << "Booking " << b->GetId()
                  << " | Apt: " << b->GetApartmentId() << " | "
                  << b->GetFrom().ToString() << " -> " << b->GetTo().ToString()
                  << " | Paid: " << (b->IsPaid() ? "yes" : "no")
                  << " | Refunded: " << (b->IsRefunded() ? "yes" : "no")
                  << "\n";
    }
}

void UI::BookingMenu::PayForBooking(AppService& service)
{
    int id = Input::GetInt("Booking ID: "); 

    try
    {
        service.PayBooking(id);
        std::cout << "Payment successful.\n";
    }
    catch (...)
    {
        std::cout << "Payment failed.\n";
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
    std::cout << "Booking ID: ";
    int id;
    std::cin >> id;

    std::cout << "Output file name: ";
    std::string file;
    std::cin >> file;

    try
    {
        service.ExportBookingToFile(id, file);
        std::cout << "Receipt saved.\n";
    }
    catch (...)
    {
        std::cout << "Failed to export receipt.\n";
    }
}
