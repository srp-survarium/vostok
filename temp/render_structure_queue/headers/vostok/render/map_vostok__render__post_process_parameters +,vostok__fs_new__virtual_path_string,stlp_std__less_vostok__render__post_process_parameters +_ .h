////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *>,vostok::render::std_allocator<stlp_std::pair<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::render::post_process_parameters *>;
class vostok::render::map<vostok::render::post_process_parameters *,stlp_std::pair<vostok::render::post_process_parameters * const,vostok::fs_new::virtual_path_string>,stlp_std::less<vostok::render::post_process_parameters *> >;

namespace vostok {
namespace render {

class map< post_process_parameters*, fs_new::virtual_path_string, std::less< post_process_parameters* > > : public std::map< post_process_parameters*, fs_new::virtual_path_string, std::less< post_process_parameters* >, std_allocator< std::pair< post_process_parameters*, fs_new::virtual_path_string > > > {
public:
	inline				map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *> >( ) { /* no source */ }
	inline	explicit	map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *> >( std::less< post_process_parameters* > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *> >(
							map< post_process_parameters*, std::pair< post_process_parameters* const, fs_new::virtual_path_string >, std::less< post_process_parameters* > > const&	arg_0
						) { /* no source */ }

	inline	map< post_process_parameters*, std::pair< post_process_parameters* const, fs_new::virtual_path_string >, std::less< post_process_parameters* > >&	operator=	(
							map< post_process_parameters*, std::pair< post_process_parameters* const, fs_new::virtual_path_string >, std::less< post_process_parameters* > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::render::post_process_parameters *,vostok::fs_new::virtual_path_string,stlp_std::less<vostok::render::post_process_parameters *> >( ) { /* no source */ }
}; // class map< post_process_parameters*, fs_new::virtual_path_string, std::less< post_process_parameters* > >

STATIC_SIZE_ASSERT(map< post_process_parameters*, fs_new::virtual_path_string, std::less< post_process_parameters* > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
