////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef HUMAN_NPC_H_INCLUDED
#define HUMAN_NPC_H_INCLUDED

#include <vostok/ai/npc.h>
#include <vostok/ai/game_object.h>
#include <vostok/ai/filter_types.h>
#include <vostok/sound/sound_producer.h>
#include <vostok/sound/sound_receiver.h>
#include <vostok/game_core/hit_receiver.h>
#include <vostok/game_core/damage_model.h>	// affect_subscriber value member
#include <vostok/resources_managed_resource.h>
#include <vostok/resources_unmanaged_resource.h>
// PDB spells the scene member vostok::render::base_scene_ptr - the same
// resource_ptr type our render tree typedefs as scene_ptr
#include <vostok/render/engine/base_classes.h>

#include "game_object_.h"
#include "game_material_visibility_parameters.h"
#include "object_weapon.h"

namespace vostok {
namespace ai {
	struct animation_item;
	struct movement_target;
	struct npc_statistics;
	struct world;
} // namespace ai
namespace collision {
	class bone_collision_data;
} // namespace collision
namespace physics {
	struct world;
} // namespace physics
namespace render {
namespace game {
	class renderer;
} // namespace game
} // namespace render
namespace sound {
	struct world;
} // namespace sound
} // namespace vostok

namespace survarium {

class animations_selector;
class bullet;
class game_world;
class human_npc_cook;
class hit_initiator;
struct hit_object;

struct animated_model_instance;
typedef resources::resource_ptr<
	animated_model_instance,
	resources::unmanaged_intrusive_base
> animated_model_instance_ptr;

class animation_space_graph;
typedef resources::resource_ptr<
	animation_space_graph,
	resources::unmanaged_intrusive_base
> animation_space_graph_ptr;

class human_npc;
typedef resources::resource_ptr<
	human_npc,
	resources::unmanaged_intrusive_base
> human_npc_ptr;

// void* human_npc::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0xabb50]: <0xabb40>|0x000|      :'235'	{

class human_npc : public ai::npc , public ai::game_object , public sound::sound_producer , public sound::sound_receiver , public hit_receiver , public game_object_ {
public:
			explicit							human_npc					( game_world& game_world );
	virtual										~human_npc					( );

	/* 0x0140 */	human_npc_ptr							next_npc;

	typedef vostok::intrusive_list< object_weapon,
			object_weapon *,
			&object_weapon::m_next,
			vostok::threading::mutex,
			vostok::size_policy,
			vostok::no_debug_policy > weapons_type;

	struct npc_game_attributes {
											npc_game_attributes	( );

				npc_game_attributes&		operator=			( npc_game_attributes& other );

public:
		/* 0x0000 */	weapons_type			weapons;
		/* 0x0030 */	float3					initial_position;
		/* 0x003c */	float3					initial_scale;
		/* 0x0048 */	float3					initial_rotation;
		/* 0x0054 */	math::color				debug_draw_color;
		/* 0x0058 */	fixed_string< 32 >		name;
		/* 0x0084 */	fixed_string< 32 >		description;
		/* 0x00b0 */	float					initial_velocity;
		/* 0x00b4 */	float					initial_luminosity;
		/* 0x00b8 */	u32						id;
		/* 0x00bc */	u32						group_id;
		/* 0x00c0 */	u32						class_id;
		/* 0x00c4 */	u32						outfit_id;
	}; // struct npc_game_attributes

	virtual	math::aabb							get_aabb					( ) const override;

	virtual	float4x4							get_eyes_matrix				( ) const override;

	virtual	math::color							get_color					( ) const override
	{
		return m_game_attributes.debug_draw_color;
	}
	virtual	float3								get_eyes_direction			( ) const override;

	virtual	void								set_eyes_direction			( float3 const& direction ) override
	{
	}

	virtual	float3								get_position				( float3 const& requester ) const override;
private:
			float3								get_position				( ) const;
public:
	virtual	float3								get_eyes_position			( ) const override;

	virtual	void								clear_resources				( ) override;

	virtual	bool								debug_draw_allowed			( ) const override;

	virtual	pcstr								get_name					( ) const override
	{
		return m_game_attributes.name.c_str( );
	}

	virtual	u32									get_id						( ) const override
	{
		return m_game_attributes.id;
	}

	virtual	u32									get_group_id				( ) const override
	{
		return m_game_attributes.group_id;
	}

	virtual	u32									get_class_id				( ) const override
	{
		return m_game_attributes.class_id;
	}

	virtual	u32									get_outfit_id				( ) const override
	{
		return m_game_attributes.outfit_id;
	}

	virtual	bool								is_patrolling				( ) const override
	{
		return m_is_patrolling;
	}

	virtual	bool								is_at_cover					( ) const override
	{
		return false;
	}
	virtual	bool								is_safe						( ) const override;
	virtual	bool								is_invisible				( ) const override { /* no source */ return false; }
	virtual	bool								is_target_in_melee_range	( ai::npc const* const target ) const override;
	virtual	bool								is_at_node					( ai::game_object const* const node ) const override;

	// inline ai::npc / ai::game_object pure-virtual overrides absent from the
	// canonical dump (no out-of-line symbol; inlined at the call sites)

	virtual	void								prepare_to_attack			( ai::npc const* const target, ai::weapon const* const gun ) override;

	virtual	void								attack						( ai::npc const* const target, ai::weapon const* const gun ) override;

	virtual	void								attack_melee				( ai::npc const* const target, ai::weapon const* const gun ) override;
	virtual	void								attack_from_cover			( ai::npc const* const target, ai::weapon const* const gun ) override;

	virtual	void								stop_attack					( ai::npc const* const target, ai::weapon const* const gun ) override;

	virtual	void								survey_area					( ) override;

	virtual	void								stop_patrolling				( ) override;

	virtual	void								reload						( ai::weapon const* const gun ) override;

	virtual	void								play_animation				( ai::animation_item const* const target ) override;

	virtual	void								move_to_position			( ai::movement_target const* const target ) override;

	virtual	void								on_sound_event				( sound::sound_producer const& sound_source ) override;

	virtual	pcstr								get_description				( ) const override
	{
		return m_game_attributes.description.c_str( );
	}

	virtual	float3								get_source_position			( float3 const& requester ) const override
	{
		return get_position( requester );
	}

	virtual	void								on_hit_event				( hit_object const& hit_source );

	virtual	ai::npc*							cast_npc					( ) override
	{
		return this;
	}
	virtual	ai::npc const*						cast_npc					( ) const override { /* no source */ return this; }
	virtual	ai::weapon*							cast_weapon					( ) override { /* no source */ return NULL; }
	virtual	ai::weapon const*					cast_weapon					( ) const override { /* no source */ return NULL; }

	virtual	float								get_velocity				( ) const override
	{
		return m_game_attributes.initial_velocity;
	}

	virtual	float								get_luminosity				( ) const override
	{
		return m_game_attributes.initial_luminosity;
	}
	virtual	float3								get_random_surface_point	( const u32 current_time ) const override;

	virtual	float4x4							local_to_cell				( float3 const& requester ) const override;

	virtual	ai::game_object const*				cast_game_object			( ) const override
	{
		return this;
	}

	virtual	void								load						(
													configs::binary_config_value const&		__formal,
													pcstr									__formal_1,
													boost::function< void( game_object_& ) >&	__formal_2
												) override
	{
	}

	virtual	void								hit							(
													hit_initiator const* const		initiator,
													const u32						bone_index,
													pcstr							damage_type,
													const float						amount,
													const float						armor_piercing,
													bullet* const					bullet
												) override;
	virtual	void								hit							(
													hit_initiator const* const		initiator,
													collision::bone_collision_data const&	bone_data,
													pcstr							damage_type,
													const float						amount,
													const float						armor_piercing,
													bullet* const					bullet
												) override;

	inline	void								set_filter					(
													std::pair< ai::game_object const*, enum ai::ignorance_types_enum > const*	arg_0,
													std::pair< ai::game_object const*, enum ai::ignorance_types_enum > const*	arg_1
												) { /* no source */ }
	inline	void								clear_filter				( ) { /* no source */ }

			void								fill_stats					( ai::npc_statistics& stats ) const;

			void								tick						( const u32 current_time_in_ms, const bool is_game_paused );

	inline	void								add_weapon					( object_weapon* arg_0 ) { /* no source */ }
	inline	void								remove_weapon				( object_weapon* arg_0 ) { /* no source */ }
			object_weapon*						pop_weapon					( );

			void								get_available_weapons		( vectora< ai::weapon* >& list_to_be_filled ) const;

			void								enable						( );

			void								set_attributes				( npc_game_attributes& attributes );
	inline	void								set_rotation				( float4x4 const& arg_0 ) { /* no source */ }

	inline	float3								get_rotation_angles			( ) const { /* no source */ return float3( ); }

			void								on_animation_end			( );
			void								on_movement_end				( );

			void								set_behaviour				( resources::unmanaged_resource_ptr new_behaviour );

			void								select_new_goal				( );

	inline	render::game::renderer&				get_dbg_render				( ) const { /* no source */ return m_renderer; }
	inline	render::scene_ptr const&			get_dbg_scene				( ) const { /* no source */ return m_scene; }

	inline	void								set_sound_dbg_mode			( bool arg_0 ) { /* no source */ }
	inline	bool								get_sound_dbg_mode			( ) const { /* no source */ return m_dbg_sound; }
	inline	float4x4 const&						get_transform				( ) { /* no source */ return m_transform; }
			void								set_transform				( float4x4 const& transform );
private:
			void								set_brain_unit				( resources::unmanaged_resource_ptr const& brain_unit );
			void								set_model					( animated_model_instance_ptr const& model );

			void								draw						( render::game::renderer& render, render::scene_ptr const& scene ) const;

			void								draw_damage_model			( render::game::renderer& render, render::scene_ptr const& scene ) const;

			void								set_default_animation		( resources::managed_resource_ptr const& default_animation );
			void								set_animation_space_graph	( animation_space_graph_ptr const& space_graph );

			void								tick_animation_player		( const u32 current_time_in_ms );

			void								render_model				( );

			void								set_translation				( float4x4 const& new_translation );

			void								up_to_terrain				( );

			void								on_affect_event				(
													pcstr							body_part_name,
													const hit_affects_type_enum		affect_type,
													const affect_event_type_enum	event_type
												) const;

	virtual	float								get_speed					( ) const override { return 1.0f; }

	inline	human_npc*							return_this					( ) { /* no source */ return this; }

	friend class human_npc_cook;

	/* 0x0000 */	/* ai::npc */
	/* 0x0004 */	/* ai::game_object */
	/* 0x000c */	/* sound::sound_producer */
	/* 0x001c */	/* sound::sound_receiver */
	/* 0x0024 */	/* hit_receiver */
	/* 0x0030 */	/* game_object_ */
private:
	/* 0x0144 */	ai::world&								m_ai_world;
	/* 0x0148 */	sound::world&							m_sound_world;
	/* 0x014c */	physics::world&							m_physics_world;
	/* 0x0150 */	game_world&								m_game_world;
	/* 0x0154 */	resources::unmanaged_resource_ptr		m_brain_unit;
	/* 0x0158 */	render::game::renderer&					m_renderer;
	/* 0x015c */	animated_model_instance_ptr				m_model_instance;
	/* 0x0160 */	game_material_visibility_parameters		m_visibility_parameters;
	/* 0x0168 */	npc_game_attributes						m_game_attributes;
	/* 0x0230 */	float4x4								m_transform;
	/* 0x0270 */	u32										m_last_tick_time_in_ms;
	/* 0x0274 */	render::scene_ptr						m_scene;
	/* 0x0278 */	resources::unmanaged_resource_ptr		m_sound_scene;
	/* 0x027c */	ai::animation_item const*				m_current_animation;
	/* 0x0280 */	ai::movement_target const*				m_current_movement_target;
	/* 0x0284 */	animations_selector*					m_animations_selector;
	/* 0x0288 */	ai::npc const*							m_current_target;
	/* 0x028c */	ai::weapon const*						m_current_weapon;
	/* 0x0290 */	bool									m_is_patrolling;
	/* 0x0298 */	affect_subscriber						m_affects_subscription;
	/* 0x02c0 */	mutable bool							m_sound_perceived;
	/* 0x02c1 */	mutable bool							m_sound_produced;
	/* 0x02c2 */	bool									m_dbg_sound;
	/* 0x02c4 */	resources::managed_resource_ptr			m_default_animation;
	/* 0x02c8 */	animation_space_graph_ptr				m_animation_space_graph;
	/* 0x02cc */	float3									m_feet_target;
	/* 0x02d8 */	float									m_feet_adjustment_speed;
}; // class human_npc

STATIC_SIZE_ASSERT(human_npc, 0x2E0);
STATIC_SIZE_ASSERT(human_npc::npc_game_attributes, 0xC8);

} // namespace survarium

#endif // #ifndef HUMAN_NPC_H_INCLUDED
