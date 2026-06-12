////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLE_GAME_PROJECT_LOADING_H_INCLUDED
#define SIMPLE_GAME_PROJECT_LOADING_H_INCLUDED

namespace survarium {

struct simple_game_project::loading {
	inline		loading	( ) { /* no source */ }

public:
	/* 0x0000 */	u32		loaded_count;
	/* 0x0004 */	bool	all_queried;
	/* 0x0005 */	bool	visuals_loaded;
	/* 0x0006 */	bool	collision_loaded;
}; // struct simple_game_project::loading

STATIC_SIZE_ASSERT(simple_game_project::loading, 0x8);

} // namespace survarium

#endif // #ifndef SIMPLE_GAME_PROJECT_LOADING_H_INCLUDED
