////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_signature *,vostok::render::resource_manager::compare_predicate<vostok::render::res_signature>,vostok::render::std_allocator<vostok::render::res_signature *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_predicate<vostok::render::res_signature>;

namespace vostok {
namespace render {

class set< res_signature*, resource_manager::compare_predicate< res_signature > > : public std::set< res_signature*, resource_manager::compare_predicate< res_signature >, std_allocator< res_signature* > > {
public:
	inline				set<vostok::render::res_signature *,vostok::render::resource_manager::compare_predicate<vostok::render::res_signature> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_signature *,vostok::render::resource_manager::compare_predicate<vostok::render::res_signature> >( resource_manager::compare_predicate< res_signature > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_signature *,vostok::render::resource_manager::compare_predicate<vostok::render::res_signature> >( ) { /* no source */ }
}; // class set< res_signature*, resource_manager::compare_predicate< res_signature > >

STATIC_SIZE_ASSERT(set< res_signature*, resource_manager::compare_predicate< res_signature > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
