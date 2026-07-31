#include "pch.h"
#include <vostok/render/culling/possible_sectors_holder.h>
#include "material.h"
#include "material_manager.h"
#include "render_model.h"
#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {
namespace render {

render_model::render_model( ) :
	m_aabbox			( math::create_identity_aabb( ) ),
	m_locators			( NULL ),
	m_locators_count	( 0 ),
	m_lods_descriptor	( NULL ),
	m_childs			( NULL ),
	m_childs_count		( 0 )
{
	// FUNCTION BODY[0x63c500]
	// ******
}

render_model::~render_model( )
{
	// claude@NOTE: adapted from legacy static_render_model::~static_render_model (children
	// vector moved into render_model as m_childs array in the shipped tree).
	for( u8 i = 0; i < m_childs_count; ++i )
		DELETE				( m_childs[i] );

	if(m_locators)
		FREE(m_locators);

	// LOCALS
	// u8 								i
	// ******

	// CALL SITE INFO
	// <0x63c479> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x63c430]: 10
	// <0x63c432>|0x002|+0x01e:'34'
	// <0>
	// <0x63c450>|0x020|+0x006:'36'
	// <0x63c456>|0x026|+0x049:'37'
	// <0>
	// <1>
	// <0x63c49f>|0x06f|+0x00a:'40'
	// <0x63c4a9>|0x079|+0x021:'41'
	// <0>
	// <0x63c4ca>|0x09a|+0x022:'43'
	// ******
}

void render_model::load_properties( configs::binary_config_value const& properties )
{
	m_aabbox.max			= vostok::math::float3(properties["bounding_box"]["max"]);
	m_aabbox.min			= vostok::math::float3(properties["bounding_box"]["min"]);

	m_locators_count		= (u16)properties.value_exists("locators") ? (u16)properties["locators"].size() : 0;

	if(m_locators_count)
	{
		m_locators	= ALLOC(model_locator_item, m_locators_count);
		for(u16 i=0; i<m_locators_count; ++i)
		{
			configs::binary_config_value const& v = properties["locators"][i];
			model_locator_item& item	= m_locators[i];
			pcstr name					= v["name"];
			int bone_idx				= v["bone_idx"];
			item.m_bone					= u16(bone_idx);
			strings::copy				( item.m_name, name );
			item.m_offset				= math::create_rotation(v["rotation"]) * math::create_translation(v["position"]);
		}
	}

	// LOCALS
	// u16 								i
	// pcstr 							name
	// ******

	// FUNCTION BODY[0x63c260]: 20
	// <0x63c26e>|0x00e|+0x027:'48'
	// <0x63c295>|0x035|+0x037:'49'
	// <0>
	// <0x63c2cc>|0x06c|+0x04e:'51'
	// <0>
	// <0x63c31a>|0x0ba|+0x009:'53'
	// <0>
	// <0x63c323>|0x0c3|+0x012:'55'
	// <0x63c335>|0x0d5|+0x022:'56'
	// <0>
	// <0x63c357>|0x0f7|+0x016:'58'
	// <0x63c36d>|0x10d|+0x00e:'59'
	// <0x63c37b>|0x11b|+0x012:'60'
	// <0x63c38d>|0x12d|+0x00c:'61'
	// <0x63c399>|0x139|+0x003:'62'
	// <0x63c39c>|0x13c|+0x012:'63'
	// <0x63c3ae>|0x14e|+0x078:'64'
	// <0>
	// <1>
	// <2>
	// ******
}

bool render_model::get_locator( pcstr locator_name, model_locator_item& result ) const
{
	for( u16 i=0; i<m_locators_count; ++i)
	{
		model_locator_item const& item = m_locators[i];
		if(strings::equal(item.m_name, locator_name))
		{
			result = item;
			return true;
		}
	}
	return false;

	// FUNCTION BODY[0x63bfe0]: 10
	// <0x63bfec>|0x00c|+0x017:'72'
	// <0>
	// <0x63c003>|0x023|+0x008:'74'
	// <0x63c00b>|0x02b|+0x040:'75'
	// <0>
	// <0x63c04b>|0x06b|+0x00e:'77'
	// <0x63c059>|0x079|-0x015:'78'
	// <0>
	// <1>
	// <0x63c044>|0x064|+0x003:'81'
	// <0x63c047>|0x067|+0x015:'82'
	// ******
}

// claude@NOTE: no legacy ancestor - legacy render_model had only the retired one-at-a-time append_surface contract, no LOD-descriptor set_children; matcher-phase work.
// STATE[STUB]
void render_model::set_children( render_surface** children_in, u8 count, model_lods_descriptor* lods )
{
	// FUNCTION BODY[0x63bda0]: 6
	// <0x63bdcd>|0x02d|-0x027:'86'
	// <0x63bda6>|0x006|+0x00a:'87'
	// <0x63bdb0>|0x010|+0x015:'88'
	// <0>
	// <0x63bdc5>|0x025|+0x00b:'90'
	// <0x63bdd0>|0x030|+0x1fa:'91'
	// ******
}

material_effects& render_surface::get_material_effects( )
{
	// TODO: fix it!!!
	if (!m_materail_effects_instance.c_ptr())
	{
		return material::nomaterial_material( get_vertex_input_type() );
	}
	else
	{
		return m_materail_effects_instance->get_material_effects();
	}

	// FUNCTION BODY[0x63bd70]: 9
	// <0>
	// <0x63bd70>|0x000|+0x019:'97'
	// <0>
	// <0x63bd89>|0x019|-0x006:'99'
	// <0>
	// <1>
	// <2>
	// <0x63bd83>|0x013|+0x005:'103'
	// <0>
	// <0x63bd88>|0x018|+0x00b:'105'
	// ******
}

render_surface::~render_surface( )
{
	material_manager::ref().remove_material_effects(m_materail_effects_instance);

	// FUNCTION BODY[0x63c6e0]: 1
	// <0x63c6e0>|0x000|+0x01c:'109'
	// ******
}

void render_surface::set_default_material( )
{
	m_materail_effects_instance = 0;

	// FUNCTION BODY[0x63c5e0]: 1
	// <0x63c5e0>|0x000|+0x02c:'114'
	// ******
}

void render_surface::set_material_effects( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name )
{
	if (mtl_instance_ptr)
	{
		material_manager::ref().remove_material_effects(m_materail_effects_instance);

		m_materail_effects_instance	= mtl_instance_ptr;

		material_manager::ref().add_material_effects(
			m_materail_effects_instance,
			material_name
		);
	}

	// FUNCTION BODY[0x63c610]: 11
	// <0x63c610>|0x000|+0x01d:'119'
	// <0>
	// <0x63c62d>|0x01d|+0x015:'121'
	// <0>
	// <0x63c642>|0x032|+0x03f:'123'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63c681>|0x071|+0x027:'128'
	// <0>
	// ******
}

void render_surface::load( configs::binary_config_value const& properties, memory::chunk_reader& chunk )
{
	(void)&chunk;

	m_aabbox.max			= vostok::math::float3(properties["bounding_box"]["max"]);
	m_aabbox.min			= vostok::math::float3(properties["bounding_box"]["min"]);

	// claude@NOTE: target body (40 stmts, sphere_origin local) also derives m_bounding_sphere
	// and streaming-texture factor - evolved past this legacy seed; matcher-phase work.

	// LOCALS
	// float3 							sphere_origin
	// ******

	// STATICS
	// static < NoType > 				 = <0x63c228>;
	// static < NoType > 				 = <0x63c20c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1b0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1bf }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1ce }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1dd }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1ec }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62b1fb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x63c060]: 40
	// <0>
	// <1>
	// <0x63c065>|0x005|+0x027:'136'
	// <0x63c08c>|0x02c|+0x029:'137'
	// <0>
	// <0x63c0b5>|0x055|+0x010:'139'
	// <0>
	// <0x63c0c5>|0x065|+0x060:'141'
	// <0>
	// <0x63c125>|0x0c5|+0x05a:'143'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63c17f>|0x11f|+0x014:'148'
	// <0>
	// <0x63c193>|0x133|+0x00a:'150'
	// <0>
	// <0x63c19d>|0x13d|+0x013:'152'
	// <0>
	// <1>
	// <0x63c1b0>|0x150|+0x00f:'155'
	// <0>
	// <1>
	// <0x63c1bf>|0x15f|+0x00f:'158'
	// <0>
	// <1>
	// <0x63c1ce>|0x16e|+0x00f:'161'
	// <0>
	// <1>
	// <0x63c1dd>|0x17d|+0x00f:'164'
	// <0>
	// <1>
	// <2>
	// <0x63c1ec>|0x18c|+0x00f:'168'
	// <0>
	// <1>
	// <0x63c1fb>|0x19b|-0x044:'171'
	// <0>
	// <1>
	// <0x63c1b7>|0x157|+0x00f:'174'
	// <0x63c1c6>|0x166|+0x00f:'174'
	// <0x63c1d5>|0x175|+0x00f:'174'
	// <0x63c1e4>|0x184|+0x00f:'174'
	// <0x63c1f3>|0x193|+0x00f:'174'
	// ******
}

void render_surface_instance::set_constants( )
{
	m_parent->set_constants	( );

	// CALL SITE INFO
	// <0x63bd48> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x63bd40]: 0
	// ******
}

// claude@NOTE: no legacy ancestor - render_surface_instance::is_occluded postdates the legacy corpus; matcher-phase work.
// STATE[STUB]
bool render_surface_instance::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x63bd50]: 1
	// <0x63bd50>|0x000|+0x01a:'183'
	// <0x63bd6a>|0x01a|-0x001:'183'
	// <0x63bd69>|0x019|+0x003:'184'
	// ******
}

render_model_instance_impl::render_model_instance_impl( ) :
	m_collision_object( this ),
	m_transform( math::float4x4( ).identity( ) )
{
	// FUNCTION BODY[0x63c590]
	// ******
}

} // namespace render
} // namespace vostok
