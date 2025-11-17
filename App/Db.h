#pragma once
#include "Admin.h"
#include "Apartment.h"
#include "Config.h"
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

    template <ModelT T> void LoadContainer()
    {
        auto& cont = GetContainerRef<T>();
        auto& lastId = GetLastIdRef<T>();
        const char* filename = GetFilenameForType<T>();

        if (!storage->Exists(filename))
        {
#ifdef LOGGING
            std::cerr << "[Error] File " << filename << " wasn't found\n";
#endif
            return;
        }

        auto lines = storage->ReadAll(filename);
        for (auto& line : lines)
        {
            auto obj = std::make_shared<T>();
            obj->Deserialize(HelperFuncs::separateLine<std::string>(line, ','));
            cont.push_back(obj);
        }

        if (!cont.empty())
        {
            int maxId = 0;
            for (auto& obj : cont)
                maxId = std::max(maxId, obj->GetId());
            lastId = maxId;
        }

        #ifdef LOGGING
        Update<T>(
                [](auto p) { return true; }, [](auto it)
                { std::cout << "\n[Logging] Item: " << it->Serialize().str(); });
#endif
    }

    template <ModelT T> void SaveContainer()
    {
        auto& cont = GetContainerRef<T>();
        const char* filename = GetFilenameForType<T>();

        std::vector<std::string> lines;
        for (auto& obj : cont)
            lines.push_back(obj->Serialize().str());

        storage->WriteAll(filename, lines);

#ifdef LOGGING
        std::cout << "[Info] Container was saved at '" << filename << "'\n";
#endif
    }

    template <ModelT T> void Add(std::shared_ptr<T> obj)
    {
        auto& cont = GetContainerRef<T>();
        auto& lastId = GetLastIdRef<T>();

        if (obj->GetId() <= 0)
        {
            obj->SetId(++lastId);
        }

        cont.push_back(obj);

#ifdef LOGGING
        std::cout << "[Info] Object was added: " << obj->Serialize().str()
                  << '\n';
#endif
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
                return true;
            }
        }

        return false;
    }

    template <ModelT T> std::shared_ptr<T> Search(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        for (const auto& ptr : cont)
        {
            if (pred(ptr))
                return ptr;
        }

        return nullptr;
    }

    template <ModelT T> Container<T> SearchAll(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();
        Container<T> result;

        for (const auto& ptr : cont)
        {
            if (pred(ptr))
                result.push_back(ptr);
        }

        return result;
    }

    template <ModelT T> bool Remove(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        for (auto it = cont.begin(); it != cont.end(); ++it)
        {
            if (pred(*it))
            {
                cont.erase(it);
                return true;
            }
        }

        return false;
    }

    template <ModelT T> void RemoveAll(Predicate<T> pred)
    {
        auto& cont = GetContainerRef<T>();

        for (auto it = cont.begin(); it != cont.end();)
        {
            if (pred(*it))
                it = cont.erase(it);
            else
                ++it;
        }
    }

    template <ModelT T> const Container<T>& GetAll()
    {
        return GetContainerRef<T>();
    }

  private:
    template <typename T> constexpr Container<T>& GetContainerRef();

    template <typename T> constexpr int& GetLastIdRef();

    template <typename T> constexpr const char* GetFilenameForType();

    template <> constexpr Container<User>& GetContainerRef<User>()
    {
        return users;
    }
    template <> constexpr Container<Admin>& GetContainerRef<Admin>()
    {
        return admins;
    }
    template <> constexpr Container<Apartment>& GetContainerRef<Apartment>()
    {
        return apartments;
    }

    template <> constexpr int& GetLastIdRef<User>()
    {
        return lastUserId;
    }
    template <> constexpr int& GetLastIdRef<Admin>()
    {
        return lastAdminId;
    }
    template <> constexpr int& GetLastIdRef<Apartment>()
    {
        return lastApartmentId;
    }

    template <> constexpr const char* GetFilenameForType<User>()
    {
        return ConfigPaths::Users;
    }

    template <> constexpr const char* GetFilenameForType<Admin>()
    {
        return ConfigPaths::Admins;
    }

    template <> constexpr const char* GetFilenameForType<Apartment>()
    {
        return ConfigPaths::Apartments;
    }
};
