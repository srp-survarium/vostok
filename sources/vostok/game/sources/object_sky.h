////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_SKY_H_INCLUDED
#define OBJECT_SKY_H_INCLUDED

#include "game_object_.h"

namespace vostok {
namespace render {
	class material_effects_instance_cook_data;
} // namespace render
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class object_sky : public game_object_ {
	typedef game_object_ super;

public:
			explicit	object_sky		( base_game_scene& w );
	virtual				~object_sky		( );

	virtual	void		load			(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert			( ) override;
	virtual	void		remove			( ) override;

protected:
			void		material_ready	(
							resources::queries_result&		data,
							render::material_effects_instance_cook_data*	cook_data,
							boost::function< void( game_object_& ) >&	cb
						);

protected:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	resources::unmanaged_resource_ptr		m_sky_material;
}; // class object_sky

STATIC_SIZE_ASSERT(object_sky, 0x118);

} // namespace survarium

#endif // #ifndef OBJECT_SKY_H_INCLUDED
