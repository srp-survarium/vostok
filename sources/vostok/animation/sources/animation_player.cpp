////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/animation_player.h>

#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/mixing_animated_object_holder.h>
#include <vostok/animation/base_interpolator.h>
#include "mixing_n_ary_tree_converter.h"
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_weight_transition_node.h"
#include "mixing_n_ary_tree_time_scale_transition_node.h"
#include "mixing_n_ary_tree_weight_node.h"
#include "mixing_n_ary_tree_time_scale_node.h"
#include "mixing_n_ary_tree_addition_node.h"
#include "mixing_n_ary_tree_subtraction_node.h"
#include "mixing_n_ary_tree_multiplication_node.h"
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"
#include "mixing_n_ary_tree_visitor.h"
#include "mixing_animation_state.h"
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID( animation_player_linkage_id );

using namespace vostok;
using namespace vostok::animation;

struct transform_getter : boost::noncopyable
{
public:
	transform_getter(
		animation_player&						animation_player,
		boost::function< float4x4 ( pcvoid ) >&	functor
	) :
		animation_player	( animation_player ),
		functor			( functor )
	{
	}

	float4x4 get_transform( pcvoid const animated_object ) const;

private:
  animation_player&							animation_player;
  boost::function< float4x4 ( pcvoid )>&	functor;
};

float4x4 transform_getter::get_transform( pcvoid const animated_object ) const
{
	float4x4 transform;
	if ( animation_player.try_get_transform( animated_object, transform ) )
		return transform;

	return functor( animated_object );
}
class n_ary_tree_time_inverter :
	public mixing::n_ary_tree_visitor,
	private boost::noncopyable
{
public:
	explicit		n_ary_tree_time_inverter	( u32 current_time_in_ms ) :
		m_current_time_in_ms				( current_time_in_ms )
	{
	}

private:
	virtual	void	visit		( mixing::n_ary_tree_animation_node& node );
	virtual	void	visit		( mixing::n_ary_tree_weight_transition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_time_scale_transition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_weight_node& node );
	virtual	void	visit		( mixing::n_ary_tree_time_scale_node& node );
	virtual	void	visit		( mixing::n_ary_tree_addition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_subtraction_node& node );
	virtual	void	visit		( mixing::n_ary_tree_multiplication_node& node );

private:
	template < typename T >
	void			propagate					( T& node )
	{
		mixing::n_ary_tree_base_node** i			= node.operands( sizeof( T ) );
		mixing::n_ary_tree_base_node** const end	= i + node.operands_count( );
		for ( ; i != end; ++i )
			(*i)->accept( *this );
	}

	u32 m_current_time_in_ms;
};

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_animation_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
	NODEFAULT						( );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_weight_node& node )
{
	VOSTOK_UNREFERENCED_PARAMETER	( node );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_time_scale_node& node )
{
	node.set_time_scale_start_time(
		m_current_time_in_ms - node.time_scale_start_time_in_ms( ),
		node.animation_time_before_scale_starts( )
	);
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_addition_node& node )
{
	propagate	( node );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_subtraction_node& node )
{
	propagate	( node );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_multiplication_node& node )
{
	propagate	( node );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_weight_transition_node& node )
{
	node.set_start_time_in_ms	( m_current_time_in_ms - node.start_time_in_ms( ) );
	node.from( ).accept			( *this );
	node.to( ).accept			( *this );
}

void n_ary_tree_time_inverter::visit( mixing::n_ary_tree_time_scale_transition_node& node )
{
	node.set_start_time_in_ms	( m_current_time_in_ms - node.start_time_in_ms( ) );
	node.from( ).accept			( *this );
	node.to( ).accept			( *this );
}

namespace vostok {
namespace animation {

animation_player::~animation_player( )
{
	reset( true );
}

void* animation_player::get_next_buffer( const u32 buffer_size )
{
	if ( &m_tree_buffers[ 0 ] == m_current_buffer )
		return m_current_buffer = &m_tree_buffers[ 1 ];

	return m_current_buffer = &m_tree_buffers[ 0 ];
}

bool animation_player::try_get_transform( pcvoid const animated_object, float4x4& result ) const
{
	mixing::animated_object_holder* const i = std::find(
		m_mixing_tree.animated_objects( ),
		m_mixing_tree.animated_objects( ) + m_mixing_tree.animated_objects_count( ),
		animated_object );

	if ( i != m_mixing_tree.animated_objects( ) + m_mixing_tree.animated_objects_count( ) && !i->need_new_transform )
	{
		result								= i->transform;
		return true;
	}

	return false;
}

bool animation_player::set_target(
	mixing::expression const&						expression,
	const u32										current_time_in_ms,
	boost::function< float4x4( pcvoid ) > const&	get_transform_functor
)
{
	mixing::n_ary_tree_converter builder	( expression );
	u32 const buffer_size					= builder.needed_buffer_size( );

	bool const first_time					= m_mixing_tree.animations_count( ) <= 0;
	pvoid const buffer_raw					=
		first_time
			? get_next_buffer( buffer_size )
			: ALLOCA( buffer_size );
	mutable_buffer buffer					( buffer_raw, buffer_size );
	mixing::n_ary_tree target_tree			= builder.constructed_n_ary_tree(
		buffer,
		first_time,
		current_time_in_ms,
		m_first_subscribed_channel
	);

	if ( first_time ) {
		m_mixing_tree_buffer_size	= buffer_size;
		m_mixing_tree				= target_tree;
		mixing::animated_object_holder* i	= m_mixing_tree.animated_objects( );
		mixing::animated_object_holder* const end	=
			i + m_mixing_tree.animated_objects_count( );
		for ( ; i != end; ++i )
			i->transform			= get_transform_functor( i->animated_object );

		return						true;
	}

	mixing::n_ary_tree_comparer comparer	( m_mixing_tree, target_tree, current_time_in_ms );
	if ( comparer.equal( ) )
		return						false;

	mixing::callback_generator_info* const callback_generators_buffer	=
		static_cast< mixing::callback_generator_info* >(
			ALLOCA( m_mixing_tree.animations_count( ) * sizeof( mixing::callback_generator_info ) )
		);
	m_mixing_tree_buffer_size			= comparer.needed_buffer_size( );
	mutable_buffer mixing_buffer(
		get_next_buffer( m_mixing_tree_buffer_size + 32 * sizeof( pvoid ) ),
		m_mixing_tree_buffer_size + 32 * sizeof( pvoid )
	);
	m_mixing_tree.set_objects_transform	( );

	transform_getter transform_getter_instance(
		*this,
		const_cast< boost::function< float4x4( pcvoid ) >& >( get_transform_functor )
	);
	mixing::n_ary_tree transition_tree	=
		mixing::n_ary_tree_transition_tree_constructor(
			mixing_buffer,
			m_mixing_tree,
			target_tree,
			comparer.animations_count( ),
			comparer.animated_objects_count( ),
			current_time_in_ms,
			m_first_subscribed_channel,
			boost::bind( &transform_getter::get_transform, &transform_getter_instance, _1 )
		).computed_tree( );

	mixing::callback_generator_info* const generators_head	=
		mixing::n_ary_tree::generate_animation_lexeme_end_events(
			m_mixing_tree,
			transition_tree,
			callback_generators_buffer,
			callback_generators_buffer + m_mixing_tree.animations_count( ),
			m_first_subscribed_channel
		);
	m_mixing_tree					= transition_tree;

	if ( generators_head ) {
		++m_in_tick;
		mixing::n_ary_tree::dispatch_callbacks(
			generators_head,
			m_first_subscribed_channel,
			current_time_in_ms,
			m_callbacks_are_actual
		);
		--m_in_tick;
		if ( !m_in_tick && !m_callbacks_are_actual )
			compact_callbacks		( );
	}

	for ( mixing::callback_generator_info const* i = generators_head; i; ) {
		mixing::callback_generator_info const* const next	= i->next;
		i->~callback_generator_info	( );
		i							= next;
	}

	return							true;

}

void animation_player::skip_time_if_needed( const u32 current_time_in_ms )
{
	if ( !m_in_tick
		&& m_mixing_tree.animations_count( ) != 0
		&& m_mixing_tree.tree_actual_time_in_ms( ) + 10000 <= current_time_in_ms )
	{
		u32 time_diff_in_ms = current_time_in_ms - m_mixing_tree.tree_actual_time_in_ms( );
		LOG_WARNING( "big time lag (%d.%03d) => skipping animation events", time_diff_in_ms / 1000, time_diff_in_ms % 1000 );

		void* buffer = ALLOCA( 0x4000 );

		serialize_state( buffer, 0x4000 );

		deserialize_state( buffer, current_time_in_ms );

		destroy_state( buffer );
	}
}

bool animation_player::tick( const u32 current_time_in_ms )
{
	skip_time_if_needed( current_time_in_ms );

	++m_in_tick;
	bool result = m_mixing_tree.tick( current_time_in_ms, m_first_subscribed_channel, m_callbacks_are_actual );
	--m_in_tick;

	if ( !m_in_tick && !m_callbacks_are_actual )
		compact_callbacks( );

	return result;
}

bool animation_player::tick_to_nearest_user_handled_callback( const u32 current_time_in_ms )
{
	skip_time_if_needed( current_time_in_ms );

	bool result;
	u32 time;
	do
	{
		u32 nearest_event_time_in_ms	= m_mixing_tree.nearest_event_time_in_ms( );
		time							= math::min( current_time_in_ms, nearest_event_time_in_ms );
		result							= tick( time );
	}
	while ( !result && time != current_time_in_ms );

	return result;
}

bool animation_player::set_target_and_tick(
	mixing::expression const&		expression,
	const u32						current_time_in_ms,
	boost::function< float4x4( pcvoid ) > const&	get_transform_functor
)
{
	if ( m_mixing_tree.animations_count( ) > 0 )
		tick								( current_time_in_ms );

	bool result							= set_target( expression, current_time_in_ms, get_transform_functor );

	tick									( current_time_in_ms );

	return result;
}

float4x4 single_object_get_transform( float4x4 const& transform, pcvoid const animated_object )
{
	VOSTOK_UNREFERENCED_PARAMETER( animated_object );
	return transform;
}

bool animation_player::set_target_and_tick(
	mixing::expression const&		expression,
	const u32						current_time_in_ms,
	float4x4 const&					transform_in_case_of_a_single_object_usage
)
{
	return set_target_and_tick( expression, current_time_in_ms, boost::bind( &single_object_get_transform, boost::cref( transform_in_case_of_a_single_object_usage ), _1 ) );
}

void animation_player::set_object_transform( float4x4 const& object_transform, pcvoid const animated_object )
{
	m_mixing_tree.set_object_transform( animated_object, object_transform );
}

float4x4 animation_player::get_object_transform( pcvoid const animated_object ) const
{
	return m_mixing_tree.get_object_transform( animated_object );
}

void animation_player::compute_bones_matrices(
	skeleton const&		skeleton,
	float4x4* const		begin,
	float4x4* const		end,
	pcvoid const		animated_object,
	u32* const			bones_masks
) const
{
	R_ASSERT_CMP							( end - begin, >=, skeleton.get_non_root_bones_count( ) );
	m_mixing_tree.compute_bones_matrices	( animated_object, skeleton, begin, end, bones_masks );
}

void animation_player::compute_bones_local_matrices(
	skeleton const&		skeleton,
	float4x4*			begin,
	float4x4*			end,
	pcvoid const		animated_object,
	u32* const			bones_masks
) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_mixing_tree.compute_bones_local_matrices( animated_object, skeleton, begin, end, bones_masks );
}

void animation_player::convert_to_object_matrices(
	skeleton const&		skeleton,
	float4x4*			local_begin,
	float4x4*			local_end,
	pcvoid const		animated_object
) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_mixing_tree.convert_to_object_matrices( animated_object, skeleton, local_begin, local_end );
}

void animation_player::reset( const bool clear_callbacks )
{
	m_mixing_tree							= mixing::n_ary_tree( );
	m_mixing_tree_buffer_size				= 0;

	if ( clear_callbacks )
	{
		destroy_subscriptions				( m_first_subscribed_channel );
		m_first_subscribed_channel = 0, new (&m_callbacks_buffer) mutable_buffer( m_callbacks_buffer_raw, callbacks_buffer_size );
	}
}

void animation_player::subscribe(
	pcstr const								channel_id,
	new_callback_type const&				callback,
	pcvoid const							callback_uid,
	resources::managed_resource_ptr const&	animation,
	const u8								event_type,
	pcvoid const							animated_object
)
{
	subscribed_channel* channel				= m_first_subscribed_channel;
	subscribed_channel* previous_channel	= 0;
	for ( ; channel ; previous_channel = channel, channel = channel->next )
	{
		if ( strings::compare( channel->channel_id, channel_id ) != 0 )
			continue;

		animation_callback* const new_callback	= static_cast<animation_callback*>( m_callbacks_buffer.c_ptr( ) );
		m_callbacks_buffer					+= sizeof( animation_callback );
		new ( new_callback ) animation_callback( callback, callback_uid, animation, event_type, animated_object );

		animation_callback* last_callback	= channel->first_callback;
		for ( ; last_callback->next ; )
			last_callback					= last_callback->next;
		last_callback->next					= new_callback;
		return;
	}

	subscribed_channel* const new_channel	= static_cast<subscribed_channel*>( m_callbacks_buffer.c_ptr( ) );
	m_callbacks_buffer						+= sizeof( subscribed_channel );
	memory::detail::call_constructor		( new_channel );
	new_channel->next						= 0;

	u32 const length						= strings::length( channel_id ) + 1;
	memory::copy							( m_callbacks_buffer.c_ptr( ), length, channel_id, length );
	new_channel->channel_id					= static_cast<pcstr>( m_callbacks_buffer.c_ptr( ) );
	m_callbacks_buffer						+= math::align_up( length, u32( 4 ) )*sizeof( char );

	animation_callback* const new_callback	= static_cast<animation_callback*>( m_callbacks_buffer.c_ptr( ) );
	m_callbacks_buffer						+= sizeof( animation_callback );
	new ( new_callback ) animation_callback( callback, callback_uid, animation, event_type, animated_object );

	new_channel->first_callback				= new_callback;
	if ( previous_channel )
		previous_channel->next				= new_channel;
	else
		m_first_subscribed_channel			= new_channel;
}

void animation_player::subscribe(
	reserved_channel_ids_enum		channel_id,
	new_callback_type const&		callback,
	pcvoid const					callback_uid,
	resources::managed_resource_ptr const&	animation,
	pcvoid const					animated_object
)
{
	char channel_id_string[] = { u8(channel_id), 0 };
	subscribe( channel_id_string, callback, callback_uid, animation, 0xFF, animated_object );
}

void animation_player::destroy_subscriptions( subscribed_channel const* const channels_head )
{
	for (subscribed_channel const* i = channels_head; i; )
	{
		for ( animation_callback* j = (*i).first_callback; j; )
		{
			animation_callback* const temp		= j;
			j									= j->next;
			temp->~animation_callback			( );
		}

		subscribed_channel const* const temp	= i;
		VOSTOK_UNREFERENCED_PARAMETER				( temp );
		i										= i->next;
		temp->~subscribed_channel				( );
	}
}

void animation_player::compact_callbacks( )
{
	m_callbacks_are_actual					= true;

	subscribed_channel const* i				= m_first_subscribed_channel;
	subscribed_channel const* first_cloned_channel	= 0;
	subscribed_channel* previous_channel	= 0;
	for ( ; i ; i = i->next )
	{
		subscribed_channel* const new_channel	= (subscribed_channel*)ALLOCA( sizeof( subscribed_channel ) );
		if ( first_cloned_channel )
			previous_channel->next			= new_channel;
		else
			first_cloned_channel			= new_channel;

		memory::detail::call_constructor		( new_channel );

		pstr channel_id;
		STR_DUPLICATEA						( channel_id, i->channel_id );
		new_channel->channel_id				= channel_id;

		new_channel->first_callback			= 0;
		new_channel->next					= 0;
		animation_callback* k				= 0;
		for ( animation_callback* j = i->first_callback ; j ; j = j->next )
		{
			if ( !j->enabled )
				continue;

			animation_callback* const new_callback	= (animation_callback*)ALLOCA( sizeof( animation_callback ) );
			new ( new_callback ) animation_callback( j->callback, j->callback_uid, j->animation, j->event_type, j->animated_object );

			if ( k )
				k->next					= new_callback;
			else
				new_channel->first_callback	= new_callback;

			k							= new_callback;
		}

		if ( new_channel->first_callback )
			previous_channel				= new_channel;
		else if ( previous_channel )
			previous_channel->next			= 0;
		else
			first_cloned_channel			= 0;
	}

	destroy_subscriptions					( m_first_subscribed_channel );

	m_first_subscribed_channel				= 0;
	m_callbacks_buffer.~mutable_buffer		( );
	new (&m_callbacks_buffer) mutable_buffer( m_callbacks_buffer_raw, callbacks_buffer_size );

	u32 channels_count						= 0;
	for ( i = first_cloned_channel ; i ; i = i->next )
	{
		subscribed_channel* const new_channel	= static_cast<subscribed_channel*>( m_callbacks_buffer.c_ptr( ) );
		++channels_count;
		m_callbacks_buffer					+= sizeof( subscribed_channel );

		if ( m_first_subscribed_channel )
			previous_channel->next			= new_channel;
		else
			m_first_subscribed_channel		= new_channel;

		memory::detail::call_constructor		( new_channel );

		u32 const length					= strings::length( i->channel_id ) + 1;
		memory::copy						( m_callbacks_buffer.c_ptr( ), length, i->channel_id, length );
		new_channel->channel_id				= static_cast<pcstr>( m_callbacks_buffer.c_ptr( ) );
		m_callbacks_buffer					+= math::align_up( length, u32( 4 ) )*sizeof( char );

		new_channel->next					= 0;
		animation_callback* k				= 0;
		for ( animation_callback* j = i->first_callback ; j ; j = j->next )
		{
			animation_callback* const new_callback	= static_cast<animation_callback*>( m_callbacks_buffer.c_ptr( ) );
			m_callbacks_buffer				+= sizeof( animation_callback );

			new ( new_callback ) animation_callback( j->callback, j->callback_uid, j->animation, j->event_type, j->animated_object );

			if ( k )
				k->next					= new_callback;
			else
				new_channel->first_callback	= new_callback;

			k							= new_callback;
		}

		previous_channel					= new_channel;
	}

	destroy_subscriptions					( first_cloned_channel );

	if ( !channels_count )
		m_first_subscribed_channel			= 0;
}

void animation_player::unsubscribe( pcstr const channel_id, pcvoid const callback_uid )
{
	subscribed_channel* channel				= m_first_subscribed_channel;
	for ( ; ; channel = channel->next )
	{
		if ( strings::compare( channel->channel_id, channel_id ) == 0 )
			break;
	}

	animation_callback* callback			= channel->first_callback;
	for ( ; callback ; callback = callback->next ) {
		if ( callback->callback_uid != callback_uid )
			continue;
		callback->callback					= new_callback_type( );
		callback->callback_uid				= 0;
		callback->enabled					= false;

		m_callbacks_are_actual				= false;
		break;
	}

	if ( m_in_tick == 0 )
		compact_callbacks					( );
}

void animation_player::unsubscribe( reserved_channel_ids_enum channel_id, pcvoid const callback_uid )
{
	char channel_id_string[]					= { u8(channel_id), 0 };
	unsubscribe									( channel_id_string, callback_uid );
}

u32 animation_player::get_state_buffer_size( ) const
{
	return 0x4000; // sushi@TODO: What this constant is. Can be figured out by usage.
}

void invert_animation_times( mixing::n_ary_tree_animation_node& animation, const u32 time_in_ms )
{
	n_ary_tree_time_inverter time_inverter( time_in_ms );

	mixing::n_ary_tree_base_node** i			= animation.operands( sizeof(mixing::n_ary_tree_animation_node) );
	mixing::n_ary_tree_base_node** const end	= i + animation.operands_count( );
	for ( ; i != end; ++i )
		(*i)->accept				( time_inverter );

	animation.animation_state( ).event_iterator.invert_times( time_in_ms );
}

void animation_player::invert_times(
	mixing::n_ary_tree&		tree,
	const u32				time_in_ms,
	const bool				check_before,
	const bool				check_after
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( check_before, check_after );

	for ( mixing::n_ary_tree_animation_node* i = tree.weight_root( ) ; i != NULL ; i = i->m_next_weight_animation )
		invert_animation_times( *i, time_in_ms );
}

void animation_player::serialize_state( void* buffer, const u32 buffer_size )
{
	*((u32*)buffer + 0) = 0xB19B00B5;
	*((u32*)buffer + 1) = m_mixing_tree_buffer_size;
	mutable_buffer tree_buffer( (void*)((u32*)buffer + 2), buffer_size );

	mixing::n_ary_tree* tree = static_cast<mixing::n_ary_tree*>( tree_buffer.c_ptr() );
	tree_buffer += sizeof( mixing::n_ary_tree );

	if ( m_mixing_tree_buffer_size )
	{
		new (tree) mixing::n_ary_tree(
			mixing::n_ary_tree_transition_tree_constructor(
				tree_buffer,
				m_mixing_tree,
				m_mixing_tree,
				m_mixing_tree.animations_count( ),
				m_mixing_tree.animated_objects_count( ),
				m_mixing_tree.tree_actual_time_in_ms( ),
				m_first_subscribed_channel,
				mixing::n_ary_tree_transition_tree_constructor::transform_functor_type( )
			).computed_tree( )
		);
	}
	else
	{
		new (tree) mixing::n_ary_tree( );
	}

	tree->adjust_animation_events_times( m_mixing_tree );

	for ( mixing::n_ary_tree_animation_node* root = tree->weight_root( ) ; root != NULL ; root = root->m_next_weight_animation )
		invert_animation_times( *root, m_mixing_tree.tree_actual_time_in_ms( ) );
}

void animation_player::deserialize_state( void* buffer, const u32 time_in_ms )
{
	m_mixing_tree_buffer_size					= *((u32*)buffer + 1);

	mixing::n_ary_tree& tree					= *(mixing::n_ary_tree*)( (u32*)buffer + 2 );
	if ( tree.animations_count( ) == 0 )
	{
		m_mixing_tree							= mixing::n_ary_tree( );
		return;
	}

	for ( mixing::n_ary_tree_animation_node* root = tree.weight_root( ) ; root != NULL ; root = root->m_next_weight_animation )
		invert_animation_times( *root, time_in_ms );

	tree.m_tree_actual_time_in_ms				= time_in_ms;

	mutable_buffer mixing_buffer( get_next_buffer( stack_buffer_size ), stack_buffer_size );

	m_mixing_tree = mixing::n_ary_tree_transition_tree_constructor(
		mixing_buffer,
		tree,
		tree,
		tree.animations_count( ),
		tree.animated_objects_count( ),
		time_in_ms,
		m_first_subscribed_channel,
		mixing::n_ary_tree_transition_tree_constructor::transform_functor_type( )
	).computed_tree( );

	tree.adjust_animation_events_times( m_mixing_tree );

	for ( mixing::n_ary_tree_animation_node* root = tree.weight_root( ) ; root != NULL ; root = root->m_next_weight_animation )
		invert_animation_times( *root, time_in_ms );
}

void animation_player::destroy_state( void* buffer )
{
	*(u32*)buffer = 0xFFBDDDDD;
	mixing::n_ary_tree* tree				= (mixing::n_ary_tree*)( (u32*)buffer + 2 );
	tree->~n_ary_tree						( );
}

u32 animation_player::last_tick_time_in_ms( ) const
{
	return m_mixing_tree.tree_actual_time_in_ms( );
}

} // namespace animation
} // namespace vostok
