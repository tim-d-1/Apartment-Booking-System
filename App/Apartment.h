#pragma once
#include <list>
#include <map>
#include <stack>
#include <cctype>
#include "Model.h"

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
        int sellerId
    );

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

    virtual std::stringstream Serialize() const override;

    virtual void Deserialize(std::vector<std::string> params) override;
private:
    static void editContainer(std::vector<std::string>* container = nullptr, const std::string& value = "", int index = -1);
};

