#ifndef VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED

#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

class camera {
public:
	camera( );

	void set_view_transform( float4x4 const& matrix );
	void set_projection_transform( float4x4 const& matrix );

	float4x4 const& get_view_transform( ) const;
	float4x4 const& get_view_inverted_transform( ) const;
	float4x4 const& get_projection_transform( ) const;

private:
	float4x4	m_view;
	float4x4	m_view_inverted;
	float4x4	m_projection;
};

STATIC_SIZE_ASSERT( camera, 0xc0 );

} // namespace render
} // namespace vostok

#include <vostok/render/engine/sources/camera_inline.h>

#endif // #ifndef VOSTOK_RENDER_ENGINE_CAMERA_H_INCLUDED
