////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_SLOT_H_INCLUDED
#define RENDER_TEXTURE_SLOT_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<64>;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
class vostok::render::res_texture;

namespace vostok {
namespace render {

class texture_slot {
public:
	inline		texture_slot	( ) { /* no source */ }
	inline		~texture_slot	( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 64 >		name;
	/* 0x004c */	u32						slot_id;
	/* 0x0050 */	res_texture_ptr			texture;
}; // class texture_slot

STATIC_SIZE_ASSERT(texture_slot, 0x54);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_SLOT_H_INCLUDED
