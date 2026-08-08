////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_PROPAGATOR_STATISTIC_H_INCLUDED
#define VOSTOK_SOUND_PROPAGATOR_STATISTIC_H_INCLUDED

#include <vostok/sound/playback_mode.h>

namespace vostok {

namespace strings { class text_tree_item; }

namespace sound {

struct propagator_statistic
{
	inline propagator_statistic( ) : next( 0 )
	{
	}

	void fill_text_tree( strings::text_tree_item* item ) const;

	inline ~propagator_statistic( )
	{
	}

	propagator_statistic* next;
	fixed_string< 260 > m_filename;
	resources::managed_resource_ptr m_rms;
	u32 m_length;
	u32 m_current_playing_time;
	playback_mode m_playback_mode;
};

STATIC_SIZE_ASSERT( propagator_statistic, 0x124 );

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_PROPAGATOR_STATISTIC_H_INCLUDED
