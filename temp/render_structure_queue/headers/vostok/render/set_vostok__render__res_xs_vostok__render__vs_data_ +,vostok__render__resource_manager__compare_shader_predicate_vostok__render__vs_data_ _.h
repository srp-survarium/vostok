////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_xs<vostok::render::vs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data>,vostok::render::std_allocator<vostok::render::res_xs<vostok::render::vs_data> *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data>;

namespace vostok {
namespace render {

class set< res_xs< vs_data >*, resource_manager::compare_shader_predicate< vs_data > > : public std::set< res_xs< vs_data >*, resource_manager::compare_shader_predicate< vs_data >, std_allocator< res_xs< vs_data >* > > {
public:
	inline				set<vostok::render::res_xs<vostok::render::vs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_xs<vostok::render::vs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data> >( resource_manager::compare_shader_predicate< vs_data > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_xs<vostok::render::vs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data> >( ) { /* no source */ }
}; // class set< res_xs< vs_data >*, resource_manager::compare_shader_predicate< vs_data > >

STATIC_SIZE_ASSERT(set< res_xs< vs_data >*, resource_manager::compare_shader_predicate< vs_data > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
