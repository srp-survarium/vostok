////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MAP_H_INCLUDED
#define RENDER_MAP_H_INCLUDED

/* INCLUDES */
class stlp_std::map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair>,vostok::render::std_allocator<stlp_std::pair<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *> > >;

/* FORWARD REFS */
class stlp_std::less<vostok::render::resource_manager::shader_name_config_pair>;
class vostok::render::map<vostok::render::resource_manager::shader_name_config_pair,stlp_std::pair<vostok::render::resource_manager::shader_name_config_pair const ,vostok::render::res_xs_hw<vostok::render::ps_data> *>,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >;

namespace vostok {
namespace render {

class map< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >*, std::less< resource_manager::shader_name_config_pair > > : public std::map< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >*, std::less< resource_manager::shader_name_config_pair >, std_allocator< std::pair< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >* > > > {
public:
	inline				map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >( ) { /* no source */ }
	inline	explicit	map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >( std::less< resource_manager::shader_name_config_pair > const& arg_0 ) { /* no source */ }
	inline	explicit	map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >(
							map< resource_manager::shader_name_config_pair, std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* >, std::less< resource_manager::shader_name_config_pair > > const&	arg_0
						) { /* no source */ }

	inline	map< resource_manager::shader_name_config_pair, std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* >, std::less< resource_manager::shader_name_config_pair > >&	operator=	(
							map< resource_manager::shader_name_config_pair, std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< ps_data >* >, std::less< resource_manager::shader_name_config_pair > > const&	arg_0
						) { /* no source */ }

	inline				~map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >( ) { /* no source */ }
}; // class map< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >*, std::less< resource_manager::shader_name_config_pair > >

STATIC_SIZE_ASSERT(map< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >*, std::less< resource_manager::shader_name_config_pair > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MAP_H_INCLUDED
