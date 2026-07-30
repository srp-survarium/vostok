#ifndef VOSTOK_RENDER_CORE_SHADER_DATA_BASE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_DATA_BASE_H_INCLUDED

namespace vostok {
namespace render {

struct shader_data_base {
	shader_data_base( ) : instruction_count( 0 ) { }

	u16 instruction_count;
};

STATIC_SIZE_ASSERT( shader_data_base, 0x2 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_DATA_BASE_H_INCLUDED
