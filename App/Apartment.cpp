#include "Apartment.h"
#include "HelperFuncs.h"

Apartment::Apartment(int id, std::string city, int capacity,
                     std::vector<std::string> livingConditions,
                     std::vector<std::string> bookingConditions,
                     std::vector<std::string> amenities,
                     std::vector<float> seasonalPricingPerWeek, int sellerId)
    : Model(id), city(std::move(city)), capacity(capacity),
      livingConditions(std::move(livingConditions)),
      bookingConditions(std::move(bookingConditions)),
      amenities(std::move(amenities)),
      seasonalPricingPerWeek(std::move(seasonalPricingPerWeek)),
      sellerId(sellerId)
{
}

std::stringstream Apartment::Serialize() const
{
    std::stringstream out;
    out << id << ',' << city << ',' << capacity << ','
        << HelperFuncs::vectorToString(livingConditions) << ','
        << HelperFuncs::vectorToString(bookingConditions) << ','
        << HelperFuncs::vectorToString(amenities) << ','
        << HelperFuncs::vectorToString(seasonalPricingPerWeek, std::to_string)
        << ',' << sellerId;
    return out;
}

void Apartment::Deserialize(std::vector<std::string> params)
{
    if (params.size() != 8)
    {
        // FAIL
        return;
    }

    id = stoi(params[0]);
    city = params[1];
    capacity = stoi(params[2]);

    livingConditions = HelperFuncs::separateLine<std::string>(params[3], '|');
    bookingConditions = HelperFuncs::separateLine<std::string>(params[4], '|');
    amenities = HelperFuncs::separateLine<std::string>(params[5], '|');
    seasonalPricingPerWeek = HelperFuncs::separateLine<float>(
            params[6], '|', [](const std::string& i) { return std::stof(i); });
    sellerId = std::stoi(params[7]);
}

void Apartment::editContainer(std::vector<std::string>* container,
                              const std::string& value, int index)
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
