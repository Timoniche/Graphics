#ifndef GRAPHICS_DYNAMICCASTOBJECT_H
#define GRAPHICS_DYNAMICCASTOBJECT_H

#include <unordered_map>
#include <typeindex>

/**
 * returns null if base is not an instance of Derived:
 * base.fast_cast<Derived>() OR fast_cast<Derived>(&base)
 * throws std::bad_cast in this case:
 * fast_cast<Derived>(base)
 * returns the address of the most derived class:
 * base.fast_cast<void>() OR fast_cast<void>(&base)
 * NB: respect the access specifications (public/protected/private inheritance)
 */
//______________________________________________________________________________________________________________________
class DynamicCastObject
{
public:
    DynamicCastObject() noexcept : m_most_derived(nullptr)
    {}

    virtual ~DynamicCastObject() = default;

    //__________________________________________________________________________________________________________________
    template<class T>
    T const *fast_cast() const
    {
        if (auto it = m_derived_hash_map.find(std::type_index(typeid(T)));
                it == m_derived_hash_map.end())
        {
            return nullptr;
        } else
        {
            return reinterpret_cast<T const *>(it->second);
        }
    }

    template<class T>
    T *fast_cast()
    {
        return const_cast<T *>((reinterpret_cast<DynamicCastObject const &>(*this)).fast_cast<T>());
    }

    //__________________________________________________________________________________________________________________
    void *fast_cast() noexcept
    {
        return m_most_derived;
    }
//______________________________________________________________________________________________________________________
public:
    template<class T>
    friend T *fast_cast(DynamicCastObject *object);

    template<class T>
    friend T const *fast_cast(DynamicCastObject const *object);

    template<class T>
    friend T &fast_cast(DynamicCastObject &object);

    template<class T>
    friend T const &fast_cast(DynamicCastObject const &object);

    friend bool operator==(DynamicCastObject const& lhs, DynamicCastObject const& rhs);
//______________________________________________________________________________________________________________________
protected:
    template<class T>
    void tie(T *t)
    {
        void *address = t;
        m_derived_hash_map[std::type_index(typeid(t))] = address;
        if (m_most_derived == nullptr ||
            m_most_derived > address)
        {
            m_most_derived = address;
        }
    }
//______________________________________________________________________________________________________________________
private:
    typedef std::unordered_map<std::type_index, void *> derived_t;
    derived_t m_derived_hash_map;
    void *m_most_derived;
};

template<>
void *DynamicCastObject::fast_cast<void>()
{
    return m_most_derived;
}

template<>
void const *DynamicCastObject::fast_cast<void>() const
{
    return m_most_derived;
}

//Friend_Wrappers:______________________________________________________________________________________________________
template<class T>
T *fast_cast(DynamicCastObject *object)
{ return object ? object->fast_cast<T>() : nullptr; }

template<>
void *fast_cast<void>(DynamicCastObject *object)
{ return object ? object->m_most_derived : nullptr; }

template<class T>
T const *fast_cast(DynamicCastObject const *object)
{ return object ? object->fast_cast<T>() : nullptr; }

template<class T>
T &fast_cast(DynamicCastObject &object)
{
    if (T *t = object.fast_cast<T>())
    { return *t; }
    else
    { throw std::bad_cast(); }
}

template<class T>
T const &fast_cast(DynamicCastObject const &object)
{
    if (T const *t = object.fast_cast<T>())
    { return *t; }
    else
    { throw std::bad_cast(); }
}
//Operators:____________________________________________________________________________________________________________
bool operator==(DynamicCastObject const& lhs, DynamicCastObject const& rhs)
{
    return lhs.m_derived_hash_map == rhs.m_derived_hash_map;
}

#endif //GRAPHICS_DYNAMICCASTOBJECT_H
