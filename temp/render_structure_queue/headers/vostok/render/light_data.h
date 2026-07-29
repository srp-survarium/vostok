////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHT_DATA_H_INCLUDED
#define RENDER_LIGHT_DATA_H_INCLUDED

/* INCLUDES */
typedef vostok::intrusive_ptr<vostok::render::light,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::light_ptr;
class vostok::render::light;

namespace vostok {
namespace render {

struct light_data {
	inline	explicit	light_data	( const u32 arg_0 ) { /* no source */ }

	inline	bool		operator<	( light_data const& arg_0 ) const { /* no source */ }

	inline	bool		operator==	( const u32 arg_0 ) const { /* no source */ }

	inline				~light_data	( ) { /* no source */ }

	/* 0x0000 */	light_ptr		light;
	/* 0x0004 */	u32				id;
}; // struct light_data

STATIC_SIZE_ASSERT(light_data, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHT_DATA_H_INCLUDED
