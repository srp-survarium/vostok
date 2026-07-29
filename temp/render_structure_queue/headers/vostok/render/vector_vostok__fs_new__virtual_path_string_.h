////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::fs_new::virtual_path_string,vostok::render::std_allocator<vostok::fs_new::virtual_path_string> >
	stlp_std::vector<vostok::fs_new::virtual_path_string >;
class vostok::fs_new::virtual_path_string;

namespace vostok {
namespace render {

class vector< fs_new::virtual_path_string > : public std::vector< fs_new::virtual_path_string > {
public:
	inline											vector<vostok::fs_new::virtual_path_string>( ) { /* no source */ }
	inline											vector<vostok::fs_new::virtual_path_string>( u32 arg_0, fs_new::virtual_path_string const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::fs_new::virtual_path_string>( u32 arg_0 ) { /* no source */ }

	inline	fs_new::virtual_path_string&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	fs_new::virtual_path_string const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::fs_new::virtual_path_string>( ) { /* no source */ }
}; // class vector< fs_new::virtual_path_string >

STATIC_SIZE_ASSERT(vector< fs_new::virtual_path_string >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
