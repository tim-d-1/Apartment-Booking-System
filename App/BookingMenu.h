#pragma once

#include "AppService.h"

namespace UI
{
class BookingMenu
{
  public:
    static void BookApartment(AppService& service);
    static void ListMyBookings(AppService& service);
    static void PayForBooking(AppService& service);
    static void CancelBooking(AppService& service);
    static void ExportReceipt(AppService& service);
};
} // namespace UI
