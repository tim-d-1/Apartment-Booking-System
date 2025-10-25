#pragma once
#include "Types.h"
#include "Admin.h"
#include "Apartment.h"

template<ModelT T>
struct TypeInfo {
    Container<T>& container;
    std::string filename;
    int& lastId;

    TypeInfo(Container<T>& c, const std::string& f, int& id)
        : container(c), filename(std::filesystem::current_path().string() + f), lastId(id) {
    }
};

template<ModelT T>
TypeInfo<T>& getTypeInfo() {
    static_assert(!std::is_same_v<T, T>,
        "TypeInfo specialization missing for this type.");
}

extern template TypeInfo<User>& getTypeInfo<User>();
extern template TypeInfo<Admin>& getTypeInfo<Admin>();
extern template TypeInfo<Apartment>& getTypeInfo<Apartment>();