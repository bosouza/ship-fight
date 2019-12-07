#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED
#include <string>

template <class T>
class singleton
{
protected:
    singleton() noexcept = default;
    singleton(const singleton &) = delete;
    singleton &operator=(const singleton &) = delete;
    virtual ~singleton() = default;

public:
    static T &get_instance() noexcept(std::is_nothrow_constructible<T>::value)
    {
        static T instance;
        return instance;
    }
};

#endif