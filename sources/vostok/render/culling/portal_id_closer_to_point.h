#ifndef VOSTOK_RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED
#define VOSTOK_RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED

namespace vostok {
namespace render {
namespace culling {

struct portal_id_closer_to_point : public std::binary_function< u32, u32, bool > {
	explicit portal_id_closer_to_point( float const* distances )
	{
		m_distances = distances;
	}

	bool operator()( u32 left, u32 right ) const
	{
		return m_distances[left] < m_distances[right];
	}

private:
	float const* m_distances;
};

STATIC_SIZE_ASSERT( portal_id_closer_to_point, 0x4 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_CULLING_PORTAL_ID_CLOSER_TO_POINT_H_INCLUDED
