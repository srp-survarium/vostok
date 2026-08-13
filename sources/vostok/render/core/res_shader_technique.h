#ifndef VOSTOK_RENDER_CORE_RES_SHADER_TECHNIQUE_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_SHADER_TECHNIQUE_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_pass.h>

namespace vostok {
namespace render {

class res_shader_technique : public resource_intrusive_base {
public:
	struct sflags {
		u32 priority : 2;
		u32 strict_b2f : 1;
		u32 has_emissive : 1;
		u32 has_distort : 1;
		u32 has_wmark : 1;
	};

	res_shader_technique( ) : m_registered( false ) { }
	~res_shader_technique( );
	void destroy_impl( ) const;
	void mark_registered( ) { m_registered = true; }
	bool is_registered( ) const { return m_registered; }
	void init_layouts( res_declaration const& declaration )
	{
		for ( u32 index = 0; index < m_passes.size( ); ++index )
			m_passes[index]->init_layout( declaration );
	}
	bool equal( res_shader_technique const& other ) const
	{
		return m_passes == other.m_passes;
	}
	bool equal( res_shader_technique const* other ) const { return equal( *other ); }
	res_pass const* get_pass( u32 index ) const { return m_passes[index].c_ptr( ); }

private:
	friend class effect_compiler;
	friend class effect_manager;
	friend class res_effect;
	friend s32 compare(
		res_shader_technique const& left,
		res_shader_technique const& right
	);

	sflags m_flags;
	vector<res_pass_ptr> m_passes;
	bool m_registered;
};

typedef intrusive_ptr<
	res_shader_technique,
	resource_intrusive_base,
	threading::single_threading_policy
> res_shader_technique_ptr;

STATIC_SIZE_ASSERT( res_shader_technique::sflags, 0x4 );
STATIC_SIZE_ASSERT( res_shader_technique, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_SHADER_TECHNIQUE_H_INCLUDED
