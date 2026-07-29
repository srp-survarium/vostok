////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SIGNATURE_LAYOUT_PAIR_H_INCLUDED
#define RENDER_SIGNATURE_LAYOUT_PAIR_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::res_input_layout,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_input_layout_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_signature const ,vostok::render::res_signature const ,vostok::threading::single_threading_policy>
	vostok::render::res_signature const _ptr;
class vostok::render::res_input_layout;
class vostok::render::res_signature const ;

/* FORWARD REFS */
class vostok::render::res_declaration;
class vostok::render::res_signature;

namespace vostok {
namespace render {

struct signature_layout_pair {
					signature_layout_pair	( res_declaration const* decl, res_signature const* signature );

	inline	bool	operator<				( res_signature const* arg_0 ) const { /* no source */ }

	inline	bool	operator==				( res_signature const* arg_0 ) const { /* no source */ }

	inline			~signature_layout_pair	( ) { /* no source */ }

	/* 0x0000 */	res_input_layout_ptr		input_layout;
	/* 0x0004 */	res_signature const _ptr	signature;
}; // struct signature_layout_pair

STATIC_SIZE_ASSERT(signature_layout_pair, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SIGNATURE_LAYOUT_PAIR_H_INCLUDED
