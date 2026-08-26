////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MATERIAL_H_INCLUDED
#define GAME_MATERIAL_H_INCLUDED

namespace survarium {

extern u16 g_material_physics_group[64];

class game_material : private boost::noncopyable {
public:
						game_material			( );

			void		load_from_config		( configs::binary_config_value const& val );

	inline	u16			id						( ) const { return m_id; }

	inline	float		resistance				( ) const { return m_material_resistance; }
	inline	float		reflection_speed_down	( ) const { return m_bullet_reflection_speed_down; }
	inline	float		ricochet_koef			( ) const { return m_ricochet_koef; }

	inline	float		width					( ) const { return m_width; }
	inline	pcstr		name					( ) const { return m_name.c_str( ); }

	inline	bool		can_place_mine			( ) const { return m_mine_can_place; }
	inline	bool		can_stick_mine			( ) const { return m_mine_can_stick; }

	typedef fixed_string< 64 > material_name;

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	material_name		m_name;
	/* 0x004c */	float				m_material_resistance;
	/* 0x0050 */	float				m_bullet_reflection_speed_down;
	/* 0x0054 */	float				m_width;
	/* 0x0058 */	float				m_ricochet_koef;
	/* 0x005c */	u16					m_id;
	/* 0x005e */	bool				m_mine_can_place;
	/* 0x005f */	bool				m_mine_can_stick;
}; // class game_material

STATIC_SIZE_ASSERT(game_material, 0x60);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_H_INCLUDED
