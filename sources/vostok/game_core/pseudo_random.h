////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PSEUDO_RANDOM_H_INCLUDED
#define PSEUDO_RANDOM_H_INCLUDED

namespace survarium {

class pseudo_random {
public:
	inline	explicit	pseudo_random	( const float time ) : m_time( time ) { }

	// \sources\weapon_recoil_calculator.cpp
			float		random_f		( const float range );
	inline	void		set_time		( const float time ) { m_time = time; }

private:
	/* 0x0000 */	float		m_time;
}; // class pseudo_random

STATIC_SIZE_ASSERT(pseudo_random, 0x4);

} // namespace survarium

#endif // #ifndef PSEUDO_RANDOM_H_INCLUDED
