#pragma once
#include "Config.h"
#include "Db.h"

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
        db.LoadContainer<Booking>();
    }

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

    Container<User> GetAllUsers()
    {
        return db.GetAll<User>();
    }

    bool RemoveApartmentAdmin(int id)
    {
        if (!IsAdmin())
            return false;

        return db.Remove<Apartment>([&](auto a) { return a->GetId() == id; });
    }

    Container<Booking> GetAllBookings()
    {
        return db.GetAll<Booking>();
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

    std::vector<std::string> ListUsers() const
    {
        if (!IsAdmin())
            throw std::runtime_error("Permission denied");

        std::vector<std::string> result;

        for (auto& u : db.GetAll<User>())
            result.push_back(u->GetUsername());

        return result;
    }

    bool IsApartmentAvailable(int apartmentId, const Date& from,
                              const Date& to) const
    {
        if (!(from < to))
            throw std::invalid_argument("Invalid date range");

        auto bookingsFor = db.SearchAll<Booking>(
                [&](std::shared_ptr<Booking> b)
                { return b->GetApartmentId() == apartmentId; });

        for (const auto& b : bookingsFor)
        {
            if (b->IsRefunded())
                continue;
            if (RangesOverlap(from, to, b->GetFrom(), b->GetTo()))
                return false;
        }
        return true;
    }

    static int DaysBetween(const Date& start, const Date& end)
    {
        Date cur = start;
        int cnt = 0;
        auto nextDay = [](Date d)
        {
            d.d++;
            static int mdays[] = {0,  31, 28, 31, 30, 31, 30,
                                  31, 31, 30, 31, 30, 31};
            int maxd = mdays[d.m];
            if (d.m == 2)
            {
                bool leap =
                        ((d.y % 4 == 0 && d.y % 100 != 0) || (d.y % 400 == 0));
                if (leap)
                    maxd = 29;
            }
            if (d.d > maxd)
            {
                d.d = 1;
                d.m++;
                if (d.m > 12)
                {
                    d.m = 1;
                    d.y++;
                }
            }
            return d;
        };

        while (cur < end)
        {
            cur = nextDay(cur);
            cnt++;
            if (cnt > 10000)
                break;
        }
        return cnt;
    }

    float CalculateBookingPrice(std::shared_ptr<Apartment> apt, Date from,
                                Date to)
    {
        if (!apt)
            throw std::runtime_error("Apartment not found");
        if (!(from < to))
            throw std::runtime_error("Invalid date range");

        float total = 0.f;

        for (Date d = from; d < to; d = d.NextDay())
        {
            Season s = SeasonFromDate(d);
            float price = apt->GetDailyPrice((int)s);

            if (price < MINIMAL_PRICE_PER_DAY)
                throw std::runtime_error("Daily price below allowed minimum");

            total += price;
        }

        return total;
    }


    std::shared_ptr<Apartment> AddApartment(const std::string& city, int capacity,
                      const std::vector<std::string>& livingConditions,
                      const std::vector<std::string>& bookingConditions,
                      const std::vector<std::string>& amenities,
                      const std::array<float, 4>& dailyPrice)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        if (city.empty())
            throw std::invalid_argument("City cannot be empty");
        if (capacity <= 0)
            throw std::invalid_argument("Capacity must be > 0");

        for (float p : dailyPrice)
            if (p < MINIMAL_PRICE_PER_DAY)
                throw std::invalid_argument(
                        "Season price below minimum\nMin: " +
                        std::to_string(MINIMAL_PRICE_PER_DAY));

        auto apt = std::make_shared<Apartment>(
                -1, city, capacity, livingConditions, bookingConditions,
                amenities, dailyPrice, currentUser->GetId());

        db.Add(apt);
        return apt;
    }

    std::shared_ptr<Booking> CreateBooking(int apartmentId, const Date& from, const Date& to)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");
        if (!(from < to))
            throw std::invalid_argument("Invalid date range");

        auto aptPtr = db.Search<Apartment>(
                [&](auto a) { return a->GetId() == apartmentId; });
        if (!aptPtr)
            throw std::runtime_error("Apartment not found");

        if (!IsApartmentAvailable(apartmentId, from, to))
            throw std::runtime_error("Apartment is not available for the "
                                     "requested dates");

        float price = CalculateBookingPrice(aptPtr, from, to);

        auto booking =
                std::make_shared<Booking>(-1, apartmentId, currentUser->GetId(),
                                          from, to, price, false, false);
        db.Add(booking);
        return booking;
    }

    Container<Booking> GetUserBookings() const
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");
        return db.SearchAll<Booking>(
                [&](auto b) { return b->GetUserId() == currentUser->GetId(); });
    }

    Container<Booking> GetBookingsForApartment(int apartmentId) const
    {
        return db.SearchAll<Booking>(
                [&](auto b) { return b->GetApartmentId() == apartmentId; });
    }

    Container<Booking> GetAllBookings() const
    {
        if (!IsAdmin())
            throw std::runtime_error("Permission denied");
        return db.GetAll<Booking>();
    }

    bool CancelBooking(int bookingId)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        auto booking = db.Search<Booking>([&](auto b)
                                          { return b->GetId() == bookingId; });
        if (!booking)
            throw std::runtime_error("Booking not found");

        if (booking->GetUserId() != currentUser->GetId() && !IsAdmin())
            throw std::runtime_error("Permission denied to cancel this "
                                     "booking");

        if (booking->IsPaid())
        {
            db.Update<Booking>([&](auto b) { return b->GetId() == bookingId; },
                               [&](auto b) { b->MarkRefunded(); });
        }
        else
        {
            db.Remove<Booking>([&](auto b) { return b->GetId() == bookingId; });
        }
        return true;
    }

    std::shared_ptr<Booking> GetBookingById(int id) const
    {
        return db.Search<Booking>([&](auto b) { return b->GetId() == id; });
    }


    bool PayBooking(int bookingId)
    {
        if (!IsAuthenticated())
            throw std::runtime_error("Not logged in");

        auto booking = db.Search<Booking>([&](auto b)
                                          { return b->GetId() == bookingId; });
        if (!booking)
            throw std::runtime_error("Booking not found");
        if (booking->GetUserId() != currentUser->GetId())
            throw std::runtime_error("Permission denied");

        db.Update<Booking>([&](auto b) { return b->GetId() == bookingId; },
                           [&](auto b) { b->MarkPaid(); });
        return true;
    }

    bool RefundBooking(int bookingId)
    {
        if (!IsAuthenticated() || !IsAdmin())
            throw std::runtime_error("Permission denied");

        auto booking = db.Search<Booking>([&](auto b)
                                          { return b->GetId() == bookingId; });
        if (!booking)
            throw std::runtime_error("Booking not found");

        if (!booking->IsPaid())
            throw std::runtime_error("Booking is not paid");

        db.Update<Booking>([&](auto b) { return b->GetId() == bookingId; },
                           [&](auto b) { b->MarkRefunded(); });
        return true;
    }

    void ExportBookingToFile(int bookingId, const std::string& filename) const
    {
        auto b = GetBookingById(bookingId);
        if (!b)
            throw std::runtime_error("Booking not found");
        std::ofstream f(filename);
        f << "Booking ID: " << b->GetId() << "\n";
        f << "Apartment ID: " << b->GetApartmentId() << "\n";
        f << "User ID: " << b->GetUserId() << "\n";
        f << "From: " << b->GetFrom().ToString() << "\n";
        f << "To: " << b->GetTo().ToString() << "\n";
        f << "Total: " << b->GetTotalPayment() << "\n";
        f << "Paid: " << (b->IsPaid() ? "yes" : "no") << "\n";
        f << "Refunded: " << (b->IsRefunded() ? "yes" : "no") << "\n";
        f.close();
    }

    float CalculateBookingPrice(int apartmentId, Date from, Date to)
    {
        auto apt = db.Search<Apartment>([&](auto a)
                                        { return a->GetId() == apartmentId; });

        if (!apt)
            throw std::runtime_error("Apartment not found");
        if (!(from < to))
            throw std::runtime_error("Invalid date range");

        float total = 0.f;
        for (Date d = from; d < to; d = d.NextDay())
        {
            Season s = SeasonFromDate(d);
            float price = apt->GetDailyPrice(int(s));

            if (price < 0)
                throw std::runtime_error("Price not set for season");

            total += price;
        }

        return total;
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

    bool UpdateSeasonPrice(int id, Season season, float price)
    {
        return db.Update<Apartment>([&](auto ap) { return ap->GetId() == id; },
                                    [&](std::shared_ptr<Apartment> ap)
                { ap->EditDailyPrice(season, price); });
    }

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
                { return ap->GetDailyPrice(season) <= maxPrice; });
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
                      return ascending ? a->GetDailyPrice(season) <
                                                 b->GetDailyPrice(season)
                                       : a->GetDailyPrice(season) >
                                                 b->GetDailyPrice(season);
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

    Container<Apartment> SearchApartments(Predicate<Apartment> pred)
    {
        return db.SearchAll(pred);
    };

    static Season SeasonFromDate(const Date& d)
    {
        // March (3), April (4), May (5)
        if (d.m >= 3 && d.m <= 5)
            return Season::Spring;

        // June (6), July (7), August (8)
        if (d.m >= 6 && d.m <= 8)
            return Season::Summer;

        // September (9), October (10), November (11)
        if (d.m >= 9 && d.m <= 11)
            return Season::Autumn;

        // December (12), January (1), February (2)
        return Season::Winter;
    }
};
