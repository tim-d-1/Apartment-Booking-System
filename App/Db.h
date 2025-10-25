#pragma once
#include "Admin.h"
#include "Apartment.h"
#include "IStorage.h"
#include "Types.h"
#include <functional>
#include <iostream>
#include <type_traits>
#include <unordered_map>

class Db final
{
    std::unique_ptr<IStorage> storage;

    Container<User> users;
    Container<Admin> admins;
    Container<Apartment> apartments;

    int lastUserId{0};
    int lastAdminId{0};

    int lastApartmentId{0};

    Db();

    Db(std::unique_ptr<IStorage> storageImpl) : storage(std::move(storageImpl))
    {
    }

  public:
    Db(const Db&) = delete;
    Db& operator=(const Db&) = delete;

    static Db& GetInstance();

    Container<User>& GetUsers()
    {
        return users;
    }

    Container<Admin>& GetAdmins()
    {
        return admins;
    }

    Container<Apartment>& GetApartments()
    {
        return apartments;
    }

    int& GetLastUserId()
    {
        return lastUserId;
    }
    int& GetLastAdminId()
    {
        return lastAdminId;
    }
    int& GetLastApartmentId()
    {
        return lastApartmentId;
    }

    std::unique_ptr<IStorage>& GetStorage()
    {
        return storage;
    }

    template <ModelT T> void LoadContainer();
    template <ModelT T> void SaveContainer();

    template <ModelT T> void Add(std::shared_ptr<T> obj);
    template <ModelT T> bool Update(Predicate<T> pred, UpdaterFunc<T> updater);

    template <ModelT T> std::shared_ptr<T> Search(Predicate<T> pred);
    template <ModelT T> Container<T> SearchAll(Predicate<T> pred);

    template <ModelT T> bool Remove(Predicate<T> pred);
    template <ModelT T> void RemoveAll(Predicate<T> pred);

    template <ModelT T> const Container<T>& GetAll();
};
