////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class vostok::render::speedtree_tree_base;

namespace vostok {
namespace render {

class std_allocator< speedtree_tree_base_ptr > {
public:
	inline										std_allocator<vostok::resources::resource_ptr<vostok::render::speedtree_tree_base,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }

	inline	speedtree_tree_base_ptr*			address		( speedtree_tree_base_ptr& arg_0 ) const { /* no source */ }
	inline	speedtree_tree_base_ptr const*		address		( speedtree_tree_base_ptr const& arg_0 ) const { /* no source */ }

	inline	speedtree_tree_base_ptr*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	speedtree_tree_base_ptr*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	speedtree_tree_base_ptr*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	speedtree_tree_base_ptr*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( speedtree_tree_base_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( speedtree_tree_base_ptr* arg_0, speedtree_tree_base_ptr const& arg_1 ) { /* no source */ }

	inline	void								destroy		( speedtree_tree_base_ptr* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< speedtree_tree_base_ptr >

STATIC_SIZE_ASSERT(std_allocator< speedtree_tree_base_ptr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
