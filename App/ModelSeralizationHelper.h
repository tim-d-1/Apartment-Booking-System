#pragma once
#include <vector>

static class ModelSeralizationHelper
{
public:
    // if index is specified & condition is not empty => replace existing condition
    // if index is specified & condition is empty => erase the element
    // doesn't work if container is nullptr or index is out-of-bound
    static void editContainer(std::vector<std::string>* container = nullptr, const std::string& condition = "", int index = -1)
    {
        if (!container || (condition.empty() && index < 0))
        {
            // FAIL
            return;
        }

        if (index < 0)
        {
            container->push_back(condition);
        }
        else if (index >= 0 && index < container->size())
        {
            if (condition.empty())
            {
                container->erase(container->begin() + index);
            }
            else
            {
                (*container)[index] = condition;
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
};

