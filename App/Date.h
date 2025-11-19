#pragma once
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
};

inline bool RangesOverlap(const Date& aStart, const Date& aEnd,
                          const Date& bStart, const Date& bEnd)
{
    if (!(aStart < aEnd) || !(bStart < bEnd))
        throw std::invalid_argument("Invalid date range (start must be < end)");

    return (aStart < bEnd) && (bStart < aEnd);
}
