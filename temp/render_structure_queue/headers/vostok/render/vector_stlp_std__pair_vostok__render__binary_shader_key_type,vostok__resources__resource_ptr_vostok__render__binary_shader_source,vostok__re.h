////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> >,vostok::render::std_allocator<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> > > >
	stlp_std::vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::render::binary_shader_source_ptr > >;
class vostok::render::binary_shader_source;

namespace vostok {
namespace render {

class vector< std::pair< binary_shader_key_type, binary_shader_source_ptr > > : public std::vector< std::pair< binary_shader_key_type, binary_shader_source_ptr > > {
public:
	inline				vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> > >( ) { /* no source */ }
	inline				vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> > >(
							u32		arg_0,
							std::pair< binary_shader_key_type, binary_shader_source_ptr > const&	arg_1
						) { /* no source */ }
	inline	explicit	vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> > >( u32 arg_0 ) { /* no source */ }

	inline	std::pair< binary_shader_key_type, binary_shader_source_ptr >&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	std::pair< binary_shader_key_type, binary_shader_source_ptr > const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<stlp_std::pair<vostok::render::binary_shader_key_type,vostok::resources::resource_ptr<vostok::render::binary_shader_source,vostok::resources::unmanaged_intrusive_base> > >( ) { /* no source */ }
}; // class vector< std::pair< binary_shader_key_type, binary_shader_source_ptr > >

STATIC_SIZE_ASSERT(vector< std::pair< binary_shader_key_type, binary_shader_source_ptr > >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
