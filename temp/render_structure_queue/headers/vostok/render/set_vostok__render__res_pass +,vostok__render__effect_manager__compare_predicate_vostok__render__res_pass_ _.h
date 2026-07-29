////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_pass *,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass>,vostok::render::std_allocator<vostok::render::res_pass *> >;

/* FORWARD REFS */
class vostok::render::effect_manager::compare_predicate<vostok::render::res_pass>;

namespace vostok {
namespace render {

class set< res_pass*, effect_manager::compare_predicate< res_pass > > : public std::set< res_pass*, effect_manager::compare_predicate< res_pass >, std_allocator< res_pass* > > {
public:
	inline				set<vostok::render::res_pass *,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_pass *,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass> >( effect_manager::compare_predicate< res_pass > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_pass *,vostok::render::effect_manager::compare_predicate<vostok::render::res_pass> >( ) { /* no source */ }
}; // class set< res_pass*, effect_manager::compare_predicate< res_pass > >

STATIC_SIZE_ASSERT(set< res_pass*, effect_manager::compare_predicate< res_pass > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
