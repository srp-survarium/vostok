////////////////////////////////////////////////////////////////////////////
//	Created		: 24.04.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef BLENDER_NOTEXTURE_H_INCLUDED
#define BLENDER_NOTEXTURE_H_INCLUDED

#include <vostok/render/engine/effect_deffer_base.h>

namespace vostok {
namespace render_dx10 {


class effect_notexture: public effect
{
public:
	effect_notexture():effect(){}
	~effect_notexture(){}

	virtual void compile(effect_compiler& compiler, const effect_compilation_options& options);

private:

}; // class effect_notexture

} // namespace render 
} // namespace vostok 


#endif // #ifndef BLENDER_NOTEXTURE_H_INCLUDED