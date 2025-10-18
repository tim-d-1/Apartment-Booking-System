#pragma once
#include "Db.h"
#include "Apartment.h"
#include "User.h"
#include "Admin.h"

class AppService {
    Db* db;
    std::shared_ptr<User> currentUser;
    Container<Apartment> userApartments;
public:
    AppService() : db(Db::GetInstance()), currentUser(nullptr) {}

    bool Login(const std::string& username, const std::string& password) {
        auto user = db->Search<User>([&](auto u) { return u->GetUsername() == username; });
        if (user && user->Authenticate(password)) {
            currentUser = user;
            userApartments = GetUserApartments();
            return true;
        }

        auto admin = db->Search<Admin>([&](auto a) { return a->GetUsername() == username; });
        if (admin && admin->Authenticate(password)) {
            currentUser = admin;
            return true;
        }

        return false;
    }

    void Logout() { currentUser.reset(); }

    std::shared_ptr<User> GetCurrentUser() const { return currentUser; }

    bool IsAuthenticated() const { return currentUser != nullptr; }

    bool IsAdmin() const {
        return std::dynamic_pointer_cast<Admin>(currentUser) != nullptr;
    }

    void RegisterUser(const std::string& username, const std::string& password) {
        if (!IsAdmin()) {
            std::cerr << "Permission denied: only admin can register new users\n";
            return;
        }
        db->Add(std::make_shared<User>(-1, username, password));
    }

    void ChangePassword(const std::string& oldPass, const std::string& newPass) {
        if (!IsAuthenticated()) {
            std::cerr << "Not logged in\n";
            return;
        }
        currentUser->ChangePassword(oldPass, newPass);
    }

    void ListUsers() {
        if (!IsAdmin()) {
            std::cerr << "Permission denied: only admin can view users\n";
            return;
        }
        for (auto& u : db->GetAll<User>()) {
            std::cout << "User: " << u->GetUsername() << "\n";
        }
    }

    void AddApartment(const std::string& city, int capacity,
        const std::vector<std::string>& livingConditions,
        const std::vector<std::string>& bookingConditions,
        const std::vector<std::string>& amenities,
        const std::vector<float>& seasonalPricingPerWeek)
    {
        if (!IsAuthenticated()) {
            std::cerr << "Not logged in\n";
            return;
        }

        auto apt = std::make_shared<Apartment>(
            -1, city, capacity,
            livingConditions,
            bookingConditions,
            amenities,
            seasonalPricingPerWeek,
            currentUser
        );

        db->Add(apt);
    }

    void RemoveApartment(int id) {
        if (!currentUser) return;
        db->Remove<Apartment>([&](std::shared_ptr<Apartment> ap) {
            return !IsAdmin() ? ap->GetSellerId() == currentUser->GetId() ? ap->GetId() == id: false: ap->GetId() == id;
            });
    }

    Container<Apartment> GetUserApartments() const {
        if (!currentUser) return {};
        return db->SearchAll<Apartment>([&](std::shared_ptr<Apartment> ap) {
            return ap->GetSellerId() == currentUser->GetId();
            });
    }

    Container<Apartment> GetAllApartments() const {
        return db->GetAll<Apartment>();
    }
};
