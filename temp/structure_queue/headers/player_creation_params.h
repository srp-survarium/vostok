////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_CREATION_PARAMS_H_INCLUDED
#define PLAYER_CREATION_PARAMS_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
class survarium::items_dictionary;
struct survarium::base_player_creation_params;
typedef vostok::resources::resource_ptr<survarium::interactive_object,vostok::resources::unmanaged_intrusive_base>
	survarium::interactive_object_ptr;
typedef vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skeleton_model_instance_ptr;
class vostok::render::skeleton_model_instance;
class survarium::interactive_object;

namespace survarium {

struct player_creation_params : public base_player_creation_params {
	inline		player_creation_params	( ) { /* no source */ }
	inline		~player_creation_params	( ) { /* no source */ }

public:
	/* 0x0000 */	/* base_player_creation_params */
	/* 0x0128 */	render::skeleton_model_instance_ptr		character_model;
	/* 0x012c */	render::skeleton_model_instance_ptr		server_character_model;
	/* 0x0130 */	base_game_scene*						game_scene;
	/* 0x0134 */	items_dictionary*						items_dictionary;
	/* 0x0138 */	interactive_object_ptr					empty_hands;
	/* 0x013c */	u8										foot_3rd_view_game_material_id;
	/* 0x013d */	u8										foot_1st_view_game_material_id;
}; // struct player_creation_params

STATIC_SIZE_ASSERT(player_creation_params, 0x140);

} // namespace survarium

#endif // #ifndef PLAYER_CREATION_PARAMS_H_INCLUDED
