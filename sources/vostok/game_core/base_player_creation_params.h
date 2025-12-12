////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PLAYER_CREATION_PARAMS_H_INCLUDED
#define BASE_PLAYER_CREATION_PARAMS_H_INCLUDED

namespace survarium {

struct survarium::breath_holding_params;
struct survarium::character_dispersion_params;
struct survarium::character_recoil_params;
struct survarium::player_initial_info;
class survarium::player_stamina;
class survarium::player_stealth;
class vostok::collision::animated_object;
class vostok::resources::resource_ptr<survarium::damage_model,vostok::resources::unmanaged_intrusive_base>;
class vostok::resources::resource_ptr<survarium::inventory,vostok::resources::unmanaged_intrusive_base>;
class vostok::resources::resource_ptr<survarium::player_parameters_modifyer,vostok::resources::unmanaged_intrusive_base>;


struct base_player_creation_params {
public:
			base_player_creation_params	( );
	inline	~base_player_creation_params( ) { /* no source */ }


public:
	/* 0x0000 */	player_initial_info					initial_info;
	/* 0x0010 */	character_recoil_params				recoil_params;
	/* 0x0020 */	character_dispersion_params			dispersion_params;
	/* 0x0058 */	breath_holding_params				breath_holding_params;
	/* 0x0080 */	player_stamina						initial_stamina;
	/* 0x00e8 */	player_stealth						initial_stealth;
	/* 0x0114 */	resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base>	damage_model;
	/* 0x0118 */	resources::resource_ptr<inventory,resources::unmanaged_intrusive_base>	inventory;
	/* 0x011c */	collision::animated_object*			damage_collision;
	/* 0x0120 */	resources::resource_ptr<player_parameters_modifyer,resources::unmanaged_intrusive_base>	player_parameters;
}; // struct base_player_creation_params

STATIC_SIZE_ASSERT(base_player_creation_params, 0x128);

} // namespace survarium

#endif // #ifndef BASE_PLAYER_CREATION_PARAMS_H_INCLUDED
