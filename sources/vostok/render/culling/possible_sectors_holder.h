#ifndef VOSTOK_RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
#define VOSTOK_RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED

// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (possible_sectors_holder is new-in-target) - matcher-phase work.
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
		u32 const count = config.size( );
		m_buffer = ALLOC( u16, count );
		m_buffer_end = m_buffer + count;

		u16* destination = m_buffer;
		configs::binary_config_value const* it = config.begin( );
		configs::binary_config_value const* it_e = config.end( );
		for ( ; it != it_e; ++it, ++destination )
			*destination = (u16)*it;
	}

	~possible_sectors_holder( )
	{
		// STATE[STUB]
	}

	bool is_possible_sector( u16 sector_id ) const
	{
		// STATE[STUB]
		(void)sector_id;
		return false;
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

#endif // #ifndef VOSTOK_RENDER_CULLING_POSSIBLE_SECTORS_HOLDER_H_INCLUDED
