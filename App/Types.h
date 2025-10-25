#pragma once
#include <type_traits>
#include <filesystem>
#include <functional>
#include "Model.h"

template<typename T>
concept ModelT = std::is_base_of_v<Model, T>;

template<ModelT T>
using Container = std::vector<std::shared_ptr<T>>;

template<typename T>
using Predicate = std::function<bool(const std::shared_ptr<T>&)>;