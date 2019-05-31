#ifndef GRAPHICS_DYNAMICCAST_H
#define GRAPHICS_DYNAMICCAST_H

#include <unordered_map>

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
    DynamicCastObject() = default;

    virtual ~DynamicCastObject() = default;

    template<class T>
    T const *fast_cast() const
    {
        char const *name = typeid(T).name();
        auto it = m_derived_hash_map.find(name);
        if (it == m_derived_hash_map.end())
        {
            return nullptr;
        }
        else
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
//______________________________________________________________________________________________________________________
protected:
    template<class T>
    void tie(T *t)
    {
        void *address = t;
        m_derived_hash_map[typeid(t).name()] = address;
    }
//______________________________________________________________________________________________________________________
private:
    typedef std::unordered_map<char const *, void *> derived_t;
    derived_t m_derived_hash_map;
};

#endif //GRAPHICS_DYNAMICCAST_H
