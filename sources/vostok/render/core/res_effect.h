#ifndef VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_shader_technique.h>
#include <vostok/render/core/texture_named_instance.h>
#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

class effect_compiler;
class effect_cook;
class effect_manager;
class res_declaration;

class res_effect : public resources::unmanaged_resource {
private:
	friend class effect_compiler;
	friend class effect_cook;
	friend class effect_manager;

public:
	res_effect( ) : m_cur_technique( 0 ), m_registered( false )
	{
		// STATE[STUB]
		// FUNCTION BODY[0x1303a0]
	}

	void push_texture_unique( res_texture* texture, pcstr path );
	void get_max_used_texture_dimension( u32& width, u32& height )
	{
		width = 0;
		height = 0;
	}
	u32 get_max_pixel_shader_instruction_count( ) const { return 0; }
	void mark_registered( ) { m_registered = true; }
	bool is_registered( ) const { return m_registered; }
	void init_layouts( res_declaration const& declaration )
	{
		for ( u32 index = 0; index < m_techniques.size( ); ++index )
			m_techniques[index]->init_layouts( declaration );
	}
	bool equal( res_effect const& other ) const
	{
		return m_techniques == other.m_techniques;
	}
	bool equal( res_effect const* other ) const { return equal( *other ); }
	bool apply( u32 technique_id, u32 pass_id )
	{
		// claude@NOTE: legacy body diverged - legacy apply is void with defaulted args; canonical returns bool from apply_pass (body already reproduces the legacy call sequence); matcher-phase work.
		// STATE[STUB]
		// FUNCTION BODY[0x668e0]
		select_technique( technique_id );
		return apply_pass( pass_id );
	}
	bool select_technique( u32 technique_id )
	{
		if ( technique_id >= m_techniques.size( ) )
			return false;
		m_cur_technique = technique_id;
		return true;
	}
	bool apply_pass( u32 id );
	res_shader_technique const* get_technique( u32 index ) const
	{
		return m_techniques[index].c_ptr( );
	}

	vector<texture_named_instance> m_used_textures;

private:
	u32 m_cur_technique;

public:
	vector<res_shader_technique_ptr> m_techniques;

private:
	bool m_registered;
	virtual ~res_effect( );
	void destroy_impl( ) const { }
};

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

typedef res_effect_ptr ref_effect;

STATIC_SIZE_ASSERT( res_effect, 0x128 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_EFFECT_H_INCLUDED
