#ifndef VOSTOK_RENDER_ENGINE_SUN_POSITION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SUN_POSITION_H_INCLUDED

namespace vostok {
namespace render {

struct cTime {
	s32		iYear;
	s32		iMonth;
	s32		iDay;
	float	dHours;
	float	dMinutes;
	float	dSeconds;
};

struct cLocation {
	float dLongitude;
	float dLatitude;
};

struct cSunCoordinates {
	float dZenithAngle;
	float dAzimuth;
};

STATIC_SIZE_ASSERT( cTime, 0x18 );
STATIC_SIZE_ASSERT( cLocation, 0x8 );
STATIC_SIZE_ASSERT( cSunCoordinates, 0x8 );

void calculate_sun_position( cTime time, cLocation location, cSunCoordinates* sun_coordinates );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SUN_POSITION_H_INCLUDED
