// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_SOUND_H_INCLUDED
#define OBJECT_SOUND_H_INCLUDED

#include "game_object_static.h"
// sound_emitter_ptr + sound_instance_proxy_ptr
#include <vostok/sound/sound_emitter.h>

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class object_sound : public game_object_static {
	typedef sound::sound_emitter_ptr sound_emitter_ptr;
	typedef game_object_static super;

public:
			explicit	object_sound			( base_game_scene& w );
	virtual				~object_sound			( );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;
	virtual	void		remove					( ) override;

protected:
			void		on_sound_resources_ready(
							resources::queries_result&		data,
							boost::function< void( game_object_& ) >&	callback
						);

protected:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	pcstr								m_sound_name;
	/* 0x0154 */	s32									m_sound_emitter_type;
	/* 0x0158 */	sound_emitter_ptr					m_sound_emitter;
	/* 0x015c */	sound::sound_instance_proxy_ptr		m_sound_instance;
}; // class object_sound

STATIC_SIZE_ASSERT(object_sound, 0x160);

} // namespace survarium

#endif // #ifndef OBJECT_SOUND_H_INCLUDED
