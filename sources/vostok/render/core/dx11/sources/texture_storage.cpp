#include "pch.h"
#include "texture_storage.h"
#include "texture_pool.h"

namespace vostok {
namespace render {

texture_storage::texture_storage( )
	: m_initialized( false )
{
	// FUNCTION BODY[0x782050]
}

texture_storage::~texture_storage( )
{
	// FUNCTION BODY[0x781fe0]
	map<u32, texture_pool*, std::less<u32> >::iterator			it	= m_pools.begin();
	map<u32, texture_pool*, std::less<u32> >::const_iterator	end	= m_pools.end();

	for( ; it != end; ++it)
		DELETE( it->second);
}

} // namespace render
} // namespace vostok
