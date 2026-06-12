////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED
#define FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED

/* INCLUDES */
enum const survarium::fingers_to_weapon_corrector::hands_enum;
class vostok::animation::linear_interpolator;
class vostok::core::noncopyable;
struct survarium::fingers_to_weapon_corrector::hand;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::render::render_model_instance;

/* FORWARD REFS */
class vostok::animation::skeleton;

namespace survarium {

class fingers_to_weapon_corrector : public core::noncopyable {
public:
						fingers_to_weapon_corrector	( );

			void		activate					(
							animation::skeleton const&				character_skeleton,
							render::render_model_instance_ptr		weapon_model,
							const bool								first_person_view
						);

			void		process						( const u32 current_time_in_ms, float4x4* matrices ) const;

			void		activate_hand				(
							const fingers_to_weapon_corrector::hands_enum	arg_0 /* fingers_to_weapon_corrector::hands_enum hand */,
							const bool		is_active,
							const u32		current_time_in_ms
						);

			float		get_hand_coefficient		( const float hand_transition_time, const bool hand_active ) const;

			void		initialize_bones_indices	( animation::skeleton const& character_skeleton );
			void		initialize_locators			( render::render_model_instance const& weapon_model, const bool first_person_view );

	inline	void		set_first_person_view		( const bool arg_0 ) { /* no source */ }

	inline				~fingers_to_weapon_corrector( ) { /* no source */ }

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	fingers_to_weapon_corrector::hand		m_hands[2];
	/* 0x0808 */	animation::linear_interpolator			m_interpolator;
	/* 0x0810 */	render::render_model_instance_ptr		m_weapon_model;
	/* 0x0814 */	bool									m_first_person_view;
}; // class fingers_to_weapon_corrector

STATIC_SIZE_ASSERT(fingers_to_weapon_corrector, 0x818);

} // namespace survarium

#endif // #ifndef FINGERS_TO_WEAPON_CORRECTOR_H_INCLUDED
