////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_URO_ANISOTROPHIC_FILTER_VALUES_H_INCLUDED
#define RENDER_ENUM_URO_ANISOTROPHIC_FILTER_VALUES_H_INCLUDED

namespace vostok {
namespace render {

enum enum_uro_anisotrophic_filter_values
{
	uro_anisotrophic_filter_off			= 0x0,
	uro_anisotrophic_filter_2x			= 0x1,
	uro_anisotrophic_filter_4x			= 0x2,
	uro_anisotrophic_filter_8x			= 0x3,
	uro_anisotrophic_filter_16x			= 0x4,
	uro_anisotrophic_filter_count		= 0x5,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_URO_ANISOTROPHIC_FILTER_VALUES_H_INCLUDED
