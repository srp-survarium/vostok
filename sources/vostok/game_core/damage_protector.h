////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_PROTECTOR_H_INCLUDED
#define DAMAGE_PROTECTOR_H_INCLUDED

#include <vostok/game_core/hit_affects_type_enum.h>

namespace survarium {

struct damage_protector : private boost::noncopyable {
public:
	typedef boost::function< float ( pcstr, pcstr, float, float ) > reduce_damage_functor_type;
	typedef boost::function< bool ( pcstr, hit_affects_type_enum ) > protect_affect_functor_type;

			damage_protector	( ) : next( NULL ) {}
	virtual ~damage_protector	( ) {}

public:
	reduce_damage_functor_type reduce_damage_functor;
	protect_affect_functor_type protect_affect_functor;
	damage_protector* next;
}; // class damage_protector

STATIC_SIZE_ASSERT(damage_protector, 0x50);

} // namespace survarium

#endif // #ifndef DAMAGE_PROTECTOR_H_INCLUDED
