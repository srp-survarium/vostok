////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef EMPTY_HANDS_H_INCLUDED
#define EMPTY_HANDS_H_INCLUDED

#include <vostok/game_core/interactive_object.h>
#include <vostok/resources_managed_resource.h>
#include <vostok/math_randoms_generator.h>	// math::random32

namespace survarium {

class empty_hands : public interactive_object {
public:
												empty_hands					( resources::managed_resource_ptr* animations, u32 animations_count );

	virtual	void								activate					( base_player& user, engine& engine ) override;

	virtual	void								deactivate					( ) override { /* no source */ }

	// STATE[STUB]
	virtual	float4x4							transform					( ) const override
	{
		return m_transform;	// buildability return

		// FUNCTION BODY[0xacc70]
		// <0xacc70>|0x000|      :'32'	{
		// ******
	}

	virtual	void								tick						( ) override { /* no source */ }

	// STATE[STUB]
	virtual	bool								is_ready_to_be_deactivated	( ) const override
	{
		return false;

		// FUNCTION BODY[0x22700]
		// <0x22700>|0x000|      :'34'	{
		// ******
	}

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const override;

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }

	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&			user_skeleton,
													float4x4* const							user_matrices,
													const u32								user_matrices_count,
													const u32								current_time_in_ms,
													float4x4&								character_head_transform,
													float4x4&								character_transform,
													animation::animation_player const&		animation_player
												) override;

	// STATE[STUB]
	virtual	bool								is_sprinting				( ) const override
	{
		return false;

		// FUNCTION BODY[0xb2740]
		// <0xb2740>|0x000|      :'47'	{
		// ******
	}

	// STATE[STUB]
	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override
	{
		// OTHER SYMBOLS
		// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
		// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
		// ******

		// FUNCTION BODY[0xaea00]
		// <0xaea00>|0x000|      :'49'	{
		// ******
	}

	// STATE[STUB]
	virtual	void								deserialize					( network_core::packet_reader& reader ) override
	{
		// OTHER SYMBOLS
		// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x111e20 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
		// ******

		// FUNCTION BODY[0x122e20]
		// <0x122e20>|0x000|      :'50'	{
		// ******
	}

	virtual										~empty_hands				( ) { /* no source */ }

	// void* empty_hands::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x607c0]: <0x4f210>|0x000|      :'31'	{

private:
	/* 0x0000 */	/* interactive_object */
	/* 0x0108 */	float4x4			m_transform;
	/* 0x0148 */	base_player*		m_user;
	/* 0x014c */	resources::managed_resource_ptr* const	m_animations;
	/* 0x0150 */	const u32			m_animations_count;
	/* 0x0154 */	math::random32		m_random;
}; // class empty_hands

STATIC_SIZE_ASSERT(empty_hands, 0x158);

} // namespace survarium

#endif // #ifndef EMPTY_HANDS_H_INCLUDED
