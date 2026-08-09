////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
//
//	Playback state shared by animation clients.
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_PLAYBACK_STATE_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_PLAYBACK_STATE_H_INCLUDED

namespace vostok {
namespace animation {

struct animation_playback_state {
	inline			animation_playback_state( ) : interval_id( 0 ), interval_time( 0.0f ) { }

	// Target reset @0x087f60 zeros both members (interval_id = 0; interval_time = 0.0f).
	inline	void	reset					( ) { interval_id = 0; interval_time = 0.0f; }

public:
	/* 0x0000 */	u32			interval_id;
	/* 0x0004 */	float		interval_time;
}; // struct animation_playback_state

STATIC_SIZE_ASSERT(animation_playback_state, 0x8);

}  // namespace animation
}  // namespace vostok

#endif // VOSTOK_ANIMATION_ANIMATION_PLAYBACK_STATE_H_INCLUDED
