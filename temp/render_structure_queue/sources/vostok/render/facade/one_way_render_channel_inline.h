////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\FACADE\ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED
#define VOSTOK\RENDER\FACADE\ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void one_way_render_channel::owner_initialize( )
{
	// FUNCTION BODY[0x12c20]: 4
	// <0>
	// <1>
	// <2>
	// <0x12c25>|0x005|+0x083:'30'
	// ******
}

// STATE[STUB]
inline void one_way_render_channel::owner_finalize( )
{
	// FUNCTION BODY[0x12cb0]: 1
	// <0x12cb1>|0x001|+0x00d:'35'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::one_way_threads_channel< vostok::intrusive_spsc_queue< vostok::sound::sound_order, vostok::sound::sound_order, 4 >, vostok::intrusive_spsc_queue< vostok::sound::sound_order, vostok::sound::sound_order, 4 > >
	// 	orders_channel_type;

	// typedef
	// 	vostok::one_way_threads_channel< vostok::intrusive_spsc_queue< vostok::sound::sound_response, vostok::sound::sound_response, 4 >, vostok::intrusive_spsc_queue< vostok::sound::sound_response, vostok::sound::sound_response, 4 > >
	// 	responses_channel_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\FACADE\ONE_WAY_RENDER_CHANNEL_INLINE_H_INCLUDED
