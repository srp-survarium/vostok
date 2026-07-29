////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TIME_PARAMETERS_H_INCLUDED
#define RENDER_TIME_PARAMETERS_H_INCLUDED

namespace vostok {
namespace render {

struct time_parameters {

	/* 0x0000 */	s32		year;
	/* 0x0004 */	s32		month;
	/* 0x0008 */	s32		day;
	/* 0x000c */	s32		hours;
	/* 0x0010 */	s32		minutes;
	/* 0x0014 */	s32		seconds;
}; // struct time_parameters

STATIC_SIZE_ASSERT(time_parameters, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TIME_PARAMETERS_H_INCLUDED
