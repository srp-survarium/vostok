// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_CREATION_PARAMS_H_INCLUDED
#define PLAYER_CREATION_PARAMS_H_INCLUDED

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/interactive_object.h>
// PDB spells the model members vostok::render::skeleton_model_instance_ptr -
// the same resource_ptr type our render tree typedefs as skeleton_model_ptr
#include <vostok/render/facade/model.h>

namespace survarium {

class base_game_scene;
class items_dictionary;

struct player_creation_params : public base_player_creation_params {

public:
	/* 0x0000 */	/* base_player_creation_params */
	/* 0x0128 */	render::skeleton_model_ptr		character_model;
	/* 0x012c */	render::skeleton_model_ptr		server_character_model;
	/* 0x0130 */	base_game_scene*				game_scene;
	/* 0x0134 */	items_dictionary*				items_dictionary;
	/* 0x0138 */	interactive_object_ptr			empty_hands;
	/* 0x013c */	u8								foot_3rd_view_game_material_id;
	/* 0x013d */	u8								foot_1st_view_game_material_id;
}; // struct player_creation_params

STATIC_SIZE_ASSERT(player_creation_params, 0x140);

} // namespace survarium

#endif // #ifndef PLAYER_CREATION_PARAMS_H_INCLUDED
