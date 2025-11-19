#include "AppService.h"
#include "MainMenu.h"

int main()
{
    AppService service;
    UI::MainMenu::Run(service);
    return 0;
}
