////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_TYPE_PARAMETERS_H_INCLUDED
#define HIT_TYPE_PARAMETERS_H_INCLUDED

#include <xray/fixed_string.h>
#include <boost/noncopyable.hpp>

namespace stalker2 {

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class body_part_parameters;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class hit_type_parameters : public boost::noncopyable {
public:
	hit_type_parameters(
		pcstr                              type,
		float                              absorption,
		float                              armor,
		float                              reduce,
		u32                                bdb_count);

	// STATE[UNVERIFIED] sushi@TODO: it was const before, now I removed it. Why was it here and why current header gen doesn't see those? Am I to blame?
	std::pair<body_part_parameters *,float>* get_bdb_coefficients() {
		return pointer_cast< std::pair<body_part_parameters*, float> * >( pointer_cast< pbyte >( this ) + sizeof( *this ) );
	}

	pcstr get_type() const { return m_type.c_str(); }

	float get_armor() const { return m_armor; }

	float get_reduce() const { return m_reduce; }

	float get_absorption() const { return m_absorption_amount; }

	u32 get_bdb_coeffs_count() const { return m_bdb_count; }

	void apply_damage(
		float                              delta,
		u32                                time_in_ms);

	// void remove_vertex(body_part_parameters*) /* no source */; sushi@TODO

	void set_parameters(
		float                              armor,
		float                              reduce,
		float                              absorbtion);

public:
	/* offset 0x0000 */ hit_type_parameters*                next;
	/* offset 0x0004 */ xray::fixed_string<16>              m_type;
	/* offset 0x0020 */ float                               m_absorption_amount;
	/* offset 0x0024 */ float                               m_armor;
	/* offset 0x0028 */ float                               m_reduce;
	/* offset 0x002c */ u32                                 m_bdb_count;
}; // class hit_type_parameters

namespace {
	typedef char size_assert[
		sizeof(hit_type_parameters) == 0x30 ? 1 : -1
	];
}

} // namespace stalker2

#endif // #ifndef HIT_TYPE_PARAMETERS_H_INCLUDED