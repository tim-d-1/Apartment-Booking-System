#pragma once
#include "Admin.h"
#include "Apartment.h"
#include "Config.h"
#include "CsvFileStorage.h"
#include "Types.h"
#include "User.h"
#include "Booking.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

class Db final
{
  private:
    std::unique_ptr<IStorage> storage;

    Container<User> users;
    Container<Admin> admins;
    Container<Apartment> apartments;
    Container<Booking> bookings;
    
    int lastBookingId{0};
    int lastUserId{0};
    int lastAdminId{0};
    int lastApartmentId{0};

  private:
    Db() : storage{std::make_unique<CsvFileStorage>()}
    {
    }

    Db(std::unique_ptr<IStorage> customStorage)
        : storage(std::move(customStorage))
    {
    }

  public:
    Db(const Db&) = delete;
    Db(Db&&) = delete;

    Db& operator=(const Db&) = delete;
    Db& operator=(Db&&) = delete;

    ~Db() = default;

    static Db& GetInstance()
    {
        static Db instance;
        return instance;
    }

    template <ModelT T> Container<T>& GetContainerRef()
    {
        if constexpr (std::is_same_v<T, User>)
            return users;
        else if constexpr (std::is_same_v<T, Admin>)
            return admins;
        else if constexpr (std::is_same_v<T, Apartment>)
            return apartments;
        else if constexpr (std::is_same_v<T, Booking>)
            return bookings;
        else
            throw std::logic_error("Unsupported model type");
    }

    template <ModelT T> int& GetLastIdRef()
    {
        if constexpr (std::is_same_v<T, User>)
            return lastUserId;
        else if constexpr (std::is_same_v<T, Admin>)
            return lastAdminId;
        else if constexpr (std::is_same_v<T, Apartment>)
            return lastApartmentId;
        else if constexpr (std::is_same_v<T, Booking>)
            return lastBookingId;
        else
            throw std::logic_error("Unsupported model type");
    }

    template <ModelT T> const char* GetFilenameForType()
    {
        if constexpr (std::is_same_v<T, User>)
            return ConfigPaths::Users;
        else if constexpr (std::is_same_v<T, Admin>)
            return ConfigPaths::Admins;
        else if constexpr (std::is_same_v<T, Apartment>)
            return ConfigPaths::Apartments;
        else if constexpr (std::is_same_v<T, Booking>)
            return ConfigPaths::Bookings;
        else
            throw std::logic_error("Unsupported model type");
    }

    template <ModelT T> void LoadContainer()
    {
        auto& cont = GetContainerRef<T>();
        auto& lastId = GetLastIdRef<T>();
        const char* filename = GetFilenameForType<T>();

        cont.clear();
        lastId = 0;

        if (!storage->Exists(filename))
        {
#ifdef LOGGING
            std::cerr << "[Info] File not found: " << filename << '\n';
#endif
            return;
        }

        auto lines = storage->ReadAll(filename);

        for (const auto& line : lines)
        {
            try
            {
                auto params = HelperFuncs::separateLine<std::string>(line, ',');
                auto obj = std::make_shared<T>();
                obj->Deserialize(params);
                cont.push_back(obj);

                lastId = std::max(lastId, obj->GetId());
            }
            catch (const std::exception& ex)
            {
#ifdef LOGGING
                std::cerr << "[Error] Failed to load: " << ex.what()
                          << "\nLine: " << line << '\n';
#endif
            }
        }
    }

    template <ModelT T> void SaveContainer()
    {
        auto& cont = GetContainerRef<T>();
        const char* filename = GetFilenameForType<T>();

        std::vector<std::string> lines;
        lines.reserve(cont.size());

        for (auto& obj : cont)
            lines.push_back(obj->Serialize().str());

        storage->WriteAll(filename, lines);
    }

    template <ModelT T> void Add(std::shared_ptr<T> obj)
    {
        auto& cont = GetContainerRef<T>();
        auto& lastId = GetLastIdRef<T>();

        if (obj->GetId() <= 0)
            obj->SetId(++lastId);

        cont.push_back(obj);
        SaveContainer<T>();
    }

    template <ModelT T> bool Update(Predicate<T> pred, UpdaterFunc<T> updater)
    {
        auto& cont = GetContainerRef<T>();

        for (auto& obj : cont)
        {
            if (pred(obj))
            {
                updater(obj);
                SaveContainer<T>();
                return true;
            }
        }
        return false;
    }

    template <ModelT T> bool Remove(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        for (auto it = cont.begin(); it != cont.end(); ++it)
        {
            if (pred(*it))
            {
                cont.erase(it);
                SaveContainer<T>();
                return true;
            }
        }
        return false;
    }

    template <ModelT T> void RemoveAll(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        bool changed = false;
        for (auto it = cont.begin(); it != cont.end();)
        {
            if (pred(*it))
            {
                it = cont.erase(it);
                changed = true;
            }
            else
                ++it;
        }

        if (changed)
            SaveContainer<T>();
    }

    template <ModelT T> std::shared_ptr<T> Search(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        for (const auto& ptr : cont)
            if (pred(ptr))
                return ptr;

        return nullptr;
    }

    template <ModelT T> Container<T> SearchAll(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();
        Container<T> result;

        for (const auto& ptr : cont)
            if (pred(ptr))
                result.push_back(ptr);

        return result;
    }

    template <ModelT T> const Container<T>& GetAll()
    {
        return GetContainerRef<T>();
    }
};
