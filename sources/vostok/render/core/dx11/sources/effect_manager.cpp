#include "pch.h"
#include <vostok/render/core/effect_loader.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_compile_data.h>
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/res_pass.h>
#include <vostok/render/core/res_shader_technique.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/dx11/effect_cook.h>
#include "manager_common_inline.h"

namespace vostok {
namespace render {

static command_line::key s_no_effects_initialize( "no_effects_initialize", "", "", "" );

struct effect_manager_call_destructor_predicate
{

	template <typename T>
	inline void	operator()	( T* resource ) const
	{
		effect_manager::call_resource_destructor( resource );
	}
}; // struct effect_manager_call_destructor_predicate

} // namespace render
} // namespace vostok

namespace vostok {
namespace render {

void effect_loader::on_effect_ready( resources::queries_result& data )
{
	if(!query_rejected)
	{
		if( !s_no_effects_initialize)
		{
			if (data[0].is_successful())
				*effect_ptr		= static_cast_resource_ptr<res_effect_ptr>(data[0].get_unmanaged_resource());
			else
				*effect_ptr	= 0;
		}
	}else
	{
		//LOG_INFO("query rejected");
	}

	effect_loader* this_ptr	= this;
	DELETE					( this_ptr );
}

effect_manager::effect_manager( )
	:
#ifdef MASTER_GOLD
	  m_shader_cache_info( memory::g_mt_allocator ),
#else // #ifdef MASTER_GOLD
	  m_shader_cache_info( debug::g_mt_allocator ),
#endif // #ifdef MASTER_GOLD
	  force_sync( false ),
	  m_loading_incomplete( false )
{
	static effect_cook effect_cooker;
	register_cook(&effect_cooker);
}

effect_manager::~effect_manager( )
{
	map<fixed_string<128>, effect_descriptor*>::iterator it  = m_effect_descriptors.begin(),
							  end = m_effect_descriptors.end();

	for( ; it != end; ++it)
		DELETE( it->second);

		it  = m_effect_descriptors_by_texture.begin();
		end = m_effect_descriptors_by_texture.end();

	for( ; it != end; ++it)
		DELETE( it->second);

	//unregister_cook(resources::render_effect_class);
}

res_pass* effect_manager::create_pass( res_pass const& pass )
{
	set<res_pass*, compare_predicate<res_pass> >::iterator const found = m_passes.find( &pass );
	if( found != m_passes.end())
		return *found;

	res_pass*	new_pass = NEW( res_pass)( pass.m_vs, pass.m_gs, pass.m_ps, pass.m_state);
	new_pass->mark_registered();

	m_passes.insert				( new_pass);
	return new_pass;
}

void effect_manager::delete_pass( res_pass const* pass )
{
	if( !pass->is_registered() )
		return;

	if( reclaim( m_passes, pass))
	{
		DELETE( pass, effect_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find compiled pass.");
}

effect_descriptor* effect_manager::get_effect_descriptor_by_name( pcstr name )
{
	map<fixed_string<128>, effect_descriptor*>::iterator it = m_effect_descriptors.find(name);

	if( it != m_effect_descriptors.end())
		return it->second;

	if (!strings::equal(name, "none"))
		R_ASSERT(0, "%s disctiptor not found!", name);

	return 0;
}

void effect_manager::add_effect(
	effect_descriptor* in_descriptor,
	custom_config_ptr const& in_config,
	res_effect* in_effect
)
{
	effect_holder_struct	holder;
	holder.effect			= in_effect;
	holder.config			= in_config;
	holder.descriptor		= in_descriptor;

	m_effects.push_back(holder);
}

void effect_manager::remove_effect( res_effect* in_effect )
{
	vector<effect_holder_struct>::iterator begin_it	= m_effects.begin();
	vector<effect_holder_struct>::iterator end_it	= m_effects.end();
	vector<effect_holder_struct>::iterator it		= begin_it;

	for (; it != end_it; ++it)
	{
		if (it->effect == in_effect)
		{
			m_effects.erase(it);
		}
	}
}

void effect_manager::on_effect_created(
	res_effect_ptr* out_effect_ptr,
	resources::queries_result& data
)
{
	if ( s_no_effects_initialize)
		return;

	if (data[0].is_successful())
	{
		*out_effect_ptr		= static_cast_resource_ptr<res_effect_ptr>(data[0].get_unmanaged_resource());
	}
	else
		*out_effect_ptr		= 0;
}

void effect_manager::on_async_effect_created(
	resources::queries_result& data,
	res_effect_ptr* out_effect_ptr,
	effect_descriptor* descriptor
)
{
	(void)&descriptor;

//	if (std::find(m_effects_deleted_in_pending.begin(), m_effects_deleted_in_pending.end(), out_effect_ptr) != m_effects_deleted_in_pending.end())
//		return;

	if ( s_no_effects_initialize)
		return;

	if (data[0].is_successful())
		*out_effect_ptr		= static_cast_resource_ptr<res_effect_ptr>(data[0].get_unmanaged_resource());
	else
		*out_effect_ptr		= 0;
}

void effect_manager::on_async_effect_created_callback(
	resources::queries_result&,
	res_effect_ptr*,
	effect_descriptor*,
	boost::function<void( )>
)
{
	// claude@NOTE: no legacy ancestor - new-in-target overload; the corpus only ever had the 3-param on_async_effect_created (already ported); matcher-phase work.
	// STATE[STUB]
}

void effect_manager::create_new_effect(
	res_effect_ptr* out_effect,
	effect_descriptor* descriptor,
	custom_config_ptr const& config,
	u32 crc
)
{
	resources::user_data_variant				user_data_variant;

	effect_compile_data* cook_data				= NEW(effect_compile_data)(descriptor, config, crc);
	user_data_variant.set						(cook_data);

	resources::query_create_resource			(
		"",
		vostok::const_buffer("", 1),
		resources::render_effect_class,
		boost::bind(&effect_manager::on_async_effect_created, this, _1, out_effect, descriptor),
		g_allocator,
		&user_data_variant
	);
}

res_effect_ptr effect_manager::create_new_effect(
	effect_descriptor& descriptor,
	custom_config_ptr const& ptr,
	u32 crc
)
{
	resources::user_data_variant				user_data_variant;

	effect_compile_data* cook_data				= NEW(effect_compile_data)(&descriptor, ptr, crc);
	user_data_variant.set						(cook_data);

	resources::user_data_variant const*			user_data_variants[] = {&user_data_variant};

	resources::creation_request requests[]		= { resources::creation_request("", vostok::const_buffer("", 1), resources::render_effect_class) };

	res_effect_ptr new_effect_ptr;

	resources::query_create_resources_and_wait	(
		requests,
		array_size(requests),
		boost::bind(&effect_manager::on_effect_created, this, &new_effect_ptr, _1),
		g_allocator,
		user_data_variants
	);

	return new_effect_ptr;
}

void effect_manager::on_effects_recompiled(
	vectora<effect_to_recompile_struct>* effects_to_recompile,
	resources::queries_result& data
)
{
	u32 request_index						= 0;
	for (vectora<effect_to_recompile_struct>::iterator it = effects_to_recompile->begin(); it != effects_to_recompile->end(); ++it, ++request_index)
	{
		if (data[request_index].is_successful())
		{

			effects_to_recompile;

			res_effect_ptr created_temp_effect	= static_cast_resource_ptr<res_effect_ptr>(data[request_index].get_unmanaged_resource());
			// TODO: add some special copy function
			it->effect->m_techniques		= created_temp_effect->m_techniques;
			it->effect->m_cur_technique		= created_temp_effect->m_cur_technique;
#ifndef MASTER_GOLD
			threading::simple_lock_raii raii(it->effect->used_shaders_lock);
			it->effect->used_shaders		= created_temp_effect->used_shaders;
#endif // #ifndef MASTER_GOLD
		}
	}
}

res_shader_technique* effect_manager::create_effect_technique(
	res_shader_technique const& element
)
{
	if( element.m_passes.empty())
		return 0;

	// Search equal in shaders array
	set<res_shader_technique*, compare_predicate<res_shader_technique> >::iterator const found =
		m_techniques.find( &element );
	if( found != m_techniques.end())
		return *found;

	// Create _new_ entry
	res_shader_technique*	new_technique = NEW( res_shader_technique); //( element);

	new_technique->m_flags = element.m_flags;
	new_technique->m_passes = element.m_passes;

	new_technique->mark_registered();
	m_techniques.insert( new_technique);
	return new_technique;
}

void effect_manager::delete_effect_technique( res_shader_technique const* technique )
{
	if( !technique->is_registered() )
		return;

	if( reclaim( m_techniques, technique))
	{
		DELETE( technique, effect_manager_call_destructor_predicate());
		return;
	}

	//LOG_ERROR( "!ERROR: Failed to find complete shader");
}

void effect_manager::recompile_shaders_async(
	vector<fs_new::virtual_path_string> const&
)
{
	// claude@NOTE: legacy body diverged - closest ancestor recompile_shaders is entirely #ifndef MASTER_GOLD over the retired used_shaders member; recompile_shaders_async has zero corpus hits; matcher-phase work.
	// STATE[STUB]
}

void effect_manager::register_effect_desctiptor( pcstr name, effect_descriptor* dectriptor )
{
 	m_effect_descriptors.insert( mk_pair(name, dectriptor));
}

} // namespace render
} // namespace vostok
