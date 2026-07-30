#include "pch.h"
#include "lights_db.h"

namespace vostok {
namespace render {

lights_db::lights_db( ) :
	m_lights_tree( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602f80]
}

lights_db::~lights_db( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602ec0]
}

light_ptr lights_db::get_sun( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x602e10]
	return m_sun;
}

light* lights_db::create( tree_operation_enum ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x602de0]
	return 0;
}

void lights_db::initialize_sun( light_data& light_to_add )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602ea0]
	m_sun = light_to_add.light;
}

void fill_light( light&, light_props* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602fb0]
}

void lights_db::add_light( u32, light_props* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x603660]
}

void lights_db::update_light( u32, light_props* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x603620]
}

void lights_db::tick( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602db0]
}

void lights_db::remove_light( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602e50]
}

} // namespace render
} // namespace vostok
