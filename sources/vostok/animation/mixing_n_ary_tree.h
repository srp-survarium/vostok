////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_MIXING_N_ARY_TREE_H_INCLUDED
#define VOSTOK_ANIMATION_MIXING_N_ARY_TREE_H_INCLUDED

#include <vostok/animation/mixing_n_ary_tree_intrusive_base.h>

#if 1//ndef DEBUG
#	define VOSTOK_NORMALIZE_ANIMATIONS_WEIGHTS
#endif // #ifndef DEBUG

namespace vostok {
namespace animation {

struct base_interpolator;
class bone_mixer;
class skeleton_bone;
class skeleton;
struct editor_animation_state;
struct subscribed_channel;
class animation_states_dumper;
class animation_player;

namespace mixing {

class n_ary_tree_animation_node;
struct animation_state;
class animated_object_holder;

class callback_generator_info {
public:
	inline callback_generator_info(
		pcvoid const								animated_object,
		resources::managed_resource_ptr const&	animation,
		const float								animation_time,
		const u16								event_type,
		const u8								channel_ids,
		const u32								user_data,
		const u8								animation_interval_id
	) :
		animation				( animation ),
		animated_object			( animated_object ),
		next					( 0 ),
		user_data				( user_data ),
		animation_time			( animation_time ),
		event_type				( event_type ),
		channel_ids				( channel_ids ),
		animation_interval_id	( animation_interval_id )
	{
	}

public:
	/* 0x0000 */	const resources::managed_resource_ptr	animation;
	/* 0x0004 */	pcvoid const							animated_object;
	/* 0x0008 */	callback_generator_info const*			next;
	/* 0x000c */	const u32								user_data;
	/* 0x0010 */	const float								animation_time;
	/* 0x0014 */	const u16								event_type;
	/* 0x0016 */	const u8								channel_ids;
	/* 0x0017 */	const u8								animation_interval_id;
}; // class callback_generator_info

STATIC_SIZE_ASSERT(callback_generator_info, 0x18);

template class VOSTOK_ANIMATION_API intrusive_ptr <
	n_ary_tree_intrusive_base,
	n_ary_tree_intrusive_base,
	threading::single_threading_policy
>;

class VOSTOK_ANIMATION_API n_ary_tree {
public:
	typedef	intrusive_ptr <
				n_ary_tree_intrusive_base,
				n_ary_tree_intrusive_base,
				threading::single_threading_policy
			>						n_ary_tree_ptr;

public:
	inline									n_ary_tree							( );
											n_ary_tree							(
												n_ary_tree_animation_node*		weight_root,
												n_ary_tree_animation_node*		time_root,
												base_interpolator const**		interpolators,
												animation_state*				animation_states,
												animation_state**				animation_events,
												animated_object_holder*			animated_objects,
												n_ary_tree_intrusive_base*		reference_counter,
												u32								animations_count,
												u32								animated_objects_count,
												u32								interpolators_count,
												u32								current_time_in_ms
											);
											~n_ary_tree							( );

			n_ary_tree&						operator=							( n_ary_tree const& other );

public:
			bool							tick								(
												u32						target_time_in_ms,
												subscribed_channel*&	channels_head,
												bool&					callbacks_are_actual
											);
	inline	void							tick_to_nearest_event				( subscribed_channel*& arg_0, bool& arg_1 ) { /* no source */ }
			u32								nearest_event_time_in_ms			( ) const;


#ifndef MASTER_GOLD
			void							dump_animation_states				( u32 const target_time_in_ms ) const;
			void							dump_tree							( u32 const target_time_in_ms ) const;
#endif // #ifndef MASTER_GOLD


			void							set_object_transform				( pcvoid animated_object, float4x4 const& object_transform );

private:
			void							set_object_transform				( n_ary_tree_animation_node& animation_node );

public:
			float4x4						get_object_transform				( pcvoid animated_object ) const;

	inline	bool							has_object							( pcvoid const arg_0 ) const { /* no source */ }

			void							compute_bones_matrices				(
												pcvoid				animated_object,
												skeleton const&		skeleton,
												float4x4* const		begin,
												float4x4* const		end,
												u32*				bones_masks
											) const;
			void							compute_bones_local_matrices		(
												pcvoid				animated_object,
												skeleton const&		skeleton,
												float4x4* const		begin,
												float4x4* const		end,
												u32*				bones_masks
											) const;
			void							convert_to_object_matrices			(
												pcvoid				animated_object,
												skeleton const&		skeleton,
												float4x4* const		begin,
												float4x4* const		end
											) const;

	inline	bool							is_consistent						( ) const {  return false; /* no source */ }
	inline	void							dump_animation_states				( animation_states_dumper& arg_0 ) const { /* no source */ }
	inline	bool							are_there_any_animations			( ) const { return m_animations_count > 0; }

	inline	u32								interpolators_count					( ) const { return m_interpolators_count	; }
	inline	base_interpolator const**		interpolators						( ) const { return m_interpolators			; }
	inline	n_ary_tree_animation_node*		weight_root							( ) const { return m_weight_root			; }
	inline	n_ary_tree_animation_node*		time_root							( ) const { return m_time_root				; }
	inline	animated_object_holder*			animated_objects					( ) const { return m_animated_objects		; }
	inline	u32								animations_count					( ) const { return m_animations_count		; }
	inline	u32								animated_objects_count				( ) const { return m_animated_objects_count	; }
	inline	u32								tree_actual_time_in_ms				( ) const { return m_tree_actual_time_in_ms	; }

#ifndef MASTER_GOLD
			void							fill_animation_states				( vectora< animation::editor_animation_state >& result );
#endif // #ifndef MASTER_GOLD

private:
	enum process_event_result_enum {
		process_event_result_animation_removed,
		process_event_result_nothing_to_do,
	}; // enum process_event_result_enum

private:
			void							initialize							( );
			void							destroy								( );

	inline	float4x4						computed_local_bone_matrix			( skeleton_bone const& arg_0 ) const { /* no source */ }

	inline	void							compute_skeleton_branch				( skeleton_bone const& arg_0, float4x4* const arg_1, float4x4 const& arg_2 ) const { /* no source */ }

	inline	void							update_animation_interval_time		( n_ary_tree_animation_node& arg_0, const u32 arg_1, const u32 arg_2 ) { /* no source */ }

			void							accumulate_object_movement			(
												n_ary_tree_animation_node&		animation_node,
												const float						animation_interval_time,
												const u32						time_in_ms
											);
	inline	void							accumulate_object_movement			(
												n_ary_tree_animation_node&		arg_0,
												u32								arg_1,
												float							arg_2,
												u32								arg_3
											) { /* no source */ }

			void							update_animation_state				(
												n_ary_tree_animation_node&		animation_node,
												const u32						start_time_in_ms,
												u32								target_time_in_ms
											);
			void							update_synchronization_group_using_integration(
												n_ary_tree_animation_node&		animation_node,
												const u32						start_time_in_ms,
												const u32						target_time_in_ms
											);
			void							update_time_synchronization_group	(
												n_ary_tree_animation_node&		animation_node,
												const u32						start_time_in_ms,
												const u32						target_time_in_ms
											);
			void							update_animation_states				( u32 start_time_in_ms, u32 target_time_in_ms );

			bool							need_new_transform					( const u32 target_time_in_ms ) const;

			void							remove_animation					( n_ary_tree_animation_node*& i, n_ary_tree_animation_node* j );

			void							process_event						( n_ary_tree_animation_node& current_animation_node, u32 event_types );
			void							process_events						( u32 target_time_in_ms, u32 event_types );

			void							update_event_iterators				( u32 target_time_in_ms );
			bool							update_event_iterators_and_dispatch_callbacks(
												const u32				target_time_in_ms,
												subscribed_channel*&	channels_head,
												bool&					callbacks_are_actual
											);
	inline	void							update_weight						( n_ary_tree_animation_node& arg_0, u32 arg_1, u32 arg_2 ) { /* no source */ }

			float							computed_animation_time				(
												n_ary_tree_animation_node&		animation,
												const float						animation_time_before_scale_starts,
												const u32						time_scale_start_time_in_ms,
												const u32						current_time_in_ms,
												const u32						target_time_in_ms,
												const float						time_scale
											) const;

			void							remove_animations					( const u32 target_time_in_ms );
			void							set_objects_transform				( );


#ifdef VOSTOK_NORMALIZE_ANIMATIONS_WEIGHTS
			void							normalize_weights					( );
#endif // #ifdef VOSTOK_NORMALIZE_ANIMATIONS_WEIGHTS


			void							adjust_animation_events_times		( n_ary_tree const& other );

	static	bool							dispatch_callbacks					(
											callback_generator_info const*		callback_generators_head,
											subscribed_channel*&				channels_head,
											u32								current_time_in_ms,
												bool&								callbacks_are_actual
											);

	static	void							update_animation_time				( animation_state& animation_state );

	static	callback_generator_info*		generate_animation_lexeme_end_events(
												n_ary_tree const&			previous_tree,
												n_ary_tree const&			new_tree,
												callback_generator_info*	callback_generators_buffer_begin,
												callback_generator_info*	callback_generators_buffer_end,
												subscribed_channel*			channels_head
											);

private:
	/* 0x0000 */	n_ary_tree_ptr						m_reference_counter;
	/* 0x0004 */	n_ary_tree_animation_node*			m_weight_root;
	/* 0x0008 */	n_ary_tree_animation_node*			m_time_root;
	/* 0x000c */	base_interpolator const**			m_interpolators;
	/* 0x0010 */	animation_state*					m_animation_states;
	/* 0x0014 */	animation_state**					m_animation_events;
	/* 0x0018 */	animated_object_holder*				m_animated_objects;
	/* 0x001c */	u32									m_animations_count;
	/* 0x0020 */	u32									m_animated_objects_count;
	/* 0x0024 */	u32									m_interpolators_count;
	/* 0x0028 */	u32									m_tree_actual_time_in_ms;
	/* 0x002c */	bool								m_is_logging_enabled;
private:
	friend class animation_player;
}; // class n_ary_tree

STATIC_SIZE_ASSERT(n_ary_tree, 0x30);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include <vostok/animation/mixing_n_ary_tree_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_MIXING_N_ARY_TREE_H_INCLUDED
