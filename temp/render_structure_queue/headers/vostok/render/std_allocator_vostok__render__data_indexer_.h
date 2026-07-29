////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::data_indexer;

namespace vostok {
namespace render {

class std_allocator< data_indexer > {
public:
	inline							std_allocator<vostok::render::data_indexer>( ) { /* no source */ }

	inline	data_indexer*			address		( data_indexer& arg_0 ) const { /* no source */ }
	inline	data_indexer const*		address		( data_indexer const& arg_0 ) const { /* no source */ }

	inline	data_indexer*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	data_indexer*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	data_indexer*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	data_indexer*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( data_indexer* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( data_indexer* arg_0, data_indexer const& arg_1 ) { /* no source */ }

	inline	void					destroy		( data_indexer* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< data_indexer >

STATIC_SIZE_ASSERT(std_allocator< data_indexer >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
