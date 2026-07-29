#ifndef VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

class res_effect : public resources::unmanaged_resource {
public:
	virtual ~res_effect( ) { }

private:
	byte m_carcass[0x20];
};

STATIC_SIZE_ASSERT( res_effect, 0x128 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED
