// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/vertex_buffer.h>
#include <vostok/render/core/resource_manager.h>
namespace vostok {
namespace render {

int	rs_dvb_size = 512+1024;

vertex_buffer::vertex_buffer		( u32 const size ) :
	m_size			( size ),
	m_position		( 0 ),
	m_discard_id	( 0 ),
	m_lock_count	( 0 ),
	m_lock_stride	( 0 )
{
	m_buffer		= resource_manager::ref().create_buffer( m_size, 0, enum_buffer_type_vertex, true, false );
	R_ASSERT		( m_buffer );
	LOG_INFO		(" vertex buffer created: %dKb", m_size/1024);
}

// This need to be reviewed
void* vertex_buffer::lock(u32 v_count, u32 v_stride, u32& v_offset)
{
	m_lock_count	= v_count;
	m_lock_stride	= v_stride;

	// Ensure there is enough space in the VB for this data
	u32	bytes_need	= v_count * v_stride;
	R_ASSERT_U		( bytes_need <= m_size && v_count);

	// Vertex-local info
	u32 vl_size     = m_size/v_stride;
	u32 vl_position	= m_position/v_stride + 1;

	// Check if there is need to flush and perform lock
	pbyte p_data	= 0;
	if (v_count+vl_position >= vl_size)
	{
		// FLUSH-LOCK
		m_position	= 0;
		v_offset	= 0;
		m_discard_id++;

		p_data		= static_cast<pbyte>( m_buffer->map( D3D_MAP_WRITE_DISCARD) );
		p_data		+= m_position;
	}
	else
	{
		// APPEND-LOCK
		m_position	= vl_position*v_stride;
		v_offset	= vl_position;

		p_data		= static_cast<pbyte>( m_buffer->map( D3D_MAP_WRITE_NO_OVERWRITE) );
		p_data		+= m_position;
	}

	ASSERT			( p_data );
	return			p_data;
}

void vertex_buffer::unlock()
{
	m_position += m_lock_count*m_lock_stride;

	ASSERT(m_buffer);

	m_buffer->unmap();
}

// void	vertex_buffer::reset_begin	()
// {
// 	old_vb = m_buffer;
// 	destroy();
// }
// void	vertex_buffer::reset_end	()
// {
// 	create				();
// 	//old_pVB				= NULL;
// }

// void vertex_buffer::clear()
// {
//     m_buffer = NULL;
//     m_size = 0;
//     m_position = 0;
//     m_discard_id = 0;
// #ifdef DEBUG
// 	dbg_lock = 0;
// #endif
// }

} // namespace render
} // namespace vostok
