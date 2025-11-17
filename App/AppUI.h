#pragma once
#include "AppService.h"
#include <iostream>
#include <limits>

class AppService;

class AppUI
{
    AppService service;

  public:
    void Run()
    {
        bool running = true;
        while (running)
        {
            if (!service.IsAuthenticated())
                ShowLoginMenu();
            else if (service.IsAdmin())
                ShowAdminMenu();
            else
                ShowUserMenu();
        }
    }

  private:
    void ShowLoginMenu();
    void ShowUserMenu();
    void ShowAdminMenu();
};
