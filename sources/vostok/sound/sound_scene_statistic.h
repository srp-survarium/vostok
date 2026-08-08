////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOUND_SCENE_STATISTIC_H_INCLUDED
#define VOSTOK_SOUND_SOUND_SCENE_STATISTIC_H_INCLUDED

#include <vostok/intrusive_list.h>
#include <vostok/sound/proxy_statistic.h>

namespace vostok {

namespace strings { class text_tree_item; }

namespace sound {

typedef intrusive_list< proxy_statistic, proxy_statistic*, &proxy_statistic::next, threading::single_threading_policy, size_policy, no_debug_policy > proxy_statistic_list;

struct sound_scene_statistic
{
	sound_scene_statistic( );
	void fill_text_tree( strings::text_tree_item* item ) const;

	inline ~sound_scene_statistic( )
	{
	}

	struct
	{
		u32 m_sound_types[4];
		u32 m_active_proxies_count;
		u32 m_propagators_count;
		u32 m_registered_receivers_count;
		u32 m_active_voices_count[2];
	} values;
	proxy_statistic_list m_proxies_statistic;
};

STATIC_SIZE_ASSERT( sound_scene_statistic, 0x34 );

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOUND_SCENE_STATISTIC_H_INCLUDED
