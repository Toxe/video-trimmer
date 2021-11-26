#pragma once

#include <memory>

template <typename T>
using auto_delete_resource = std::unique_ptr<T, void (*)(T*)>;
