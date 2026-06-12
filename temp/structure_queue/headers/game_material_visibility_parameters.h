////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
#define GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED

namespace survarium {

class game_material_visibility_parameters {
public:
	inline	explicit	game_material_visibility_parameters	( float arg_0 ) { /* no source */ }

	inline	float		get_transparency_value				( ) const { /* no source */ }

private:
	/* 0x0000 */	float		m_transparency;
}; // class game_material_visibility_parameters

STATIC_SIZE_ASSERT(game_material_visibility_parameters, 0x4);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
