#pragma once
#include <type_traits>
#include "Model.h"
#include "Apartment.h"
#include "Admin.h"

template<typename T>
concept ModelT = std::is_base_of_v<Model, T>;

template<ModelT T>
using Container = std::vector<std::shared_ptr<T>>;

template<typename T>
using Predicate = std::function<bool(const std::shared_ptr<T>&)>;

template<ModelT T>
struct TypeInfo {
    Container<T>& container;
    std::string filename;
    int& lastId;

    TypeInfo(Container<T>& c, std::string f, int& id)
        : container(c), filename(std::move(f)), lastId(id) {
    }
};

template<ModelT T>
TypeInfo<T>& getTypeInfo() {
    static_assert(!std::is_same_v<T, T>,
        "TypeInfo specialization missing for this type.");
}