#include "AppService.h"
#include "MainMenu.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    AppService service;
    UI::MainMenu::Run(service);
    return 0;
}
