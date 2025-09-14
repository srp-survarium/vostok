////////////////////////////////////////////////////////////////////////////
//	Created		: 08.06.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef EFFECT_POST_PROCESS_DEFERRED_TRANSPARENCY_H_INCLUDED
#define EFFECT_POST_PROCESS_DEFERRED_TRANSPARENCY_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_post_process_deferred_transparency: public effect_descriptor
{
public:
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_post_process_deferred_transparency

} // namespace render 
} // namespace vostok 


#endif // #ifndef EFFECT_POST_PROCESS_DEFERRED_TRANSPARENCY_H_INCLUDED