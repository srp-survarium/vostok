////////////////////////////////////////////////////////////////////////////
//	Created		: 13.10.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOLUMETRIC_SOUND_H_INCLUDED
#define VOLUMETRIC_SOUND_H_INCLUDED

#include "object.h"
#include <vostok/sound/sound_emitter.h>

namespace vostok {
namespace collision {
	class geometry_instance;
} // namespace collision
} // namespace vostok

namespace survarium {

class object_volumetric_sound : public game_object_
{
	typedef game_object_				super;
public:
					object_volumetric_sound	( game_scene& w );
	virtual			~object_volumetric_sound( );
	virtual void	load					( configs::binary_config_value const& t );
	virtual void	load_contents			( );
	virtual void	unload_contents			( );
private:
			void	on_collision_object_loaded	( game_object_ptr_ const& object );
			void	on_config_loaded			( resources::queries_result& data );
			void	on_sound_loaded				( resources::queries_result& data );
private:
	fs::path_string							m_sound_name;
	vostok::sound::sound_emitter_ptr			m_emitter;
	vostok::sound::sound_instance_proxy_ptr	m_proxy;
	vostok::sound::sound_scene_ptr			m_sound_scene;
	vostok::sound::world_user&				m_world_user;
	vostok::collision::geometry_instance*		m_collision_geometry;
	float									m_radius;
}; // class volumetric_sound

} // namespace survarium

#endif // #ifndef VOLUMETRIC_SOUND_H_INCLUDED