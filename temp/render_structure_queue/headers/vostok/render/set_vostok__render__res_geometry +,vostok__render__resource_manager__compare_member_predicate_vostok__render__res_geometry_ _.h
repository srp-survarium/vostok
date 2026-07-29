////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_geometry *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry>,vostok::render::std_allocator<vostok::render::res_geometry *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry>;

namespace vostok {
namespace render {

class set< res_geometry*, resource_manager::compare_member_predicate< res_geometry > > : public std::set< res_geometry*, resource_manager::compare_member_predicate< res_geometry >, std_allocator< res_geometry* > > {
public:
	inline				set<vostok::render::res_geometry *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_geometry *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry> >( resource_manager::compare_member_predicate< res_geometry > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_geometry *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry> >( ) { /* no source */ }
}; // class set< res_geometry*, resource_manager::compare_member_predicate< res_geometry > >

STATIC_SIZE_ASSERT(set< res_geometry*, resource_manager::compare_member_predicate< res_geometry > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
