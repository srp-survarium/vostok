////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::speedtree_forest::tree_render_info;

namespace vostok {
namespace render {

class std_allocator< speedtree_forest::tree_render_info > {
public:
	inline											std_allocator<vostok::render::speedtree_forest::tree_render_info>( ) { /* no source */ }

	inline	speedtree_forest::tree_render_info*		address		( speedtree_forest::tree_render_info& arg_0 ) const { /* no source */ }
	inline	speedtree_forest::tree_render_info const*	address		( speedtree_forest::tree_render_info const& arg_0 ) const { /* no source */ }

	inline	speedtree_forest::tree_render_info*		allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	speedtree_forest::tree_render_info*		allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	speedtree_forest::tree_render_info*		_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	speedtree_forest::tree_render_info*		_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( speedtree_forest::tree_render_info* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	(
														speedtree_forest::tree_render_info*		arg_0,
														speedtree_forest::tree_render_info const&	arg_1
													) { /* no source */ }

	inline	void									destroy		( speedtree_forest::tree_render_info* arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< speedtree_forest::tree_render_info >

STATIC_SIZE_ASSERT(std_allocator< speedtree_forest::tree_render_info >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
