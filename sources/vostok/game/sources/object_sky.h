////////////////////////////////////////////////////////////////////////////
//	Created		: 22.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SURVARIUM_OBJECT_SKY_H_INCLUDED
#define SURVARIUM_OBJECT_SKY_H_INCLUDED

#include "object.h"
#include <vostok/render/facade/game_renderer.h>

namespace vostok {
	namespace render {
		struct material_effects_instance_cook_data;
	} // namespace render
} // namespace vostok

namespace survarium{

class object_sky :public game_object_static
{
	typedef game_object_static				super;
public:
					object_sky				( game_world& w );
	virtual			~object_sky				( );
	virtual void	load					( configs::binary_config_value const& t );
	virtual void	load_contents			( );
	virtual void	unload_contents			( );
			void	material_ready			( resources::queries_result& data, vostok::render::material_effects_instance_cook_data* cook_data );
}; // class object_sky

} // namespace survarium

#endif // #ifndef SURVARIUM_OBJECT_SKY_H_INCLUDED