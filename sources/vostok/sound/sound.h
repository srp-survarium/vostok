////////////////////////////////////////////////////////////////////////////
//	Created		: 22.04.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SOUND_H_INCLUDED
#define VOSTOK_SOUND_SOUND_H_INCLUDED

#include <vostok/sound/api.h>
#include <vostok/sound/playback_mode.h>
#include <vostok/sound/sound_cone_type.h>

namespace vostok {
namespace sound {

class sound_receiver;
class sound_producer;

typedef	resources::resource_ptr <
		resources::unmanaged_resource,
		resources::unmanaged_intrusive_base
	> sound_scene_ptr;

class sound_scene;

class VOSTOK_SOUND_API noncopyable 
{
protected:
	inline						noncopyable	( ) { }
	inline						~noncopyable( ) { }

private:  // emphasize the following members are private
								noncopyable	( const noncopyable& );
	inline	noncopyable const&	operator=	( const noncopyable& );
}; // class noncopyable 


enum command_result_enum
{
	command_result_executed,
	command_result_postponed
};

// sound graph path search limits (retail keeps them in this unnamed enum)
enum
{
	max_different_paths_count	= 4,
	max_portals_in_path			= 32,
};

struct play_params
{
	float3					m_position;
	sound_producer const*	m_producer;
	sound_receiver const*	m_ignorable_receiver;
	u32						m_id;
	playback_mode			m_mode;
	bool					m_is_positional;
};

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SOUND_H_INCLUDED
