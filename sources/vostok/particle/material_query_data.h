////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PARTICLE_MATERIAL_QUERY_DATA_H_INCLUDED
#define PARTICLE_MATERIAL_QUERY_DATA_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/facade/vertex_input_type.h>

namespace vostok {
namespace particle {

class particle_emitter_instance;

struct material_query_data {
	fs_new::virtual_path_string		material_name;
	particle_emitter_instance*		instance;
	render::enum_vertex_input_type	vertex_type;
}; // struct material_query_data

STATIC_SIZE_ASSERT(material_query_data, 0x11C);

} // namespace particle
} // namespace vostok

#endif // #ifndef PARTICLE_MATERIAL_QUERY_DATA_H_INCLUDED
