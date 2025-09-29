#pragma once
#include <string>
#include <type_traits>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Apartment.h"

template<typename T>
concept ModelT = std::is_base_of_v<Model, T>;

template<typename T>
using Predicate = std::function<bool(std::shared_ptr<T>)>;

template<ModelT T>
using Container = std::list<std::unique_ptr<T>>;

const std::vector<std::string> files = {
                            "Data/Users.csv",
                            "Data/Admins.csv",
                            "Data/Apartments.csv" };

class Db final
{
    static Db* db;

    Db() {}

    Container<Apartment> apartments;
public:
    Db(Db& other) = delete;
    Db(Db&& other) = delete;

    void operator=(const Db&) = delete;

    static Db* GetInstance()
    {
        if (db == nullptr) {
            db = new Db();
        }
        return db;
    }
};

