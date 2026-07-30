#include "pch.h"
#include "speedtree.h"

namespace vostok {
namespace render {

void* speed_tree_allocator::Alloc( size_t size )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64440]
	return MALLOC( size, "speed_tree_allocator" );
}

void speed_tree_allocator::Free( void* block )
{
	// STATE[STUB]
	// FUNCTION BODY[0x64490]
	if ( block )
		FREE( block );
}

void initialize_speedtree( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a880]
}

void finalize_speedtree( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76aa10]
}

void print_speedtree_errors( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x76a910]
}

} // namespace render
} // namespace vostok
