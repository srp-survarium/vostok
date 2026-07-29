////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_effect *,vostok::render::effect_manager::compare_predicate<vostok::render::res_effect>,vostok::render::std_allocator<vostok::render::res_effect *> >;

/* FORWARD REFS */
class vostok::render::effect_manager::compare_predicate<vostok::render::res_effect>;

namespace vostok {
namespace render {

class set< res_effect*, effect_manager::compare_predicate< res_effect > > : public std::set< res_effect*, effect_manager::compare_predicate< res_effect >, std_allocator< res_effect* > > {
public:
	inline				set<vostok::render::res_effect *,vostok::render::effect_manager::compare_predicate<vostok::render::res_effect> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_effect *,vostok::render::effect_manager::compare_predicate<vostok::render::res_effect> >( effect_manager::compare_predicate< res_effect > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_effect *,vostok::render::effect_manager::compare_predicate<vostok::render::res_effect> >( ) { /* no source */ }
}; // class set< res_effect*, effect_manager::compare_predicate< res_effect > >

STATIC_SIZE_ASSERT(set< res_effect*, effect_manager::compare_predicate< res_effect > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
