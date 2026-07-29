////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::flash_movie *,vostok::render::std_allocator<survarium::flash_movie *> >
	stlp_std::vector<survarium::flash_movie * >;
class survarium::flash_movie *;

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_movie*;

namespace vostok {
namespace render {

class vector< survarium::flash_movie* > : public std::vector< survarium::flash_movie* > {
public:
	inline										vector<survarium::flash_movie *>	( ) { /* no source */ }
	inline										vector<survarium::flash_movie *>	( u32 arg_0, survarium::flash_movie* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<survarium::flash_movie *>	( u32 arg_0 ) { /* no source */ }

	inline	survarium::flash_movie*&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	survarium::flash_movie* const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32									size								( ) const { /* no source */ }

	inline										~vector<survarium::flash_movie *>	( ) { /* no source */ }
}; // class vector< survarium::flash_movie* >

STATIC_SIZE_ASSERT(vector< survarium::flash_movie* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
