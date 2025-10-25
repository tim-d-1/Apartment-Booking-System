#include <string>
#include <cctype>
#include <vector>
#include <fstream>

struct HelperFuncs
{
    static bool isValidPassword(const std::string& password) {
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

    template<typename T>
    static std::string vectorToString(
        const std::vector<T>& vector,
        std::string(*toStringConverter)(T i) = nullptr)
    {
        std::string result;

        for (const auto& item : vector)
        {
            if constexpr (!std::is_same<T, std::string>()) result += toStringConverter(item) + '|';
            else result += item + '|';
        }
        return result;
    }

    /**
     * @brief Extracts each word from a string, divided by a separator.
     * @param line The input string to be processed.
     * @param separator The character that divides the words. Defaults to ','.
     * @return A vector of strings, where each element is a word.
     */
    template<typename T>
    static std::vector<T> separateLine(
        const std::string& line,
        const char separator = '|',
        T(*typeConverter)(const std::string& i) = nullptr)
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
};