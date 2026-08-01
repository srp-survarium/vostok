#include "pch.h"

#include <vostok/render/core/dx11/effect_constant_storage.h>

namespace vostok {
namespace render {

bool constant_data_predicate( data_indexer const& left, data_indexer const& right )
{
	return left.class_id < right.class_id;
}

effect_constant_storage::effect_constant_storage( ) : m_constant_buffer( 0 )
{
	// FUNCTION BODY[0x55f480]
}

effect_constant_storage::~effect_constant_storage( )
{
	// FUNCTION BODY[0x55f450]
	clear( );
}

bool effect_constant_storage::is_equal(
	u32 const* left,
	u32 const* right,
	u32 count
)
{
	// FUNCTION BODY[0x55f3d0]
	for ( u32 i = 0; i < count; ++i )
		if ( left[i] != right[i] )
			return false;
	return true;
}

void effect_constant_storage::clear( )
{
	// FUNCTION BODY[0x55f410]
	while ( m_constant_buffer ) {
		fixed_constants_data_buffer* current = m_constant_buffer;
		m_constant_buffer = m_constant_buffer->next;
		DELETE( current );
	}
	m_indexers.clear( );
}

} // namespace render
} // namespace vostok
