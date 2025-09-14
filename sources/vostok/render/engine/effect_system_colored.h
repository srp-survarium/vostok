////////////////////////////////////////////////////////////////////////////
//	Created		: 17.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef BLENDER_SYSTEM_COLORED_H_INCLUDED
#define BLENDER_SYSTEM_COLORED_H_INCLUDED

#include "vostok/render/core/effect_descriptor.h"

namespace vostok {
namespace render_dx10 {

class effect_system_colored: public effect
{
public:
	effect_system_colored () : effect() {}
	~effect_system_colored() {}

	enum techniques{ solid, stenciled, voided };

	virtual void compile( effect_compiler& compiler, const effect_compilation_options& options);

private:

};

} // namespace render_dx10
} // namespace vostok

#endif // #ifndef BLENDER_SYSTEM_COLORED_H_INCLUDED