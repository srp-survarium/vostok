////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_URO_AMBIENT_OCCLUSION_QUALITY_VALUES_H_INCLUDED
#define RENDER_ENUM_URO_AMBIENT_OCCLUSION_QUALITY_VALUES_H_INCLUDED

namespace vostok {
namespace render {

enum enum_uro_ambient_occlusion_quality_values
{
	uro_ambient_occlusion_quality_off		= 0x0,
	uro_ambient_occlusion_quality_ssao		= 0x1,
	uro_ambient_occlusion_quality_hbao		= 0x2,
	uro_ambient_occlusion_quality_ssdo		= 0x3,
	uro_ambient_occlusion_quality_count		= 0x4,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_URO_AMBIENT_OCCLUSION_QUALITY_VALUES_H_INCLUDED
