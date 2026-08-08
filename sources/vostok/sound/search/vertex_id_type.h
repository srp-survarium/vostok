////////////////////////////////////////////////////////////////////////////
//	Created		: 16.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_SEARCH_VERTEX_ID_TYPE_H_INCLUDED
#define VOSTOK_SOUND_SEARCH_VERTEX_ID_TYPE_H_INCLUDED

namespace vostok {
namespace sound {
namespace search {

struct vertex_id_type
{
	inline			vertex_id_type	( ) :
		portal_id				( u32(-1) ),
		incoming_sector_index	( u32(-1) ),
		source_to_portal_distance( math::float_max )
	{
	}

	inline bool operator ==	( vertex_id_type const& other ) const
	{
		return portal_id == other.portal_id && incoming_sector_index == other.incoming_sector_index;
	}

	inline bool operator !=	( vertex_id_type const& other ) const
	{
		return !(*this == other);
	}

	u32		portal_id;
	u32		incoming_sector_index;
	float	source_to_portal_distance;
}; // struct vertex_id_type

} // namespace search
} // namespace sound

namespace ai {
namespace vertex_manager {

inline u32 hash_value( sound::search::vertex_id_type const& vertex_id )
{
	return vertex_id.portal_id;
}

} // namespace vertex_manager
} // namespace ai

STATIC_SIZE_ASSERT( sound::search::vertex_id_type, 0xC );

} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_SEARCH_VERTEX_ID_TYPE_H_INCLUDED
