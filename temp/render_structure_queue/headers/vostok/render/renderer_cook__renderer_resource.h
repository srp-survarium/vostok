////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED
#define RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct renderer_cook::renderer_resource : public resources::unmanaged_resource {
	inline		renderer_resource	( ) { /* no source */ }
	virtual		~renderer_resource	( ) { /* no source */ }
}; // struct renderer_cook::renderer_resource

STATIC_SIZE_ASSERT(renderer_cook::renderer_resource, 0x108);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED
