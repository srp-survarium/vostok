////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_URO_ANTIALIASING_METHOD_VALUES_H_INCLUDED
#define RENDER_ENUM_URO_ANTIALIASING_METHOD_VALUES_H_INCLUDED

namespace vostok {
namespace render {

enum enum_uro_antialiasing_method_values
{
	uro_antialiasing_method_off			= 0x0,
	uro_antialiasing_method_FXAA		= 0x1,
	uro_antialiasing_method_2xTAA		= 0x2,
	uro_antialiasing_method_count		= 0x3,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_URO_ANTIALIASING_METHOD_VALUES_H_INCLUDED
