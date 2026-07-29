////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;

namespace vostok {
namespace render {

class std_allocator< fs_new::virtual_path_string > {
public:
	inline											std_allocator<vostok::fs_new::virtual_path_string>( ) { /* no source */ }

	inline	fs_new::virtual_path_string*			address		( fs_new::virtual_path_string& arg_0 ) const { /* no source */ }
	inline	fs_new::virtual_path_string const*		address		( fs_new::virtual_path_string const& arg_0 ) const { /* no source */ }

	inline	fs_new::virtual_path_string*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fs_new::virtual_path_string*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	fs_new::virtual_path_string*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	fs_new::virtual_path_string*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( fs_new::virtual_path_string* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	( fs_new::virtual_path_string* arg_0, fs_new::virtual_path_string const& arg_1 ) { /* no source */ }

	inline	void									destroy		( fs_new::virtual_path_string* arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< fs_new::virtual_path_string >

STATIC_SIZE_ASSERT(std_allocator< fs_new::virtual_path_string >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
