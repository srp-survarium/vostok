////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_TYPE_PARAMETERS_H_INCLUDED
#define HIT_TYPE_PARAMETERS_H_INCLUDED

#include <xray/fixed_string.h>
#include <boost/noncopyable.hpp>

namespace stalker2 {

class body_part_parameters;

class hit_type_parameters : private boost::noncopyable {
public:
	hit_type_parameters (
		const char* type,
		float absorption,
		float armor,
		float reduce,
		unsigned int bdb_count
		);

	void apply_damage(
		float delta,
		unsigned int time_in_ms
		);

	void set_parameters(
		float armor,
		float reduce,
		float absorbtion
		);

	inline std::pair<body_part_parameters*, float> const* get_body_parts( ) const {
		return pointer_cast< std::pair<body_part_parameters*, float> const* >( pointer_cast< pcbyte >( this ) + sizeof( *this ) );
	}

private:
	hit_type_parameters* next;
	const xray::fixed_string<16> m_type;
	float m_absorption_amount;
	float m_armor;
	float m_reduce;
	const unsigned int m_bdb_count;
}; // class hit_type_parameters

} // namespace stalker2

#endif // #ifndef HIT_TYPE_PARAMETERS_H_INCLUDED