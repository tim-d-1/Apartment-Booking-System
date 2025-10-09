#include <string>
#include <cctype>
#include <vector>

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

// if index is specified & condition is not empty => replace existing condition
// if index is specified & condition is empty => erase the element
// doesn't work if container is nullptr or index is out-of-bound
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

static std::string vectorToString(const std::vector<std::string>& vector)
{
    std::string result;
    for (const auto& str : vector)
    {
        result += str + "|";
    }
    return result;
}