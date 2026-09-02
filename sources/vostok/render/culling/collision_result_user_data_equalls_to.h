// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED
#define VOSTOK_RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED
#include <vostok/collision/object.h>

namespace vostok {
namespace render {
namespace culling {

struct collision_result_user_data_equalls_to :
	public std::unary_function< collision::triangle_result const&, bool >
{
	explicit collision_result_user_data_equalls_to( u32 user_data )
	{
		m_user_data = user_data;
	}

	bool operator()( collision::triangle_result const& result ) const
	{
		return reinterpret_cast<u32>( result.object->user_data( ) ) == m_user_data;
	}

private:
	u32 m_user_data;
};

STATIC_SIZE_ASSERT( collision_result_user_data_equalls_to, 0x4 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CULLING_COLLISION_RESULT_USER_DATA_EQUALLS_TO_H_INCLUDED
