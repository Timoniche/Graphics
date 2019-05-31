#ifndef GRAPHICS_DYNAMICCAST_H
#define GRAPHICS_DYNAMICCAST_H

/**
 * dynamic_cast<Derived*>(&base); returns null if base is not an instance of Derived.
 * dynamic_cast<Derived&>(base); throws std::bad_cast in this case.
 * dynamic_cast<void*>(base); returns the address of the most derived class
 * dynamic_cast respect the access specifications (public, protected and private inheritance)
 */
class DynamicCast
{

};

#endif //GRAPHICS_DYNAMICCAST_H
