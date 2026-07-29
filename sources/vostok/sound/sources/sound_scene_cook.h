////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_SCENE_COOK_H_INCLUDED
#define SOUND_SCENE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace sound {

class sound_world;

class sound_scene_cook :	public resources::translate_query_cook,
							private boost::noncopyable
{
public:
							sound_scene_cook		( sound_world& world );

	virtual	void			translate_query			( resources::query_result_for_cook& parent );
	virtual	void			delete_resource			( resources::resource_base* resource );
private:
	sound_world&			m_sound_world;
}; // class sound_scene_cook

STATIC_SIZE_ASSERT( sound_scene_cook, 0x24 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_SCENE_COOK_H_INCLUDED
