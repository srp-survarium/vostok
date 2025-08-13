////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef AFFECTS_THRESHOLD_H_INCLUDED
#define AFFECTS_THRESHOLD_H_INCLUDED

#include <boost/noncopyable.hpp>

namespace stalker2 {

class body_part_parameters;

class affects_threshold : public boost::noncopyable {
public:
	affects_threshold(
		float value,
		unsigned int affects_count,
		body_part_parameters *const bodypart
		);

private:
	affects_threshold* next;
	const float m_value;
	const unsigned int m_affects_count;
	body_part_parameters* const m_bodypart;
}; // class affects_threshold

} // namespace stalker2

#endif // #ifndef AFFECTS_THRESHOLD_H_INCLUDED