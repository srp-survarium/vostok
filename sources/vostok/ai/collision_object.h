////////////////////////////////////////////////////////////////////////////
//	Created		: 03.02.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_COLLISION_OBJECT_H_INCLUDED
#define VOSTOK_AI_COLLISION_OBJECT_H_INCLUDED

namespace vostok {
namespace ai {

struct game_object;

struct VOSTOK_NOVTABLE collision_object
{
	virtual	game_object&	get_game_object	( ) const		= 0;
	virtual	bool			ray_test		(
								float3 const& origin,
								float3 const& direction,
								float max_distance,
								float& distance
							) const							= 0;
	virtual	float3			get_origin		( ) const		= 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR	( collision_object );
}; // struct collision_object

} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_COLLISION_OBJECT_H_INCLUDED