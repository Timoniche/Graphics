#ifndef GRAPHICS_DYNAMICCASTOBJECT_H
#define GRAPHICS_DYNAMICCASTOBJECT_H

#include <unordered_map>
#include <typeindex>

/**
 * Problems:
 * dynamic_cast<Derived*>(&base); returns null if base is not an instance of Derived.
 * dynamic_cast<Derived&>(base); throws std::bad_cast in this case.
 * dynamic_cast<void*>(base); returns the address of the most derived class
 * dynamic_cast respect the access specifications (public, protected and private inheritance)
 */
//______________________________________________________________________________________________________________________
class DynamicCastObject
{
public:
    DynamicCastObject() : m_most_derived(nullptr)
    {}

    virtual ~DynamicCastObject() = default;

    //__________________________________________________________________________________________________________________
    template<class T>
    T const *fast_cast() const
    {
        auto it = m_derived_hash_map.find(std::type_index(typeid(T)));
        if (it == m_derived_hash_map.end())
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
        DynamicCastObject const &this_const = *this;
        return const_cast<T *>(this_const.fast_cast<T>());
    }

    void *fast_cast()
    {
        return m_most_derived;
    }
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


#endif //GRAPHICS_DYNAMICCASTOBJECT_H
