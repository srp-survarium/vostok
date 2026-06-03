////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/animation_player.h>

#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/base_interpolator.h>
#include "mixing_n_ary_tree_converter.h"
#include "mixing_n_ary_tree_comparer.h"
#include "mixing_n_ary_tree_animation_node.h"
#include "mixing_n_ary_tree_node_comparer.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"
#include "mixing_n_ary_tree_visitor.h"
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID( animation_player_linkage_id );

namespace vostok {
namespace animation {

// sushi@TODO: Missing somehow in headers.
struct transform_getter : boost::noncopyable
{
public:
	float4x4 get_transform( pcvoid const animated_object ) const;

private:
  animation_player&							animation_player;
  boost::function< float4x4 ( pcvoid )>&	functor;
};

// STATE[STUB]
float4x4 transform_getter::get_transform( pcvoid const animated_object ) const
{
	// LOCALS
	// float4x4 						transform
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x11d090>|0x000|+0x00b:'43'	{
	// <0>
	// <0x11d09b>|0x00b|+0x017:'45'
	// <0x11d0b2>|0x022|+0x018:'46'
	// <0>
	// <0x11d0ca>|0x03a|-0x008:'48'
	// <0x11d0c2>|0x032|+0x017:'49'
	// <0x11d0d9>|0x049|      :'49'	}
	// ******
}
/*
// STATE[STUB]
void* n_ary_tree_time_inverter::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY
	// ******
}
*/

// sushi@TODO: Missing somehow in headers.
class n_ary_tree_time_inverter : mixing::n_ary_tree_visitor, boost::noncopyable
{
public:
	virtual	void	visit		( mixing::n_ary_tree_animation_node& node );
	virtual	void	visit		( mixing::n_ary_tree_weight_transition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_time_scale_transition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_weight_node& node );
	virtual	void	visit		( mixing::n_ary_tree_time_scale_node& node );
	virtual	void	visit		( mixing::n_ary_tree_addition_node& node );
	virtual	void	visit		( mixing::n_ary_tree_subtraction_node& node );
	virtual	void	visit		( mixing::n_ary_tree_multiplication_node& node );

private:
  u32 m_current_time_in_ms;
};


// STATE[STUB]
void n_ary_tree_time_inverter::visit(
	mixing::n_ary_tree_weight_node&		arg_0 /* vostok::animation::mixing::n_ary_tree_weight_node& node */
)
{
	// FUNCTION BODY
	// ******
}

// STATE[STUB]
void n_ary_tree_time_inverter::visit(
	mixing::n_ary_tree_time_scale_node&		arg_0 /* vostok::animation::mixing::n_ary_tree_time_scale_node& node */
)
{
	// FUNCTION BODY
	// <0>
	// <0x11d0f0>|0x000|+0x00d:'80'
	// ******
}

// STATE[STUB]
void n_ary_tree_time_inverter::visit(
	mixing::n_ary_tree_addition_node&		arg_0 /* vostok::animation::mixing::n_ary_tree_addition_node& node */
)
{
	// CALL SITE INFO
	// <0x11d11e> -> void < unknown >( vostok::animation::mixing::n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0x11d100>|0x000|+0x016:'85'
	// <0x11d116>|0x016|+0x014:'86'
	// ******
}

// STATE[STUB]
void n_ary_tree_time_inverter::visit(
	mixing::n_ary_tree_weight_transition_node&	arg_0 /* vostok::animation::mixing::n_ary_tree_weight_transition_node& node */
)
{
	// CALL SITE INFO
	// <0x11d14a> -> void < unknown >( vostok::animation::mixing::n_ary_tree_visitor& )
	// <0x11d155> -> void < unknown >( vostok::animation::mixing::n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <0x11d133>|0x003|+0x00b:'104'
	// <0x11d13e>|0x00e|+0x00e:'105'
	// <0x11d14c>|0x01c|+0x00d:'106'
	// ******
}

// STATE[UNCHECKED]
animation_player::~animation_player( )
{
	reset( true );

	// FUNCTION BODY
	// <0x56fee3>|0x003|+0x008:'126'
	// ******
}

// STATE[STUB]
void* animation_player::get_next_buffer( const u32 buffer_size )
{
	return NULL;

	// FUNCTION BODY
	// <0x11d160>|0x000|+0x000:'130'	{
	// <0>
	// <0x11d160>|0x000|+0x017:'132'
	// <0x11d177>|0x017|-0x003:'132'
	// <0x11d174>|0x014|+0x002:'133'
	// <0x11d176>|0x016|+0x007:'134'
	// <0x11d17d>|0x01d|      :'134'	}
	// ******
}

// STATE[STUB]
bool animation_player::try_get_transform( pcvoid const animated_object, float4x4& result ) const
{
	return false;

	// FUNCTION BODY
	// <0x56fdb0>|0x000|+0x006:'137'	{
	// <0x56fdb6>|0x006|+0x020:'138'
	// <0x56fdd6>|0x026|+0x015:'139'
	// <0x56fdeb>|0x03b|+0x010:'140'
	// <0x56fdfb>|0x04b|+0x009:'141'
	// <0>
	// <1>
	// <0x56fe04>|0x054|-0x006:'144'
	// <0x56fdfe>|0x04e|+0x009:'145'
	// <0x56fe07>|0x057|      :'145'	}
	// ******
}

// STATE[STUB]
bool animation_player::set_target(
	mixing::expression const&						expression,
	const u32										current_time_in_ms,
	boost::function< float4x4( pcvoid ) > const&	get_transform_functor
)
{
	// LOCALS
	// mutable_buffer 					buffer
	// mixing::n_ary_tree 				target_tree
	// mixing::n_ary_tree_converter 	builder
	// mutable_buffer 					mixing_buffer
	// mixing::n_ary_tree 				transition_tree
	// transform_getter 				transform_getter_instance
	// mixing::n_ary_tree_comparer 		comparer
	// const bool 						first_time
	// mixing::animated_object_holder* 	i
	// mixing::callback_generator_info* const generators_head
	// ******

	return false;

	// FUNCTION BODY
	// <0x570b10>|0x000|+0x00f:'173'	{
	// <0>
	// <0x570b1f>|0x00f|+0x00c:'175'
	// <0>
	// <1>
	// <0x570b2b>|0x01b|+0x015:'178'
	// <0x570b40>|0x030|+0x025:'179'
	// <0>
	// <0x570b65>|0x055|+0x00a:'181'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x570b6f>|0x05f|+0x01f:'189'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x570b8e>|0x07e|+0x00a:'197'
	// <0>
	// <0x570b98>|0x088|+0x006:'199'
	// <0x570b9e>|0x08e|+0x00e:'200'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x570bac>|0x09c|+0x027:'206'
	// <0x570bd3>|0x0c3|+0x02e:'207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x570c01>|0x0f1|+0x042:'216'
	// <0>
	// <1>
	// <2>
	// <0x570c43>|0x133|+0x013:'220'
	// <0x570c56>|0x146|+0x006:'221'
	// <0x570c5c>|0x14c|+0x039:'222'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x570c95>|0x185|+0x014:'227'
	// <0>
	// <1>
	// <2>
	// <0x570ca9>|0x199|+0x009:'231'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x570cb2>|0x1a2|+0x017:'246'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x570cc9>|0x1b9|+0x00c:'257'
	// <0>
	// <0x570cd5>|0x1c5|+0x003:'259'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x570cd8>|0x1c8|+0x071:'271'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <0x570d49>|0x239|+0x01c:'296'
	// <0>
	// <0x570d65>|0x255|+0x014:'298'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x570d79>|0x269|+0x007:'304'
	// <0>
	// <0x570d80>|0x270|+0x01f:'306'
	// <0x570d9f>|0x28f|+0x016:'307'
	// <0>
	// <0x570db5>|0x2a5|+0x009:'309'
	// <0x570dbe>|0x2ae|+0x006:'310'
	// <0>
	// <1>
	// <0x570dc4>|0x2b4|+0x002:'313'
	// <0x570dc6>|0x2b6|+0x003:'314'
	// <0x570dc9>|0x2b9|+0x009:'315'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x570dd2>|0x2c2|+0x017:'320'
	// <0x570de9>|0x2d9|-0x1b7:'321'
	// <0x570c32>|0x122|+0x1e0:'322'
	// <0x570e12>|0x302|-0x1de:'322'
	// <0x570c34>|0x124|+0x052:'323'
	// <0x570c86>|0x176|+0x18e:'323'
	// <0x570e14>|0x304|      :'323'	}
	// ******

}

// STATE[STUB]
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
	}


	// FUNCTION BODY
	// <0>
	// <1>
	// <0x5709c6>|0x006|+0x03b:'329'
	// <0x570a01>|0x041|+0x004:'330'
	// <0x570a05>|0x045|+0x0c4:'331'	LOG_WARNING( "big time lag (%d.%03d) => skipping animation events", time_diff_in_ms / 1000, time_diff_in_ms % 1000 );
	// <0>
	// <0x570ac9>|0x109|+0x00c:'333'	void* buffer = ALLOCA( 0x4000 );
	// <0>
	// <0x570ad5>|0x115|+0x00d:'335'	serialize_state( buffer, 0x4000 );
	// <0>
	// <0x570ae2>|0x122|+0x00a:'337'	deserialize_state( buffer, current_time_in_ms );
	// <0>
	// <0x570aec>|0x12c|+0x018:'339'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[UNCHECKED]
bool animation_player::tick( const u32 current_time_in_ms )
{
	skip_time_if_needed( current_time_in_ms );

	++m_in_tick;
	bool result = m_mixing_tree.tick( current_time_in_ms, m_first_subscribed_channel, m_callbacks_are_actual );
	--m_in_tick;

	if ( !m_in_tick && !m_callbacks_are_actual )
		compact_callbacks( );

	return result;

	// FUNCTION BODY
	// <0>
	// <5>
	// <0x570e3a>|0x00a|+0x006:'353'
	// <0>
	// <0x570e40>|0x010|+0x007:'355'
	// <0>
	// <0x570e47>|0x017|+0x01b:'357'
	// <0>
	// <0x570e62>|0x032|+0x015:'359'
	// <0>
	// <1>
	// <2>
	// <0x570e77>|0x047|+0x009:'363'
	// <0x570e80>|0x050|+0x006:'364'
	// <0>
	// <3>
	// ******
}

// STATE[STUB]
bool animation_player::tick_to_nearest_user_handled_callback( const u32 current_time_in_ms )
{
	skip_time_if_needed( current_time_in_ms );

	return false;

	// FUNCTION BODY
	// <0x570eda>|0x00a|+0x006:'373'
	// <0>
	// <1>
	// <0x570ee0>|0x010|+0x01f:'376'
	// <0x570eff>|0x02f|+0x010:'377'
	// <0x570f0f>|0x03f|+0x04a:'378'
	// <0x570f59>|0x089|+0x00c:'379'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
bool animation_player::set_target_and_tick(
	mixing::expression const&		expression,
	const u32						current_time_in_ms,
	boost::function< float4x4( pcvoid ) > const&	get_transform_functor
)
{
	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x570e90>|0x000|+0x00a:'388'
	// <0>
	// <1>
	// <2>
	// <0x570e9a>|0x00a|+0x008:'392'
	// <0>
	// <1>
	// <0x570ea2>|0x012|+0x012:'395'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x570eb4>|0x024|+0x008:'404'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x570ebc>|0x02c|+0x002:'412'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[UNCHECKED]
float4x4 single_object_get_transform( float4x4 const& transform, pcvoid const animated_object )
{
	VOSTOK_UNREFERENCED_PARAMETER( animated_object );
	return transform;

	// FUNCTION BODY
	// <0>
	// <0x56fcb0>|0x000|+0x015:'422'
	// ******
}

// STATE[STUB]
bool animation_player::set_target_and_tick(
	mixing::expression const&		expression,
	const u32						current_time_in_ms,
	float4x4 const&					transform_in_case_of_a_single_object_usage
)
{
	return false;

	// FUNCTION BODY
	// <0x570f7e>|0x00e|+0x06c:'427'
	// ******
}

// STATE[UNCHECKED]
void animation_player::set_object_transform( float4x4 const& object_transform, pcvoid const animated_object )
{
	m_mixing_tree.set_object_transform( animated_object, object_transform );

	// FUNCTION BODY
	// <0x5706e0>|0x000|+0x010:'432'
	// ******
}

// STATE[UNCHECKED]
float4x4 animation_player::get_object_transform( pcvoid const animated_object ) const
{
	return m_mixing_tree.get_object_transform( animated_object );

	// FUNCTION BODY
	// <0x5706c1>|0x001|+0x011:'437'
	// ******
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0>
	// <0x5706a0>|0x000|+0x01a:'448'
	// ******
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0>
	// <0x570680>|0x000|+0x01a:'454'
	// ******
}

// STATE[UNCHECKED]
void animation_player::convert_to_object_matrices(
	skeleton const&		skeleton,
	float4x4*			local_begin,
	float4x4*			local_end,
	pcvoid const		animated_object
) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_mixing_tree.convert_to_object_matrices( animated_object, skeleton, local_begin, local_end );

	// FUNCTION BODY
	// <0>
	// <0x570660>|0x000|+0x015:'460'
	// ******
}

// STATE[STUB]
void animation_player::reset( const bool clear_callbacks )
{
	// FUNCTION BODY
	// <0x56fe3a>|0x00a|+0x057:'465'
	// <0x56fe91>|0x061|+0x009:'466'
	// <0>
	// <0x56fe9a>|0x06a|+0x005:'468'
	// <0>
	// <0x56fe9f>|0x06f|+0x00b:'470'
	// <0>
	// <1>
	// <0x56feaa>|0x07a|+0x021:'473'
	// <0>
	// ******
}

// STATE[STUB]
void animation_player::subscribe(
	pcstr const								channel_id,
	new_callback_type const&				callback,
	pcvoid const							callback_uid,
	resources::managed_resource_ptr const&	animation,
	const u8								event_type,
	pcvoid const							animated_object
)
{
	// CALL SITE INFO
	// <0x570448> -> < unknown >
	// ******

	// FUNCTION BODY
	// <0x5702a0>|0x000|+0x007:'492'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5702a7>|0x007|+0x006:'497'
	// <0x5702ad>|0x00d|+0x002:'498'
	// <0x5702af>|0x00f|+0x004:'499'
	// <0>
	// <0x5702b3>|0x013|+0x110:'501'
	// <0>
	// <1>
	// <0x5703c3>|0x123|+0x006:'504'
	// <0x5703c9>|0x129|+0x00e:'505'
	// <0x5703d7>|0x137|+0x049:'506'
	// <0>
	// <0x570420>|0x180|+0x00a:'508'
	// <0x57042a>|0x18a|-0x007:'508'
	// <0>
	// <1>
	// <0x570423>|0x183|+0x00b:'511'
	// <0>
	// <0x57042e>|0x18e|-0x138:'513'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5702f6>|0x056|+0x006:'520'
	// <0x5702fc>|0x05c|+0x00e:'521'
	// <0x57030a>|0x06a|+0x006:'522'
	// <0>
	// <1>
	// <0x570310>|0x070|+0x01a:'525'
	// <0x57032a>|0x08a|+0x015:'526'
	// <0x57033f>|0x09f|+0x009:'527'
	// <0x570348>|0x0a8|+0x01b:'528'
	// <0>
	// <0x570363>|0x0c3|+0x006:'530'
	// <0x570369>|0x0c9|+0x00e:'531'
	// <0x570377>|0x0d7|+0x0c2:'532'
	// <0x570439>|0x199|+0x056:'532'
	// <0x57048f>|0x1ef|+0x003:'533'
	// <0>
	// <0x570492>|0x1f2|+0x004:'535'
	// <0x570496>|0x1f6|+0x00b:'536'
	// <0>
	// <0x5704a1>|0x201|-0x070:'538'
	// <0x570431>|0x191|+0x068:'539'
	// <0x570499>|0x1f9|+0x00e:'539'
	// <0x5704a7>|0x207|      :'539'	}
	// ******
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0>
	// <0x570601>|0x001|+0x02d:'550'
	// ******
}

// STATE[UNCHECKED]
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


	// FUNCTION BODY
	// <0x56fd51>|0x001|+0x00f:'555'
	// <0>
	// <0x56fd60>|0x010|+0x039:'557'
	// <0x56fd99>|0x049|-0x032:'557'
	// <0>
	// <0x56fd67>|0x017|+0x002:'559'
	// <0x56fd69>|0x019|+0x003:'560'
	// <0x56fd6c>|0x01c|+0x031:'561'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56fd9d>|0x04d|+0x00a:'566'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void animation_player::compact_callbacks( )
{
	// LOCALS
	// u32 								channels_count
	// subscribed_channel const* 		first_cloned_channel
	// subscribed_channel* 				previous_channel
	// subscribed_channel const* 		i
	// animation_callback* 				k
	// subscribed_channel* const 		new_channel
	// animation_callback* 				k
	// ******

	// CALL SITE INFO
	// <0x57003e> -> < unknown >
	// <0x5701f8> -> < unknown >
	// ******

	// FUNCTION BODY
	// <0>
	// <0x56ff39>|0x009|+0x003:'574'
	// <0>
	// <0x56ff3c>|0x00c|+0x006:'576'
	// <0x56ff42>|0x012|+0x00f:'577'
	// <0x56ff51>|0x021|+0x003:'578'
	// <0x56ff54>|0x024|+0x163:'579'
	// <0x5700b7>|0x187|-0x154:'579'
	// <0>
	// <0x56ff63>|0x033|+0x00f:'581'
	// <0x56ff72>|0x042|+0x004:'582'
	// <0x56ff76>|0x046|+0x003:'583'
	// <0x56ff79>|0x049|+0x002:'584'
	// <0x56ff7b>|0x04b|+0x006:'585'
	// <0>
	// <0x56ff81>|0x051|+0x006:'587'
	// <0>
	// <0x56ff87>|0x057|+0x027:'589'
	// <0x56ffae>|0x07e|+0x003:'590'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x56ffb1>|0x081|+0x01f:'595'
	// <0>
	// <0x56ffd0>|0x0a0|+0x00a:'597'
	// <0>
	// <1>
	// <0x56ffda>|0x0aa|+0x00c:'600'
	// <0>
	// <0x56ffe6>|0x0b6|+0x099:'602'
	// <0x57007f>|0x14f|+0x007:'603'
	// <0x570086>|0x156|+0x003:'604'
	// <0x570089>|0x159|+0x002:'605'
	// <0x57008b>|0x15b|+0x003:'606'
	// <0x57008e>|0x15e|+0x010:'607'
	// <0>
	// <1>
	// <0x57009e>|0x16e|+0x005:'610'
	// <0>
	// <0x5700a3>|0x173|+0x007:'612'
	// <0x5700aa>|0x17a|+0x003:'613'
	// <0x5700ad>|0x17d|+0x002:'614'
	// <0>
	// <1>
	// <0x5700af>|0x17f|+0x003:'617'
	// <0>
	// <1>
	// <0x5700b2>|0x182|+0x002:'620'
	// <0x5700b4>|0x184|+0x01a:'621'
	// <0>
	// <1>
	// <0x5700ce>|0x19e|+0x00b:'624'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5700d9>|0x1a9|+0x027:'629'
	// <0>
	// <0x570100>|0x1d0|+0x007:'631'
	// <0x570107>|0x1d7|+0x155:'632'
	// <0x57025c>|0x32c|+0x017:'632'
	// <0x570273>|0x343|-0x161:'632'
	// <0>
	// <1>
	// <2>
	// <0x570112>|0x1e2|+0x005:'636'
	// <0x570117>|0x1e7|+0x007:'637'
	// <0x57011e>|0x1ee|+0x00c:'638'
	// <0x57012a>|0x1fa|+0x006:'639'
	// <0x570130>|0x200|+0x002:'640'
	// <0x570132>|0x202|+0x006:'641'
	// <0>
	// <0x570138>|0x208|+0x006:'643'
	// <0>
	// <0x57013e>|0x20e|+0x01e:'645'
	// <0x57015c>|0x22c|+0x00a:'646'
	// <0x570166>|0x236|+0x002:'647'
	// <0x570168>|0x238|+0x016:'648'
	// <0>
	// <1>
	// <2>
	// <0x57017e>|0x24e|+0x0db:'652'
	// <0x570259>|0x329|-0x0c5:'652'
	// <0>
	// <1>
	// <0x570194>|0x264|+0x002:'655'
	// <0x570196>|0x266|+0x007:'656'
	// <0>
	// <0x57019d>|0x26d|+0x09c:'658'
	// <0x570239>|0x309|+0x007:'659'
	// <0x570240>|0x310|+0x003:'660'
	// <0x570243>|0x313|+0x002:'661'
	// <0x570245>|0x315|+0x009:'662'
	// <0x57024e>|0x31e|+0x017:'663'
	// <0>
	// <1>
	// <0x570265>|0x335|+0x011:'666'
	// <0>
	// <1>
	// <0x570276>|0x346|+0x007:'669'
	// <0>
	// <0x57027d>|0x34d|+0x006:'671'
	// <0x570283>|0x353|+0x00a:'672'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// ******
}

// STATE[STUB]
void animation_player::unsubscribe( pcstr const channel_id, pcvoid const callback_uid )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5704be>|0x00e|+0x03f:'690'
	// <0x5704fd>|0x04d|-0x02d:'691'
	// <0>
	// <0x5704d0>|0x020|+0x032:'693'
	// <0>
	// <1>
	// <2>
	// <0x570502>|0x052|+0x00e:'697'
	// <0>
	// <0x570510>|0x060|+0x014:'699'
	// <0>
	// <0x570524>|0x074|+0x0a5:'701'
	// <0>
	// <1>
	// <2>
	// <0x5705c9>|0x119|+0x00f:'705'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5705d8>|0x128|+0x00c:'712'
	// <0x5705e4>|0x134|+0x006:'713'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void animation_player::unsubscribe( reserved_channel_ids_enum channel_id, pcvoid const callback_uid )
{
	char channel_id_string[]					= { u8(channel_id), 0 };
	unsubscribe									( channel_id_string, callback_uid );

	// FUNCTION BODY
	// <0x570640>|0x000|+0x004:'723'
	// <0x570644>|0x004|+0x014:'724'
	// ******
}

// STATE[UNCHECKED]
u32 animation_player::get_state_buffer_size( ) const
{
	return 0x4000; // sushi@TODO: What this constant is. Can be figured out by usage.

	// FUNCTION BODY
	// <0x56fc90>|0x000|+0x005:'729'
	// ******
}

// STATE[STUB]
void invert_animation_times( mixing::n_ary_tree_animation_node& animation, const u32 time_in_ms )
{
	// LOCALS
	// n_ary_tree_time_inverter 		time_inverter
	// ******

	// CALL SITE INFO
	// <0x56fd04> -> void < unknown >( mixing::n_ary_tree_visitor& )
	// ******

	// FUNCTION BODY
	// <0>
	// <0x56fcd8>|0x008|+0x020:'735'
	// <0x56fcf8>|0x028|+0x015:'736'
	// <0>
	// <0x56fd0d>|0x03d|+0x031:'738'
	// ******
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0>
	// <4>
	// <0x56fe11>|0x001|+0x007:'748'
	// <0x56fe18>|0x008|+0x011:'749'
	// <0>
	// <4>
	// ******
}

// STATE[UNCHECKED]
void animation_player::serialize_state( void* buffer, const u32 buffer_size )
{
	*((u32*)buffer + 0) = 0xB19B00B5;
	*((u32*)buffer + 1) = m_mixing_tree_buffer_size;
	mutable_buffer tree_buffer( (void*)((u32*)buffer + 2), buffer_size ); // sushi@NOTE: Assembly has +4 twice, so possibly buffer itself is modified

	mixing::n_ary_tree* tree = static_cast<mixing::n_ary_tree*>( tree_buffer.c_ptr() );
	tree_buffer += sizeof( mixing::n_ary_tree );

	if ( m_mixing_tree_buffer_size )
	{
		mixing::n_ary_tree_transition_tree_constructor tree_constructor(
			tree_buffer,
			m_mixing_tree,
			m_mixing_tree,
			m_mixing_tree.animations_count( ),
			m_mixing_tree.animated_objects_count( ),
			m_mixing_tree.tree_actual_time_in_ms( ),
			m_first_subscribed_channel,
			mixing::n_ary_tree_transition_tree_constructor::transform_functor_type( )
		);
		*tree = tree_constructor.computed_tree( );
	}
	else
	{
		new (tree) mixing::n_ary_tree( );
	}

	tree->adjust_animation_events_times( m_mixing_tree ); // sushi@TODO: To call this I added animation_player as friend class

	for ( mixing::n_ary_tree_animation_node* root = tree->weight_root( ) ; root != NULL ; root = root->m_next_weight_animation )
		invert_animation_times( *root, m_mixing_tree.tree_actual_time_in_ms( ) );

	// FUNCTION BODY
	// <0>
	// <10>
	// <0x5708b5>|0x015|+0x00a:'840'
	// <0>
	// <1>
	// <0x5708bf>|0x01f|+0x00b:'843'	*((u32*)buffer + 1) = m_mixing_tree_buffer_size;
	// <0>
	// <1>
	// <0x5708ca>|0x02a|+0x009:'846'	mutable_buffer tree_buffer( (void*)((u32*)buffer + 2), buffer_size );
	// <0>
	// <13>
	// <0x5708d3>|0x033|+0x089:'861'
	// <0>
	// <0x57095c>|0x0bc|+0x004:'863'
	// <0x570960>|0x0c0|+0x027:'864'
	// <0>
	// <1>
	// <0x570987>|0x0e7|+0x00c:'867'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x570993>|0x0f3|+0x01e:'872'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void animation_player::deserialize_state( void* buffer, const u32 time_in_ms )
{
	// LOCALS
	// mutable_buffer 					mixing_buffer
	// ******

	// FUNCTION BODY
	// <0x570700>|0x000|+0x009:'880'	{
	// <0>
	// <1>
	// <2>
	// <0x570709>|0x009|+0x006:'884'
	// <0>
	// <0x57070f>|0x00f|+0x002:'886'
	// <0x570711>|0x011|+0x003:'887'
	// <0>
	// <1>
	// <0x570714>|0x014|+0x00d:'890'
	// <0>
	// <0x570721>|0x021|+0x015:'892'
	// <0x570736>|0x036|+0x038:'893'
	// <0>
	// <1>
	// <2>
	// <0x57076e>|0x06e|+0x01f:'897'
	// <0>
	// <1>
	// <0x57078d>|0x08d|+0x00a:'900'
	// <0>
	// <0x570797>|0x097|+0x025:'902'
	// <0>
	// <17>
	// <0x5707bc>|0x0bc|+0x0a9:'921'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x570865>|0x165|+0x008:'927'
	// <0>
	// <1>
	// <2>
	// <0x57086d>|0x16d|-0x10c:'931'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x570761>|0x061|+0x12f:'938'
	// <0x570890>|0x190|      :'938'	}
	// ******
}

// STATE[STUB]
void animation_player::destroy_state( void* buffer )
{
	*(u32*)buffer = 0xFFBDDDDD;
	// mixing::n_ary_tree::destroy( );


	// FUNCTION BODY
	// <0>
	// <1>
	// <0x56ff13>|0x003|+0x006:'944'
	// <0x56ff19>|0x009|+0x003:'945'
	// <0>
	// <0x56ff1c>|0x00c|+0x010:'947'
	// <0>
	// ******
}

// STATE[UNCHECKED]
u32 animation_player::last_tick_time_in_ms( ) const
{
	return m_mixing_tree.tree_actual_time_in_ms( );

	// FUNCTION BODY
	// <0x56fca0>|0x000|+0x006:'953'
	// ******
}

} // namespace animation
} // namespace vostok