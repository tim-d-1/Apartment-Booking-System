#pragma once
#include "Config.h"
#include "Model.h"
#include <array>
#include <sstream>
#include <stdexcept>

class Apartment : public Model
{
    std::string city;
    std::array<float, 4> seasonalDailyPrice{-1, -1, -1, -1};

    std::vector<std::string> livingConditions;
    std::vector<std::string> bookingConditions;
    std::vector<std::string> amenities;

    int capacity{};
    int sellerId;

  public:
    Apartment() : Model(), capacity(0), sellerId(0)
    {
    }

    Apartment(int id, const std::string& city, int capacity,
              const std::vector<std::string>& livingConditions,
              const std::vector<std::string>& bookingConditions,
              const std::vector<std::string>& amenities,
              const std::array<float, 4>& dailyPrice, int sellerId)
        : Model(id), city(city), seasonalDailyPrice(dailyPrice),
          livingConditions(livingConditions),
          bookingConditions(bookingConditions), amenities(amenities),
          capacity(capacity), sellerId(sellerId)
    {
    }

    Apartment(const Apartment& other)
        : Model(other.id), city(other.city),
          seasonalDailyPrice(other.seasonalDailyPrice),
          livingConditions(other.livingConditions),
          bookingConditions(other.bookingConditions),
          amenities(other.amenities), capacity(other.capacity),
          sellerId(other.sellerId)
    {
    }

    Apartment(Apartment&& other) noexcept
        : Model(other.id), city(std::move(other.city)),
          seasonalDailyPrice(std::move(other.seasonalDailyPrice)),
          livingConditions(std::move(other.livingConditions)),
          bookingConditions(std::move(other.bookingConditions)),
          amenities(std::move(other.amenities)), capacity(other.capacity),
          sellerId(other.sellerId)
    {
        other.id = 0;
    }

    Apartment& operator=(const Apartment& other)
    {
        if (this != &other)
        {
            id = other.id;
            city = other.city;
            seasonalDailyPrice = other.seasonalDailyPrice;
            livingConditions = other.livingConditions;
            bookingConditions = other.bookingConditions;
            amenities = other.amenities;
            capacity = other.capacity;
            sellerId = other.sellerId;
        }
        return *this;
    }

    Apartment& operator=(Apartment&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            city = std::move(other.city);
            seasonalDailyPrice = std::move(other.seasonalDailyPrice);
            livingConditions = std::move(other.livingConditions);
            bookingConditions = std::move(other.bookingConditions);
            amenities = std::move(other.amenities);
            capacity = other.capacity;
            sellerId = other.sellerId;

            other.id = 0;
        }
        return *this;
    }

    virtual ~Apartment() override
    {
        std::cout << "Квартирний об'єкт " << id << "знищено.\n ";
    }

    void SetCity(const std::string& c)
    {
        if (c.empty())
            throw std::invalid_argument("Місто не може бути порожнім. ");
        city = c;
    }

    void SetCapacity(int c)
    {
        if (c <= 0)
            throw std::invalid_argument("Ємність має бути > 0. ");
        capacity = c;
    }

    void EditDailyPrice(Season season, float price)
    {
        if (price < MINIMAL_PRICE_PER_DAY / 7)
            throw std::runtime_error("Ціна занадто низька ");

        seasonalDailyPrice[(int)season] = price;
    }

    int GetSellerId() const
    {
        return sellerId;
    }

    static void EditVector(std::vector<std::string>& vec,
                           const std::string& value, int index)
    {
        if (index < -1 || index >= (int)vec.size())
            throw std::out_of_range("Недійсний індекс для редагування контейнера. ");

        if (index == -1)
        {
            if (value.empty())
                throw std::invalid_argument("Неможливо додати порожній рядок. ");
            vec.push_back(value);
            return;
        }

        if (value.empty())
        {
            vec.erase(vec.begin() + index);
            return;
        }

        vec[index] = value;
    }

    const std::string& GetCity() const
    {
        return city;
    }
    int GetCapacity() const
    {
        return capacity;
    }

    float GetDailyPrice(int season) const
    {
        if (season < 0 || season >= seasonalDailyPrice.size())
            throw std::out_of_range("Недійсний індекс сезону ");
        return seasonalDailyPrice[season];
    }

    void EditCapacity(int newCapacity)
    {
        if (newCapacity <= 0)
            throw std::invalid_argument("Ємність повинна бути позитивною ");
        capacity = newCapacity;
    }

    void CreateLivingCondition(const std::string& data)
    {
        EditVector(livingConditions, data, -1);
    }

    void UpdateLivingCondition(const std::string& data, int index)
    {
        EditVector(livingConditions, data, index);
    }

    void DeleteLivingCondition(int index)
    {
        EditVector(livingConditions, "", index);
    }

    void CreateBookingCondition(const std::string& data)
    {
        EditVector(bookingConditions, data, -1);
    }
    void UpdateBookingCondition(const std::string& data, int index)
    {
        EditVector(bookingConditions, data, index);
    }
    void DeleteBookingCondition(int index)
    {
        EditVector(bookingConditions, "", index);
    }

    void CreateAmenitites(const std::string& data)
    {
        EditVector(amenities, data, -1);
    }
    void UpdateAmenitites(const std::string& data, int index)
    {
        EditVector(amenities, data, index);
    }
    void DeleteAmenitites(int index)
    {
        EditVector(amenities, "", index);
    }

    std::vector<std::string> GetLivingConditions()
    {
        return livingConditions;
    }

    std::vector<std::string> GetAmenities()
    {
        return amenities;
    }

    std::vector<std::string> GetBookingConditions()
    {
        return bookingConditions;
    }

    virtual std::stringstream Serialize() const override
    {
        std::stringstream ss;

        ss << id << "," << city << "," << capacity << ","
           << HelperFuncs::arrayToString<float, 4>(seasonalDailyPrice, '|', std::to_string)
           << "," << HelperFuncs::vectorToString(livingConditions, '|') << ","
           << HelperFuncs::vectorToString(bookingConditions, '|') << ","
           << HelperFuncs::vectorToString(amenities, '|') << "," << sellerId;

        return ss;
    }

    virtual void Deserialize(std::vector<std::string> params) override
    {
        if (params.size() != 8)
            throw std::runtime_error("Квартира::Очікується десеріалізація 8 "
                                     "поля. ");

        id = std::stoi(params[0]);
        SetCity(params[1]);
        SetCapacity(std::stoi(params[2]));

        seasonalDailyPrice = HelperFuncs::separateLineArr<float, 4>(
                params[3], '|',
                [](const std::string& s) { return std::stof(s); });

        livingConditions =
                HelperFuncs::separateLine<std::string>(params[4], '|');

        bookingConditions =
                HelperFuncs::separateLine<std::string>(params[5], '|');

        amenities = HelperFuncs::separateLine<std::string>(params[6], '|');

        sellerId = std::stoi(params[7]);
    }
};
