////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_declaration *,vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration>,vostok::render::std_allocator<vostok::render::res_declaration *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration>;

namespace vostok {
namespace render {

class set< res_declaration*, resource_manager::compare_predicate< res_declaration > > : public std::set< res_declaration*, resource_manager::compare_predicate< res_declaration >, std_allocator< res_declaration* > > {
public:
	inline				set<vostok::render::res_declaration *,vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_declaration *,vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration> >( resource_manager::compare_predicate< res_declaration > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_declaration *,vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration> >( ) { /* no source */ }
}; // class set< res_declaration*, resource_manager::compare_predicate< res_declaration > >

STATIC_SIZE_ASSERT(set< res_declaration*, resource_manager::compare_predicate< res_declaration > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
