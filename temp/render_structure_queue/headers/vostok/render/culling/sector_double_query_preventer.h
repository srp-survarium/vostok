////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED
#define RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED

/* INCLUDES */
class vostok::buffer_vector<vostok::render::vector<vostok::math::frustum> >;
class vostok::buffer_vector<vostok::render::vector<vostok::render::culling::aab_rect> >;
class vostok::render::vector<vostok::render::culling::sector_double_query_preventer::frustum_image>;

/* FORWARD REFS */
class const vostok::math::float3[4];
class vostok::math::aabb;
class vostok::math::frustum;
class vostok::render::culling::aab_rect;
class vostok::render::culling::spatial_sector;
class vostok::render::system_renderer;

namespace vostok {
namespace render {
namespace culling {

class sector_double_query_preventer {
public:
			sector_double_query_preventer	( spatial_sector const* sectors, u32 sectors_count );
			~sector_double_query_preventer	( );

	void	clear							( );

	bool	is_possible_points_for_frustum	( const float3[4]& vertices, u32 sector_id ) const;

	void	add_ss_aab_rect					( aab_rect const& rect, u32 sector_id );
	bool	is_possible_ss_aab_rect			( aab_rect const& rect, u32 sector_id ) const;

	void	add_frustum						( math::frustum const& f, u32 sector_id );

	bool	is_aabb_in_sector				( math::aabb const& bbox, u32 sector_id ) const;
	bool	is_visible_aabb					( math::aabb const& bbox, u16 const* sectors_begin, u16 const* sectors_end ) const;

	void	make_frustum_images				( float3 const* furthest_vertices );

	void	render							( system_renderer& r );

	u32		frustums_count					( ) const;

private:
	/* 0x0000 */	void*		m_buffer_for_frustum_vectors;
	/* 0x0004 */	buffer_vector< vector< math::frustum > >*	m_sectors_max_frustums;
	/* 0x0008 */	void*		m_buffer_for_rect_vectors;
	/* 0x000c */	buffer_vector< vector< aab_rect > >*	m_sectors_max_rects;
	/* 0x0010 */	vector< sector_double_query_preventer::frustum_image >	m_frustum_images;
}; // class sector_double_query_preventer

STATIC_SIZE_ASSERT(sector_double_query_preventer, 0x1C);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_H_INCLUDED
