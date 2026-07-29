////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_AAB_RECT_H_INCLUDED
#define RENDER_CULLING_AAB_RECT_H_INCLUDED

namespace vostok {
namespace render {
namespace culling {

class aab_rect {
public:
	inline	aab_rect&		modify		( float3 const& arg_0 ) { /* no source */ }
	inline	aab_rect&		modify		( float2 const& arg_0 ) { /* no source */ }

	inline	bool			intersects	( aab_rect const& arg_0 ) const { /* no source */ }

	inline	bool			contains	( aab_rect const& another ) const { /* no source */ }

private:
	inline					aab_rect	( ) { /* no source */ }

public:
	/* 0x0000 */	float2		min;
	/* 0x0008 */	float2		max;
}; // class aab_rect

STATIC_SIZE_ASSERT(aab_rect, 0x10);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_AAB_RECT_H_INCLUDED
