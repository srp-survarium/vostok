////////////////////////////////////////////////////////////////////////////
//	Created		: 04.05.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_RECEIVER_H_INCLUDED
#define SOUND_RECEIVER_H_INCLUDED

#include <vostok/sound/api.h>
#include <vostok/sound/sound.h>
#include <vostok/sound/sound_object_commands.h>
#include <vostok/sound/functor_command.h>
#include <vostok/sound/atomic_half3.h>


namespace vostok {
namespace sound {

class sound_producer;
class receiver_collision;
class world_user;

class VOSTOK_SOUND_API sound_receiver : private sound::noncopyable 
{
public:
						sound_receiver						( );
	virtual 			~sound_receiver						( );		
	virtual void		on_sound_event						( sound_producer const& sound_source ) = 0;
protected:
			void		set_position						( float3 const& position );
public:
			void		on_register_receiver_order_created	( atomic_half3* position );
private:
	atomic_half3*		m_position;
}; // class sound_receiver

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_RECEIVER_H_INCLUDED
