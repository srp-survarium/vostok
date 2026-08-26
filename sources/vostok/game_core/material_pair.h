////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PAIR_H_INCLUDED
#define MATERIAL_PAIR_H_INCLUDED

namespace survarium {

class game_material;
class game_material_manager;

class material_pair : private boost::noncopyable {
public:
												material_pair	( );
														~material_pair	( );

			void										load_from_config( game_material_manager const& manager, configs::binary_config_value const& val );

	inline	game_material const*						first_material	( ) const { return m_first_material; }
	inline	game_material const*						second_material	( ) const { return m_second_material; }
	inline	resources::unmanaged_resource_ptr const&	sound			( ) const { return m_sound_emitter; }
	__declspec(noinline)
	inline	resources::unmanaged_resource_ptr const&	decal1			( ) const { return m_decal1; }
	__declspec(noinline)
	inline	float										decal1_size		( ) const { return m_decal1_size; }
	inline	resources::unmanaged_resource_ptr const&	decal2			( ) const { return m_decal2; }
	inline	float										decal2_size		( ) const { return m_decal2_size; }
	__declspec(noinline)
	inline	bool										has_particle	( ) const { return !m_particles.empty( ); }
			resources::unmanaged_resource_ptr const&	particle		( ) const;

	inline	void										set_decal1		( resources::unmanaged_resource_ptr decal ) { m_decal1 = decal; }
	inline	void										set_decal2		( resources::unmanaged_resource_ptr decal ) { m_decal2 = decal; }
	inline	void										set_sound		( resources::unmanaged_resource_ptr sound ) { m_sound_emitter = sound; }

			void										add_particle	( resources::unmanaged_resource_ptr particle );

	enum { particles_count = 8 };
	typedef vectora< resources::unmanaged_resource_ptr > particles_container;

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	particles_container							m_particles;
	/* 0x0010 */	resources::unmanaged_resource_ptr				m_sound_emitter;
	/* 0x0014 */	resources::unmanaged_resource_ptr				m_decal1;
	/* 0x0018 */	float											m_decal1_size;
	/* 0x001c */	resources::unmanaged_resource_ptr				m_decal2;
	/* 0x0020 */	float											m_decal2_size;
	/* 0x0024 */	game_material const*							m_first_material;
	/* 0x0028 */	game_material const*							m_second_material;
	/* 0x002c */	mutable u32										m_current_particle_idx;
}; // class material_pair

STATIC_SIZE_ASSERT(material_pair, 0x30);

} // namespace survarium

#endif // #ifndef MATERIAL_PAIR_H_INCLUDED
