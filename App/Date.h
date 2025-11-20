#pragma once
#include "Types.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

struct Date
{
    int y{};
    int m{};
    int d{};

    Date() = default;
    Date(int yy, int mm, int dd) : y(yy), m(mm), d(dd)
    {
        Validate();
    }

    void Validate() const
    {
        if (y < 1900 || m < 1 || m > 12 || d < 1 || d > 31)
            throw std::invalid_argument("Invalid date");

        static int mdays[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};
        int maxd = mdays[m];

        if (m == 2)
        {
            bool leap = ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0));
            if (leap)
                maxd = 29;
        }
        if (d > maxd)
            throw std::invalid_argument("Invalid day for month");
    }

    static Date FromString(const std::string& s)
    {
        if (s.size() != 10 || s[4] != '-' || s[7] != '-')
            throw std::invalid_argument("Date format must be YYYY-MM-DD");

        int yy = std::stoi(s.substr(0, 4));
        int mm = std::stoi(s.substr(5, 2));
        int dd = std::stoi(s.substr(8, 2));
        return Date(yy, mm, dd);
    }

    Date NextDay() const
    {
        int nd = d + 1;
        int nm = m;
        int ny = y;

        static int mdays[] = {0,  31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};
        int maxd = mdays[nm];

        if (nm == 2)
        {
            bool leap = ((ny % 4 == 0 && ny % 100 != 0) || (ny % 400 == 0));
            if (leap)
                maxd = 29;
        }

        if (nd > maxd)
        {
            nd = 1;
            nm++;
            if (nm > 12)
            {
                nm = 1;
                ny++;
            }
        }

        return Date(ny, nm, nd);
    }

    std::string ToString() const
    {
        std::ostringstream ss;
        ss << std::setw(4) << std::setfill('0') << y << '-' << std::setw(2)
           << std::setfill('0') << m << '-' << std::setw(2) << std::setfill('0')
           << d;
        return ss.str();
    }

    bool operator<(const Date& o) const
    {
        if (y != o.y)
            return y < o.y;
        if (m != o.m)
            return m < o.m;
        return d < o.d;
    }

    bool operator==(const Date& o) const
    {
        return y == o.y && m == o.m && d == o.d;
    }

    bool operator<=(const Date& o) const
    {
        return (*this < o) || (*this == o);
    }
    bool operator>(const Date& o) const
    {
        return !(*this <= o);
    }
    bool operator>=(const Date& o) const
    {
        return !(*this < o);
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

inline bool RangesOverlap(const Date& aStart, const Date& aEnd,
                          const Date& bStart, const Date& bEnd)
{
    if (!(aStart < aEnd) || !(bStart < bEnd))
        throw std::invalid_argument("Invalid date range (start must be < end)");

    return (aStart < bEnd) && (bStart < aEnd);
}
