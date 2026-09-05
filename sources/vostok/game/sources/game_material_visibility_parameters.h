// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
#define GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED

namespace survarium {

class game_material_visibility_parameters {
public:
	inline	explicit	game_material_visibility_parameters	( float arg_0 ) : m_transparency( arg_0 ) { }

	// sushi@TODO: Legacy transparency getter model; bind an original material-query consumer.
	inline	float		get_transparency_value				( ) const { return m_transparency; }

private:
	/* 0x0000 */	float		m_transparency;
}; // class game_material_visibility_parameters

STATIC_SIZE_ASSERT(game_material_visibility_parameters, 0x4);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_VISIBILITY_PARAMETERS_H_INCLUDED
