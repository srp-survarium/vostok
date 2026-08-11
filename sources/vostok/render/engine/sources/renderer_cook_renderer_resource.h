#ifndef RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED
#define RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED

#include "renderer_cook.h"

namespace vostok {
namespace render {

struct renderer_cook::renderer_resource : public resources::unmanaged_resource {
}; // struct renderer_cook::renderer_resource

STATIC_SIZE_ASSERT(renderer_cook::renderer_resource, 0x108);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_COOK_RENDERER_RESOURCE_H_INCLUDED
