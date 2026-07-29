////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2009
//	Author		: Mykhailo Parfeniuk
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef BLENDER_COMBINE_H_INCLUDED
#define BLENDER_COMBINE_H_INCLUDED

namespace vostok {
namespace render {


class blender_combine: public blender
{
public:
	virtual void compile(blender_compiler& compiler, const blender_compilation_options& options);
}; // class blender_combine

} // namespace render 
} // namespace vostok 


#endif // #ifndef BLENDER_COMBINE_H_INCLUDED