#include "pch.h"
#include "stage_screen_image.h"

#include <vostok/math_float2.h>
#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class scr_quad_effect : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override;
};

STATIC_SIZE_ASSERT( scr_quad_effect, 0x4 );

struct screen_image_vertex {
	screen_image_vertex( float x, float y, float u, float v ) :
		position	( x, y ),
		text_coords	( u, v )
	{
	}

	float2 position;
	float2 text_coords;
};

STATIC_SIZE_ASSERT( screen_image_vertex, 0x10 );

void scr_quad_effect::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x86360]
}

bool stage_screen_image::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b8b0]
	return false;
}

void stage_screen_image::execute( res_texture_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b8c0]
}

} // namespace render
} // namespace vostok
