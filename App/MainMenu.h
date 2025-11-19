#pragma once

class AppService;

namespace UI
{
class MainMenu
{
  public:
    static void Run(AppService& service);
    static void ShowUserMenu(AppService& service);
    static void ShowAdminMenu(AppService& service);
    static void ShowGuestMenu(AppService& service);
};
} // namespace UI
