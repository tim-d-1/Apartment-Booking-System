#pragma once
class AppService;

namespace UI
{
class AuthMenu
{
  public:
    static void Login(AppService& service);
    static void Register(AppService& service);
    static void Logout(AppService& service);
};
} // namespace UI
