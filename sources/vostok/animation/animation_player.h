////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_PLAYER_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_PLAYER_H_INCLUDED

#include <vostok/animation/animation_callback.h>
#include <vostok/animation/mixing_n_ary_tree.h>

struct transform_getter;

namespace vostok {
namespace render {
namespace debug {
	class renderer;
} // namespace render
} // namespace vostok

namespace animation {

struct editor_animation_state;

namespace mixing {
	class expression;
	struct n_ary_tree_base_node;
} // namespace mixing

template class VOSTOK_ANIMATION_API boost::function< void (vostok::animation::mixing::expression const&, u32) >;

enum reserved_channel_ids_enum {
	channel_id_on_animation_end				= 1,
	channel_id_on_animation_interval_end,
	channel_id_on_animation_lexeme_end,

	channel_id_max,
}; // enum reserved_channel_ids_enum

class VOSTOK_ANIMATION_API animation_player
{
	friend struct ::transform_getter;

public:
						animation_player		( );
						~animation_player		( );

			bool		tick									( u32 current_time_in_ms );
			bool		tick_to_nearest_user_handled_callback	( u32 current_time_in_ms );

			bool		set_target_and_tick			(
							mixing::expression const&						expression,
							const u32										current_time_in_ms,
							boost::function< float4x4( pcvoid ) > const&	get_transform_functor
						);
			bool		set_target_and_tick			(
							mixing::expression const&		expression,
							const u32						current_time_in_ms,
							float4x4 const&					transform_in_case_of_a_single_object_usage
						);

			u32			last_tick_time_in_ms		( ) const;
			u32			get_state_buffer_size		( ) const;

public:
			void		serialize_state				( void* buffer, u32 buffer_size );
			void		deserialize_state			( void* buffer, u32 time_in_ms );

			void		subscribe					(
							pcstr									channel_id,
							new_callback_type const&				callback,
							pcvoid									callback_uid,
							resources::managed_resource_ptr const&	animation,
							const u8								event_type,
							pcvoid const							animated_object
						);
			void		subscribe					(
							reserved_channel_ids_enum				channel_id,
							new_callback_type const&				callback,
							pcvoid									callback_uid,
							resources::managed_resource_ptr const&	animation,
							pcvoid const							animated_object
						);
			void		unsubscribe					( pcstr channel_id, pcvoid callback_uid );
			void		unsubscribe					( reserved_channel_ids_enum channel_id, pcvoid callback_uid );

			void		set_object_transform		( float4x4 const& object_transform, pcvoid const animated_object );
			float4x4	get_object_transform		( pcvoid const animated_object ) const;

public:
			void		compute_bones_matrices		(
							skeleton const&		skeleton,
							float4x4*			begin,
							float4x4*			end,
							pcvoid const		animated_object,
							u32*				bones_masks
						) const;
			void		compute_bones_local_matrices(
							skeleton const&		skeleton,
							float4x4*			begin,
							float4x4*			end,
							pcvoid const		animated_object,
							u32*				bones_masks
						) const;
			void		convert_to_object_matrices	(
							skeleton const&		skeleton,
							float4x4*			local_begin,
							float4x4*			local_end,
							pcvoid const		animated_object
						) const;

public:
	inline	bool		has_object					( pcvoid const arg_0 ) const { /* no source */ }
	inline	void		enable_logging				( const bool arg_0 ) { /* no source */ }
	inline	bool		are_there_any_animations	( ) const { return m_mixing_tree.are_there_any_animations(); }

public:
#ifndef MASTER_GOLD
			void		set_controller_callback		( boost::function<void (mixing::expression const&, u32)> const& callback );
			void		reset						( bool clear_callbacks = false );
			void		fill_animation_states		( vectora< editor_animation_state >& result );
#endif // #ifndef MASTER_GOLD

			void		reset						( bool clear_callbacks );
	inline	void		dump_animation_states		( animation_states_dumper& arg_0 ) const { /* no source */ }

private:
			bool		set_target					(
							mixing::expression const&						expression,
							const u32										current_time_in_ms,
							boost::function< float4x4( pcvoid ) > const&	get_transform_functor
						);
			pvoid		get_next_buffer				( u32 buffer_size );
			void		compact_callbacks			( );
			bool		try_get_transform			( pcvoid animated_object, float4x4& result ) const;
			void		skip_time_if_needed			( const u32 current_time_in_ms );


public:
	static	void		serialize_empty_state		( void* arg_0, u32 arg_1 ) { /* no source */ }
	static	void		destroy_state				( void* buffer );

private:
	static	void		destroy_subscriptions		( subscribed_channel const* channels_head );
	static	void		invert_times				(
							mixing::n_ary_tree&		tree,
							const u32				time_in_ms,
							const bool				check_before,
							const bool				check_after
						);
	static	void		serialize_state_impl		(
							void*							arg_0,
							const u32						arg_1,
							mixing::n_ary_tree const&		arg_2,
							const u32						arg_3,
							subscribed_channel*&			arg_4
						) { /* no source */ }


public:
	enum {
		stack_buffer_size		= 4096*sizeof( pvoid ),
		callbacks_buffer_size	= 320*sizeof( pvoid ),
	};

private:
	typedef char			buffer_type[ stack_buffer_size ];

private:
	buffer_type					m_tree_buffers[ 2 ];
	char						m_callbacks_buffer_raw[ callbacks_buffer_size ];
	mixing::n_ary_tree			m_mixing_tree;
	buffer_type*				m_current_buffer;
	subscribed_channel*			m_first_subscribed_channel;
	mutable_buffer				m_callbacks_buffer;
	u32							m_mixing_tree_buffer_size;
	u16							m_in_tick;
	bool						m_callbacks_are_actual;
	bool						m_is_logging_enabled;

#ifndef MASTER_GOLD
private:
	boost::function< void ( mixing::expression const&, u32 ) >	m_controller_callback;
#endif // #ifndef MASTER_GOLD
}; // class animation_player

STATIC_SIZE_ASSERT(animation_player, 0x8548);

typedef	resources::resource_ptr <
	animation_player,
	resources::unmanaged_intrusive_base
> animation_player_ptr;

			void		test_animation_player_new	(
								skeleton_animation_ptr skeleton_animation0,
								skeleton_animation_ptr skeleton_animation1,
								skeleton const& skeleton
							);

} // namespace animation
} // namespace vostok

#include <vostok/animation/animation_player_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_PLAYER_H_INCLUDED
