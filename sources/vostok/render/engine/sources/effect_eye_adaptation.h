////////////////////////////////////////////////////////////////////////////
//	Created		: 11.05.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef EFFECT_EYE_ADAPTATION_H_INCLUDED
#define EFFECT_EYE_ADAPTATION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_eye_adaptation: public effect_descriptor
{
public:
	effect_eye_adaptation() {}
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_eye_adaptation

} // namespace render 
} // namespace vostok 

#endif // #ifndef EFFECT_EYE_ADAPTATION_H_INCLUDED