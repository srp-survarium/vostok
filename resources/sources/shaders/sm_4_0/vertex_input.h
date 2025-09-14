////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_VERTEX_INPUT_TYPE_H_INCLUDED
#define SHADERS_SM_4_0_VERTEX_INPUT_TYPE_H_INCLUDED

#include "common_defines.h"

#if CONFIG_VERTEX_INPUT_TYPE==STATIC_VERTEX_INPUT_TYPE
#	include "static_mesh_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==SKELETON_VERTEX_INPUT_TYPE
#	include "skeleton_mesh_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==PARTICLE_VERTEX_INPUT_TYPE
#	include "particle_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==PARTICLE_SUBUV_VERTEX_INPUT_TYPE
#	include "particle_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==PARTICLE_BEAMTRAIL_VERTEX_INPUT_TYPE
#	include "particle_beamtrail_vertex_input.h"	
#elif CONFIG_VERTEX_INPUT_TYPE==DECAL_VERTEX_INPUT_TYPE
#	include "decal_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==LENSFLARES_VERTEX_INPUT_TYPE
#	include "lensflares_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==TERRAIN_VERTEX_INPUT_TYPE
#	include "terrain_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==GRASSMESH_VERTEX_INPUT_TYPE
#	include "grassmesh_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==GRASSBILLBOARD_VERTEX_INPUT_TYPE
#	include "grassmesh_vertex_input.h"	
#elif CONFIG_VERTEX_INPUT_TYPE==SPEEDTREE_BRANCH_VERTEX_INPUT_TYPE
#	include "speedtree_branch_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==SPEEDTREE_FROND_VERTEX_INPUT_TYPE
#	include "speedtree_frond_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==SPEEDTREE_LEAFMESH_VERTEX_INPUT_TYPE
#	include "speedtree_leafmesh_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==SPEEDTREE_LEAFCARD_VERTEX_INPUT_TYPE
#	include "speedtree_leafcard_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==SPEEDTREE_BILLBOARD_VERTEX_INPUT_TYPE
#	include "speedtree_billboard_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==POST_PROCESS_VERTEX_INPUT_TYPE
#	include "postprocess_vertex_input.h"
#elif CONFIG_VERTEX_INPUT_TYPE==WIRES_VERTEX_INPUT_TYPE
#	include "wires_vertex_input.h"
#else // NULL_VERTEX_INPUT_TYPE
#	include "null_mesh_vertex_input.h"
#endif

#endif // #ifndef SHADERS_SM_4_0_VERTEX_INPUT_TYPE_H_INCLUDED


