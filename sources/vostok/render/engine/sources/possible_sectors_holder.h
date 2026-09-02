// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
#include <vostok/configs_binary_config_value.h>
#include <vostok/detail_noncopyable.h>

namespace vostok {
namespace render {
namespace culling {

class possible_sectors_holder : private core::noncopyable {
public:
	explicit possible_sectors_holder( configs::binary_config_value config ) :
		m_buffer		( 0 ),
		m_buffer_end	( 0 )
	{
		u32 index = 0;
		u32 const count = config.size( );
		m_buffer = ALLOC( u16, count );
		m_buffer_end = m_buffer + count;
		configs::binary_config_value const* it = config.begin( );
		configs::binary_config_value const* it_e = config.end( );
		for ( ; it != it_e; ++it, ++index )
		{
			m_buffer[index] = (u16)*it;
		}
	}

	~possible_sectors_holder( )
	{
		FREE( m_buffer );
	}

	bool is_possible_sector( u16 sector_id ) const
	{
		return std::find( m_buffer, m_buffer_end, sector_id ) != m_buffer_end;
	}

	u16 const* begin( ) const
	{
		return m_buffer;
	}

	u16 const* end( ) const
	{
		return m_buffer_end;
	}

private:
	u16* m_buffer;
	u16* m_buffer_end;
};

STATIC_SIZE_ASSERT( possible_sectors_holder, 0x8 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
