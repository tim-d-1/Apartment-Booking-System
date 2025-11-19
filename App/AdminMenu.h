#pragma once

class AppService;

namespace UI
{
class AdminMenu
{
  public:
    static void Show(AppService& service);

  private:
    static void ListAllApartments(AppService& service);
    static void RemoveApartment(AppService& service);
    static void ListAllBookings(AppService& service);
    static void CancelBooking(AppService& service);
    static void ListUsers(AppService& service);
};
} // namespace UI
