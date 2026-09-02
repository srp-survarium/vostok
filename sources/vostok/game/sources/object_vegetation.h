// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OBJECT_VEGETATION_H_INCLUDED
#define OBJECT_VEGETATION_H_INCLUDED

#include "game_object_.h"

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class object_vegetation : public game_object_ {
	typedef	game_object_	super;

public:
				explicit	object_vegetation	( base_game_scene& s );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert				( ) override;
	virtual	void		remove				( ) override;

private:
			void		on_grass_loaded		( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

private:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	resources::unmanaged_resource_ptr		m_grass;

}; // class object_vegetation

STATIC_SIZE_ASSERT(object_vegetation, 0x118);

} // namespace survarium

#endif // #ifndef OBJECT_VEGETATION_H_INCLUDED
