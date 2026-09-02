// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_PHYSICS_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
#define VOSTOK_PHYSICS_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace physics {

class animated_model_instance_cook : public resources::translate_query_cook {
public:
					animated_model_instance_cook	( );

	virtual	void	translate_request_path			( pcstr request, fs_new::virtual_path_string& new_request ) const override;
	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource					( resources::resource_base* resource ) override;

private:
			void	on_config_loaded				( resources::queries_result& data );
			void	on_skeleton_config_loaded		( resources::queries_result& data );
			void	on_subresources_loaded			( resources::queries_result& data );


private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	memory::base_allocator*		m_allocator;
}; // class animated_model_instance_cook


STATIC_SIZE_ASSERT(animated_model_instance_cook, 0x24);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_ANIMATED_MODEL_INSTANCE_COOK_H_INCLUDED
