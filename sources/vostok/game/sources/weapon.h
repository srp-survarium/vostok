////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/animation_callback.h>
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/model.h>
#include <vostok/resources_unmanaged_resource.h>

#include "fingers_to_weapon_corrector.h"
#include "rifle_scope.h"

namespace vostok {
namespace animation {
	struct animation_callback_params;
} // namespace animation
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok


namespace survarium {

class base_game_scene;
class game_world_ui;
class player;

class weapon_cook;
class weapon_sound_effect;

class weapon : public weapon_core {
	// the cook constructs/tears down weapon directly, touching private pfx/animation
	// members; codegen-neutral
	friend	class	weapon_cook;
	// weapon_sound_effect::on_sound_event reads m_game_scene directly (inlined) and
	// resolves the first/third-view branch through weapon-private members; codegen-neutral
	friend	class	weapon_sound_effect;
public:
								weapon								(
									u32		first_view_death_animations_count,
									u32		third_view_death_animations_count,
									u32		preview_animations_count
								);
	virtual						~weapon								( );

			void				play_weapon_shell_pfx				( );

			void				update_dispersion_visual_representation( );

	inline	u32					first_view_death_animations_count	( ) const { return m_first_view_death_animations_count; }
	inline	u32					third_view_death_animations_count	( ) const { return m_third_view_death_animations_count; }
	inline	u32					preview_animations_count			( ) const { return m_preview_animations_count; }
	// the death animations live in the trailing region of the weapon's combined malloc
	inline	resources::managed_resource_ptr const*	death_animations( ) const { return ( resources::managed_resource_ptr const* )( this + 1 ); }

	// the methods below mangle private (AAE / EAE) in the shipped PDB - declaring
	// them private keeps each out-of-line symbol pairable with the target
private:
			void				load_weapon							(
									render::skeleton_model_ptr const&	base_model,
									rifle_scope_ptr const&		rifle_scope
								);

	virtual	void				set_fire_bullet_transform			( float4x4 const& transform ) override;

	virtual	void				tick								( ) override;

	virtual	void				set_transform						( float4x4 const& transform ) override;
	virtual	void				set_target							( const weapon_targets new_target ) override;
	virtual	void				set_next_fire_queue_type			( ) override;
	virtual	void				set_next_ammo_type					( ) override;

	virtual	void				on_ammo_empty						( ) override;

			void				show_crosshair						( );
			void				hide_crosshair						( );

	inline	bool				is_in_scene							( ) const { return m_is_in_scene; }

	inline	void				set_fire_pfx						( resources::queries_result& arg_0 ) { /* no source */ }
	inline	void				set_shells_pfx						( resources::queries_result& arg_0 ) { /* no source */ }

			void				update_pfx_transform				( );

			void				play_weapon_fire_pfx				( );

			void				show_laser_pointer					( );

			void				set_ui_ammo							( bool update_total_count );

	virtual	void				activate							( base_player& user, engine& engine ) override;

	virtual	void				deactivate							( ) override;

	virtual	void				on_before_fire						( ) override;
	virtual	void				on_after_fire						( ) override;
	virtual	void				on_reload							( ) override;
	virtual	void				on_chamber_a_round					( ) override;
	virtual	void				on_reload_started					( ) override;
	virtual	void				on_show								( ) override;
	virtual	void				on_hide								( ) override;
	virtual	void				on_unload_chambered_round			( ) override;

	virtual	void				instant_aim_start					( ) override;
	virtual	void				instant_aim_end						( ) override;

	virtual	void				assign_game_ui						( game_world_ui* ui ) override		{ m_game_ui = ui; }

	virtual	void				on_skeleton_matrices_changed		(
									const u32					current_time_in_ms,
									float4x4 const&				weapon_transform,
									float4x4 const* const		weapon_matrices_begin,
									float4x4 const* const		weapon_matrices_end,
									float4x4 const&				user_transform,
									float4x4* const				user_matrices_begin,
									float4x4* const				user_matrices_end,
									float4x4 const&				__formal
								) override;

	virtual	void				process_finger_correction			( const u32 current_time_in_ms, float4x4* const user_matrices ) override;

			animation::callback_return_type_enum	on_foot_step						( animation::animation_callback_params& params );
			animation::callback_return_type_enum	on_shell_extraction_event			( animation::animation_callback_params& params );

			float4x4			calculate_locator					(
									render::model_locator_item const&		locator,
									float4x4 const*							matrices,
									const u32								matrices_count
								);

			animation::callback_return_type_enum	on_hand_correction_event			(
									animation::animation_callback_params&	params,
									const fingers_to_weapon_corrector::hands_enum	hand
								);
	virtual	void				on_user_sprint						( bool user_is_sprinting ) override;

	inline	bool				is_player_current					( ) const { /* no source */ return false; }

	// buildability cast through the incomplete player (weapon_core stores base_player*)
	inline	player&				user								( ) const { return *( player* )get_user( ); }

	inline	base_game_scene*	get_game_scene						( ) { return m_game_scene; }

private:
	/* 0x0000 */	/* weapon_core */
	/* 0x0498 */	float4x4								m_barrel_transform;
	/* 0x04d8 */	float4x4								m_scope_transform;
	/* 0x0518 */	float4x4								m_transform;
	/* 0x0558 */	float4x4								m_left_toe_transform;
	/* 0x0598 */	float4x4								m_right_toe_transform;
	/* 0x05d8 */	render::model_locator_item				m_barrel_locator;
	/* 0x063c */	render::model_locator_item				m_scope_locator;
	/* 0x06a0 */	fingers_to_weapon_corrector				m_fingers_corrector;
	/* 0x0eb8 */	render::light_props						m_weapon_fire_light_props;
	/* 0x0fa8 */	resources::unmanaged_resource_ptr*		m_fire_pfx_list;
	/* 0x0fac */	resources::unmanaged_resource_ptr*		m_shells_pfx_list;
	/* 0x0fb0 */	u8										m_fire_pfx_count;
	/* 0x0fb1 */	u8										m_shells_pfx_count;
	/* 0x0fb2 */	u8										m_current_shell_pfx_id;
	/* 0x0fb3 */	u8										m_current_fire_pfx_id;
	/* 0x0fb4 */	render::skeleton_model_ptr				model;
	/* 0x0fb8 */	game_world_ui*							m_game_ui;
	/* 0x0fbc */	rifle_scope_ptr							m_rifle_scope;
	/* 0x0fc0 */	base_game_scene*						m_game_scene;
	/* 0x0fc4 */	u32										m_weapon_fire_light_id;
	/* 0x0fc8 */	u32										m_current_fire_light_anim_time;
	/* 0x0fcc */	const u32								m_fire_light_anim_length;
	/* 0x0fd0 */	u32										m_last_tick_time_in_ms;
	/* 0x0fd4 */	u32										m_left_toe_bone_index;
	/* 0x0fd8 */	u32										m_right_toe_bone_index;
	/* 0x0fdc */	const u32								m_first_view_death_animations_count;
	/* 0x0fe0 */	const u32								m_third_view_death_animations_count;
	/* 0x0fe4 */	const u32								m_preview_animations_count;
	/* 0x0fe8 */	bool									m_firing_light_added;
	/* 0x0fe9 */	bool									m_is_in_scene;
	/* 0x0fea */	bool									m_is_scope_aimed;
}; // class weapon

STATIC_SIZE_ASSERT(weapon, 0xFF0);

} // namespace survarium

#endif // #ifndef WEAPON_H_INCLUDED
