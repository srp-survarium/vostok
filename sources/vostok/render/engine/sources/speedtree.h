#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_H_INCLUDED

#include <speedtree/core/memory.h>

namespace vostok {
namespace render {

struct speed_tree_allocator : public SpeedTree::CAllocator {
	virtual void* Alloc( size_t size );
	virtual void Free( void* block );
};

STATIC_SIZE_ASSERT( speed_tree_allocator, 0x4 );

void initialize_speedtree( );
void finalize_speedtree( );
void print_speedtree_errors( );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_H_INCLUDED
