#ifndef VOSTOK_RENDER_ENGINE_RES_TEXTURE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RES_TEXTURE_H_INCLUDED

#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

class res_texture : public resource_intrusive_base {
public:
	void destroy_impl( ) const { }
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RES_TEXTURE_H_INCLUDED
