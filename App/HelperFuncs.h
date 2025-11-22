#include <cctype>
#include <fstream>
#include <functional>
#include <string>
#include <vector>
#include "Types.h"

struct HelperFuncs
{
    static bool isValidPassword(const std::string& password)
    {
        if (password.length() < 8)
        {
            return false;
        }

        bool hasNumber = false;
        bool hasLetter = false;

        for (const char& c : password)
        {
            if (std::isdigit(c))
            {
                hasNumber = true;
            }
            else if (std::isalpha(c))
            {
                hasLetter = true;
            }

            if (hasNumber && hasLetter)
            {
                break;
            }
        }

        return hasNumber && hasLetter;
    }

    static const char* SeasonName(Season s)
    {
        switch (s)
        {
        case Season::Spring:
            return "Весна ";
        case Season::Summer:
            return "Літо ";
        case Season::Autumn:
            return "Осінь ";
        case Season::Winter:
            return "Зима ";
        }
        return "Невідомий ";
    }

    /**
     * Joins elements of a vector into a single string.
     * * @tparam T The type of elements in the vector.
     * @param vec The input vector.
     * @param separator The delimiter between elements (default: "|").
     * @param converter An optional function to convert T to string.
     * REQUIRED if T is not std::string or implicitly convertible to string.
     */
    template <typename T>
    static std::string vectorToString(const std::vector<T>& vec,
                                      const char separator = '|',
                                      std::string (*converter)(T) = nullptr)
    {
        if (vec.empty())
        {
            return "";
        }

        std::string out;

        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (i > 0)
            {
                out += separator;
            }

            const auto& item = vec[i];

            if constexpr (std::is_same_v<T, std::string>)
            {
                out += item;
            }
            else
            {
                if (converter)
                {
                    out += converter(item);
                }
                else
                {
                    throw std::invalid_argument("Необхідний конвертер для "
                                                "нерядкові типи ");
                }
            }
        }

        return out;
    }

    template <typename T, size_t Size>
    static std::string arrayToString(const std::array<T, Size>& arr,
                                     const char separator = '|',
                                     std::string (*converter)(T) = nullptr)
    {
        std::string out;

        for (size_t i = 0; i < Size; ++i)
        {
            if (i > 0)
            {
                out += separator;
            }

            const auto& item = arr[i];

            if constexpr (std::is_same_v<T, std::string>)
            {
                out += item;
            }
            else
            {
                if (converter)
                {
                    out += converter(item);
                }
                else
                {
                    throw std::invalid_argument("Необхідний конвертер для "
                                                "нерядкові типи ");
                }
            }
        }

        return out;
    }

    /**
     * @brief Extracts each word from a string, divided by a separator.
     * @param line The input string to be processed.
     * @param separator The character that divides the words. Defaults to ','.
     * @return A vector of strings, where each element is a word.
     */
    template <typename T>
    static std::vector<T> separateLine(
            const std::string& line, const char separator = '|',
            T (*typeConverter)(const std::string& i) = nullptr)
    {
        std::vector<T> result;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, separator))
        {
            if constexpr (std::is_same<T, std::string>())
            {
                result.push_back(token);
                continue;
            }
            result.push_back(typeConverter(token));
        }

        return result;
    }

    template <typename T, size_t Size>
    static std::array<T, Size> separateLineArr(
            const std::string& line, const char separator = '|',
            T (*typeConverter)(const std::string& i) = nullptr)
    {
        std::array<T, Size> result{};
        std::stringstream ss(line);
        std::string token;

        int i = -1;
        while (std::getline(ss, token, separator))
        {
            ++i;
            if constexpr (std::is_same<T, std::string>())
            {
                result[i] = token;
                continue;
            }
            result[i] = typeConverter(token);
        }

        return result;
    }
};
