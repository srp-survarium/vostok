////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEED_TREE_ALLOCATOR_H_INCLUDED
#define RENDER_SPEED_TREE_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class SpeedTree::CAllocator;

namespace vostok {
namespace render {

struct speed_tree_allocator : public SpeedTree::CAllocator {
	virtual	void*		Alloc					( u32 size ) override;

	virtual	void		Free					( void* block ) override;

	inline				speed_tree_allocator	( ) { /* no source */ }
	virtual				~speed_tree_allocator	( ) { /* no source */ }
}; // struct speed_tree_allocator

STATIC_SIZE_ASSERT(speed_tree_allocator, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEED_TREE_ALLOCATOR_H_INCLUDED
