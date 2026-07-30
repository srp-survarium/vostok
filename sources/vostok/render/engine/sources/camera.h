#ifndef VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED

#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

class camera {
public:
	camera( )
	{
		m_view.identity( );
		m_view_inverted.identity( );
		m_projection.identity( );
	}

	void set_view_transform( float4x4 const& matrix )
	{
		m_view = matrix;
		m_view_inverted = invert4x3( m_view );
	}

	void set_projection_transform( float4x4 const& matrix )
	{
		m_projection = matrix;
	}

	float4x4 const& get_view_transform( ) const
	{
		return m_view;
	}

	float4x4 const& get_view_inverted_transform( ) const
	{
		return m_view_inverted;
	}

	float4x4 const& get_projection_transform( ) const
	{
		return m_projection;
	}

private:
	float4x4	m_view;
	float4x4	m_view_inverted;
	float4x4	m_projection;
};

STATIC_SIZE_ASSERT( camera, 0xc0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED
