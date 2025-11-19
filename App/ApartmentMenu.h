#pragma once

#include "AppService.h"

namespace UI
{
class ApartmentMenu
{
  public:
    static void Show(AppService& service);

  private:
    static void ListAll(AppService& service);
    static void Filter(AppService& service);
    static void Sort(AppService& service);
    static void ViewDetails(AppService& service);
};
} // namespace UI
