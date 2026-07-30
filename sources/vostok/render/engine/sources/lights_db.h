#ifndef VOSTOK_RENDER_ENGINE_LIGHTS_DB_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LIGHTS_DB_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/facade/light_props.h>

#include "light.h"

namespace vostok {

namespace collision {

struct space_partitioning_tree;

} // namespace collision

namespace render {

struct light_data {
	explicit light_data( u32 in_id ) :
		id( in_id )
	{
	}

	bool operator<( light_data const& other ) const
	{
		return id < other.id;
	}

	bool operator==( u32 other_id ) const
	{
		return id == other_id;
	}

	~light_data( ) { }

	light_ptr	light;
	u32			id;
};

STATIC_SIZE_ASSERT( light_data, 0x8 );

class lights_db {
public:
	typedef vector<light_data> lights_type;

	lights_db( );
	~lights_db( );

	void initialize_sun( light_data& light_to_add );
	void add_light( u32 id, light_props* props );
	void update_light( u32 id, light_props* props );
	void remove_light( u32 id );
	void tick( float time_delta );

	light_ptr get_sun( ) const;

	lights_type const& get_lights( ) const
	{
		return m_lights;
	}

	collision::space_partitioning_tree& lights_tree( ) const
	{
		return *m_lights_tree;
	}

private:
	enum tree_operation_enum {
		tree_operation_add,
		tree_operation_no_operation,
	};

	light* create( tree_operation_enum operation ) const;

private:
	lights_type								m_lights;
	light_ptr								m_sun;
	collision::space_partitioning_tree*		m_lights_tree;
};

STATIC_SIZE_ASSERT( lights_db, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_LIGHTS_DB_H_INCLUDED
