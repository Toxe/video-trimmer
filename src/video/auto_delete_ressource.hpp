#pragma once

#include <memory>

template <typename T>
using auto_delete_ressource = std::unique_ptr<T, void (*)(T*)>;
