////////////////////////////////////////////////////////////////////////////
//	Created		: 27.04.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_OBJECT_COMMANDS_H_INCLUDED
#define SOUND_OBJECT_COMMANDS_H_INCLUDED

#include <vostok/intrusive_spsc_queue.h>
#include <vostok/intrusive_mpsc_queue.h>
#include <vostok/sound/sound.h>

namespace vostok {
namespace sound {

struct sound_order : public boost::noncopyable
{
			sound_order					( );
	virtual	~sound_order				( );
	
	virtual void	execute	( );

	sound_order*				m_next_for_orders;
	sound_order*				m_next_for_postponed_orders;
}; // struct sound_order

struct sound_response : public boost::noncopyable
{
			sound_response				( );
	virtual	~sound_response				( );
	
	virtual void	execute	( );

	sound_response*				m_next;
}; // struct sound_response

} // namespace sound
} // namespace vostok


#endif // #ifndef SOUND_OBJECT_COMMANDS_H_INCLUDED
