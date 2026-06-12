////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NORMAL_RANDOM_H_INCLUDED
#define NORMAL_RANDOM_H_INCLUDED

namespace survarium {

class normal_random {
public:
	inline	explicit	normal_random	( s32 arg_0 ) { /* no source */ }
	inline	explicit	normal_random	( ) { /* no source */ }

			float		rand_n			( float sigma );

	inline	void		set_seed		( s32 arg_0 ) { /* no source */ }
	inline	s32			seed			( ) const { return m_seed; }

	inline	s32			maxI			( ) { /* no source */ }

	inline	s32			randI			( s32 arg_0, s32 arg_1 ) { /* no source */ }
	inline	s32			randI			( s32 arg_0 ) { /* no source */ }
	inline	s32			randI			( ) { /* no source */ }
	inline	s32			randIs			( s32 arg_0, s32 arg_1 ) { /* no source */ }
	inline	s32			randIs			( s32 arg_0 ) { /* no source */ }

	inline	float		maxF			( ) { /* no source */ }

	inline	float		randF			( float arg_0, float arg_1 ) { /* no source */ }
	inline	float		randF			( float arg_0 ) { /* no source */ }
	inline	float		randF			( ) { /* no source */ }
	inline	float		randFs			( float arg_0, float arg_1 ) { /* no source */ }
	inline	float		randFs			( float arg_0 ) { /* no source */ }


private:
	/* 0x0000 */	s32		m_seed;
}; // class normal_random

STATIC_SIZE_ASSERT(normal_random, 0x4);

inline float normal_random::rand_n( float sigma )
{
	return 0.0f;
}

} // namespace survarium

#endif // #ifndef NORMAL_RANDOM_H_INCLUDED
