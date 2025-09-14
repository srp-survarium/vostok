////////////////////////////////////////////////////////////////////////////
//	Created		: 12.11.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTY_VISUALIZERS_H_INCLUDED
#define PROPERTY_VISUALIZERS_H_INCLUDED

namespace vostok {
namespace particle_editor {

ref class property_visualizer_domain : public editor_base::property_visualizer
{
public:
					property_visualizer_domain	( configs::lua_config_value const& v );
	virtual			~property_visualizer_domain	( );
	virtual void	draw						( math::float4x4 const& world_matrix, vostok::render::debug::renderer& r, render::scene_pointer scene );

private:
	configs::lua_config_value*		m_cfg;
}; // class property_visualizers

} // namespace particle_editor
} // namespace vostok

#endif // #ifndef PROPERTY_VISUALIZERS_H_INCLUDED