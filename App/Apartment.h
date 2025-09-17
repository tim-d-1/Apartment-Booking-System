#pragma once
#include <string>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include "IModel.h"
#include "ModelSeralizationHelper.h"

const float MINIMAL_PRICE_PER_WEEK = 100;

class Apartment
    : IModel
{
    std::vector<float> seasonalPricingPerWeek{ -1, -1, -1, -1 };
    std::vector<std::string> livingConditions;
    std::vector<std::string> bookingConditions;
    std::vector<std::string> amenities;
    int capacity;

public:
    const std::string City;

    Apartment(int id, std::string city, int capacity,
        std::vector<std::string> livingConditions,
        std::vector<std::string> bookingConditions,
        std::vector<std::string> amenities,
        std::vector<float> seasonalPricingPerWeek) :
        IModel(id),
        City{ city },
        capacity{ capacity }, livingConditions{ livingConditions }, bookingConditions{ bookingConditions },
        amenities{ amenities },
        seasonalPricingPerWeek{ seasonalPricingPerWeek }
    {
    }

#pragma region CRUD

    void CreateLivingCondition(const std::string& data)
    {
        ModelSeralizationHelper::editContainer(&livingConditions, data, -1);
    }

    void UpdateLivingCondition(const std::string& data, int index)
    {
        ModelSeralizationHelper::editContainer(&livingConditions, data, index);
    }

    void DeleteLivingCondition(int index)
    {
        ModelSeralizationHelper::editContainer(&livingConditions, "", index);
    }

    void CreateBookingCondition(const std::string& data)
    {
        ModelSeralizationHelper::editContainer(&bookingConditions, data, -1);
    }

    void UpdateBookingCondition(const std::string& data, int index)
    {
        ModelSeralizationHelper::editContainer(&bookingConditions, data, index);
    }

    void DeleteBookingCondition(int index)
    {
        ModelSeralizationHelper::editContainer(&bookingConditions, "", index);
    }

    void CreateAmenitites(const std::string& data)
    {
        ModelSeralizationHelper::editContainer(&amenities, data, -1);
    }

    void UpdateAmenitites(const std::string& data, int index)
    {
        ModelSeralizationHelper::editContainer(&amenities, data, index);
    }

    void DeleteAmenitites(int index)
    {
        ModelSeralizationHelper::editContainer(&amenities, "", index);
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
#pragma endregion

    virtual std::stringstream Serialize() {
        std::stringstream out; 
        out << City << ','
            << capacity << ','
            << ModelSeralizationHelper::vectorToString(bookingConditions) << ','
            << ModelSeralizationHelper::vectorToString(amenities) << ','
            << ModelSeralizationHelper::vectorToString(livingConditions);
    }
};

