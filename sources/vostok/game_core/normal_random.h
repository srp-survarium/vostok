////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NORMAL_RANDOM_H_INCLUDED
#define NORMAL_RANDOM_H_INCLUDED

namespace survarium {

class normal_random {
public:
	inline				normal_random	( ) : m_seed( 1 ) { }
	inline	explicit	normal_random	( s32 arg_0 ) { /* no source */ }

			float		rand_n			( float sigma );

	inline	void		set_seed		( const s32 arg_0 ) { /* no source */ }
	inline	s32			seed			( ) const { return m_seed; }

private:
	inline	s32			maxI			( ) { return 32767; }

	inline	s32			randI			( ) { return ( m_seed = m_seed * 0x343fd + 0x269ec3 ) >> 16 & 0x7fff; }
	inline	s32			randI			( s32 arg_0 ) { /* no source */ }
	inline	s32			randI			( s32 arg_0, s32 arg_1 ) { /* no source */ }
	inline	s32			randIs			( s32 arg_0 ) { /* no source */ }
	inline	s32			randIs			( s32 arg_0, s32 arg_1 ) { /* no source */ }

	inline	float		maxF			( ) { return 32767.0f; }

	inline	float		randF			( ) { return float( randI( ) ) / maxF( ); }
	inline	float		randF			( float arg_0 ) { /* no source */ }
	inline	float		randF			( float arg_0, float arg_1 ) { /* no source */ }
	inline	float		randFs			( float arg_0 ) { /* no source */ }
	inline	float		randFs			( float arg_0, float arg_1 ) { /* no source */ }


private:
	/* 0x0000 */	s32		m_seed;
}; // class normal_random

STATIC_SIZE_ASSERT(normal_random, 0x4);

inline float normal_random::rand_n( float sigma )
{
	const float c_one_over_sigma_exp = 1.2539185f;
	if ( sigma == 0.0f )
		return 0.0f;
	float y;
	do {
		y = -logf( randF( ) );
	} while ( randF( ) > expf( -math::sqr( y - 1.0f ) * 0.5f ) );
	if ( randI( ) & 1 )
		return y * sigma * c_one_over_sigma_exp;
	else
		return -y * sigma * c_one_over_sigma_exp;
}

} // namespace survarium

#endif // #ifndef NORMAL_RANDOM_H_INCLUDED
