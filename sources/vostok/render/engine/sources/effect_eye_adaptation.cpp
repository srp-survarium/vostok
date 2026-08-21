#include "pch.h"
#include "effect_eye_adaptation.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_eye_adaptation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "eye_adaptation", NULL, "eye_adaptation", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "eye_adaptation", NULL, "eye_adaptation_copy", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
