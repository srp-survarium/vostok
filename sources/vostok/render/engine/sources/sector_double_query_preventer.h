#ifndef VOSTOK_RENDER_ENGINE_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/math_aabb.h>
#include <vostok/math_color.h>
#include <vostok/math_frustum.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/culling/aab_rect.h>
#include <vostok/render/culling/portal_sector_structure.h>

namespace vostok {
namespace render {

class system_renderer;

namespace culling {

class sector_double_query_preventer {
public:
	struct frustum_image {
		frustum_image( ) : c( 0, 0, 0, 0 ) { }

		float3 vertices[8];
		math::color c;
	};

	STATIC_SIZE_ASSERT( frustum_image, 0x64 );

	sector_double_query_preventer( spatial_sector const* sectors, u32 sectors_count );
	~sector_double_query_preventer( );

	void clear( );
	bool is_possible_points_for_frustum( float3 const (&vertices)[4], u32 sector_id ) const;
	void add_ss_aab_rect( aab_rect const& rect, u32 sector_id );
	bool is_possible_ss_aab_rect( aab_rect const& rect, u32 sector_id ) const;
	void add_frustum( math::frustum const& f, u32 sector_id );
	bool is_aabb_in_sector( math::aabb const& bbox, u32 sector_id ) const;
	bool is_visible_aabb( math::aabb const& bbox, u16 const* sectors_begin, u16 const* sectors_end ) const;
	void make_frustum_images( float3 const* furthest_vertices );
	void render( system_renderer& r );
	u32 frustums_count( ) const;

private:
	void* m_buffer_for_frustum_vectors;
	buffer_vector<vector<math::frustum> >* m_sectors_max_frustums;
	void* m_buffer_for_rect_vectors;
	buffer_vector<vector<aab_rect> >* m_sectors_max_rects;
	vector<frustum_image> m_frustum_images;
};

STATIC_SIZE_ASSERT( sector_double_query_preventer, 0x1C );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED
