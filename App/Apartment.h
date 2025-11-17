#pragma once
#include "Model.h"
#include <sstream>
#include <stdexcept>

const float MINIMAL_PRICE_PER_WEEK = 100;

class Apartment : public Model
{
    std::string city;
    std::vector<float> seasonalPricingPerWeek;
    std::vector<std::string> livingConditions;
    std::vector<std::string> bookingConditions;
    std::vector<std::string> amenities;
    int capacity{};
    int sellerId;

  public:
    Apartment()
        : Model(), seasonalPricingPerWeek(4, -1), capacity(0), sellerId(0)
    {
    }

    Apartment(int id, const std::string& city, int capacity,
              const std::vector<std::string>& living,
              const std::vector<std::string>& booking,
              const std::vector<std::string>& amen,
              const std::vector<float>& pricing, int sellerId)
        : Model(id)
    {
        SetCity(city);
        SetCapacity(capacity);
        SetSeasonalPricing(pricing);
        livingConditions = living;
        bookingConditions = booking;
        amenities = amen;
        this->sellerId = sellerId;
    }

    Apartment(const Apartment& other)
        : Model(other.id), city(other.city),
          seasonalPricingPerWeek(other.seasonalPricingPerWeek),
          livingConditions(other.livingConditions),
          bookingConditions(other.bookingConditions),
          amenities(other.amenities), capacity(other.capacity),
          sellerId(other.sellerId)
    {
    }

    Apartment(Apartment&& other) noexcept
        : Model(other.id), city(std::move(other.city)),
          seasonalPricingPerWeek(std::move(other.seasonalPricingPerWeek)),
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
            seasonalPricingPerWeek = other.seasonalPricingPerWeek;
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
            seasonalPricingPerWeek = std::move(other.seasonalPricingPerWeek);
            livingConditions = std::move(other.livingConditions);
            bookingConditions = std::move(other.bookingConditions);
            amenities = std::move(other.amenities);
            capacity = other.capacity;
            sellerId = other.sellerId;

            other.id = 0;
        }
        return *this;
    }

    virtual ~Apartment() override = default;

    void SetCity(const std::string& c)
    {
        if (c.empty())
            throw std::invalid_argument("City cannot be empty.");
        city = c;
    }

    void SetCapacity(int c)
    {
        if (c <= 0)
            throw std::invalid_argument("Capacity must be > 0.");
        capacity = c;
    }

    void SetSeasonalPricing(const std::vector<float>& p)
    {
        if (p.size() != 4)
            throw std::invalid_argument("Seasonal pricing must contain exactly "
                                        "4 values.");

        for (float price : p)
            if (price < MINIMAL_PRICE_PER_WEEK)
                throw std::invalid_argument("Price is below minimal allowed.");

        seasonalPricingPerWeek = p;
    }

    int GetSellerId() const
    {
        return sellerId;
    }

    static void EditVector(std::vector<std::string>& vec,
                           const std::string& value, int index)
    {
        if (index < -1 || index >= (int)vec.size())
            throw std::out_of_range("Invalid index for container edit.");

        if (index == -1)
        {
            if (value.empty())
                throw std::invalid_argument("Cannot add empty string.");
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

    float GetSeasonPrice(int season) const
    {
        if (season < 0 || season >= seasonalPricingPerWeek.size())
            throw std::out_of_range("Invalid season index");
        return seasonalPricingPerWeek[season];
    }

    void EditCapacity(int newCapacity)
    {
        if (newCapacity <= 0)
            throw std::invalid_argument("Capacity must be positive");
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

    void EditSeasonalPricingPerWeek(int season, float price)
    {
        if (season < 0 || season > 3)
            throw std::out_of_range("Season index invalid.");
        if (price < MINIMAL_PRICE_PER_WEEK)
            throw std::invalid_argument("Price too low.");

        seasonalPricingPerWeek[season] = price;
    }

    virtual std::stringstream Serialize() const override
    {
        std::stringstream ss;

        ss << id << "," << city << "," << capacity << ","
           << HelperFuncs::vectorToString(seasonalPricingPerWeek, '|') << ","
           << HelperFuncs::vectorToString(livingConditions, '|') << ","
           << HelperFuncs::vectorToString(bookingConditions, '|') << ","
           << HelperFuncs::vectorToString(amenities, '|') << "," << sellerId;

        return ss;
    }

    virtual void Deserialize(std::vector<std::string> params) override
    {
        if (params.size() != 8)
            throw std::runtime_error("Apartment::Deserialize expected 8 "
                                     "fields.");

        id = std::stoi(params[0]);
        SetCity(params[1]);
        SetCapacity(std::stoi(params[2]));

        seasonalPricingPerWeek = HelperFuncs::separateLine<float>(
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
