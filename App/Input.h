#pragma once

#pragma once
#include <iostream>
#include <limits>
#include <string>

struct Input
{
    static int GetInt(const std::string& prompt)
    {
        int x;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> x)
                break;

            std::cout << "Invalid number. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return x;
    }

    static float GetFloat(const std::string& prompt)
    {
        float x;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> x)
                break;

            std::cout << "Invalid number. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return x;
    }

    static std::string GetLine(const std::string& prompt)
    {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin >> std::ws, s);
        return s;
    }
};
