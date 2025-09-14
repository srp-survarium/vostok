////////////////////////////////////////////////////////////////////////////
//	Created		: 13.02.2009
//	Author		: Mykhailo Parfeniuk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

effect_descriptor::effect_descriptor()
{
	
}

effect_descriptor::~effect_descriptor()
{
	
}

void effect_descriptor::compile(effect_compiler& compiler, custom_config_value const& config)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &compiler, &config );
}

} // namespace render 
} // namespace vostok