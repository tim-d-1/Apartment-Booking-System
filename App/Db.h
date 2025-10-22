#pragma once
#include <type_traits>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "Types.h"
#include "CsvFileStorage.h"
#include <typeindex>
#include <any>

class Db final
{
    std::unique_ptr<IStorage> storage;

    Container<User> users;
    Container<Admin> admins;
    Container<Apartment> apartments;

    int lastUserId{ 0 };
    int lastAdminId{ 0 };
    int lastApartmentId{ 0 };

    Db();

    Db(std::unique_ptr<IStorage> storageImpl)
        : storage(std::move(storageImpl)) {
    }
public:
    Db(const Db&) = delete;
    Db& operator=(const Db&) = delete;

    static Db& GetInstance() {
        static Db instance(std::make_unique<CsvFileStorage>());
        return instance;
    }

    Container<User>& GetUsers() { return users; }
    Container<Admin>& GetAdmins() { return admins; }
    Container<Apartment>& GetApartments() { return apartments; }

    int& GetLastUserId() { return lastUserId; }
    int& GetLastAdminId() { return lastAdminId; }
    int& GetLastApartmentId() { return lastApartmentId; }

    std::unique_ptr<IStorage>& GetStorage() { return storage; }

    template<ModelT T> void LoadContainer();
    template<ModelT T> void SaveContainer();

    template<ModelT T> void Add(std::shared_ptr<T> obj);
    template<ModelT T> bool Update(int id, std::function<void(std::shared_ptr<T>)> updater);

    template<ModelT T> std::shared_ptr<T> Search(Predicate<T> pred);
    template<ModelT T> Container<T> SearchAll(Predicate<T> pred);

    template<ModelT T> bool Remove(Predicate<T> pred);
    template<ModelT T> void RemoveAll(Predicate<T> pred);

    template<ModelT T> const Container<T>& GetAll();
};

