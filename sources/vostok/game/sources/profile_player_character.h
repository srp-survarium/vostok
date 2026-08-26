////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROFILE_PLAYER_CHARACTER_H_INCLUDED
#define PROFILE_PLAYER_CHARACTER_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class lobby_menu;
struct player_profile;

class player;
typedef resources::resource_ptr<
	player,
	resources::unmanaged_intrusive_base
> player_ptr;

class profile_player_character : private boost::noncopyable {
public:
			explicit	profile_player_character	( lobby_menu& lobby_menu );
	inline				~profile_player_character	( ) { /* no source */ }

			void		update						( const u32 current_time_in_ms );

			void		clear_resources				( );

			void		profile_changed				( player_profile const* profile );

private:
			void		query_profile_contents		( player_profile const* profile );

			void		on_player_ready				( resources::queries_result& data, player_profile* profile_to_cook );

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	player_ptr		m_player;
	/* 0x0004 */	lobby_menu&		m_lobby_menu;
}; // class profile_player_character

STATIC_SIZE_ASSERT(profile_player_character, 0x8);

} // namespace survarium

#endif // #ifndef PROFILE_PLAYER_CHARACTER_H_INCLUDED
