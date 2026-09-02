// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/effect_descriptor.h>
namespace vostok {
namespace render {

effect_descriptor::effect_descriptor( )
{
}

effect_descriptor::~effect_descriptor( )
{
}

void effect_descriptor::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &compiler, &config );
}

} // namespace render
} // namespace vostok
