////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_COOK_DATA_H_INCLUDED
#define RENDER_GRASS_COOK_DATA_H_INCLUDED

/* INCLUDES */
class vostok::resources::query_result_for_cook;
struct vostok::render::grass_layer_data;
struct vostok::render::grass_layer_desc;
struct vostok::render::grass_world;

namespace vostok {
namespace render {

struct grass_cook_data {

	/* 0x0000 */	grass_layer_desc**						desc;
	/* 0x0004 */	grass_layer_data**						data;
	/* 0x0008 */	resources::query_result_for_cook*		parent_query;
	/* 0x000c */	grass_world*							result;
}; // struct grass_cook_data

STATIC_SIZE_ASSERT(grass_cook_data, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_COOK_DATA_H_INCLUDED
