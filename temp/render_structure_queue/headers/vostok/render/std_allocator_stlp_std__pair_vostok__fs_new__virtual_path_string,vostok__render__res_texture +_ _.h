////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<vostok::fs_new::virtual_path_string,vostok::render::res_texture *>;

namespace vostok {
namespace render {

class std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > > {
public:
	inline				std_allocator<stlp_std::pair<vostok::fs_new::virtual_path_string,vostok::render::res_texture *> >( ) { /* no source */ }

	inline	std::pair< fs_new::virtual_path_string, res_texture* >*	address		( std::pair< fs_new::virtual_path_string, res_texture* >& arg_0 ) const { /* no source */ }
	inline	std::pair< fs_new::virtual_path_string, res_texture* > const*	address		( std::pair< fs_new::virtual_path_string, res_texture* > const& arg_0 ) const { /* no source */ }

	inline	std::pair< fs_new::virtual_path_string, res_texture* >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< fs_new::virtual_path_string, res_texture* >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< fs_new::virtual_path_string, res_texture* >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< fs_new::virtual_path_string, res_texture* >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::pair< fs_new::virtual_path_string, res_texture* >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::pair< fs_new::virtual_path_string, res_texture* >*	arg_0,
							std::pair< fs_new::virtual_path_string, res_texture* > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::pair< fs_new::virtual_path_string, res_texture* >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
