#pragma once
#include "Admin.h"
#include "Apartment.h"
#include "Db.h"
#include "User.h"

#include <memory>
#include <optional>
#include <stdexcept>

class AppService
{
    Db& db;
    std::shared_ptr<User> currentUser;

  public:
    AppService() : db(Db::GetInstance()), currentUser(nullptr)
    {
        db.LoadContainer<User>();
        db.LoadContainer<Admin>();
        db.LoadContainer<Apartment>();
    }

    // ---------------- AUTH ----------------

    bool Login(const std::string& username, const std::string& password)
    {
        auto user = db.Search<User>([&](auto u)
                                    { return u->GetUsername() == username; });

        if (user && user->Authenticate(password))
        {
            currentUser = user;
            return true;
        }

        auto admin = db.Search<Admin>([&](auto a)
                                      { return a->GetUsername() == username; });

        if (admin && admin->Authenticate(password))
        {
            currentUser = admin;
            return true;
        }

        return false;
    }

    void Logout()
    {
        currentUser.reset();
    }

    bool IsAuthenticated() const
    {
        return currentUser != nullptr;
    }

    bool IsAdmin() const
    {
        return std::dynamic_pointer_cast<Admin>(currentUser) != nullptr;
    }

    std::shared_ptr<User> GetCurrentUser() const
    {
        return currentUser;
    }

    // -------------- USER MANAGEMENT --------------

    void RegisterUser(const std::string& username, const std::string& password)
    {
        if (username.empty())
            throw std::invalid_argument("Username cannot be empty");
        if (!HelperFuncs::isValidPassword(password))
            throw std::invalid_argument("Invalid password format");

        auto exists = db.Search<User>([&](auto u)
                                      { return u->GetUsername() == username; });

        if (exists)
            throw std::runtime_error("User already exists");

        db.Add(std::make_shared<User>(-1, username, password));
    }

    void ChangePassword(const std::string& oldPass, const std::string& newPass)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        if (!HelperFuncs::isValidPassword(newPass))
            throw std::invalid_argument("Invalid new password");

        currentUser->ChangePassword(oldPass, newPass);
        db.SaveContainer<User>();
    }

    // Admin-only
    std::vector<std::string> ListUsers() const
    {
        if (!IsAdmin())
            throw std::runtime_error("Permission denied");

        std::vector<std::string> result;

        for (auto& u : db.GetAll<User>())
            result.push_back(u->GetUsername());

        return result;
    }

    // -------------- APARTMENT CRUD -------------

    void AddApartment(const std::string& city, int capacity,
                      const std::vector<std::string>& livingConditions,
                      const std::vector<std::string>& bookingConditions,
                      const std::vector<std::string>& amenities,
                      const std::vector<float>& seasonalPricingPerWeek)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        if (city.empty())
            throw std::invalid_argument("City cannot be empty");
        if (capacity <= 0)
            throw std::invalid_argument("Capacity must be > 0");
        if (seasonalPricingPerWeek.size() != 4)
            throw std::invalid_argument("Exactly 4 seasonal prices required");

        for (float p : seasonalPricingPerWeek)
            if (p < MINIMAL_PRICE_PER_WEEK)
                throw std::invalid_argument("Season price below minimum");

        auto apt = std::make_shared<Apartment>(
                -1, city, capacity, livingConditions, bookingConditions,
                amenities, seasonalPricingPerWeek, currentUser->GetId());

        db.Add(apt);
    }

    bool RemoveApartment(int id)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        if (IsAdmin())
        {
            return db.Remove<Apartment>([&](auto ap)
                                        { return ap->GetId() == id; });
        }

        return db.Remove<Apartment>(
                [&](auto ap)
                {
                    return ap->GetSellerId() == currentUser->GetId() &&
                           ap->GetId() == id;
                });
    }

    bool UpdateApartmentCity(int id, const std::string& newCity)
    {
        if (newCity.empty())
            throw std::invalid_argument("City cannot be empty");

        return db.Update<Apartment>([&](auto ap) { return ap->GetId() == id; },
                                    [&](auto ap) { ap->SetCity(newCity); });
    }

    bool UpdateApartmentCapacity(int id, int newCapacity)
    {
        if (newCapacity <= 0)
            throw std::invalid_argument("Capacity must be > 0");

        return db.Update<Apartment>([&](auto ap) { return ap->GetId() == id; },
                                    [&](auto ap)
                                    { ap->EditCapacity(newCapacity); });
    }

    bool UpdateSeasonPrice(int id, int season, float price)
    {
        return db.Update<Apartment>(
                [&](auto ap) { return ap->GetId() == id; }, [&](auto ap)
                { ap->EditSeasonalPricingPerWeek(season, price); });
    }

    // -------------- GETTERS -------------

    Container<Apartment> GetAllApartments() const
    {
        return db.GetAll<Apartment>();
    }

    Container<Apartment> GetMyApartments() const
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        return db.SearchAll<Apartment>(
                [&](auto ap)
                { return ap->GetSellerId() == currentUser->GetId(); });
    }

    std::shared_ptr<Apartment> GetApartmentById(int id) const
    {
        return db.Search<Apartment>([&](auto ap) { return ap->GetId() == id; });
    }

    // ---------------- SEARCH / FILTER ----------------

    Container<Apartment> SearchByCity(const std::string& city) const
    {
        return db.SearchAll<Apartment>([&](auto ap)
                                       { return ap->GetCity() == city; });
    }

    Container<Apartment> SearchByCapacity(int minCap) const
    {
        return db.SearchAll<Apartment>([&](auto ap)
                                       { return ap->GetCapacity() >= minCap; });
    }

    Container<Apartment> SearchByMaxPrice(float maxPrice, int season = 0) const
    {
        return db.SearchAll<Apartment>(
                [&](auto ap)
                { return ap->GetSeasonPrice(season) <= maxPrice; });
    }

    Container<Apartment> Filter(
            const std::function<bool(std::shared_ptr<Apartment>)>& pred) const
    {
        return db.SearchAll<Apartment>(pred);
    }

    Container<Apartment> SortByPrice(bool ascending = true,
                                     int season = 0) const
    {
        auto results = db.GetAll<Apartment>();
        std::sort(results.begin(), results.end(),
                  [&](auto a, auto b)
                  {
                      return ascending ? a->GetSeasonPrice(season) <
                                                 b->GetSeasonPrice(season)
                                       : a->GetSeasonPrice(season) >
                                                 b->GetSeasonPrice(season);
                  });
        return results;
    }

    Container<Apartment> SortByCapacity(bool ascending = true) const
    {
        auto results = db.GetAll<Apartment>();
        std::sort(results.begin(), results.end(),
                  [&](auto a, auto b)
                  {
                      return ascending ? a->GetCapacity() < b->GetCapacity()
                                       : a->GetCapacity() > b->GetCapacity();
                  });
        return results;
    }
};
