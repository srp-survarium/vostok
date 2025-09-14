////////////////////////////////////////////////////////////////////////////
//	Created		: 30.05.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef EFFECT_SPEEDTREE_SELECTION_H_INCLUDED
#define EFFECT_SPEEDTREE_SELECTION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>

namespace vostok {
namespace render {

class effect_speedtree_selection: public effect_descriptor
{
public:
	enum
	{
		branch	= 0,
		frond,
		leafmesh,
		leafcard,
		billboard,
	};
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_speedtree_selection

} // namespace render 
} // namespace vostok 


#endif // #ifndef EFFECT_SPEEDTREE_SELECTION_H_INCLUDED