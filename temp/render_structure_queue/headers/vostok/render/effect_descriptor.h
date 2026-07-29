////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_DESCRIPTOR_H_INCLUDED
#define RENDER_EFFECT_DESCRIPTOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::custom_config_value;
class vostok::render::effect_compiler;
class vostok::render::vector<vostok::fs_new::virtual_path_string>;

namespace vostok {
namespace render {

class effect_descriptor {
public:
					effect_descriptor	( );
	virtual			~effect_descriptor	( );

	virtual	bool	should_recompile_when_global_changes( vector< fs_new::virtual_path_string > const& in_changed_defines ) const { /* no source */ }

	virtual	void	compile				( effect_compiler& compiler, custom_config_value const& config );
}; // class effect_descriptor

STATIC_SIZE_ASSERT(effect_descriptor, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_DESCRIPTOR_H_INCLUDED
