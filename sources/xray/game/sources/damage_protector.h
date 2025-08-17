////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_PROTECTOR_H_INCLUDED
#define DAMAGE_PROTECTOR_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace stalker2 {

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

enum hit_affects_type_enum;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct damage_protector : public boost::noncopyable {
public:
    damage_protector( ): next(NULL) {}
	virtual ~damage_protector( );

public:
	boost::function< float ( pcstr, pcstr, float, float ) > reduce_damage_functor;
	boost::function< bool ( pcstr, hit_affects_type_enum ) > protect_affect_functor;
	damage_protector* next;
}; // class damage_protector

namespace { 
	typedef char size_assert[
		sizeof(damage_protector) == 0x50 ? 1 : -1
	];
}

} // namespace stalker2

#endif // #ifndef DAMAGE_PROTECTOR_H_INCLUDED