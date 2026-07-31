#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_speedtree_selection : public effect_descriptor {
public:
	enum
	{
		branch	= 0,
		frond,
		leafmesh,
		leafcard,
		billboard,
	};

	effect_speedtree_selection( ) { }
	virtual ~effect_speedtree_selection( ) { }

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;
};

STATIC_SIZE_ASSERT( effect_speedtree_selection, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SPEEDTREE_SELECTION_H_INCLUDED
