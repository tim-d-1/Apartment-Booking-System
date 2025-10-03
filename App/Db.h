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
using Container = std::list<std::shared_ptr<T>>;

const std::vector<std::string> files = {
                            "Data/Users.csv",
                            "Data/Admins.csv",
                            "Data/Apartments.csv" };

class Db final
{
    static Db* db;

    Container<User> users;
    Container<Admin> admins;
    Container<Apartment> apartments;

    int lastUserId{ 0 };
    int lastAdminId{ 0 };
    int lastApartmentId{ 0 };

    Db() = default;

    template<ModelT T>
    Container<T>& GetContainer() {
        if constexpr (std::is_same_v<T, User>) {
            return reinterpret_cast<Container<T>&>(users);
        }
        else if constexpr (std::is_same_v<T, Admin>) {
            return reinterpret_cast<Container<T>&>(admins);
        }
        else if constexpr (std::is_same_v<T, Apartment>) {
            return reinterpret_cast<Container<T>&>(apartments);
        }
        else {
            static_assert(always_false<T>, "Unsupported type for Db");
        }
    }

    template<typename>
    inline static constexpr bool always_false = false;
public:
    Db(const Db&) = delete;
    Db& operator=(const Db&) = delete;

    static Db* GetInstance() {
        if (db == nullptr) {
            db = new Db();
        }
        return db;
    }

    template<ModelT T>
    void Add(std::shared_ptr<T> obj) {
        if (obj->GetId() <= 0) {
            if constexpr (std::is_same_v<T, User>)
                obj->SetId(++lastUserId);
            else if constexpr (std::is_same_v<T, Admin>)
                obj->SetId(++lastAdminId);
            else if constexpr (std::is_same_v<T, Apartment>)
                obj->SetId(++lastApartmentId);
        }
        GetContainer<T>().push_back(obj);
    }

    template<ModelT T>
    std::shared_ptr<T> Search(Predicate<T> pred) {
        auto& cont = GetContainer<T>();
        for (const auto& ptr : cont) {
            if (pred(ptr)) return ptr;
        }
        return nullptr;
    }

    template<ModelT T>
    Container<T> SearchAll(Predicate<T> pred) {
        Container<T> res;
        auto& cont = GetContainer<T>();
        for (const auto& ptr : cont) {
            if (pred(ptr)) res.push_back(ptr);
        }
        return res;
    }

    template<ModelT T>
    bool Remove(Predicate<T> pred) {
        auto& cont = GetContainer<T>();
        for (auto it = cont.begin(); it != cont.end(); ++it) {
            if (pred(*it)) {
                cont.erase(it);
                return true;
            }
        }
        return false;
    }

    template<ModelT T>
    const Container<T>& GetAll() {
        return GetContainer<T>();
    }
};

Db* Db::db = nullptr;

