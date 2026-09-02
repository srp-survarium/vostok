// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_MOON_POSITION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MOON_POSITION_H_INCLUDED
namespace vostok {
namespace render {

struct time_parameters {
	s32 year;
	s32 month;
	s32 day;
	s32 hours;
	s32 minutes;
	s32 seconds;
};

STATIC_SIZE_ASSERT( time_parameters, 0x18 );

void calculate_moon_position(
	time_parameters const&	parameters,
	double					latitude,
	double					longitude,
	double&					azimuth,
	double&					zenith
);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MOON_POSITION_H_INCLUDED
