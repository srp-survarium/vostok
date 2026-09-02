// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SCENE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>
#include <boost/noncopyable.hpp>

namespace vostok {
namespace render {

class scene;

class scene_cook :
	public resources::translate_query_cook,
	public boost::noncopyable
{
public:
	scene_cook( );

	virtual void translate_query( resources::query_result_for_cook& in_out_query ) override;
	virtual void delete_resource( resources::resource_base* resource ) override;

private:
	void on_particle_world_created(
		resources::queries_result&			result,
		scene*								created_resource,
		resources::query_result_for_cook*	in_out_query
	);
};

STATIC_SIZE_ASSERT( scene_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_COOK_H_INCLUDED
