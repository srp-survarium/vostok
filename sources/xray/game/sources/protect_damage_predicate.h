////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PROTECT_DAMAGE_PREDICATE_H_INCLUDED
#define PROTECT_DAMAGE_PREDICATE_H_INCLUDED

#include <boost/noncopyable.hpp>

namespace stalker2 {

class damage_protector;

/* survarium::protect_damage_predicate */

struct protect_damage_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	protect_damage_predicate(
		float							   armor_piercing,
		pcstr							   damage_type, 
		pcstr							   body_type_name,
		float							   amount) :
		m_body_type_name				   (body_type_name),
		m_damage_type					   (damage_type),
		m_armor_piercing				   (armor_piercing),
		m_amount					       (amount) {}

	void operator()(
		damage_protector*                  protector);

private:
	/* offset 0x0000 */ pcstr                               m_body_type_name;
	/* offset 0x0004 */ pcstr                               m_damage_type;
	/* offset 0x0008 */ float                               m_armor_piercing;
	/* offset 0x000c */ float                               m_amount;
}; // struct protect_damage_predicate
} // namespace stalker2

#endif // #ifndef PROTECT_DAMAGE_PREDICATE_H_INCLUDED