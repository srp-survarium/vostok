////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CTIME_H_INCLUDED
#define RENDER_CTIME_H_INCLUDED

namespace vostok {
namespace render {

struct cTime {

	/* 0x0000 */	s32			iYear;
	/* 0x0004 */	s32			iMonth;
	/* 0x0008 */	s32			iDay;
	/* 0x000c */	float		dHours;
	/* 0x0010 */	float		dMinutes;
	/* 0x0014 */	float		dSeconds;
}; // struct cTime

STATIC_SIZE_ASSERT(cTime, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CTIME_H_INCLUDED
