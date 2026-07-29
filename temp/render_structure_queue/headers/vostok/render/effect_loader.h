////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_LOADER_H_INCLUDED
#define RENDER_EFFECT_LOADER_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::resources::queries_result;

namespace vostok {
namespace render {

struct effect_loader {
	inline	explicit	effect_loader	( res_effect_ptr* arg_0 ) { /* no source */ }

			void		on_effect_ready	( resources::queries_result& data );

	/* 0x0000 */	res_effect_ptr*		effect_ptr;
	/* 0x0004 */	bool				query_rejected;
}; // struct effect_loader

STATIC_SIZE_ASSERT(effect_loader, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_LOADER_H_INCLUDED
