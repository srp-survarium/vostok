////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::render_model_instance_impl *,vostok::render::std_allocator<vostok::render::render_model_instance_impl *> >
	stlp_std::vector<vostok::render::render_model_instance_impl * >;
class vostok::render::render_model_instance_impl *;

/* FORWARD REFS */
class vostok::render::render_model_instance_impl;
class vostok::render::render_model_instance_impl*;

namespace vostok {
namespace render {

class vector< render_model_instance_impl* > : public std::vector< render_model_instance_impl* > {
public:
	inline											vector<vostok::render::render_model_instance_impl *>( ) { /* no source */ }
	inline											vector<vostok::render::render_model_instance_impl *>( u32 arg_0, render_model_instance_impl* const& arg_1 ) { /* no source */ }
	inline	explicit								vector<vostok::render::render_model_instance_impl *>( u32 arg_0 ) { /* no source */ }

	inline	render_model_instance_impl*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	render_model_instance_impl* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline											~vector<vostok::render::render_model_instance_impl *>( ) { /* no source */ }
}; // class vector< render_model_instance_impl* >

STATIC_SIZE_ASSERT(vector< render_model_instance_impl* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
