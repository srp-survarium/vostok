////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base>,vostok::render::std_allocator<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> > >
	stlp_std::vector<vostok::render::render_model_instance_impl_ptr >;
class vostok::render::render_model_instance_impl;

namespace vostok {
namespace render {

class vector< render_model_instance_impl_ptr > : public std::vector< render_model_instance_impl_ptr > {
public:
	inline										vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
	inline										vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0, render_model_instance_impl_ptr const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0 ) { /* no source */ }

	inline	render_model_instance_impl_ptr&		operator[]	( u32 arg_0 ) { /* no source */ }
	inline	render_model_instance_impl_ptr const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::resources::resource_ptr<vostok::render::render_model_instance_impl,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
}; // class vector< render_model_instance_impl_ptr >

STATIC_SIZE_ASSERT(vector< render_model_instance_impl_ptr >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
