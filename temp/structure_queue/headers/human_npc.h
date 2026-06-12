////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef HUMAN_NPC_H_INCLUDED
#define HUMAN_NPC_H_INCLUDED

/* INCLUDES */
enum const survarium::affect_event_type_enum;
enum const survarium::hit_affects_type_enum;
class vostok::render::game::renderer;
class vostok::sound::sound_producer;
class vostok::sound::sound_receiver;
class survarium::animations_selector;
class survarium::game_material_visibility_parameters;
class survarium::game_object_;
class survarium::game_world;
struct vostok::ai::animation_item;
struct vostok::ai::game_object;
struct vostok::ai::movement_target;
struct vostok::ai::npc;
struct vostok::ai::weapon;
struct vostok::ai::world;
struct vostok::physics::world;
struct vostok::sound::world;
struct survarium::affect_subscriber;
struct survarium::hit_receiver;
struct survarium::human_npc::npc_game_attributes;
class vostok::ai::animation_item const* const;
class vostok::ai::game_object const* const;
class vostok::ai::movement_target const* const;
class vostok::ai::npc const* const;
class vostok::ai::weapon const* const;
class vostok::math::aabb;
class vostok::math::color;
class survarium::bullet* const;
class survarium::hit_initiator const* const;
typedef vostok::resources::resource_ptr<survarium::animated_model_instance,vostok::resources::unmanaged_intrusive_base>
	survarium::animated_model_instance_ptr;
typedef vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base>
	survarium::animation_space_graph_ptr;
typedef vostok::resources::resource_ptr<survarium::human_npc,vostok::resources::unmanaged_intrusive_base>
	survarium::human_npc_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::render::base_scene;
class vostok::resources::managed_resource;
class vostok::resources::unmanaged_resource;
class survarium::animated_model_instance;
class survarium::animation_space_graph;
class survarium::human_npc;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class stlp_std::pair<vostok::ai::game_object const *,enum vostok::ai::ignorance_types_enum>;
class vostok::ai::npc_statistics;
class vostok::collision::bone_collision_data;
class vostok::configs::binary_config_value;
class vostok::vectora<vostok::ai::weapon *>;
class survarium::hit_object;
class survarium::object_weapon;

namespace survarium {

class human_npc : public ai::npc , public ai::game_object , public sound::sound_producer , public sound::sound_receiver , public hit_receiver , public game_object_ {
public:
			explicit							human_npc					( game_world& game_world );
	virtual										~human_npc					( );

	virtual	math::aabb							get_aabb					( ) const override;
	virtual	float4x4							get_eyes_matrix				( ) const override;
	virtual	math::color							get_color					( ) const override { /* no source */ }
	virtual	float3								get_eyes_direction			( ) const override;
	virtual	void								set_eyes_direction			( float3 const& direction ) override { /* no source */ }

	virtual	float3								get_position				( float3 const& requester ) const override;
			float3								get_position				( ) const;
	virtual	float3								get_eyes_position			( ) const override;

	virtual	void								clear_resources				( ) override;

	virtual	bool								debug_draw_allowed			( ) const override;

	virtual	pcstr								get_name					( ) const override { /* no source */ }
	virtual	u32									get_id						( ) const override { /* no source */ }
	virtual	u32									get_group_id				( ) const override { /* no source */ }
	virtual	u32									get_class_id				( ) const override { /* no source */ }
	virtual	u32									get_outfit_id				( ) const override { /* no source */ }

	virtual	bool								is_patrolling				( ) const override { /* no source */ }
	virtual	bool								is_at_cover					( ) const override { /* no source */ }
	virtual	bool								is_safe						( ) const override;
	virtual	bool								is_invisible				( ) const override { /* no source */ }
	virtual	bool								is_target_in_melee_range	( ai::npc const* const target ) const override;
	virtual	bool								is_at_node					( ai::game_object const* const node ) const override;

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

	virtual	pcstr								get_description				( ) const override { /* no source */ }
	virtual	float3								get_source_position			( float3 const& requester ) const override { /* no source */ }

	virtual	void								on_hit_event				( hit_object const& hit_source );

	virtual	ai::npc*							cast_npc					( ) override { /* no source */ }
	virtual	ai::npc const*						cast_npc					( ) const override { /* no source */ }
	virtual	ai::weapon*							cast_weapon					( ) override { /* no source */ }
	virtual	ai::weapon const*					cast_weapon					( ) const override { /* no source */ }

	virtual	float								get_velocity				( ) const override { /* no source */ }
	virtual	float								get_luminosity				( ) const override { /* no source */ }
	virtual	float3								get_random_surface_point	( const u32 current_time ) const override;

	virtual	float4x4							local_to_cell				( float3 const& requester ) const override;

	virtual	ai::game_object const*				cast_game_object			( ) const override { /* no source */ }

	virtual	void								load						(
													configs::binary_config_value const&		__formal,
													pcstr									__formal,
													boost::function< void( game_object_& ) >&	__formal
												) override { /* no source */ }

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

			void								set_attributes				( human_npc::npc_game_attributes& attributes );
	inline	void								set_rotation				( float4x4 const& arg_0 ) { /* no source */ }

	inline	float3								get_rotation_angles			( ) const { /* no source */ }

			void								on_animation_end			( );
			void								on_movement_end				( );

			void								set_behaviour				( resources::unmanaged_resource_ptr new_behaviour );

			void								select_new_goal				( );

	inline	render::game::renderer&				get_dbg_render				( ) const { /* no source */ }
	inline	render::base_scene_ptr const&		get_dbg_scene				( ) const { /* no source */ }

	inline	void								set_sound_dbg_mode			( bool arg_0 ) { /* no source */ }
	inline	bool								get_sound_dbg_mode			( ) const { /* no source */ }
	inline	float4x4 const&						get_transform				( ) { /* no source */ }
			void								set_transform				( float4x4 const& transform );
			void								set_brain_unit				( resources::unmanaged_resource_ptr const& brain_unit );
			void								set_model					( animated_model_instance_ptr const& model );

			void								draw						( render::game::renderer& render, render::base_scene_ptr const& scene ) const;

			void								draw_damage_model			( render::game::renderer& render, render::base_scene_ptr const& scene ) const;

			void								set_default_animation		( resources::managed_resource_ptr const& default_animation );
			void								set_animation_space_graph	( animation_space_graph_ptr const& space_graph );

			void								tick_animation_player		( const u32 current_time_in_ms );

			void								render_model				( );

			void								set_translation				( float4x4 const& new_translation );

			void								up_to_terrain				( );

			void								on_affect_event				(
													pcstr							body_part_name,
													const hit_affects_type_enum		arg_1 /* hit_affects_type_enum affect_type */,
													const affect_event_type_enum	arg_2 /* affect_event_type_enum event_type */
												) const;

	virtual	float								get_speed					( ) const override { /* no source */ }

	inline	human_npc*							return_this					( ) { /* no source */ }

private:
	/* 0x0000 */	/* ai::npc */
	/* 0x0004 */	/* ai::game_object */
	/* 0x000c */	/* sound::sound_producer */
	/* 0x001c */	/* sound::sound_receiver */
	/* 0x0024 */	/* hit_receiver */
	/* 0x0030 */	/* game_object_ */
	/* 0x0140 */	human_npc_ptr							next_npc;
	/* 0x0144 */	ai::world&								m_ai_world;
	/* 0x0148 */	sound::world&							m_sound_world;
	/* 0x014c */	physics::world&							m_physics_world;
	/* 0x0150 */	game_world&								m_game_world;
	/* 0x0154 */	resources::unmanaged_resource_ptr		m_brain_unit;
	/* 0x0158 */	render::game::renderer&					m_renderer;
	/* 0x015c */	animated_model_instance_ptr				m_model_instance;
	/* 0x0160 */	game_material_visibility_parameters		m_visibility_parameters;
	/* 0x0168 */	human_npc::npc_game_attributes			m_game_attributes;
	/* 0x0230 */	float4x4								m_transform;
	/* 0x0270 */	u32										m_last_tick_time_in_ms;
	/* 0x0274 */	render::base_scene_ptr					m_scene;
	/* 0x0278 */	resources::unmanaged_resource_ptr		m_sound_scene;
	/* 0x027c */	ai::animation_item const*				m_current_animation;
	/* 0x0280 */	ai::movement_target const*				m_current_movement_target;
	/* 0x0284 */	animations_selector*					m_animations_selector;
	/* 0x0288 */	ai::npc const*							m_current_target;
	/* 0x028c */	ai::weapon const*						m_current_weapon;
	/* 0x0290 */	bool									m_is_patrolling;
	/* 0x0298 */	affect_subscriber						m_affects_subscription;
	/* 0x02c0 */	bool									m_sound_perceived;
	/* 0x02c1 */	bool									m_sound_produced;
	/* 0x02c2 */	bool									m_dbg_sound;
	/* 0x02c4 */	resources::managed_resource_ptr			m_default_animation;
	/* 0x02c8 */	animation_space_graph_ptr				m_animation_space_graph;
	/* 0x02cc */	float3									m_feet_target;
	/* 0x02d8 */	float									m_feet_adjustment_speed;
}; // class human_npc

STATIC_SIZE_ASSERT(human_npc, 0x2E0);

} // namespace survarium

#endif // #ifndef HUMAN_NPC_H_INCLUDED
