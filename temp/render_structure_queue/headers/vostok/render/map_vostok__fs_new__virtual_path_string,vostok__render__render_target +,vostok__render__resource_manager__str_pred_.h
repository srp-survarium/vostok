////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred,vostok::render::std_allocator<stlp_std::pair<vostok::fs_new::virtual_path_string,vostok::render::render_target *> > >;

/* FORWARD REFS */
class vostok::render::map<vostok::fs_new::virtual_path_string,stlp_std::pair<vostok::fs_new::virtual_path_string const ,vostok::render::render_target *>,vostok::render::resource_manager::str_pred>;
class vostok::render::resource_manager::str_pred;

namespace vostok {
namespace render {

class map< fs_new::virtual_path_string, render_target*, resource_manager::str_pred > : public std::map< fs_new::virtual_path_string, render_target*, resource_manager::str_pred, std_allocator< std::pair< fs_new::virtual_path_string, render_target* > > > {
public:
	inline				map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred>( ) { /* no source */ }
	inline	explicit	map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred>( resource_manager::str_pred const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred>(
							map< fs_new::virtual_path_string, std::pair< fs_new::virtual_path_string const , render_target* >, resource_manager::str_pred > const&	arg_0
						) { /* no source */ }

	inline	map< fs_new::virtual_path_string, std::pair< fs_new::virtual_path_string const , render_target* >, resource_manager::str_pred >&	operator=	(
							map< fs_new::virtual_path_string, std::pair< fs_new::virtual_path_string const , render_target* >, resource_manager::str_pred > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred>( ) { /* no source */ }
}; // class map< fs_new::virtual_path_string, render_target*, resource_manager::str_pred >

STATIC_SIZE_ASSERT(map< fs_new::virtual_path_string, render_target*, resource_manager::str_pred >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
