#pragma once

#include <utility>

template <typename T>
class AutoDeleteResource {
    T* ptr_;
    void (*dealloc_)(T*);

public:
    AutoDeleteResource() : ptr_{nullptr}, dealloc_{nullptr} { }
    AutoDeleteResource(T* ptr, void (*dealloc)(T*)) : ptr_{ptr}, dealloc_{dealloc} { }

    AutoDeleteResource(const AutoDeleteResource& other) = delete;  // copy constructor

    AutoDeleteResource(AutoDeleteResource&& other) noexcept  // move constructor
        : ptr_(std::exchange(other.ptr_, nullptr)),
          dealloc_(std::exchange(other.dealloc_, nullptr))
    {
    }

    AutoDeleteResource& operator=(const AutoDeleteResource& other) = delete;  // copy assignment

    AutoDeleteResource& operator=(AutoDeleteResource&& other) noexcept  // move assignment
    {
        std::swap(ptr_, other.ptr_);
        std::swap(dealloc_, other.dealloc_);
        return *this;
    }

    ~AutoDeleteResource()
    {
        if (ptr_ && dealloc_)
            dealloc_(ptr_);
    }

    [[nodiscard]] T* get() const { return ptr_; }
    [[nodiscard]] T** get_ptr() { return &ptr_; }

    [[nodiscard]] bool operator!() const { return !ptr_; }
    [[nodiscard]] T* operator->() const { return ptr_; }
};
