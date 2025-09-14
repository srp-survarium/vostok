////////////////////////////////////////////////////////////////////////////
//	Created		: 24.04.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef BLENDER_NOTEXTURE_H_INCLUDED
#define BLENDER_NOTEXTURE_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_wireframe_colored: public effect_descriptor
{
public:
	effect_wireframe_colored():effect_descriptor(){}
	~effect_wireframe_colored(){}

	virtual	void compile(effect_compiler& compiler, custom_config_value const& config);
	//virtual	void cache	(custom_config_value const& config);

private:

}; // class effect_wireframe_colored

} // namespace render 
} // namespace vostok 


#endif // #ifndef BLENDER_NOTEXTURE_H_INCLUDED