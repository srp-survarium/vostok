////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LOAD_TEXTURE_DELEGATE_H_INCLUDED
#define RENDER_LOAD_TEXTURE_DELEGATE_H_INCLUDED

/* INCLUDES */
class vostok::math::rectangle<vostok::math::int2>;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::resources::queries_result;

namespace vostok {
namespace render {

struct load_texture_delegate {
	inline			load_texture_delegate	(
						res_texture_ptr						arg_0,
						math::rectangle< math::int2 >		arg_1,
						u32									arg_2,
						pcstr								arg_3
					) { /* no source */ }

	inline	void	execute					( resources::queries_result& arg_0 ) { /* no source */ }

	inline			~load_texture_delegate	( ) { /* no source */ }

private:
	/* 0x0000 */	math::rectangle< math::int2 >		dest_rect;
	/* 0x0010 */	res_texture_ptr						dest_texture;
	/* 0x0014 */	pcstr								user_name;
	/* 0x0018 */	u32									arr_ind;
}; // struct load_texture_delegate

STATIC_SIZE_ASSERT(load_texture_delegate, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LOAD_TEXTURE_DELEGATE_H_INCLUDED
