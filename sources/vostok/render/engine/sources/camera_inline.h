#ifndef VOSTOK_RENDER_ENGINE_CAMERA_INLINE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CAMERA_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline camera::camera( )
{
	m_view.identity			( );
	m_view_inverted.identity( );
	m_projection.identity	( );
}

inline void camera::set_view_transform( float4x4 const& matrix )
{
	m_view			= matrix;
	m_view_inverted	= invert4x3( m_view );
}

inline void camera::set_projection_transform( float4x4 const& matrix )
{
	m_projection	= matrix;
}

inline float4x4 const& camera::get_view_transform( ) const
{
	return m_view;
}

inline float4x4 const& camera::get_view_inverted_transform( ) const
{
	return m_view_inverted;
}

inline float4x4 const& camera::get_projection_transform( ) const
{
	return m_projection;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CAMERA_INLINE_H_INCLUDED
