////////////////////////////////////////////////////////////////////////////
//	Created		: 28.12.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef EFFECT_POST_PROCESS_ANTIALIASING_H_INCLUDED
#define EFFECT_POST_PROCESS_ANTIALIASING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_post_process_antialiasing: public effect_descriptor
{
public:
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_post_process_antialiasing

} // namespace render 
} // namespace vostok 


#endif // #ifndef EFFECT_POST_PROCESS_ANTIALIASING_H_INCLUDED