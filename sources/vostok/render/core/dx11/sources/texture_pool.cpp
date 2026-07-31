#include "pch.h"
#include "texture_pool.h"

namespace vostok {
namespace render {

texture_pool::~texture_pool( )
{
	// FUNCTION BODY[0x781f60]
	for ( u32 i = 0; i < m_textures.size( ); ++i )
		resource_manager::ref( ).release_impl( m_textures[i].texture );
}

} // namespace render
} // namespace vostok
