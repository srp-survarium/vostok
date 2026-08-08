////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SCENE_H_INCLUDED
#define VOSTOK_SOUND_SCENE_H_INCLUDED

#include <vostok/sound/sound.h>
#include <xaudio2fx.h>
#include <vostok/unmanaged_allocation_resource.h>
#include <vostok/memory_single_size_buffer_allocator.h>
#include <vostok/fixed_string.h>
#include <vostok/collision/object.h>
#include <vostok/render/culling/portal_sector_structure.h>
#include <vostok/sound/sound_receiver.h>
#include "sound_instance_proxy_internal.h"
#include "sound_voice.h"
#include "panning_lut.h"
#include <vostok/sound/sound_scene_statistic.h>
#ifndef MASTER_GOLD
#include "debug_snapshot.h"
#endif // MASTER_GOLD


namespace vostok {

namespace resources {
class query_result_for_cook;
} // namespace resources

namespace sound {

class sound_debug_stats;
class sound_world;
struct effect_cross_fader;
struct sound_scene_creation_params;
struct create_sound_propagator_params;
class sound_environment;

namespace statistics {

struct proxy_stats
{
	float3			m_position;
	float3			m_direction;
	emitter_type	m_emitter_type; // single, composite, collection
	sound_type		m_sound_type; // point, spot, volumetric
	u32				m_id;
};

struct propagator_stats {
	fs::path_string m_filename;
	float3			m_position;
	sound_rms_ptr	m_rms;
	u32				m_owner_proxy_id;
	float			m_inner_radius;
	float			m_outer_radius;
	u32				m_before_playing_offset;
	u32				m_after_playing_offset;
	u32				m_sound_lenght;
	u32				m_playing_offset;
	bool			m_has_voice;
}; // struct propagator_stats

struct sound_scene_stats {
	buffer_vector<propagator_stats>*	m_propagators;
	u32									m_current_playing_files_count;
	u32									m_active_proxies_count;
}; // struct sound_scene_stats

} // namespace statistaics

class receiver_collision :	private sound::noncopyable
{
public:
	typedef non_null<collision::object>::ptr			collision_ptr;
public:
							receiver_collision		( sound_receiver* receiver, atomic_half3* position );
							~receiver_collision		( );

	inline	collision_ptr	get_collision_object	( ) const { R_ASSERT( m_collision ); return m_collision; }
	inline	void			set_position			( float3 const& position ) { m_position->set( position ); }
	inline	float3			get_position			( ) const { R_ASSERT ( m_position ); return m_position->get( ); }
			void			delete_position			( sound_scene& scene );
	// do not call any methods of returned sound_receiver
	inline	sound_receiver*	get_sound_receiver		( ) const { return m_receiver; }
private:
	atomic_half3*			m_position;
	sound_receiver*			m_receiver;
	collision::object*		m_collision;
public:
	receiver_collision*		m_next;
}; // struct receiver_collision

class sound_scene :	public resources::unmanaged_resource,
					private boost::noncopyable
{
public:
								sound_scene					(
									sound_world& world,
									sound_scene_creation_params const& creation_params,
									IXAudio2SubmixVoice* submix_voice,
									u32 dbg_id,
									resources::query_result_for_cook& parent
								);
								~sound_scene				( );

	void						tick						( sound_world& world, u32 time_delta );

	// proxies
	sound_instance_proxy_ptr	new_point_sound_instance_proxy
								(
									sound_emitter_ptr const& emitter,
									sound_propagator_emitter const& propagator_emitter,
									world_user& user
								);

	sound_instance_proxy_ptr	new_spot_sound_instance_proxy
								(
									sound_emitter_ptr const& emitter,
									sound_propagator_emitter const& propagator_emitter,
									world_user& user,
									const sound_cone_type cone_type
								);

	sound_instance_proxy_ptr	new_volumetric_sound_instance_proxy
								(
									sound_emitter_ptr const& emitter,
									sound_propagator_emitter const& propagator_emitter,
									world_user& user,
									collision::geometry_instance& geometry,
									float radius
								);

	sound_instance_proxy_ptr	new_hud_sound_instance_proxy
								(
									sound_emitter_ptr emitter,
									sound_propagator_emitter const& propagator_emitter,
									world_user& user
								);

		void					set_graph					( render::culling::portal_sector_structure_ptr& graph );
		bool					graph_exist					( ) const;

		void					free_sound_instance_proxy	( sound_instance_proxy* proxy );

		// propagators
		void					emit_sound_propagators			
								(	
									sound_instance_proxy_internal& proxy,
									playback_mode mode,
									u32 playback_id,
									sound_producer const* const producer,
									sound_receiver const* const ignorable_receiver
								);

		void					emit_sound_propagators			
								(	
									sound_instance_proxy_internal& proxy,
									source_params const& params,
									playback_mode mode,
									u32 playback_id,
									sound_producer const* const producer,
									sound_receiver const* const ignorable_receiver
								);

	new_sound_propagator*		create_sound_propagator
								(	
									sound_propagator_emitter const& owner,
									sound_instance_proxy_internal& proxy,
									playback_mode mode,
									u32 playback_id,
									u32 playing_offset,
									u32 before_playing_offset,
									u32 after_playing_offset,
									sound_producer const* const producer,
									sound_receiver const* const ignorable_receiver = 0
								);

				void			delete_sound_propagator	
											( 
												sound_instance_proxy_internal& proxy,
												new_sound_propagator* propagator
											);

				void			stop_produce_sound			( sound_instance_proxy_internal& proxy ) const;
				void			stop_propagate_sound		( sound_instance_proxy_internal& proxy );
				void			pause_produce_sound			( sound_instance_proxy_internal& proxy ) const;
				void			pause_propagate_sound		( sound_instance_proxy_internal& proxy ) const;

				void			resume_produce_sound		( sound_instance_proxy_internal& proxy );
				void			resume_propagate_sound		( sound_instance_proxy_internal& proxy ) const;


				void			register_receiver			( sound_receiver* receiver, atomic_half3* position );
				void			unregister_receiver			( world_user& user, sound_receiver* receiver );

				void			set_listener_properties		(	
																float3 const& position,
																float3 const& orient_front,
																float3 const& orient_top
															);


	inline	float3				get_listenet_position		( ) const { return m_list_position.get( ); }
	inline	float3				get_listenet_orient_front	( ) const { return m_list_orient_front.get( ); }
	inline	float3				get_listenet_orient_top		( ) const { return m_list_orient_top.get( ); }

	IXAudio2SubmixVoice*		get_submix_voice			( ) const { return m_submix_voice; }
				void			fade_in						( sound_world& world, u32 time_in_msec );
				void			fade_out					( u32 time_in_msec );

				void			stop						( );

				atomic_half3*	create_receiver_position	( );
				void			delete_receiver_position	( atomic_half3* pos );

				void			add_active_voice			( sound_voice& voice );
				void			remove_active_voice			( sound_voice& voice );

				void			calculate_3d_sound			( sound_voice& voice, panning_lut_ptr panning_lut );
				void			clear_resources				( );

				void			update_stats				( sound_debug_stats& stats ) const;

#ifndef MASTER_GOLD
				void			enable_debug_stream_writing			( );
				void			disable_debug_stream_writing		( );
				bool			is_debug_stream_writing_enabled		( ) const { return m_is_debug_stream_writing_enabled != 0; }
				void			dump_debug_stream_writing			( ) const;
#endif //#ifndef MASTER_GOLD

			sound_scene_statistic*	create_statistic				( ) const;
				void			delete_statistic					( sound_scene_statistic* statistic ) const;

				void			pause								( );
				void			resume								( );
				bool			is_paused							( ) const;

				void			find_path					( float3 const& destination_point, vectora< fixed_vector< u32, 32 > >& result_paths ) const;
				float3			get_portal_center			( u32 portal_id ) const;
				float3			get_portal_nearest_point	( u32 portal_id, float3 segment_start, float3 segment_end ) const;
				bool			is_segment_pass_portal		( u32 portal_id, float3 segment_start, float3 segment_end ) const;

				void			insert_environment					( sound_environment& environment, float4x4 const& transform );
				void			add_environment_params				( pcstr name, XAUDIO2FX_REVERB_I3DL2_PARAMETERS* params, u32& id );
				u32				get_environment_params_id			( pcstr name );
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS*
								get_environment_params				( pcstr name );
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS*
								get_environment_params				( u32 id );
				sound_environment*
								get_current_environment				( );
	IXAudio2SubmixVoice*		get_current_effect_submix			( );

private:
	// orders
	void			emit_sound_propagators_impl
								(
									create_sound_propagator_params const& params
								);

private:
			void		init_allocators						( resources::query_result_for_cook& parent );
			void		on_unmanaged_resources_allocated	( resources::queries_result& queries );

			void		update_receivers_position			( );
			void		notify_receivers					( );
			void		notify_listener						( sound_world const& world );

			void		calculate_channel_matrix			(
																panning_lut_ptr const& panning_lut,
																sound_instance_proxy_internal const& proxy,
																float3 const& graph_position,
																float distance,
																float attenuation,
																float* channels_result,
																float& lp_filter_result
															) const;
			void		x3daudio_calculate					( sound_world const&, sound_voice& );
			void		process_fade						( sound_world& world, u64 time_delta );
			void		pause_propagate_all_sounds			( ) const;
			void		resume_propagate_all_sounds			( ) const;
			void		calculate_in_graph_position			( float3 const& proxy_position );
	IXAudio2SubmixVoice*
						create_environment_submix_voice	( sound_world const& world ) const;
private:
	typedef memory::single_size_buffer_allocator<	sizeof( sound_instance_proxy_internal ),
													threading::multi_threading_policy
												>	sound_proxies_allocator;

	typedef memory::single_size_buffer_allocator<	sizeof( new_sound_propagator ),
													threading::single_threading_policy
												>	sound_propagators_allocator;

	typedef memory::single_size_buffer_allocator<	sizeof( atomic_half3 ),
													threading::single_threading_policy
												>	receiver_position_allocator;

	typedef memory::single_size_buffer_allocator<	sizeof( receiver_collision ),
													threading::single_threading_policy
												>	receiver_collision_allocator;

	typedef intrusive_list	<	sound_instance_proxy_internal,
								sound_instance_proxy_internal*,
								&sound_instance_proxy_internal::m_next_for_sound_world,
								threading::single_threading_policy,
								size_policy,
								no_debug_policy
							>	proxies_list;

	typedef intrusive_list	<	receiver_collision,
								receiver_collision*,
								&receiver_collision::m_next,
								threading::mutex,
								size_policy,
								no_debug_policy
							>	sound_receivers_list;

	typedef intrusive_list	<	sound_voice,
								sound_voice*,
								&sound_voice::m_next_for_active,
								threading::mutex,
								size_policy,
								no_debug_policy
							>	active_voices;


public:
	sound_scene*		m_next;
private:
	typedef std::pair< fixed_string< 64 >, XAUDIO2FX_REVERB_I3DL2_PARAMETERS* > environment_parameters_pair;
	vectora< environment_parameters_pair >					m_environment_parameters;
	sound_world&											m_world;

	atomic_half3											m_list_position;
	atomic_half3											m_list_orient_front;
	atomic_half3											m_list_orient_top;

	resources::unmanaged_allocation_resource_ptr			m_memory_arena_resources_ptr;
	render::culling::portal_sector_structure_ptr			m_graph;

	u32 const												m_proxies_count;
	uninitialized_reference< sound_proxies_allocator >		m_proxies_allocator;

	u32	const												m_propagators_count;
	uninitialized_reference< sound_propagators_allocator >	m_propagators_allocator;

	u32	const												m_receivers_count;
	uninitialized_reference< receiver_position_allocator >	m_receiver_positions_allocator;

	uninitialized_reference< receiver_collision_allocator > m_receiver_collisions_allocator;

	collision::space_partitioning_tree*						m_spatial_tree;
	collision::space_partitioning_tree*						m_environments_tree;
	sound_receivers_list									m_receivers;

	proxies_list											m_active_proxies;

	active_voices											m_active_voices;

	IXAudio2SubmixVoice*									m_submix_voice;
	IXAudio2SubmixVoice*									m_fade_in_environment;
	IXAudio2SubmixVoice*									m_fade_out_environment;
	u32														m_environment_fade_time;
	sound_environment*										m_default_environment;
	effect_cross_fader*										m_environment_crossfader;

#ifndef MASTER_GOLD
	debug_snapshot*											m_debug_snapshot;
	threading::atomic32_type								m_is_debug_stream_writing_enabled;
#endif // #ifndef MASTER_GOLD

	float													m_fade_vol_per_msec;
	float													m_volume;
	u32														m_fade_in_time;
	u32														m_fade_out_time;
	u32														m_dbg_id;
	bool													m_is_paused;
	bool													m_is_active;
	bool													m_is_listener_position_set;
	enum
	{
		none,
		fade_in_state,
		fade_out_state
	}														m_fade_state;

}; // class sound_scene

#ifdef MASTER_GOLD
STATIC_SIZE_ASSERT( sound_scene, 0x270 );
#endif // MASTER_GOLD

typedef intrusive_list	<	sound_scene,
							sound_scene*,
							&sound_scene::m_next,
							threading::single_threading_policy
						>	sound_scenes_list;

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SCENE_H_INCLUDED
