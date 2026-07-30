#ifndef VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED

namespace vostok {
namespace render {

inline float frac( float value )
{
	// FUNCTION BODY[0x611700]
	// FUNCTION BODY[0x60d450]
	// FUNCTION BODY[0x5f2af0]
	// FUNCTION BODY[0x63b300]
	return value - static_cast< unsigned int >( value );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_HELP_MATH_H_INCLUDED
