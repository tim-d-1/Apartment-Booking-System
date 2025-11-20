#pragma once
#include "Model.h"
#include <filesystem>
#include <functional>
#include <type_traits>

template <typename T>
concept ModelT = std::is_base_of_v<Model, T>;

template <ModelT T> using Container = std::vector<std::shared_ptr<T>>;

template <typename T>
using Predicate = std::function<bool(const std::shared_ptr<T>&)>;

template <typename T>
using UpdaterFunc = std::function<void(std::shared_ptr<T>)>;

enum class Season : int
{
    Spring = 0,
    Summer = 1,
    Autumn = 2,
    Winter = 3
};
