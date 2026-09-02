// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_STATIC_TYPE_H_INCLUDED
#define VOSTOK_RENDER_CORE_STATIC_TYPE_H_INCLUDED
namespace vostok {
namespace render {

struct static_type {
	static u16 type_id_counter;

	template < typename T >
	static u16 get_type_id( )
	{
		// Observed instantiations: pcstr [0x636e0], bool [0x12bd90],
		// and u16 [0x67fb0].
		static u16 current_id = ++type_id_counter;
		return current_id;
	}
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_STATIC_TYPE_H_INCLUDED
