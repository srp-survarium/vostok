////////////////////////////////////////////////////////////////////////////
//	Created		: 16.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_OVERDRAW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_OVERDRAW_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_editor_show_overdraw: public effect_descriptor
{
public:
	enum
	{
		num_overdraw_layers = 10,
	};
	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
}; // class effect_editor_show_overdraw

} // namespace render 
} // namespace vostok 


#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_EDITOR_SHOW_OVERDRAW_H_INCLUDED