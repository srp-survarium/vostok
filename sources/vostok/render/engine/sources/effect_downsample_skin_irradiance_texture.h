////////////////////////////////////////////////////////////////////////////
//	Created		: 22.08.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED
#define VOSTOK_RENDER_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_downsample_skin_irradiance_texture: public effect_descriptor
{
public:
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_downsample_skin_irradiance_texture

} // namespace render 
} // namespace vostok 


#endif // #ifndef VOSTOK_RENDER_EFFECT_DOWNSAMPLE_SKIN_IRRADIANCE_TEXTURE_H_INCLUDED