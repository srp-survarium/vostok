#ifndef VOSTOK_RENDER_ENGINE_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED

#include "effect_material_base.h"

namespace vostok {
namespace render {

class decal_default_material_effect : public effect_material_base {
public:
	explicit decal_default_material_effect( bool is_forward ) :
		m_is_forward( is_forward )
	{
	}

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	) override;

private:
	bool m_is_forward;
};

STATIC_SIZE_ASSERT( decal_default_material_effect, 0x8 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_DECAL_DEFAULT_MATERIAL_EFFECT_H_INCLUDED
