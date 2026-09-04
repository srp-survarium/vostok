////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_PROXY_STATISTIC_H_INCLUDED
#define VOSTOK_SOUND_PROXY_STATISTIC_H_INCLUDED

#include <vostok/intrusive_list.h>
#include <vostok/sound/sound_type.h>
#include <vostok/sound/sound_cone_type.h>
#include <vostok/sound/emitter_type.h>
#include <vostok/sound/propagator_statistic.h>

namespace vostok {

namespace strings { class text_tree_item; }

namespace sound {

typedef intrusive_list< propagator_statistic, propagator_statistic*, &propagator_statistic::next, threading::single_threading_policy, size_policy, no_debug_policy > propagator_statistic_list;

struct proxy_statistic
{
	inline proxy_statistic( ) : next( 0 )
	{
	}

	void fill_text_tree( strings::text_tree_item* item, bool draw_propagators_stats ) const;

	proxy_statistic* next;
	float3 m_position;
	float3 m_direction;
	propagator_statistic_list m_propagator_statistics;
	u32 m_id;
	sound_type m_sound_type;
	sound_cone_type m_cone_type;
	emitter_type m_emitter_type;
};

STATIC_SIZE_ASSERT( proxy_statistic, 0x3C );

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_PROXY_STATISTIC_H_INCLUDED
