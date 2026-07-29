#ifndef VOSTOK_RENDER_ENGINE_RES_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RES_GEOMETRY_H_INCLUDED

#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

class res_geometry : public resource_intrusive_base {
public:
	void destroy_impl( ) const { }
};

typedef intrusive_ptr<
	res_geometry,
	resource_intrusive_base,
	threading::single_threading_policy
> res_geometry_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RES_GEOMETRY_H_INCLUDED
