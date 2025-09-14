////////////////////////////////////////////////////////////////////////////
//	Created		: 12.11.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTY_VISUALIZERS_H_INCLUDED
#define PROPERTY_VISUALIZERS_H_INCLUDED

namespace vostok {
namespace editor_base {

public interface class property_visualizer
{
	virtual void draw		( math::float4x4 const& world_matrix, vostok::render::debug::renderer& r, render::scene_pointer scene ) =0;
};

} // namespace editor_base
} // namespace vostok

#endif // #ifndef PROPERTY_VISUALIZERS_H_INCLUDED