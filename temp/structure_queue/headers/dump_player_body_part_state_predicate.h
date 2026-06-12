////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef DUMP_PLAYER_BODY_PART_STATE_PREDICATE_H_INCLUDED
#define DUMP_PLAYER_BODY_PART_STATE_PREDICATE_H_INCLUDED

/* INCLUDES */
struct survarium::damage_info_type;
class survarium::body_part_parameters* const;

namespace survarium {

struct dump_player_body_part_state_predicate : public boost::noncopyable {
	inline			dump_player_body_part_state_predicate( damage_info_type& arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void	operator()	( body_part_parameters* const arg_0 ) const { /* no source */ }

	inline			~dump_player_body_part_state_predicate( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	damage_info_type&		stats;
	/* 0x0004 */	u32						current_time;
}; // struct dump_player_body_part_state_predicate

STATIC_SIZE_ASSERT(dump_player_body_part_state_predicate, 0x8);

} // namespace survarium

#endif // #ifndef DUMP_PLAYER_BODY_PART_STATE_PREDICATE_H_INCLUDED
