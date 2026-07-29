////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_RENDER_SURFACE_H_INCLUDED
#define RENDER_SKELETON_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_surface;

/* FORWARD REFS */
class vostok::render::vector<vostok::math::float4x4>;

namespace vostok {
namespace render {

class skeleton_render_surface : public render_surface {
public:
					skeleton_render_surface	( );

	virtual	void	update					( vector< float4x4 > const& arg_0 ) = 0;

	virtual	bool	is_dynamic				( ) const override { /* no source */ }

	virtual			~skeleton_render_surface( ) { /* no source */ }
}; // class skeleton_render_surface

STATIC_SIZE_ASSERT(skeleton_render_surface, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_RENDER_SURFACE_H_INCLUDED
