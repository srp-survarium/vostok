////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PLAYER_CREATION_PARAMS_H_INCLUDED
#define BASE_PLAYER_CREATION_PARAMS_H_INCLUDED

#include <vostok/game_core/player_initial_info.h>
#include <vostok/game_core/character_recoil_params.h>
#include <vostok/game_core/character_dispersion_params.h>
#include <vostok/game_core/breath_holding_params.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/player_stealth.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/player_parameters_modifyer.h>

namespace vostok {
namespace collision {
	class animated_object;
}
}

namespace survarium {

struct base_player_creation_params {
public:
	/* 0x0000 */	player_initial_info					initial_info;
	/* 0x0010 */	character_recoil_params				recoil_params;
	/* 0x0020 */	character_dispersion_params			dispersion_params;
	/* 0x0058 */	breath_holding_params				breath_holding_params;
	/* 0x0080 */	player_stamina						initial_stamina;
	/* 0x00e8 */	player_stealth						initial_stealth;
	/* 0x0114 */	damage_model_ptr					damage_model;
	/* 0x0118 */	inventory_ptr						inventory;
	/* 0x011c */	collision::animated_object*			damage_collision;
	/* 0x0120 */	player_parameters_modifyer_ptr		player_parameters;
}; // struct base_player_creation_params

STATIC_SIZE_ASSERT(base_player_creation_params, 0x128);

} // namespace survarium

#endif // #ifndef BASE_PLAYER_CREATION_PARAMS_H_INCLUDED
