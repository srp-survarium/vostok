////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef HIT_TYPE_PARAMETERS_H_INCLUDED
#define HIT_TYPE_PARAMETERS_H_INCLUDED

namespace survarium {

class body_part_parameters;


class hit_type_parameters : public boost::noncopyable {
public:
										hit_type_parameters	(
											pcstr		type,
											float		absorption,
											float		armor,
											float		reduce,
											u32			bdb_count
										);


			std::pair<body_part_parameters *,float>*
										get_bdb_coefficients( )
			{
				return pointer_cast< std::pair<body_part_parameters*, float> * >( pointer_cast< pbyte >( this ) + sizeof( *this ) );
			}

	inline	pcstr						get_type			( ) const { return m_type.c_str(); }
	inline	float						get_armor			( ) const { return m_armor; }
	inline	float						get_reduce			( ) const { return m_reduce; }
	inline	float						get_absorption		( ) const { return m_absorption_amount; }
	inline	u32							get_bdb_coeffs_count( ) const { return m_bdb_count; }

			void						apply_damage		( float delta, u32 time_in_ms );
	inline	void						remove_vertex		( body_part_parameters* arg_0 ) { /* no source */ }
			void						set_parameters		( float armor, float reduce, float absorbtion );

public:
	/* 0x0000 */	hit_type_parameters*	next;
	/* 0x0004 */	fixed_string<16>		m_type;
	/* 0x0020 */	float					m_absorption_amount;
	/* 0x0024 */	float					m_armor;
	/* 0x0028 */	float					m_reduce;
	/* 0x002c */	u32						m_bdb_count;
}; // class hit_type_parameters

STATIC_SIZE_ASSERT(hit_type_parameters, 0x30);

} // namespace survarium

#endif // #ifndef HIT_TYPE_PARAMETERS_H_INCLUDED
