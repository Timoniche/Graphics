#ifndef GRAPHICS_DCOBITS_H
#define GRAPHICS_DCOBITS_H

#include <unordered_map>
#include <cassert>

/**
 * for simple inheritance (<=32 classes)
 */
//______________________________________________________________________________________________________________________
class DCOBits
{
public:
    DCOBits() noexcept : m_most_derived(this)
    {
        size_t id = Type<DCOBits>::id();
        _mask |= (1 << id);
    }

    virtual ~DCOBits() = default;

    //__________________________________________________________________________________________________________________
    static size_t gen_id()
    {
        static size_t _id = 0;
        return ++_id;
    }

    template<typename T>
    struct Type
    {
        static size_t id()
        {
            static const size_t type_id = gen_id();
            return type_id;
        }
    };

    //__________________________________________________________________________________________________________________
    template<typename T>
    T const *fast_cast() const
    {
        size_t id = Type<T>::id();
        if (!(1 << id & _mask))
        {
            return nullptr;
        } else
        {
            return reinterpret_cast<T const *>(m_most_derived);
        }
    }

    template<typename T>
    T *fast_cast()
    {
        return const_cast<T *>((reinterpret_cast<DCOBits const &>(*this)).fast_cast<T>());
    }

    //__________________________________________________________________________________________________________________
    void *fast_cast() noexcept
    {
        return m_most_derived;
    }
//______________________________________________________________________________________________________________________
public:
    template<typename T>
    friend T *fast_cast(DCOBits *object);

    template<typename T>
    friend T const *fast_cast(DCOBits const *object);

    template<typename T>
    friend T &fast_cast(DCOBits &object);

    template<typename T>
    friend T const &fast_cast(DCOBits const &object);

    friend bool operator==(DCOBits const &lhs, DCOBits const &rhs);
//______________________________________________________________________________________________________________________
protected:
    template<typename T>
    void tie(T *t)
    {
        size_t id = Type<T>::id();
        assert(id <= 32 && "id is more than 32");
        _mask |= (1 << id);
    }
//______________________________________________________________________________________________________________________
private:
    uint32_t _mask = 0;
    void *m_most_derived;
};

template<>
void *DCOBits::fast_cast<void>()
{
    return m_most_derived;
}

template<>
void const *DCOBits::fast_cast<void>() const
{
    return m_most_derived;
}

//Friend_Wrappers:______________________________________________________________________________________________________
template<typename T>
T *fast_cast(DCOBits *object)
{ return object ? object->fast_cast<T>() : nullptr; }

template<>
void *fast_cast<void>(DCOBits *object)
{ return object ? object->m_most_derived : nullptr; }

template<typename T>
T const *fast_cast(DCOBits const *object)
{ return object ? object->fast_cast<T>() : nullptr; }

template<typename T>
T &fast_cast(DCOBits &object)
{
    if (T *t = object.fast_cast<T>())
    { return *t; }
    else
    { throw std::bad_cast(); }
}

template<typename T>
T const &fast_cast(DCOBits const &object)
{
    if (T const *t = object.fast_cast<T>())
    { return *t; }
    else
    { throw std::bad_cast(); }
}

//Operators:____________________________________________________________________________________________________________
bool operator==(DCOBits const &lhs, DCOBits const &rhs)
{
    return lhs._mask == rhs._mask && lhs.m_most_derived == rhs.m_most_derived;
}

#endif //GRAPHICS_DCOBITS_H
