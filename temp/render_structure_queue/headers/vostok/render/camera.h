////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CAMERA_H_INCLUDED
#define RENDER_CAMERA_H_INCLUDED

namespace vostok {
namespace render {

class camera {
public:
	inline						camera						( ) { /* no source */ }

	inline	void				set_view_transform			( float4x4 const& matrix ) { /* no source */ }
	inline	void				set_projection_transform	( float4x4 const& arg_0 ) { /* no source */ }
	inline	float4x4 const&		get_view_transform			( ) const { /* no source */ }
	inline	float4x4 const&		get_view_inverted_transform	( ) const { /* no source */ }
	inline	float4x4 const&		get_projection_transform	( ) const { /* no source */ }

private:
	/* 0x0000 */	float4x4	m_view;
	/* 0x0040 */	float4x4	m_view_inverted;
	/* 0x0080 */	float4x4	m_projection;
}; // class camera

STATIC_SIZE_ASSERT(camera, 0xC0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CAMERA_H_INCLUDED
