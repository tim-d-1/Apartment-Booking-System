#include "Db.h"
#include "CsvFileStorage.h"
#include "TypeInfo.h"

Db& Db::GetInstance()
{
    static Db instance(std::make_unique<CsvFileStorage>());
    return instance;
}

template <ModelT T> void Db::LoadContainer()
{
    auto& typeInfo = getTypeInfo<T>();

    if (!storage->Exists(typeInfo.TypeFilename))
        return /* fail */;

    auto lines = storage->ReadAll(typeInfo.TypeFilename);

    for (auto& line : lines)
    {
        auto object = std::make_shared<T>();

        object->Deserialize(separateLine<std::string>(line));
        typeInfo.TypeContainer.push_back(object);
    }

    if (!typeInfo.TypeContainer.empty())
    {
        int maxId = 0;

        for (auto& obj : typeInfo.TypeContainer)
            maxId = std::max(maxId, obj->GetId());

        typeInfo.TypeLastId = maxId;
    }
}

template <ModelT T> void Db::SaveContainer()
{
    auto& typeInfo = getTypeInfo<T>();
    std::vector<std::string> lines;

    for (auto& obj : typeInfo.TypeContainer)
        lines.push_back(obj->Serialize());

    storage->WriteAll(typeInfo.TypeFilename, lines);
}

template <ModelT T> void Db::Add(std::shared_ptr<T> obj)
{
    if (obj->GetId() <= 0)
    {
        if constexpr (std::is_same_v<T, User>)
            obj->SetId(++lastUserId);
        else if constexpr (std::is_same_v<T, Admin>)
            obj->SetId(++lastAdminId);
        else if constexpr (std::is_same_v<T, Apartment>)
            obj->SetId(++lastApartmentId);
    }
    getTypeInfo<T>().TypeContainer.push_back(obj);
}

template <ModelT T> bool Db::Update(Predicate<T> pred, UpdaterFunc<T> updater)
{
    auto& cont = getTypeInfo<T>().TypeContainer;

    for (auto& obj : cont)
    {
        if (pred(pred))
        {
            updater(obj);
            return true;
        }
    }

    return false;
}

template <ModelT T> std::shared_ptr<T> Db::Search(Predicate<T> pred)
{
    auto& cont = GetContainer<T>();

    for (const auto& ptr : cont)
    {
        if (pred(ptr))
            return ptr;
    }

    return nullptr;
}

template <ModelT T> Container<T> Db::SearchAll(Predicate<T> pred)
{
    Container<T> res;
    auto& cont = getTypeInfo<T>().TypeContainer;

    for (const auto& ptr : cont)
    {
        if (pred(ptr))
            res.push_back(ptr);
    }

    return res;
}

template <ModelT T> bool Db::Remove(Predicate<T> pred)
{
    auto& cont = getTypeInfo<T>().TypeContainer;

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

template <ModelT T> void Db::RemoveAll(Predicate<T> pred)
{
    auto& cont = getTypeInfo<T>().TypeContainer;

    for (auto it = cont.begin(); it != cont.end();)
    {
        if (pred(*it))
        {
            it = cont.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

template <ModelT T> const Container<T>& Db::GetAll()
{
    return getTypeInfo<T>().TypeContainer;
}
