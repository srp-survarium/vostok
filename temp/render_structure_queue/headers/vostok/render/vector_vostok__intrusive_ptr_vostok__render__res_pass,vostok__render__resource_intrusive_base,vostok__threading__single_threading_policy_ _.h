////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>,vostok::render::std_allocator<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> > >
	stlp_std::vector<vostok::render::res_pass_ptr >;
class vostok::render::res_pass;

namespace vostok {
namespace render {

class vector< res_pass_ptr > : public std::vector< res_pass_ptr > {
public:
	inline							vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( ) { /* no source */ }
	inline							vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( u32 arg_0, res_pass_ptr const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( u32 arg_0 ) { /* no source */ }

	inline	res_pass_ptr&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	res_pass_ptr const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::intrusive_ptr<vostok::render::res_pass,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( ) { /* no source */ }
}; // class vector< res_pass_ptr >

STATIC_SIZE_ASSERT(vector< res_pass_ptr >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
