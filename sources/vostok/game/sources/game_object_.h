////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_OBJECT__H_INCLUDED
#define GAME_OBJECT__H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>
#include <boost/function.hpp>

namespace vostok {
namespace configs {
	class binary_config_value;
} // namespace configs
} // namespace vostok

namespace survarium {

class base_game_scene;

// pulled ahead of batch 7 (a base of batch 6's human_npc); out-of-line bodies
// in the object.cpp compiland
class game_object_ : public resources::unmanaged_resource , public boost::noncopyable {
public:
			explicit			game_object_	( base_game_scene& s );

	virtual	void				load			(
									configs::binary_config_value const&		arg_0,
									pcstr									arg_1,
									boost::function< void( game_object_& ) >&	arg_2
								) = 0;

	// STATE[STUB]
	virtual	void				insert			( )
	{
		// FUNCTION BODY[0x4f210]
		// <0x4f210>|0x000|      :'28'	{
		// ******
	}

	virtual	void				remove			( ) { /* no source */ }

	inline	base_game_scene&	get_game_scene	( ) { /* no source */ return m_game_scene; }

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	base_game_scene&	m_game_scene;
}; // class game_object_

STATIC_SIZE_ASSERT(game_object_, 0x110);

} // namespace survarium

#endif // #ifndef GAME_OBJECT__H_INCLUDED
