////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SAMPLER_SLOT_H_INCLUDED
#define RENDER_SAMPLER_SLOT_H_INCLUDED

/* INCLUDES */
struct ID3D11SamplerState;
class vostok::fixed_string<64>;

namespace vostok {
namespace render {

class sampler_slot {
public:
	inline		sampler_slot( ) { /* no source */ }
	inline		sampler_slot( fixed_string< 64 > const& arg_0, u32 arg_1, ID3D11SamplerState* arg_2 ) { /* no source */ }

	/* 0x0000 */	fixed_string< 64 >		name;
	/* 0x004c */	u32						slot_id;
	/* 0x0050 */	ID3D11SamplerState*		state;
}; // class sampler_slot

STATIC_SIZE_ASSERT(sampler_slot, 0x54);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SAMPLER_SLOT_H_INCLUDED
