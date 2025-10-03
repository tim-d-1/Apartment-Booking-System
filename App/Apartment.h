#pragma once
#include <string>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include "User.h"

const float MINIMAL_PRICE_PER_WEEK = 100;

class Apartment : public Model {
    std::vector<float> seasonalPricingPerWeek{ -1, -1, -1, -1 };
    std::vector<std::string> livingConditions;
    std::vector<std::string> bookingConditions;
    std::vector<std::string> amenities;
    int capacity{};
    std::shared_ptr<User> seller;
public:
    const std::string City;

    Apartment(int id, std::string city, int capacity,
        std::vector<std::string> livingConditions,
        std::vector<std::string> bookingConditions,
        std::vector<std::string> amenities,
        std::vector<float> seasonalPricingPerWeek,
        std::shared_ptr<User> seller)
        : Model(id), City(std::move(city)), capacity(capacity),
        livingConditions(std::move(livingConditions)),
        bookingConditions(std::move(bookingConditions)),
        amenities(std::move(amenities)),
        seasonalPricingPerWeek(std::move(seasonalPricingPerWeek)),
        seller(std::move(seller)) {
    }

    ~Apartment() override {
        std::cout << "Apartment destroyed: " << City << "\n"; // REMOVE
    }

#pragma region CRUD

    void CreateLivingCondition(const std::string& data)
    {
        editContainer(&livingConditions, data, -1);
    }

    void UpdateLivingCondition(const std::string& data, int index)
    {
        editContainer(&livingConditions, data, index);
    }

    void DeleteLivingCondition(int index)
    {
        editContainer(&livingConditions, "", index);
    }

    void CreateBookingCondition(const std::string& data)
    {
        editContainer(&bookingConditions, data, -1);
    }

    void UpdateBookingCondition(const std::string& data, int index)
    {
        editContainer(&bookingConditions, data, index);
    }

    void DeleteBookingCondition(int index)
    {
        editContainer(&bookingConditions, "", index);
    }

    void CreateAmenitites(const std::string& data)
    {
        editContainer(&amenities, data, -1);
    }

    void UpdateAmenitites(const std::string& data, int index)
    {
        editContainer(&amenities, data, index);
    }

    void DeleteAmenitites(int index)
    {
        editContainer(&amenities, "", index);
    }

    void EditCapacity(int capacity)
    {
        if (capacity < 0)
        {
            // FAIL
            return;
        }

        this->capacity = capacity;
    }

    // seasons: spring = 0; summer = 1; fall = 2; winter = 3;
    void EditSeasonalPricingPerWeek(int season, float price)
    {
        if (season < 0 || season > 3 || price < MINIMAL_PRICE_PER_WEEK)
        {
            // FAIL
            return;
        }

        seasonalPricingPerWeek[season] = price;
    }

    std::shared_ptr<User> GetSeller() const
    {
        return seller;
    }

#pragma endregion

    std::stringstream Serialize() const override {
        std::stringstream out;
        out << City << ','
            << capacity << ','
            << vectorToString(bookingConditions) << ','
            << vectorToString(amenities) << ','
            << vectorToString(livingConditions);
        return out;
    }
};

