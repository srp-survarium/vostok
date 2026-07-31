#ifndef VOSTOK_RENDER_CORE_EFFECT_DESCRIPTOR_H_INCLUDED
#define VOSTOK_RENDER_CORE_EFFECT_DESCRIPTOR_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/memory.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_descriptor {
public:
	effect_descriptor( );
	virtual ~effect_descriptor( );

	virtual bool should_recompile_when_global_changes(
		vector< fs_new::virtual_path_string > const&
	) const
	{
		// claude@NOTE: no legacy ancestor - should_recompile_when_global_changes has no legacy ancestor; matcher-phase work.
		// STATE[STUB]
		// FUNCTION BODY[0xab100]
		return false;
	}

	virtual void compile(
		effect_compiler&			compiler,
		custom_config_value const&	config
	);
};

STATIC_SIZE_ASSERT( effect_descriptor, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_EFFECT_DESCRIPTOR_H_INCLUDED
