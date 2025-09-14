////////////////////////////////////////////////////////////////////////////
//	Created		: 21.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_SCENE_COOK_H_INCLUDED
#define VOSTOK_RENDER_SCENE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class scene;

class scene_cook : public resources::translate_query_cook, public boost::noncopyable	
{
public:
	scene_cook										();
	virtual	void			translate_query			( resources::query_result_for_cook&	parent );
	virtual void			delete_resource			( vostok::resources::resource_base* resource );
	
private:
	void			on_particle_world_created(resources::queries_result& result, vostok::render::scene* created_resource, resources::query_result_for_cook* in_out_query);
}; // class scene_cook

void register_scene_cook();
void unregister_scene_cook();

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_SCENE_COOK_H_INCLUDED