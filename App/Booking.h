#pragma once
#include "Date.h"
#include "Model.h"
#include <sstream>

struct BookingPriceBreakdown
{
    float total = 0.f;

    int daysPerSeason[4] = {0, 0, 0, 0};

    float pricePerSeason[4] = {0, 0, 0, 0};

    float dailyPrice[4] = {0, 0, 0, 0};
};

class Booking : public Model
{
    int apartmentId{};
    int userId{};
    Date from;
    Date to;
    float totalPayment{0.0f};
    bool paid{false};
    bool refunded{false};

  public:
    Booking() = default;

    Booking(int id, int apartmentId, int userId, const Date& from,
            const Date& to, float totalPayment, bool paid = false,
            bool refunded = false)
        : Model(id), apartmentId(apartmentId), userId(userId), from(from),
          to(to), totalPayment(totalPayment), paid(paid), refunded(refunded)
    {
        if (!(from < to))
            throw std::invalid_argument("Booking: from must be < to");
    }

    int GetApartmentId() const
    {
        return apartmentId;
    }
    int GetUserId() const
    {
        return userId;
    }
    Date GetFrom() const
    {
        return from;
    }
    Date GetTo() const
    {
        return to;
    }
    float GetTotalPayment() const
    {
        return totalPayment;
    }
    bool IsPaid() const
    {
        return paid;
    }
    bool IsRefunded() const
    {
        return refunded;
    }

    void MarkPaid()
    {
        if (paid)
            throw std::runtime_error("Already paid");
        paid = true;
    }
    void MarkRefunded()
    {
        if (!paid)
            throw std::runtime_error("Cannot refund unpaid");
        refunded = true;
    }

    virtual std::stringstream Serialize() const override
    {
        std::stringstream ss;
        ss << id << "," << apartmentId << "," << userId << ","
           << from.ToString() << "," << to.ToString() << "," << totalPayment
           << "," << (paid ? "1" : "0") << "," << (refunded ? "1" : "0");
        return ss;
    }

    virtual void Deserialize(std::vector<std::string> params) override
    {
        if (params.size() != 8)
            throw std::runtime_error("Booking::Deserialize expected 8 fields");

        id = std::stoi(params[0]);
        apartmentId = std::stoi(params[1]);
        userId = std::stoi(params[2]);
        from = Date::FromString(params[3]);
        to = Date::FromString(params[4]);
        totalPayment = std::stof(params[5]);
        paid = (params[6] == "1");
        refunded = (params[7] == "1");
    }
};
