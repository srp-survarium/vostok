////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_PROTECTOR_H_INCLUDED
#define DAMAGE_PROTECTOR_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace stalker2 {

enum hit_affects_type_enum;

class damage_protector : public boost::noncopyable {
public:
    inline damage_protector( ): 
		reduce_damage_functor( ),
		protect_affect_functor( ),
		next(NULL)
    { }

    inline virtual ~damage_protector()
	{ }

public:
	boost::function< float ( char const*, char const*, float, float ) > reduce_damage_functor;
	boost::function< bool ( char const*, hit_affects_type_enum ) > protect_affect_functor;
	damage_protector* next;
}; // class damage_protector

} // namespace stalker2

#endif // #ifndef DAMAGE_PROTECTOR_H_INCLUDED