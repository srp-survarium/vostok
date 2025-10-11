////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core.h"

namespace survarium {

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_cc''()
void `dynamic initializer for 's_ik_use_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_ik_use_on_legs_cc''()
void `dynamic initializer for 's_ik_use_on_legs_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_vertical_eanble_cc''()
void `dynamic initializer for 's_recoil_vertical_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_horizontal_eanble_cc''()
void `dynamic initializer for 's_recoil_horizontal_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_back_eanble_cc''()
void `dynamic initializer for 's_recoil_back_eanble_cc''( )
{
}

// STATE[STUB]
// void survarium::`dynamic initializer for 's_recoil_enable_cc''()
void `dynamic initializer for 's_recoil_enable_cc''( )
{
}

// STATE[STUB]
// survarium::weapon_core::weapon_core()
weapon_core::weapon_core( )
{
}

// STATE[STUB]
// void survarium::weapon_core::~weapon_core()
void weapon_core::~weapon_core( )
{
	// LOCALS
	// vostok::ai::fsm_state* 		state<1>
	// ******

	// FUNCTION BODY
	// <0x5a4ce2>|0x000|0x000:'100'
	// <0x5a4cf5>|0x013|0x013:'101'
	// <0x5a4d03>|0x021|0x00e|[1]:'102'
	// 1
	// 2
	// <0x5a4d1c>|0x03a|0x019:'105'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_magazine_capacity(unsigned short)
void weapon_core::set_magazine_capacity( u16 magazine_capacity )
{
	// FUNCTION BODY
	// <0x5a3139>|0x000|0x000:'110'
	// 1
	// <0x5a3145>|0x00c|0x00c:'112'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::initialize_weapon_logic(vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&, vostok::resources::resource_ptr<survarium::weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&)
void weapon_core::initialize_weapon_logic(
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	inactive_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	show_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	hide_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	idle_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	reload_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	fire_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	aim_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	aim_fire_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	chamber_a_round_state,
	vostok::resources::resource_ptr<weapon_core_base_state,vostok::resources::unmanaged_intrusive_base> const&	chamber_a_round_aimed_state)
{
	// LOCALS
	// weapon_core_base_state* 		idle
	// weapon_core_base_state* 		aim_fire
	// weapon_core_base_state* 		reload
	// weapon_core_base_state* 		hide
	// weapon_core_base_state* 		fire
	// weapon_core_base_state* 		aim
	// weapon_core_base_state* 		chamber_a_round
	// weapon_core_base_state* 		inactive
	// weapon_core_base_state* 		show
	// weapon_core_base_state* 		chamber_a_round_aimed
	// ******

	// FUNCTION BODY
	// 1
	// <0x5a7034>|0x000|0x000:'128'
	// <0x5a7057>|0x023|0x023:'129'
	// 1
	// <0x5a7063>|0x02f|0x00c:'131'
	// <0x5a7078>|0x044|0x015:'132'
	// <0x5a708d>|0x059|0x015:'133'
	// <0x5a70a2>|0x06e|0x015:'134'
	// <0x5a70b7>|0x083|0x015:'135'
	// <0x5a70cc>|0x098|0x015:'136'
	// <0x5a70e1>|0x0ad|0x015:'137'
	// <0x5a70f6>|0x0c2|0x015:'138'
	// 1
	// <0x5a710b>|0x0d7|0x015:'140'
	// <0x5a711c>|0x0e8|0x011:'141'
	// 1
	// <0x5a7131>|0x0fd|0x015:'143'
	// <0x5a7158>|0x124|0x027:'144'
	// 1
	// <0x5a716d>|0x139|0x015:'146'
	// <0x5a7175>|0x141|0x008:'147'
	// <0x5a717d>|0x149|0x008:'148'
	// <0x5a7185>|0x151|0x008:'149'
	// <0x5a718d>|0x159|0x008:'150'
	// <0x5a7195>|0x161|0x008:'151'
	// <0x5a719d>|0x169|0x008:'152'
	// <0x5a71a5>|0x171|0x008:'153'
	// <0x5a71ad>|0x179|0x008:'154'
	// 1
	// 2
	// <0x5a71b5>|0x181|0x008:'157'
	// 1
	// <0x5a71bd>|0x189|0x008:'159'
	// <0x5a71d2>|0x19e|0x015:'160'
	// 1
	// <0x5a71e7>|0x1b3|0x015:'162'
	// <0x5a71fc>|0x1c8|0x015:'163'
	// <0x5a7211>|0x1dd|0x015:'164'
	// <0x5a7226>|0x1f2|0x015:'165'
	// <0x5a723b>|0x207|0x015:'166'
	// <0x5a7250>|0x21c|0x015:'167'
	// <0x5a7265>|0x231|0x015:'168'
	// <0x5a727a>|0x246|0x015:'169'
	// <0x5a728f>|0x25b|0x015:'170'
	// <0x5a72a0>|0x26c|0x011:'171'
	// <0x5a72b5>|0x281|0x015:'172'
	// <0x5a72bb>|0x287|0x006:'173'
	// 1
	// <0x5a72d0>|0x29c|0x015:'175'
	// <0x5a737b>|0x347|0x0ab:'176'
	// <0x5a7441>|0x40d|0x0c6:'177'
	// <0x5a7507>|0x4d3|0x0c6:'178'
	// <0x5a75cd>|0x599|0x0c6:'179'
	// 1
	// <0x5a7693>|0x65f|0x0c6:'181'
	// <0x5a7759>|0x725|0x0c6:'182'
	// <0x5a7763>|0x72f|0x00a:'183'
	// <0x5a7827>|0x7f3|0x0c4:'184'
	// <0x5a783c>|0x808|0x015:'185'
	// <0x5a7900>|0x8cc|0x0c4:'186'
	// <0x5a79c4>|0x990|0x0c4:'187'
	// <0x5a7a8a>|0xa56|0x0c6:'188'
	// <0x5a7b50>|0xb1c|0x0c6:'189'
	// <0x5a7c16>|0xbe2|0x0c6:'190'
	// <0x5a7cdc>|0xca8|0x0c6:'191'
	// 1
	// <0x5a7da2>|0xd6e|0x0c6:'193'
	// <0x5a7e68>|0xe34|0x0c6:'194'
	// <0x5a7f2e>|0xefa|0x0c6:'195'
	// <0x5a7ff4>|0xfc0|0x0c6:'196'
	// <0x5a80ba>|0x1086|0x0c6:'197'
	// <0x5a8180>|0x114c|0x0c6:'198'
	// 1
	// <0x5a8246>|0x1212|0x0c6:'200'
	// <0x5a830c>|0x12d8|0x0c6:'201'
	// <0x5a8316>|0x12e2|0x00a:'202'
	// <0x5a83da>|0x13a6|0x0c4:'203'
	// <0x5a83ef>|0x13bb|0x015:'204'
	// <0x5a84b3>|0x147f|0x0c4:'205'
	// <0x5a8579>|0x1545|0x0c6:'206'
	// <0x5a863f>|0x160b|0x0c6:'207'
	// <0x5a8705>|0x16d1|0x0c6:'208'
	// 1
	// <0x5a87cb>|0x1797|0x0c6:'210'
	// <0x5a8891>|0x185d|0x0c6:'211'
	// <0x5a88bb>|0x1887|0x02a:'212'
	// <0x5a897f>|0x194b|0x0c4:'213'
	// <0x5a8a45>|0x1a11|0x0c6:'214'
	// <0x5a8b09>|0x1ad5|0x0c4:'215'
	// <0x5a8bcf>|0x1b9b|0x0c6:'216'
	// <0x5a8c95>|0x1c61|0x0c6:'217'
	// 1
	// <0x5a8d5b>|0x1d27|0x0c6:'219'
	// <0x5a8e21>|0x1ded|0x0c6:'220'
	// <0x5a8e2b>|0x1df7|0x00a:'221'
	// <0x5a8eef>|0x1ebb|0x0c4:'222'
	// <0x5a8f04>|0x1ed0|0x015:'223'
	// <0x5a8fc8>|0x1f94|0x0c4:'224'
	// <0x5a908c>|0x2058|0x0c4:'225'
	// <0x5a9152>|0x211e|0x0c6:'226'
	// <0x5a9218>|0x21e4|0x0c6:'227'
	// <0x5a92de>|0x22aa|0x0c6:'228'
	// <0x5a93a4>|0x2370|0x0c6:'229'
	// 1
	// <0x5a9468>|0x2434|0x0c4:'231'
	// <0x5a952e>|0x24fa|0x0c6:'232'
	// <0x5a95f4>|0x25c0|0x0c6:'233'
	// <0x5a96b8>|0x2684|0x0c4:'234'
	// <0x5a977e>|0x274a|0x0c6:'235'
	// <0x5a9844>|0x2810|0x0c6:'236'
	// 1
	// <0x5a990a>|0x28d6|0x0c6:'238'
	// <0x5a99d0>|0x299c|0x0c6:'239'
	// <0x5a99da>|0x29a6|0x00a:'240'
	// <0x5a9a9e>|0x2a6a|0x0c4:'241'
	// <0x5a9ab3>|0x2a7f|0x015:'242'
	// <0x5a9b77>|0x2b43|0x0c4:'243'
	// <0x5a9c3b>|0x2c07|0x0c4:'244'
	// <0x5a9d01>|0x2ccd|0x0c6:'245'
	// <0x5a9dc7>|0x2d93|0x0c6:'246'
	// <0x5a9e8d>|0x2e59|0x0c6:'247'
	// <0x5a9f17>|0x2ee3|0x08a:'248'
	// 1
	// <0x5a9f9c>|0x2f68|0x085:'250'
	// 1
	// <0x5a9fb1>|0x2f7d|0x015:'252'
	// <0x5aa03b>|0x3007|0x08a:'253'
	// <0x5aa0c5>|0x3091|0x08a:'254'
	// <0x5aa14a>|0x3116|0x085:'255'
	// <0x5aa1d4>|0x31a0|0x08a:'256'
	// <0x5aa25e>|0x322a|0x08a:'257'
	// <0x5aa2e8>|0x32b4|0x08a:'258'
	// <0x5aa2f2>|0x32be|0x00a:'259'
	// 1
	// 2
	// <0x5aa377>|0x3343|0x085:'262'
	// 1
	// <0x5aa381>|0x334d|0x00a:'264'
	// <0x5aa40b>|0x33d7|0x08a:'265'
	// <0x5aa495>|0x3461|0x08a:'266'
	// <0x5aa51a>|0x34e6|0x085:'267'
	// <0x5aa5a4>|0x3570|0x08a:'268'
	// <0x5aa62e>|0x35fa|0x08a:'269'
	// <0x5aa6b8>|0x3684|0x08a:'270'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_skeleton(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&)
void weapon_core::set_skeleton( vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const& skeleton )
{
	// FUNCTION BODY
	// <0x5a42fa>|0x000|0x000:'276'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::target_and_animation_ended_predicate(const survarium::weapon_targets) const
bool weapon_core::target_and_animation_ended_predicate( weapon_targets target ) const
{
	return false;
	// FUNCTION BODY
	// <0xbc609>|0x000|0x000:'286'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core::get_weapon_and_hands_animation_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core::get_weapon_and_hands_animation_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				weapon_user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5927a4 }, type_index: TypeIndex(0x66ef) })
	// ******

	// FUNCTION BODY
	// <0x5a3769>|0x000|0x000:'291'
	// ******
}

// STATE[STUB]
// vostok::animation::body_part_masks_enum survarium::weapon_core::get_body_part_mask_for_user() const
vostok::animation::body_part_masks_enum weapon_core::get_body_part_mask_for_user( ) const
{
	// FUNCTION BODY
	// <0x5a3459>|0x000|0x000:'296'
	// ******
}

// STATE[STUB]
// void survarium::`dynamic initializer for 'epsilon''()
void `dynamic initializer for 'epsilon''( )
{
}

// STATE[STUB]
// float survarium::weapon_core::horizontal_recoil_value() const
float weapon_core::horizontal_recoil_value( ) const
{
	// LOCALS
	// float 						result
	// float 						total_horizontal_coeff
	// ******

	return 0.0f;
	// FUNCTION BODY
	// 1
	// 2
	// <0xbc669>|0x000|0x000:'312'
	// <0xbc6b9>|0x050|0x050:'313'
	// <0xbc704>|0x09b|0x04b:'314'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::vertical_recoil_value() const
float weapon_core::vertical_recoil_value( ) const
{
	// LOCALS
	// float 						result
	// float 						total_vertical_coeff
	// ******

	return 0.0f;
	// FUNCTION BODY
	// 1
	// 2
	// <0xbc719>|0x000|0x000:'321'
	// 1
	// <0xbc769>|0x050|0x050:'323'
	// <0xbc7b0>|0x097|0x047:'324'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core::selected_animations(vostok::mutable_buffer&, const bool) const
vostok::animation::mixing::expression weapon_core::selected_animations( vostok::mutable_buffer& buffer, bool is_third_view ) const
{
	// LOCALS
	// vostok::animation::mixing::expression const& expression_for_weapon_and_hands
	// std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> result
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x5a3a50>|0x000|0x000:'332'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// <0x5a3a5f>|0x00f|0x00f:'346'
	// 1
	// <0x5a3c1c>|0x1cc|0x1bd:'348'
	// <0x5a3c5c>|0x20c|0x040:'349'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::update_recoil(unsigned int, const float)
void weapon_core::update_recoil( u32 current_time_in_ms, float time_scale )
{
	// FUNCTION BODY
	// <0x5a3717>|0x000|0x000:'354'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::update_dispersion(const bool, unsigned int)
void weapon_core::update_dispersion( bool is_moving, u32 current_time_in_ms )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5923dc }, type_index: TypeIndex(0x916d) })
	// ******

	// FUNCTION BODY
	// <0x5a33b9>|0x000|0x000:'359'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x5a33c5>|0x00c|0x00c:'367'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::update_breath_vibration(const bool, unsigned int, const float)
void weapon_core::update_breath_vibration( bool is_holding_breath, u32 current_time_in_ms, float time_scale )
{
	// FUNCTION BODY
	// <0x5a39f7>|0x000|0x000:'372'
	// <0x5a3a0a>|0x013|0x013:'373'
	// <0x5a3a1d>|0x026|0x013:'374'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::tick()
void weapon_core::tick( )
{
	// LOCALS
	// player_input const& 			input
	// vostok::resources::resource_ptr<interactive_object,vostok::resources::unmanaged_intrusive_base> target_active_object
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5948c2 }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594978 }, type_index: TypeIndex(0x6700) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594998 }, type_index: TypeIndex(0x6700) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5949d6 }, type_index: TypeIndex(0x6747) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594a05 }, type_index: TypeIndex(0x6747) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594a38 }, type_index: TypeIndex(0x6747) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594a57 }, type_index: TypeIndex(0x6747) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594a7c }, type_index: TypeIndex(0x6747) })
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x5a58a9>|0x000|0x000:'381'
	// <0x5a58c7>|0x01e|0x01e:'382'
	// 1
	// <0x5a58e1>|0x038|0x01a:'384'
	// 1
	// <0x5a58f0>|0x047|0x00f:'386'
	// 1
	// <0x5a5960>|0x0b7|0x070:'388'
	// <0x5a596d>|0x0c4|0x00d:'389'
	// <0x5a597c>|0x0d3|0x00f:'390'
	// <0x5a598a>|0x0e1|0x00e:'391'
	// <0x5a599a>|0x0f1|0x010:'392'
	// <0x5a599c>|0x0f3|0x002:'393'
	// 1
	// 2
	// <0x5a59a4>|0x0fb|0x008:'396'
	// 1
	// <0x5a59c6>|0x11d|0x022:'398'
	// <0x5a59dd>|0x134|0x017:'399'
	// <0x5a59f5>|0x14c|0x018:'400'
	// <0x5a5a09>|0x160|0x014:'401'
	// <0x5a5a14>|0x16b|0x00b:'402'
	// <0x5a5a3c>|0x193|0x028:'403'
	// <0x5a5a47>|0x19e|0x00b:'404'
	// <0x5a5a59>|0x1b0|0x012:'405'
	// <0x5a5a5b>|0x1b2|0x002:'406'
	// 1
	// 2
	// <0x5a5a7e>|0x1d5|0x023:'409'
	// 1
	// 2
	// 3
	// <0x5a5a8c>|0x1e3|0x00e:'413'
	// 1
	// <0x5a5a9a>|0x1f1|0x00e:'415'
	// <0x5a5aa9>|0x200|0x00f:'416'
	// 1
	// 2
	// <0x5a5ab3>|0x20a|0x00a:'419'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_show()
void weapon_core::instant_show( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x591c8f }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a2c77>|0x000|0x000:'424'
	// <0x5a2c81>|0x00a|0x00a:'425'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_hide()
void weapon_core::instant_hide( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x591c65 }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a2c57>|0x000|0x000:'430'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::load_magazine()
void weapon_core::load_magazine( )
{
	// LOCALS
	// u16 							load
	// u16 							amount
	// ******

	// FUNCTION BODY
	// <0x5a4b99>|0x000|0x000:'435'
	// <0x5a4bd7>|0x03e|0x03e:'436'
	// <0x5a4c17>|0x07e|0x040:'437'
	// <0x5a4c96>|0x0fd|0x07f:'438'
	// 1
	// <0x5a4cb0>|0x117|0x01a:'440'
	// <0x5a4cbe>|0x125|0x00e:'441'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::chamber_a_round()
void weapon_core::chamber_a_round( )
{
	// FUNCTION BODY
	// <0x5a30d9>|0x000|0x000:'446'
	// <0x5a30e5>|0x00c|0x00c:'447'
	// <0x5a30f1>|0x018|0x00c:'448'
	// 1
	// <0x5a30fd>|0x024|0x00c:'450'
	// <0x5a3115>|0x03c|0x018:'451'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::unload_ammo()
void weapon_core::unload_ammo( )
{
	// LOCALS
	// u16 							ammo_to_add
	// ******

	// FUNCTION BODY
	// <0x5a4a49>|0x000|0x000:'456'
	// <0x5a4a7d>|0x034|0x034:'457'
	// 1
	// <0x5a4a82>|0x039|0x005:'459'
	// <0x5a4a90>|0x047|0x00e:'460'
	// <0x5a4a9c>|0x053|0x00c:'461'
	// 1
	// <0x5a4aaa>|0x061|0x00e:'463'
	// <0x5a4ab6>|0x06d|0x00c:'464'
	// 1
	// 2
	// <0x5a4ac0>|0x077|0x00a:'467'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_reload()
void weapon_core::instant_reload( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594867 }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a5847>|0x000|0x000:'473'
	// 1
	// <0x5a5851>|0x00a|0x00a:'475'
	// 1
	// <0x5a5859>|0x012|0x008:'477'
	// 1
	// <0x5a5869>|0x022|0x010:'479'
	// <0x5a5877>|0x030|0x00e:'480'
	// 1
	// <0x5a5885>|0x03e|0x00e:'482'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_chamber_a_round()
void weapon_core::instant_chamber_a_round( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592391 }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a3359>|0x000|0x000:'487'
	// <0x5a3365>|0x00c|0x00c:'488'
	// 1
	// <0x5a3371>|0x018|0x00c:'490'
	// 1
	// <0x5a337b>|0x022|0x00a:'492'
	// 1
	// <0x5a3383>|0x02a|0x008:'494'
	// 1
	// <0x5a3393>|0x03a|0x010:'496'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::reload_one_round()
void weapon_core::reload_one_round( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x593a2f }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a48c0>|0x000|0x000:'502'
	// <0x5a494c>|0x08c|0x08c:'503'
	// <0x5a4964>|0x0a4|0x018:'504'
	// 1
	// <0x5a4a21>|0x161|0x0bd:'506'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_aim_start()
void weapon_core::instant_aim_start( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5920a6 }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x5a3079>|0x000|0x000:'511'
	// <0x5a30b0>|0x037|0x037:'512'
	// 1
	// <0x5a30b8>|0x03f|0x008:'514'
	// <0x5a30c2>|0x049|0x00a:'515'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_aim_end()
void weapon_core::instant_aim_end( )
{
	// FUNCTION BODY
	// <0x5a2c37>|0x000|0x000:'520'
	// <0x5a2c41>|0x00a|0x00a:'521'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::weapon_core::get_dispersed_bullet_dir()
vostok::math::float3 weapon_core::get_dispersed_bullet_dir( )
{
	// LOCALS
	// float 						dispersion_angle
	// float 						dispersion_amount
	// vostok::math::float3 const& 	rot_axis
	// float 						random_k
	// vostok::math::float3 		bullet_direction
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// 1
	// <0x5a4792>|0x000|0x000:'527'
	// <0x5a47d2>|0x040|0x040:'528'
	// <0x5a47e9>|0x057|0x017:'529'
	// <0x5a4807>|0x075|0x01e:'530'
	// <0x5a484e>|0x0bc|0x047:'531'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x5a488c>|0x0fa|0x03e:'539'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::get_dispersion() const
float weapon_core::get_dispersion( ) const
{
	return 0.0f;
	// FUNCTION BODY
	// <0x5a4767>|0x000|0x000:'544'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_fire(const unsigned int)
void weapon_core::instant_fire( u32 current_time_in_ms )
{
	// LOCALS
	// s32 							i<1>
	// vostok::math::float3 const& 	velocity<2>
	// vostok::math::float3 const& 	bullet_direction<2>
	// ******

	// SKIPPED BLOCKS
	// <0x5a56e4><2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5947f9 }, type_index: TypeIndex(0x6700) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59482e }, type_index: TypeIndex(0x91d9) })
	// ******

	// FUNCTION BODY
	// <0x5a5669>|0x000|0x000:'549'
	// <0x5a5675>|0x00c|0x00c:'550'
	// <0x5a5681>|0x018|0x00c:'551'
	// 1
	// <0x5a5699>|0x030|0x018:'553'
	// <0x5a56a5>|0x03c|0x00c|[1]:'554'
	// <0x5a56ea>|0x081|0x045:'555'
	// 1
	// <0x5a56fc>|0x093|0x012:'557'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// <0x5a5746>|0x0dd|0x04a:'568'
	// <0x5a57b4>|0x14b|0x06e:'569'
	// 1
	// <0x5a57b9>|0x150|0x005:'571'
	// <0x5a57c7>|0x15e|0x00e:'572'
	// <0x5a57d1>|0x168|0x00a:'573'
	// <0x5a57d3>|0x16a|0x002:'574'
	// 1
	// 2
	// 3
	// <0x5a57eb>|0x182|0x018:'578'
	// 1
	// <0x5a57fb>|0x192|0x010:'580'
	// <0x5a5809>|0x1a0|0x00e:'581'
	// 1
	// <0x5a5817>|0x1ae|0x00e:'583'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_fire_bullet_transform(vostok::math::float4x4 const&)
void weapon_core::set_fire_bullet_transform( vostok::math::float4x4 const& fire_bullet_transform )
{
	// FUNCTION BODY
	// <0x5a2c09>|0x000|0x000:'588'
	// <0x5a2c13>|0x00a|0x00a:'589'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::on_sprint_animation_ended(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum weapon_core::on_sprint_animation_ended( vostok::animation::animation_callback_params& params )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59202c }, type_index: TypeIndex(0x9185) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592052 }, type_index: TypeIndex(0x9185) })
	// ******

	// FUNCTION BODY
	// <0x5a3009>|0x000|0x000:'594'
	// 1
	// 2
	// 3
	// 4
	// <0x5a3010>|0x007|0x007:'599'
	// 1
	// 2
	// 3
	// <0x5a302e>|0x025|0x01e:'603'
	// 1
	// 2
	// 3
	// <0x5a3054>|0x04b|0x026:'607'
	// 1
	// <0x5a305e>|0x055|0x00a:'609'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_target(const survarium::weapon_targets)
void weapon_core::set_target( weapon_targets target )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59421c }, type_index: TypeIndex(0x671f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594350 }, type_index: TypeIndex(0x9182) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594487 }, type_index: TypeIndex(0x9182) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59460e }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a518a>|0x000|0x000:'614'
	// 1
	// <0x5a5196>|0x00c|0x00c:'616'
	// <0x5a51bf>|0x035|0x029:'617'
	// <0x5a51c8>|0x03e|0x009:'618'
	// 1
	// <0x5a51da>|0x050|0x012:'620'
	// <0x5a51e0>|0x056|0x006:'621'
	// <0x5a51e9>|0x05f|0x009:'622'
	// <0x5a51ef>|0x065|0x006:'623'
	// 1
	// 2
	// 3
	// <0x5a51f6>|0x06c|0x007:'627'
	// <0x5a520b>|0x081|0x015:'628'
	// <0x5a5229>|0x09f|0x01e:'629'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x5a5236>|0x0ac|0x00d:'636'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x5a5362>|0x1d8|0x12c:'643'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a5499>|0x30f|0x137:'649'
	// <0x5a54c2>|0x338|0x029:'650'
	// <0x5a54c8>|0x33e|0x006:'651'
	// <0x5a54d1>|0x347|0x009:'652'
	// <0x5a54d7>|0x34d|0x006:'653'
	// 1
	// 2
	// <0x5a54de>|0x354|0x007:'656'
	// 1
	// <0x5a54e8>|0x35e|0x00a:'658'
	// 1
	// <0x5a54fe>|0x374|0x016:'660'
	// <0x5a55fa>|0x470|0x0fc:'661'
	// 1
	// <0x5a5610>|0x486|0x016:'663'
	// <0x5a562e>|0x4a4|0x01e:'664'
	// <0x5a5635>|0x4ab|0x007:'665'
	// <0x5a5637>|0x4ad|0x002:'666'
	// 1
	// 2
	// 3
	// <0x5a563e>|0x4b4|0x007:'670'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::reset_fire_queue()
void weapon_core::reset_fire_queue( )
{
	// FUNCTION BODY
	// 1
	// 2
	// <0x5a2f49>|0x000|0x000:'677'
	// 1
	// <0x5a2f5b>|0x012|0x012:'679'
	// <0x5a2f6f>|0x026|0x014:'680'
	// <0x5a2f7d>|0x034|0x00e:'681'
	// 1
	// <0x5a2f95>|0x04c|0x018:'683'
	// <0x5a2f97>|0x04e|0x002:'684'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_next_fire_queue_type()
void weapon_core::set_next_fire_queue_type( )
{
	// FUNCTION BODY
	// <0x5a2bb7>|0x000|0x000:'691'
	// <0x5a2bd2>|0x01b|0x01b:'692'
	// <0x5a2bdc>|0x025|0x00a:'693'
	// <0x5a2bde>|0x027|0x002:'694'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_next_ammo_type()
void weapon_core::set_next_ammo_type( )
{
	// LOCALS
	// profile_slot_enum 			next_slot
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x5a5089>|0x000|0x000:'702'
	// 1
	// <0x5a5090>|0x007|0x007:'704'
	// 1
	// <0x5a50a5>|0x01c|0x015:'706'
	// 1
	// <0x5a50b4>|0x02b|0x00f:'708'
	// 1
	// <0x5a50c9>|0x040|0x015:'710'
	// 1
	// <0x5a50d6>|0x04d|0x00d:'712'
	// <0x5a50d8>|0x04f|0x002:'713'
	// 1
	// <0x5a50dd>|0x054|0x005:'715'
	// <0x5a50fd>|0x074|0x020:'716'
	// 1
	// <0x5a50ff>|0x076|0x002:'718'
	// 1
	// <0x5a5107>|0x07e|0x008:'720'
	// <0x5a5113>|0x08a|0x00c:'721'
	// <0x5a5158>|0x0cf|0x045:'722'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_ammunition(vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&)
void weapon_core::set_ammunition( vostok::resources::resource_ptr<weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const& ammunition_to_set )
{
	// FUNCTION BODY
	// <0x5a4739>|0x000|0x000:'727'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::load_ammo()
void weapon_core::load_ammo( )
{
	// FUNCTION BODY
	// <0x5a4ea0>|0x000|0x000:'732'
	// 1
	// <0x5a4f2f>|0x08f|0x08f:'734'
	// <0x5a4f3d>|0x09d|0x00e:'735'
	// 1
	// <0x5a4f45>|0x0a5|0x008:'737'
	// 1
	// <0x5a4f69>|0x0c9|0x024:'739'
	// 1
	// <0x5a4f75>|0x0d5|0x00c:'741'
	// 1
	// <0x5a4fbf>|0x11f|0x04a:'743'
	// <0x5a4fc9>|0x129|0x00a:'744'
	// 1
	// <0x5a5045>|0x1a5|0x07c:'746'
	// 1
	// <0x5a5055>|0x1b5|0x010:'748'
	// <0x5a505f>|0x1bf|0x00a:'749'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::on_reload_started()
void weapon_core::on_reload_started( )
{
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::on_animation_ik_interval(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum weapon_core::on_animation_ik_interval( vostok::animation::animation_callback_params& params )
{
	// FUNCTION BODY
	// 1
	// <0x5a35f9>|0x000|0x000:'762'
	// 1
	// <0x5a3600>|0x007|0x007:'764'
	// 1
	// <0x5a3614>|0x01b|0x014:'766'
	// <0x5a362b>|0x032|0x017:'767'
	// <0x5a3652>|0x059|0x027:'768'
	// <0x5a3669>|0x070|0x017:'769'
	// <0x5a368d>|0x094|0x024:'770'
	// <0x5a36a4>|0x0ab|0x017:'771'
	// <0x5a36c8>|0x0cf|0x024:'772'
	// <0x5a36df>|0x0e6|0x017:'773'
	// 1
	// <0x5a3701>|0x108|0x022:'775'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_animation_callback(char const*, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_core::set_animation_callback( pcstr channel_id, void const* callback_uid, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const& animation_callback )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5932d3 }, type_index: TypeIndex(0x9183) })
	// ******

	// FUNCTION BODY
	// <0x5a4299>|0x000|0x000:'780'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::remove_animation_callback(char const*, void const*)
void weapon_core::remove_animation_callback( pcstr channel_id, void const* callback_uid )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x591b96 }, type_index: TypeIndex(0x9186) })
	// ******

	// FUNCTION BODY
	// <0x5a2b77>|0x000|0x000:'785'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const&)
void weapon_core::set_animation_callback( vostok::animation::reserved_channel_ids_enum channel_id, void const* callback_uid, boost::function<enum vostok::animation::callback_return_type_enum __cdecl(vostok::animation::animation_callback_params &)> const& animation_callback )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59327e }, type_index: TypeIndex(0x9182) })
	// ******

	// FUNCTION BODY
	// <0x5a4249>|0x000|0x000:'790'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::remove_animation_callback(vostok::animation::reserved_channel_ids_enum, void const*)
void weapon_core::remove_animation_callback( vostok::animation::reserved_channel_ids_enum channel_id, void const* callback_uid )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x591b66 }, type_index: TypeIndex(0x9185) })
	// ******

	// FUNCTION BODY
	// <0x5a2b47>|0x000|0x000:'795'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::activate(survarium::base_player&, survarium::engine&)
void weapon_core::activate( base_player& user, engine& engine )
{
	// LOCALS
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> const& ammo1
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> const& ammo2
	// profile_slot_enum 			ammo1_slot
	// profile_slot_enum 			ammo2_slot
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594fe1 }, type_index: TypeIndex(0x92c9) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5950b5 }, type_index: TypeIndex(0xa8e7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5950db }, type_index: TypeIndex(0xa8e7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5950f2 }, type_index: TypeIndex(0x6727) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5951cb }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5952aa }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5953a1 }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59549e }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5955a0 }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5956a2 }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5956d6 }, type_index: TypeIndex(0x916e) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5956f5 }, type_index: TypeIndex(0x916e) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x595726 }, type_index: TypeIndex(0x9173) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x595ce3 }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x595dec }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x595ef5 }, type_index: TypeIndex(0x9183) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x595ffe }, type_index: TypeIndex(0x9183) })
	// ******

	// FUNCTION BODY
	// <0x5a5f6b>|0x000|0x000:'800'
	// <0x5a5f85>|0x01a|0x01a:'801'
	// <0x5a5fa0>|0x035|0x01b:'802'
	// <0x5a5faf>|0x044|0x00f:'803'
	// <0x5a5fca>|0x05f|0x01b:'804'
	// <0x5a5fd7>|0x06c|0x00d:'805'
	// <0x5a5fef>|0x084|0x018:'806'
	// <0x5a5ffc>|0x091|0x00d:'807'
	// <0x5a6009>|0x09e|0x00d:'808'
	// <0x5a6039>|0x0ce|0x030:'809'
	// <0x5a6069>|0x0fe|0x030:'810'
	// 1
	// <0x5a6076>|0x10b|0x00d:'812'
	// <0x5a608e>|0x123|0x018:'813'
	// 1
	// 2
	// <0x5a60a6>|0x13b|0x018:'816'
	// <0x5a60cc>|0x161|0x026:'817'
	// 1
	// <0x5a60f4>|0x189|0x028:'819'
	// 1
	// <0x5a6103>|0x198|0x00f:'821'
	// <0x5a61dd>|0x272|0x0da:'822'
	// <0x5a62bc>|0x351|0x0df:'823'
	// <0x5a63b9>|0x44e|0x0fd:'824'
	// <0x5a64b6>|0x54b|0x0fd:'825'
	// <0x5a65b8>|0x64d|0x102:'826'
	// <0x5a66ba>|0x74f|0x102:'827'
	// <0x5a66ea>|0x77f|0x030:'828'
	// 1
	// <0x5a6709>|0x79e|0x01f:'830'
	// 1
	// 2
	// <0x5a6740>|0x7d5|0x037:'833'
	// 1
	// 2
	// <0x5a68af>|0x944|0x16f:'836'
	// <0x5a68d7>|0x96c|0x028:'837'
	// 1
	// 2
	// <0x5a68e8>|0x97d|0x011:'840'
	// <0x5a68f8>|0x98d|0x010:'841'
	// 1
	// <0x5a6908>|0x99d|0x010:'843'
	// <0x5a691f>|0x9b4|0x017:'844'
	// <0x5a6936>|0x9cb|0x017:'845'
	// <0x5a6942>|0x9d7|0x00c:'846'
	// 1
	// <0x5a6995>|0xa2a|0x053:'848'
	// <0x5a69a6>|0xa3b|0x011:'849'
	// 1
	// <0x5a69f9>|0xa8e|0x053:'851'
	// <0x5a69fe>|0xa93|0x005:'852'
	// <0x5a6a8c>|0xb21|0x08e:'853'
	// 1
	// 2
	// <0x5a6b1e>|0xbb3|0x092:'856'
	// 1
	// <0x5a6b9f>|0xc34|0x081:'858'
	// 1
	// <0x5a6bb0>|0xc45|0x011:'860'
	// <0x5a6bbb>|0xc50|0x00b:'861'
	// 1
	// 2
	// <0x5a6bc8>|0xc5d|0x00d:'864'
	// <0x5a6bd3>|0xc68|0x00b:'865'
	// 1
	// <0x5a6bde>|0xc73|0x00b:'867'
	// 1
	// <0x5a6bed>|0xc82|0x00f:'869'
	// <0x5a6cfb>|0xd90|0x10e:'870'
	// <0x5a6e04>|0xe99|0x109:'871'
	// <0x5a6f0d>|0xfa2|0x109:'872'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::deactivate()
void weapon_core::deactivate( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59304c }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59306d }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59308e }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5930af }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x593116 }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x593138 }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59315a }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59317c }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x59319e }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5931c0 }, type_index: TypeIndex(0x9186) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5931ec }, type_index: TypeIndex(0x9185) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x593212 }, type_index: TypeIndex(0x9185) })
	// ******

	// FUNCTION BODY
	// <0x5a4019>|0x000|0x000:'878'
	// 1
	// <0x5a4028>|0x00f|0x00f:'880'
	// <0x5a404e>|0x035|0x026:'881'
	// <0x5a406f>|0x056|0x021:'882'
	// <0x5a4090>|0x077|0x021:'883'
	// 1
	// 2
	// <0x5a40b1>|0x098|0x021:'886'
	// <0x5a40c1>|0x0a8|0x010:'887'
	// <0x5a40d1>|0x0b8|0x010:'888'
	// <0x5a40de>|0x0c5|0x00d:'889'
	// 1
	// <0x5a40ee>|0x0d5|0x010:'891'
	// 1
	// <0x5a40f6>|0x0dd|0x008:'893'
	// <0x5a4118>|0x0ff|0x022:'894'
	// <0x5a413a>|0x121|0x022:'895'
	// <0x5a415c>|0x143|0x022:'896'
	// <0x5a417e>|0x165|0x022:'897'
	// <0x5a41a0>|0x187|0x022:'898'
	// 1
	// <0x5a41c2>|0x1a9|0x022:'900'
	// 1
	// 2
	// 3
	// <0x5a41d0>|0x1b7|0x00e:'904'
	// 1
	// 2
	// 3
	// <0x5a41ee>|0x1d5|0x01e:'908'
	// 1
	// 2
	// 3
	// 4
	// <0x5a4214>|0x1fb|0x026:'913'
	// 1
	// <0x5a4222>|0x209|0x00e:'915'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::is_ready_to_be_deactivated() const
bool weapon_core::is_ready_to_be_deactivated( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a32e9>|0x000|0x000:'920'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::on_player_model_added()
void weapon_core::on_player_model_added( )
{
	// FUNCTION BODY
	// <0x5a2f27>|0x000|0x000:'925'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::on_player_model_removed()
void weapon_core::on_player_model_removed( )
{
	// FUNCTION BODY
	// <0x5a2f07>|0x000|0x000:'930'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void weapon_core::update_bones_matrices(
	vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&	user_skeleton,
	vostok::math::float4x4*				user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	vostok::math::float4x4&				character_head_transform,
	vostok::math::float4x4&				character_transform,
	vostok::animation::animation_player const&	user_animation_player)
{
	// LOCALS
	// u32 							weapon_bone_index
	// u32 							head_bone_index
	// vostok::math::float4x4* 		weapon_matrices
	// u32 							weapon_matrices_count
	// u32 							root_bone_index
	// vostok::math::float4x4 const& weapon_transform
	// vostok::math::float4x4 const& user_transform
	// vostok::math::float4x4* 		e<2>
	// vostok::math::float4x4* 		i<2>
	// u32 							j<3>
	// ******

	// STATICS
	// static bool 					draw_bones_matrices = <0x4c26664>;
	// ******

	// SKIPPED BLOCKS
	// <0x5a2a10><1>
	// <0x5a5dbb><2>
	// <0x5a5de0><3>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594b6d }, type_index: TypeIndex(0x916f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594bfb }, type_index: TypeIndex(0xa8e7) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594d3d }, type_index: TypeIndex(0x9173) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594d95 }, type_index: TypeIndex(0x6783) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594e33 }, type_index: TypeIndex(0x6727) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594e7b }, type_index: TypeIndex(0x6781) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x594ee3 }, type_index: TypeIndex(0x6727) })
	// ******

	// FUNCTION BODY
	// <0x5a5b41>|0x000|0x000:'943'
	// <0x5a5b50>|0x00f|0x00f:'944'
	// 1
	// <0x5a5bda>|0x099|0x08a:'946'
	// 1
	// <0x5a5c00>|0x0bf|0x026:'948'
	// <0x5a5c1a>|0x0d9|0x01a:'949'
	// 1
	// <0x5a5c39>|0x0f8|0x01f:'951'
	// 1
	// <0x5a5c62>|0x121|0x029:'953'
	// 1
	// <0x5a5c88>|0x147|0x026:'955'
	// <0x5a5cac>|0x16b|0x024:'956'
	// 1
	// 2
	// 3
	// 4
	// <0x5a5ceb>|0x1aa|0x03f:'961'
	// <0x5a5cf6>|0x1b5|0x00b:'962'
	// 1
	// 2
	// 3
	// <0x5a5d13>|0x1d2|0x01d:'966'
	// 1
	// <0x5a5d4b>|0x20a|0x038:'968'
	// <0x5a5d60>|0x21f|0x015:'969'
	// 1
	// 2
	// <0x5a5d79>|0x238|0x019:'972'
	// <0x5a5d97>|0x256|0x01e:'973'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// <0x5a5db2>|0x271|0x01b:'993'
	// 1
	// <0x5a5dbd>|0x27c|0x00b:'995'
	// 1
	// 2
	// <0x5a5de2>|0x2a1|0x025:'998'
	// <0x5a5dfa>|0x2b9|0x018:'999'
	// <0x5a5dfc>|0x2bb|0x002:'1000'
	// 1
	// 2
	// 3
	// 4
	// <0x5a5dfe>|0x2bd|0x002:'1005'
	// <0x5a5e1e>|0x2dd|0x020:'1006'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// <0x5a5e35>|0x2f4|0x017:'1017'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a5e7d>|0x33c|0x048:'1023'
	// <0x5a5ea3>|0x362|0x026:'1024'
	// <0x5a5ecb>|0x38a|0x028:'1025'
	// 1
	// <0x5a5ee5>|0x3a4|0x01a:'1027'
	// <0x5a5f0b>|0x3ca|0x026:'1028'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::is_sprinting() const
bool weapon_core::is_sprinting( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a32c7>|0x000|0x000:'1033'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::on_skeleton_matrices_changed(const unsigned int, vostok::math::float4x4 const&, vostok::math::float4x4 const* const, vostok::math::float4x4 const* const, vostok::math::float4x4 const&, vostok::math::float4x4* const, vostok::math::float4x4* const, vostok::math::float4x4 const&)
void weapon_core::on_skeleton_matrices_changed(
	u32									current_time_in_ms,
	vostok::math::float4x4 const&		weapon_transform,
	vostok::math::float4x4 const*		weapon_matrices_begin,
	vostok::math::float4x4 const*		weapon_matrices_end,
	vostok::math::float4x4 const&		user_transform,
	vostok::math::float4x4*				user_matrices_begin,
	vostok::math::float4x4*				user_matrices_end,
	vostok::math::float4x4 const&		user_weapon_transform)
{
	// FUNCTION BODY
	// <0x5a2e8b>|0x000|0x000:'1047'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::process_finger_correction(const unsigned int, vostok::math::float4x4* const)
void weapon_core::process_finger_correction( u32 current_time_in_ms, vostok::math::float4x4* user_matrices )
{
	// FUNCTION BODY
	// <0x5a2e49>|0x000|0x000:'1055'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void weapon_core::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// LOCALS
	// u8 							state_id<1>
	// bool 						found<1>
	// vostok::ai::fsm_state const* current<1>
	// vostok::ai::fsm_state const* i<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5929d0 }, type_index: TypeIndex(0x66ed) })
	// ******

	// FUNCTION BODY
	// <0x5a3819>|0x000|0x000:'1060'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a3829>|0x010|0x010:'1066'
	// <0x5a3841>|0x028|0x018:'1067'
	// 1
	// <0x5a3859>|0x040|0x018:'1069'
	// 1
	// <0x5a386c>|0x053|0x013:'1071'
	// 1
	// <0x5a387e>|0x065|0x012:'1073'
	// <0x5a3891>|0x078|0x013:'1074'
	// <0x5a38a4>|0x08b|0x013:'1075'
	// <0x5a38b7>|0x09e|0x013:'1076'
	// 1
	// <0x5a38ca>|0x0b1|0x013:'1078'
	// <0x5a38d8>|0x0bf|0x00e:'1079'
	// 1
	// <0x5a38eb>|0x0d2|0x013:'1081'
	// 1
	// <0x5a390e>|0x0f5|0x023|[1]:'1083'
	// 1
	// 2
	// <0x5a3921>|0x108|0x013:'1086'
	// 1
	// <0x5a3937>|0x11e|0x016:'1088'
	// <0x5a393b>|0x122|0x004:'1089'
	// <0x5a393f>|0x126|0x004:'1090'
	// <0x5a394e>|0x135|0x00f|[2]:'1091'
	// <0x5a397d>|0x164|0x02f:'1092'
	// <0x5a3985>|0x16c|0x008:'1093'
	// <0x5a3989>|0x170|0x004:'1094'
	// 1
	// <0x5a398b>|0x172|0x002:'1096'
	// 1
	// <0x5a398d>|0x174|0x002:'1098'
	// 1
	// <0x5a3999>|0x180|0x00c:'1100'
	// <0x5a39a6>|0x18d|0x00d:'1101'
	// 1
	// <0x5a39d2>|0x1b9|0x02c:'1103'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::deserialize(vostok::network_core::packet_reader&)
void weapon_core::deserialize( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id<1>
	// u8 							state_id<1>
	// vostok::ai::fsm_state* 		current<1>
	// vostok::ai::fsm_state* 		i<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5936f3 }, type_index: TypeIndex(0x66ee) })
	// ******

	// FUNCTION BODY
	// <0x5a445b>|0x000|0x000:'1109'
	// <0x5a4467>|0x00c|0x00c:'1110'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5a4471>|0x016|0x00a:'1116'
	// <0x5a448b>|0x030|0x01a:'1117'
	// <0x5a44ae>|0x053|0x023:'1118'
	// 1
	// <0x5a44c2>|0x067|0x014:'1120'
	// 1
	// <0x5a44d3>|0x078|0x011:'1122'
	// <0x5a44e5>|0x08a|0x012:'1123'
	// <0x5a44f7>|0x09c|0x012:'1124'
	// <0x5a4508>|0x0ad|0x011:'1125'
	// 1
	// <0x5a451c>|0x0c1|0x014:'1127'
	// <0x5a452c>|0x0d1|0x010:'1128'
	// <0x5a45ae>|0x153|0x082:'1129'
	// <0x5a45b0>|0x155|0x002:'1130'
	// 1
	// <0x5a45eb>|0x190|0x03b:'1132'
	// <0x5a45f9>|0x19e|0x00e:'1133'
	// 1
	// <0x5a460a>|0x1af|0x011:'1135'
	// 1
	// <0x5a462d>|0x1d2|0x023|[1]:'1137'
	// 1
	// 2
	// <0x5a463e>|0x1e3|0x011:'1140'
	// 1
	// <0x5a4650>|0x1f5|0x012:'1142'
	// <0x5a465b>|0x200|0x00b:'1143'
	// <0x5a465f>|0x204|0x004:'1144'
	// <0x5a4666>|0x20b|0x007|[2]:'1145'
	// <0x5a4695>|0x23a|0x02f:'1146'
	// <0x5a46a1>|0x246|0x00c:'1147'
	// <0x5a46a7>|0x24c|0x006:'1148'
	// 1
	// <0x5a46a9>|0x24e|0x002:'1150'
	// 1
	// <0x5a46ab>|0x250|0x002:'1152'
	// <0x5a46b7>|0x25c|0x00c:'1153'
	// <0x5a46c9>|0x26e|0x012:'1154'
	// 1
	// <0x5a46f5>|0x29a|0x02c:'1156'
	// 1
	// <0x5a4707>|0x2ac|0x012:'1158'
	// 1
	// 2
	// <0x5a4715>|0x2ba|0x00e:'1161'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::instant_idle_predicate() const
bool weapon_core::instant_idle_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a4e49>|0x000|0x000:'1166'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::could_be_used(survarium::base_player const&) const
bool weapon_core::could_be_used( base_player const& user ) const
{
	// LOCALS
	// u8 							broken_hands_count
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592264 }, type_index: TypeIndex(0x916d) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x5a3259>|0x000|0x000:'1171'
	// <0x5a3287>|0x02e|0x02e:'1172'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::could_be_aimed(survarium::base_player const&) const
bool weapon_core::could_be_aimed( base_player const& user ) const
{
	// LOCALS
	// u8 							broken_hands_count
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592214 }, type_index: TypeIndex(0x916d) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x5a3209>|0x000|0x000:'1177'
	// <0x5a3237>|0x02e|0x02e:'1178'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::computed_backward_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_backward_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592e83 }, type_index: TypeIndex(0x916f) })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5a3e29>|0x000|0x000:'1203'
	// <0x5a3e59>|0x030|0x030:'1204'
	// <0x5a3e6c>|0x043|0x013:'1205'
	// <0x5a3ecc>|0x0a3|0x060:'1206'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::computed_horizontal_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_horizontal_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592dc3 }, type_index: TypeIndex(0x916f) })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5a3d69>|0x000|0x000:'1218'
	// <0x5a3d99>|0x030|0x030:'1219'
	// <0x5a3dac>|0x043|0x013:'1220'
	// <0x5a3e0c>|0x0a3|0x060:'1221'
	// ******
}

// STATE[STUB]
// float survarium::weapon_core::computed_vertical_recoil_time(const float, const float, const unsigned int, const unsigned int, const unsigned int, const float)
float weapon_core::computed_vertical_recoil_time(
	float		animation_length,
	float		animation_time_before_time_scale_starts,
	u32			time_scale_start_time_in_ms,
	u32			current_time_in_ms,
	u32			target_time_in_ms,
	float		time_scale)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592d03 }, type_index: TypeIndex(0x916f) })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5a3ca9>|0x000|0x000:'1233'
	// <0x5a3cd9>|0x030|0x030:'1234'
	// <0x5a3cec>|0x043|0x013:'1235'
	// <0x5a3d4c>|0x0a3|0x060:'1236'
	// ******
}

// STATE[STUB]
// fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> survarium::weapon_core::backward_recoil_time_calculator()
fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> weapon_core::backward_recoil_time_calculator( )
{
	// FUNCTION BODY
	// <0x5a3fc9>|0x000|0x000:'1241'
	// ******
}

// STATE[STUB]
// fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> survarium::weapon_core::horizontal_recoil_time_calculator()
fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> weapon_core::horizontal_recoil_time_calculator( )
{
	// FUNCTION BODY
	// <0x5a3f79>|0x000|0x000:'1246'
	// ******
}

// STATE[STUB]
// fastdelegate::FastDelegate<float __cdecl(float,float,unsigned int,unsigned int,unsigned int,float)> survarium::weapon_core::vertical_recoil_time_calculator()
fastdelegate::FastDelegate<float __cdecl(float,float,u32,u32,u32,float)> weapon_core::vertical_recoil_time_calculator( )
{
	// FUNCTION BODY
	// <0x5a3f29>|0x000|0x000:'1251'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::set_inventory(survarium::inventory*, survarium::profile_slot_enum)
void weapon_core::set_inventory( inventory* inv, profile_slot_enum slot )
{
	// FUNCTION BODY
	// <0x5a2e27>|0x000|0x000:'1256'
	// ******
}

// STATE[STUB]
// survarium::profile_slot_enum survarium::weapon_core::get_ammo_slot(survarium::ammo_id_enum)
profile_slot_enum weapon_core::get_ammo_slot( ammo_id_enum slot_id )
{
	// FUNCTION BODY
	// <0x5a2dd9>|0x000|0x000:'1261'
	// 1
	// 2
	// <0x5a2df2>|0x019|0x019:'1264'
	// 1
	// <0x5a2dfe>|0x025|0x00c:'1266'
	// 1
	// <0x5a2e0a>|0x031|0x00c:'1268'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::get_ammo_info(survarium::weapon_ammo_info&)
void weapon_core::get_ammo_info( weapon_ammo_info& info )
{
	// LOCALS
	// inventory& 					inv
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> slot2_itm
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> slot1_itm
	// ******

	// FUNCTION BODY
	// <0x5a432a>|0x000|0x000:'1274'
	// <0x5a434e>|0x024|0x024:'1275'
	// <0x5a435d>|0x033|0x00f:'1276'
	// 1
	// <0x5a436e>|0x044|0x011:'1278'
	// <0x5a437a>|0x050|0x00c:'1279'
	// <0x5a4398>|0x06e|0x01e:'1280'
	// 1
	// <0x5a43b6>|0x08c|0x01e:'1282'
	// <0x5a43eb>|0x0c1|0x035:'1283'
	// 1
	// <0x5a4421>|0x0f7|0x036:'1285'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::must_chamber_a_round_predicate() const
bool weapon_core::must_chamber_a_round_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a31a9>|0x000|0x000:'1290'
	// <0x5a31b5>|0x00c|0x00c:'1291'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::must_chamber_a_round_aimed_predicate() const
bool weapon_core::must_chamber_a_round_aimed_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a35b9>|0x000|0x000:'1296'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::must_chamber_a_round_and_animation_ended_predicate() const
bool weapon_core::must_chamber_a_round_and_animation_ended_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a3549>|0x000|0x000:'1301'
	// <0x5a3555>|0x00c|0x00c:'1302'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate() const
bool weapon_core::must_chamber_a_round_aimed_and_animation_ended_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a37d9>|0x000|0x000:'1307'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::is_ready_to_shoot() const
bool weapon_core::is_ready_to_shoot( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a2ac9>|0x000|0x000:'1312'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::is_trying_to_aim() const
bool weapon_core::is_trying_to_aim( ) const
{
	// LOCALS
	// player_input const& 			input
	// u32 							just_toggled
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5924b2 }, type_index: TypeIndex(0x916f) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x5a3499>|0x000|0x000:'1317'
	// <0x5a34b7>|0x01e|0x01e:'1318'
	// 1
	// 2
	// 3
	// 4
	// <0x5a34cb>|0x032|0x014:'1323'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::is_not_trying_to_aim_predicate() const
bool weapon_core::is_not_trying_to_aim_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a37b7>|0x000|0x000:'1328'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::can_and_must_reload_predicate() const
bool weapon_core::can_and_must_reload_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5a4df0>|0x000|0x000:'1333'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core::can_and_must_reload_and_animation_ended_predicate() const
bool weapon_core::can_and_must_reload_and_animation_ended_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// 1
	// 2
	// <0x5a5ad9>|0x000|0x000:'1340'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::unload_chambered_round()
void weapon_core::unload_chambered_round( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x591dbd }, type_index: TypeIndex(0x6700) })
	// ******

	// FUNCTION BODY
	// <0x5a2d69>|0x000|0x000:'1345'
	// <0x5a2d75>|0x00c|0x00c:'1346'
	// <0x5a2d81>|0x018|0x00c:'1347'
	// 1
	// <0x5a2d8d>|0x024|0x00c:'1349'
	// <0x5a2da5>|0x03c|0x018:'1350'
	// 1
	// <0x5a2daf>|0x046|0x00a:'1352'
	// ******
}

// STATE[STUB]
// unsigned short survarium::weapon_core::maximum_ammo_in_weapon() const
u16 weapon_core::maximum_ammo_in_weapon( ) const
{
	// LOCALS
	// bool 						chamber_a_round_but_not_on_reload
	// ******

	return 0;
	// FUNCTION BODY
	// <0x5a2a79>|0x000|0x000:'1357'
	// <0x5a2aa5>|0x02c|0x02c:'1358'
	// ******
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::on_hand_ik_event(vostok::animation::animation_callback_params&, const survarium::hand_to_weapon_ik_processor::hands_enum)
vostok::animation::callback_return_type_enum weapon_core::on_hand_ik_event( vostok::animation::animation_callback_params& params, hand_to_weapon_ik_processor::hands_enum hand )
{
	// LOCALS
	// bool 						active
	// ******

	// FUNCTION BODY
	// <0x5a2d09>|0x000|0x000:'1363'
	// <0x5a2d15>|0x00c|0x00c:'1364'
	// <0x5a2d21>|0x018|0x00c:'1365'
	// <0x5a2d31>|0x028|0x010:'1366'
	// <0x5a2d4f>|0x046|0x01e:'1367'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::on_user_sprint(bool)
void weapon_core::on_user_sprint( bool user_is_sprinting )
{
	// LOCALS
	// bool 						left_hand_ik_is_active
	// ******

	// FUNCTION BODY
	// <0x5a2ca9>|0x000|0x000:'1372'
	// <0x5a2cd0>|0x027|0x027:'1373'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_idle_start()
void weapon_core::instant_idle_start( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x592188 }, type_index: TypeIndex(0x916f) })
	// ******

	// FUNCTION BODY
	// <0x5a3167>|0x000|0x000:'1378'
	// 1
	// <0x5a3171>|0x00a|0x00a:'1380'
	// <0x5a3192>|0x02b|0x021:'1381'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_idle_end()
void weapon_core::instant_idle_end( )
{
	// FUNCTION BODY
	// <0x5a2a57>|0x000|0x000:'1386'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_toggle_start()
void weapon_core::instant_toggle_start( )
{
	// FUNCTION BODY
	// <0x5a2a37>|0x000|0x000:'1391'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core::instant_toggle_end()
void weapon_core::instant_toggle_end( )
{
	// FUNCTION BODY
	// <0x5a2a17>|0x000|0x000:'1396'
	// ******
}

	/* TYPEDEFS

	typedef
		long
		counter_type;

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium
