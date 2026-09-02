// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
#define GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED

namespace survarium {

// pulled ahead of batch 11 (a value member of this batch's human_npc)
class game_material_visibility_parameters {
public:
	inline	explicit	game_material_visibility_parameters	( float arg_0 ) : m_transparency( arg_0 ) { /* no source */ }

	inline	float		get_transparency_value				( ) const { /* no source */ return m_transparency; }

private:
	/* 0x0000 */	float		m_transparency;
}; // class game_material_visibility_parameters

STATIC_SIZE_ASSERT(game_material_visibility_parameters, 0x4);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
