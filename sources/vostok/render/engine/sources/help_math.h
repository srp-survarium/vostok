// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED
namespace vostok {
namespace render {

static inline float frac( float f )
{
	return		( math::abs( f ) - math::abs( static_cast< int >( f ) ) );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED
