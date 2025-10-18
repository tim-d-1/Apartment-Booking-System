#pragma once
#include <string>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <cctype>
#include "User.h"

const float MINIMAL_PRICE_PER_WEEK = 100;

class Apartment : public Model {
    std::string city;
    std::vector<float> seasonalPricingPerWeek{ -1, -1, -1, -1 };
    std::vector<std::string> livingConditions;
    std::vector<std::string> bookingConditions;
    std::vector<std::string> amenities;
    int capacity{};
    int sellerId;
public:
    

    Apartment(
        int id,
        std::string city,
        int capacity,
        std::vector<std::string> livingConditions,
        std::vector<std::string> bookingConditions,
        std::vector<std::string> amenities,
        std::vector<float> seasonalPricingPerWeek,
        int sellerId)
        : Model(id), city(std::move(city)), capacity(capacity),
        livingConditions(std::move(livingConditions)),
        bookingConditions(std::move(bookingConditions)),
        amenities(std::move(amenities)),
        seasonalPricingPerWeek(std::move(seasonalPricingPerWeek)),
        sellerId(sellerId) {
    }

    ~Apartment() override {}

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

    int GetSellerId() const
    {
        return sellerId;
    }

#pragma endregion

    /*
        int id,
        std::string city,
        int capacity,
        std::vector<std::string> livingConditions,
        std::vector<std::string> bookingConditions,
        std::vector<std::string> amenities,
        std::vector<float> seasonalPricingPerWeek,
        std::shared_ptr<User> seller
    */

    virtual std::stringstream Serialize() const override {
        std::stringstream out;
        out << id << ','
            << city << ','
            << capacity << ','
            << vectorToString(livingConditions) << ','
            << vectorToString(bookingConditions) << ','
            << vectorToString(amenities) << ','
            << vectorToString(seasonalPricingPerWeek, std::to_string) << ','
            << sellerId;
        return out;
    }

    virtual void Deserialize(std::vector<std::string> params) override {
        if (params.size() != 8) {
            // FAIL
            return;
        }

        id = stoi(params[0]);
        city = params[1];
        capacity = stoi(params[2]);

        livingConditions = separateLine<std::string>(params[3], '|');
        bookingConditions = separateLine<std::string>(params[4], '|');
        amenities = separateLine<std::string>(params[5], '|');
        seasonalPricingPerWeek = separateLine<float>(params[6], '|', [](auto str) { return std::stof(str); });
        sellerId = std::stoi(params[7]);
    }
private:
    static void editContainer(std::vector<std::string>* container = nullptr, const std::string& value = "", int index = -1)
    {
        if (!container || (value.empty() && index < 0))
        {
            // FAIL
            return;
        }

        if (index < 0)
        {
            container->push_back(value);
        }
        else if (index >= 0 && index < container->size())
        {
            if (value.empty())
            {
                container->erase(container->begin() + index);
            }
            else
            {
                (*container)[index] = value;
            }
        }
    }
};

