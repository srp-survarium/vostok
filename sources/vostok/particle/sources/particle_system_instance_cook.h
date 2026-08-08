////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_SYSTEM_INSTANCE_COOK_H_INCLUDED
#define PARTICLE_SYSTEM_INSTANCE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {
	struct material_effects_instance_cook_data;
}
namespace particle {

class particle_system;
class particle_emitter;
class particle_system_instance_impl;
struct material_query_data;

class particle_system_instance_cook :	
	public resources::translate_query_cook,
	public boost::noncopyable
{
	typedef resources::translate_query_cook	super;
	
public:
	particle_system_instance_cook							();
	virtual			~particle_system_instance_cook		( ) {}
	
	virtual	void	translate_query			( resources::query_result_for_cook & parent );
	virtual void	delete_resource			( resources::resource_base* res );
	
private:
	void			on_materials_loaded		( resources::queries_result& data, particle_system_instance_impl* created_resource, material_query_data* material_data, render::material_effects_instance_cook_data* cook_data );
	void			on_sub_resources_loaded	( resources::queries_result& data );
}; // class particle_system_instance_cook

} // namespace particle
} // namespace vostok

#endif // #ifndef PARTICLE_SYSTEM_INSTANCE_COOK_H_INCLUDED
