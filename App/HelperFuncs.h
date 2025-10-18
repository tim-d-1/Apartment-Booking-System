#include <string>
#include <cctype>
#include <vector>
#include <fstream>

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
static std::string vectorToString(const std::vector<T>& vector, std::string(*toStringPolicy)(T i) = { return i; })
{
    std::string result;
    for (const auto& item : vector)
    {
        result += toStringPolicy(item) + "|";
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
    const char separator = ',', 
    T(*typeConverter)(const std::string& i) = nullptr)
{
    std::vector<T> result;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, separator))
    {
        result.push_back(typeConverter ? typeConverter(token): token);
    }

    return result;
}

std::vector<std::string> readAllLines(const std::string& path) {
    std::ifstream is(path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(is, line)) lines.push_back(line);
    return lines;
}