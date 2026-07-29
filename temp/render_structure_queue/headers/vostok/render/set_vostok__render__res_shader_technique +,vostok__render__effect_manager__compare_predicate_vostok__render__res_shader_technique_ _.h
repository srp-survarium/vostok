////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_shader_technique *,vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique>,vostok::render::std_allocator<vostok::render::res_shader_technique *> >;

/* FORWARD REFS */
class vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique>;

namespace vostok {
namespace render {

class set< res_shader_technique*, effect_manager::compare_predicate< res_shader_technique > > : public std::set< res_shader_technique*, effect_manager::compare_predicate< res_shader_technique >, std_allocator< res_shader_technique* > > {
public:
	inline				set<vostok::render::res_shader_technique *,vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_shader_technique *,vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique> >( effect_manager::compare_predicate< res_shader_technique > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_shader_technique *,vostok::render::effect_manager::compare_predicate<vostok::render::res_shader_technique> >( ) { /* no source */ }
}; // class set< res_shader_technique*, effect_manager::compare_predicate< res_shader_technique > >

STATIC_SIZE_ASSERT(set< res_shader_technique*, effect_manager::compare_predicate< res_shader_technique > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
