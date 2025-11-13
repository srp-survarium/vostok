////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_character_controller.h"

#include "bullet_include.h"

namespace vostok {
namespace physics {

/*
// STATE[STUB]
// void vostok::physics::`dynamic initializer for 's_step_height_command''()
void `dynamic initializer for 's_step_height_command''( ) {
}

// STATE[STUB]
// void vostok::physics::`dynamic initializer for 's_character_sliping_speed_multiplier_cc''()
void `dynamic initializer for 's_character_sliping_speed_multiplier_cc''( ) {
}

// STATE[STUB]
// void vostok::physics::`dynamic initializer for 's_cc_max_allowed_penetration_cc''()
void `dynamic initializer for 's_cc_max_allowed_penetration_cc''( ) {
}

// STATE[STUB]
// void vostok::physics::`dynamic initializer for 's_cc_prevent_step_bouncing_cc''()
void `dynamic initializer for 's_cc_prevent_step_bouncing_cc''( ) {
}

// STATE[STUB]
// void vostok::physics::`dynamic atexit destructor for 's_step_height_command''()
void `dynamic atexit destructor for 's_step_height_command''( ) {
}
*/

// STATE[STUB]
// btVector3 vostok::physics::getNormalizedVector(btVector3 const&)
btVector3 getNormalizedVector( btVector3 const& v )
{
	// FUNCTION BODY
	// <0x584e69>|0x000|0x000:'61'
	// <0x584ed2>|0x069|0x069:'62'
	// <1>
	// <2>
	// <0x584f14>|0x0ab|0x042:'65'
	// ******
}

// STATE[STUB]
// btVector3 vostok::physics::computeReflectionDirection(btVector3 const&, btVector3 const&)
btVector3 computeReflectionDirection( btVector3 const& direction, btVector3 const& normal )
{
	// FUNCTION BODY
	// <0x584ae6>|0x000|0x000:'75'
	// ******
}

// STATE[STUB]
// btVector3 vostok::physics::parallelComponent(btVector3 const&, btVector3 const&)
btVector3 parallelComponent( btVector3 const& direction, btVector3 const& normal )
{
	// FUNCTION BODY
	// <0x584640>|0x000|0x000:'83'
	// <0x584674>|0x034|0x034:'84'
	// ******
}

// STATE[STUB]
// btVector3 vostok::physics::perpindicularComponent(btVector3 const&, btVector3 const&)
btVector3 perpindicularComponent( btVector3 const& direction, btVector3 const& normal )
{
	// FUNCTION BODY
	// <0x584a66>|0x000|0x000:'92'
	// ******
}

// STATE[STUB]
// void vostok::physics::setup_game_material_groups(unsigned short const*, const unsigned short)
void setup_game_material_groups( u16 const* game_material_groups, u16 game_materials_count )
{
	// FUNCTION BODY
	// <1>
	// <0x584530>|0x000|0x000:'201'
	// ******
}

class character_move_test_callback : public btCollisionWorld::ClosestConvexResultCallback , public boost::noncopyable {
public:
						character_move_test_callback	( btCollisionObject* self, btVector3 const& up_vector, float minSlopeDot );

	virtual	float		addSingleResult					( btCollisionWorld::LocalConvexResult& arg_0, bool arg_1 ) override { /* no source */ return 0.0f; }


private:
	/* 0x0000 */	/* btCollisionWorld::ClosestConvexResultCallback */
	/* 0x0060 */	/* boost::noncopyable */
	/* 0x0060 */	btVector3				m_up_vector;
	/* 0x0070 */	btCollisionObject*		m_self;
	/* 0x0074 */	float					m_minSlopeDot;
}; // class character_move_test_callback

STATIC_SIZE_ASSERT(character_move_test_callback, 0x80);


// STATE[STUB]
character_move_test_callback::character_move_test_callback( btCollisionObject* self, btVector3 const& up_vector, float minSlopeDot ):
	ClosestConvexResultCallback( from_vostok( float3() ), from_vostok( float3() ) )
{
}

/*
// STATE[STUB]
// void* vostok::physics::bullet_character_controller::`scalar deleting destructor'(unsigned int)
void* bullet_character_controller::`scalar deleting destructor'( )
{
	return NULL;
	// FUNCTION BODY
	// <0xde4c6>|0x000|0x000:'238'
	// <0xde4d6>|0x010|0x010:'239'
	// <1>
	// <2>
	// <0xde4e3>|0x01d|0x00d:'242'
	// <1>
	// <0xde4ea>|0x024|0x007:'244'
	// <0xde4fa>|0x034|0x010:'245'
	// <1>
	// <2>
	// <0xde4ff>|0x039|0x005:'248'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0xde5a3>|0x0dd|0x0a4:'262'
	// <0xde5ce>|0x108|0x02b:'263'
	// <0xde5dc>|0x116|0x00e:'264'
	// <1>
	// <0xde5e5>|0x11f|0x009:'266'
	// ******
}
*/
// STATE[STUB]
bullet_character_controller::bullet_character_controller(
	btPairCachingGhostObject*	ghost_object,
	float2 const&				stand_shape_dim,
	float2 const&				crouch_shape_dim,
	s16							collisionFilterGroup,
	s16							collisionFilterMask) :
	m_shape		( btCapsuleShape( 0.0f, 0.0f ) )
{
	// FUNCTION BODY
	// <0x584e4b>|0x000|0x000:'310'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::~bullet_character_controller()
bullet_character_controller::~bullet_character_controller( )
{
}

// STATE[STUB]
// btVector3 vostok::physics::bullet_character_controller::updateTargetPositionBasedOnCollision(btVector3 const&, btVector3 const&, float, float)
btVector3 bullet_character_controller::updateTargetPositionBasedOnCollision(
	btVector3 const&	hitNormal,
	btVector3 const&	target_pos,
	float				__formal,
	float				normalMag
)
{
	// LOCALS
	// float 						movement_length
	// btVector3 					movementDirection
	// btVector3 					reflectDir
	// ******

	// FUNCTION BODY
	// <0x585470>|0x000|0x000:'319'
	// <0x5854ac>|0x03c|0x03c:'320'
	// <0x5854b1>|0x041|0x005:'321'
	// <0x5854dc>|0x06c|0x02b:'322'
	// <1>
	// <0x585526>|0x0b6|0x04a:'324'
	// <1>
	// <0x585539>|0x0c9|0x013:'326'
	// <1>
	// <0x585541>|0x0d1|0x008:'328'
	// <0x5855b3>|0x143|0x072:'329'
	// <1>
	// <2>
	// <0x585607>|0x197|0x054:'332'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x585659>|0x1e9|0x052:'342'
	// <0x585677>|0x207|0x01e:'343'
	// <1>
	// <2>
	// <0x58569f>|0x22f|0x028:'346'
	// ******
}

// STATE[STUB]
void bullet_character_controller::updateAction( btCollisionWorld* collisionWorld, float deltaTime )
{
	// FUNCTION BODY
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5863f9>|0x000|0x000:'356'
	// <0x58643a>|0x041|0x041:'357'
	// <1>
	// <0x586446>|0x04d|0x00c:'359'
	// <1>
	// <2>
	// <0x586534>|0x13b|0x0ee:'362'
	// <0x586539>|0x140|0x005:'363'
	// <0x586546>|0x14d|0x00d:'364'
	// <1>
	// <0x58663a>|0x241|0x0f4:'366'
	// <0x586648>|0x24f|0x00e:'367'
	// <0x586655>|0x25c|0x00d:'368'
	// <0x58674e>|0x355|0x0f9:'369'
	// ******
}

// STATE[STUB]
void bullet_character_controller::player_step( float dt )
{
	// LOCALS
	// btTransform 					new_transform
	// btVector3 					step_up_correction
	// ******

	// STATICS
	// static bool 					use_shape_size = <0x10000>;
	// ******

	// FUNCTION BODY
	// <0x58622d>|0x000|0x000:'374'
	// <1>
	// <2>
	// <0x586263>|0x036|0x036:'377'
	// <0x5862a3>|0x076|0x040:'378'
	// <1>
	// <2>
	// <3>
	// <0x5862fe>|0x0d1|0x05b:'382'
	// <1>
	// <0x586319>|0x0ec|0x01b:'384'
	// <1>
	// <0x58631d>|0x0f0|0x004:'386'
	// <1>
	// <2>
	// <0x586328>|0x0fb|0x00b:'389'
	// <1>
	// <0x586331>|0x104|0x009:'391'
	// <0x58633a>|0x10d|0x009:'392'
	// <1>
	// <2>
	// <0x586341>|0x114|0x007:'395'
	// <1>
	// <0x58634a>|0x11d|0x009:'397'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x58635b>|0x12e|0x011:'404'
	// <0x5863a3>|0x176|0x048:'405'
	// <0x5863b3>|0x186|0x010:'406'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// ******
}

// STATE[STUB]
float bullet_character_controller::recover_from_penetration( )
{
	// LOCALS
	// float 						shape_y
	// float 						maxPen
	// btAlignedObjectArray<btPersistentManifold *> manifold_array
	// s32 							i
	// s32 							j
	// btVector3 					pos_on_shape
	// ******

	// CALL SITE INFO
	// <0x5850c3> -> void <unknown>(btOverlappingPairCache*, btDispatcherInfo const&, btDispatcher*)
	// <0x585123> -> int <unknown>() const
	// <0x5851a7> -> btAlignedObjectArray<btBroadphasePair>& <unknown>()
	// <0x5851c4> -> void <unknown>(btAlignedObjectArray<btPersistentManifold *>&)
	// <0x585350> -> int <unknown>() const
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x58506d>|0x000|0x000:'429'
	// <1>
	// <2>
	// <3>
	// <0x5850a6>|0x039|0x039:'433'
	// <1>
	// <0x5850c5>|0x058|0x01f:'435'
	// <1>
	// <2>
	// <0x5850df>|0x072|0x01a:'438'
	// <1>
	// <0x5850e8>|0x07b|0x009:'440'
	// <1>
	// <0x585108>|0x09b|0x020:'442'
	// <1>
	// <0x585131>|0x0c4|0x029:'444'
	// <1>
	// <0x585192>|0x125|0x061:'446'
	// <1>
	// <0x5851b0>|0x143|0x01e:'448'
	// <0x5851b5>|0x148|0x005:'449'
	// <1>
	// <2>
	// <0x5851c6>|0x159|0x011:'452'
	// <1>
	// <0x5851f0>|0x183|0x02a:'454'
	// <0x5851f7>|0x18a|0x007:'455'
	// <0x585206>|0x199|0x00f:'456'
	// <1>
	// <0x585217>|0x1aa|0x011:'458'
	// <1>
	// <2>
	// <3>
	// <0x585230>|0x1c3|0x019:'462'
	// <1>
	// <0x585235>|0x1c8|0x005:'464'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x585241>|0x1d4|0x00c:'469'
	// <1>
	// <0x585250>|0x1e3|0x00f:'471'
	// <1>
	// <0x585256>|0x1e9|0x006:'473'
	// <1>
	// <2>
	// <3>
	// <0x585264>|0x1f7|0x00e:'477'
	// <0x5852bd>|0x250|0x059:'478'
	// <1>
	// <0x5852dc>|0x26f|0x01f:'480'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58535a>|0x2ed|0x07e:'486'
	// <0x58537e>|0x311|0x024:'487'
	// <0x585388>|0x31b|0x00a:'488'
	// <0x58538d>|0x320|0x005:'489'
	// ******
}

// STATE[STUB]
void bullet_character_controller::step_up( bool change_shape_size, btVector3& pos_up_correction )
{
	// FUNCTION BODY
	// <1>
	// <2>
	// <0x585c06>|0x000|0x000:'496'
	// <1>
	// <2>
	// <0x585c10>|0x00a|0x00a:'499'
	// <1>
	// <2>
	// <0x585c32>|0x02c|0x022:'502'
	// <1>
	// <0x585c5e>|0x058|0x02c:'504'
	// <1>
	// <0x585ca0>|0x09a|0x042:'506'
	// <0x585cc4>|0x0be|0x024:'507'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// ******
}

// STATE[STUB]
void bullet_character_controller::step_forward_and_strafe( btVector3 const& walkMove )
{
	// LOCALS
	// btVector3 					target_pos
	// btTransform 					start
	// btTransform 					end
	// float 						fraction
	// character_move_test_callback callback
	// btVector3 					sweepDirNegative
	// btVector3 					currentDir
	// ******

	// FUNCTION BODY
	// <0x585786>|0x000|0x000:'553'
	// <1>
	// <2>
	// <0x5857c7>|0x041|0x041:'556'
	// <1>
	// <0x5857ca>|0x044|0x003:'558'
	// <1>
	// <2>
	// <3>
	// <0x5857dc>|0x056|0x012:'562'
	// <0x585852>|0x0cc|0x076:'563'
	// <0x585940>|0x1ba|0x0ee:'564'
	// <1>
	// <0x585946>|0x1c0|0x006:'566'
	// <1>
	// <0x585962>|0x1dc|0x01c:'568'
	// <1>
	// <0x58596d>|0x1e7|0x00b:'570'
	// <1>
	// <2>
	// <3>
	// <0x585972>|0x1ec|0x005:'574'
	// <1>
	// <2>
	// <3>
	// <0x5859f1>|0x26b|0x07f:'578'
	// <1>
	// <0x585a18>|0x292|0x027:'580'
	// <0x585a40>|0x2ba|0x028:'581'
	// <1>
	// <0x585a42>|0x2bc|0x002:'583'
	// <1>
	// <2>
	// <0x585a71>|0x2eb|0x02f:'586'
	// <1>
	// <0x585a8a>|0x304|0x019:'588'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x585a9b>|0x315|0x011:'593'
	// <0x585ac5>|0x33f|0x02a:'594'
	// <0x585aec>|0x366|0x027:'595'
	// <0x585b12>|0x38c|0x026:'596'
	// <1>
	// <0x585b27>|0x3a1|0x015:'598'
	// <1>
	// ******
}

// STATE[STUB]
void bullet_character_controller::step_down( float dt, bool change_size_only, btVector3 const& pos_up_correction )
{
	// LOCALS
	// character_move_test_callback callback
	// btTransform 					start
	// btTransform 					finish
	// ******

	// CALL SITE INFO
	// <0x5861dc> -> float <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x585cdc>|0x000|0x000:'627'
	// <1>
	// <2>
	// <0x585d1c>|0x040|0x040:'630'
	// <0x585d1f>|0x043|0x003:'631'
	// <1>
	// <0x585d43>|0x067|0x024:'633'
	// <1>
	// <0x585dcd>|0x0f1|0x08a:'635'
	// <0x585de3>|0x107|0x016:'636'
	// <1>
	// <2>
	// <3>
	// <0x585de6>|0x10a|0x003:'640'
	// <1>
	// <2>
	// <3>
	// <0x585de9>|0x10d|0x003:'644'
	// <1>
	// <2>
	// <3>
	// <0x585eca>|0x1ee|0x0e1:'648'
	// <1>
	// <0x585eef>|0x213|0x025:'650'
	// <0x585f19>|0x23d|0x02a:'651'
	// <1>
	// <0x585f1b>|0x23f|0x002:'653'
	// <1>
	// <2>
	// <0x585f44>|0x268|0x029:'656'
	// <1>
	// <0x585f5a>|0x27e|0x016:'658'
	// <0x585f6c>|0x290|0x012:'659'
	// <1>
	// <0x585fc0>|0x2e4|0x054:'661'
	// <1>
	// <0x586039>|0x35d|0x079:'663'
	// <0x5860ae>|0x3d2|0x075:'664'
	// <0x5860c2>|0x3e6|0x014:'665'
	// <0x5860c9>|0x3ed|0x007:'666'
	// <0x5860d0>|0x3f4|0x007:'667'
	// <1>
	// <0x5860d5>|0x3f9|0x005:'669'
	// <1>
	// <2>
	// <0x586188>|0x4ac|0x0b3:'672'
	// <1>
	// <2>
	// <0x58618d>|0x4b1|0x005:'675'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <0x5861a9>|0x4cd|0x01c:'720'
	// <0x5861d5>|0x4f9|0x02c:'721'
	// <0x5861e1>|0x505|0x00c:'722'
	// <1>
	// ******
}

// STATE[STUB]
void bullet_character_controller::set_desired_walk_vector( btVector3 const& walk_vector )
{
	// FUNCTION BODY
	// <0x584fde>|0x000|0x000:'728'
	// <0x584fe5>|0x007|0x007:'729'
	// <1>
	// <0x584ffb>|0x01d|0x016:'731'
	// <0x585027>|0x049|0x02c:'732'
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::pre_step(float)
void bullet_character_controller::pre_step( float dt )
{
	// LOCALS
	// float 						__formal
	// ******

	// FUNCTION BODY
	// <0x5856c0>|0x000|0x000:'739'
	// <0x5856f7>|0x037|0x037:'740'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x58570f>|0x04f|0x018:'748'
	// <1>
	// <0x585726>|0x066|0x017:'750'
	// <0x585727>|0x067|0x001:'751'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58573c>|0x07c|0x015:'757'
	// <1>
	// ******
}

// STATE[STUB]
// bool vostok::physics::bullet_character_controller::can_jump() const
bool bullet_character_controller::can_jump( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5845f1>|0x000|0x000:'1077'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::jump()
void bullet_character_controller::jump( )
{
	// FUNCTION BODY
	// <0x5849f1>|0x000|0x000:'1082'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x584a2c>|0x03b|0x03b:'1089'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::end_jump()
void bullet_character_controller::end_jump( )
{
	// FUNCTION BODY
	// <0x584520>|0x000|0x000:'1094'
	// ******
}

// STATE[STUB]
// bool vostok::physics::bullet_character_controller::on_ground() const
bool bullet_character_controller::on_ground( ) const
{
	return false;
	// FUNCTION BODY
	// <0x5845b1>|0x000|0x000:'1105'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::setup_shape_dim(vostok::math::float2 const&)
void bullet_character_controller::setup_shape_dim( float2 const& shape_dim )
{
	// FUNCTION BODY
	// <1>
	// <0x584549>|0x000|0x000:'1111'
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::setup_crouch_state(bool)
void bullet_character_controller::setup_crouch_state( bool crouch )
{
	// LOCALS
	// btVector3 					prev_shape_offset
	// btVector3 					orign
	// ******

	// CALL SITE INFO
	// <0x5849bc> -> void <unknown>(btCollisionShape*)
	// <0x5849e1> -> void <unknown>(btBroadphaseProxy*, btDispatcher*)
	// ******

	// FUNCTION BODY
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x584809>|0x000|0x000:'1122'
	// <0x584896>|0x08d|0x08d:'1123'
	// <0x58489e>|0x095|0x008:'1124'
	// <1>
	// <0x5848a0>|0x097|0x002:'1126'
	// <0x5848ac>|0x0a3|0x00c:'1127'
	// <0x5848fc>|0x0f3|0x050:'1128'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x58490f>|0x106|0x013:'1134'
	// <0x584986>|0x17d|0x077:'1135'
	// <0x58499c>|0x193|0x016:'1136'
	// <1>
	// <0x5849aa>|0x1a1|0x00e:'1138'
	// <1>
	// <0x5849be>|0x1b5|0x014:'1140'
	// <0x5849c5>|0x1bc|0x007:'1141'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::insert(btDynamicsWorld*)
void bullet_character_controller::insert( btDynamicsWorld* world )
{
	// CALL SITE INFO
	// <0x584bde> -> void <unknown>(btCollisionObject*, short, short)
	// <0x584be9> -> void <unknown>(btActionInterface*)
	// ******

	// FUNCTION BODY
	// <0x584bb4>|0x000|0x000:'1152'
	// <0x584bb8>|0x004|0x004:'1153'
	// <0x584bbf>|0x00b|0x007:'1154'
	// <0x584be0>|0x02c|0x021:'1155'
	// <0x584beb>|0x037|0x00b:'1156'
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
// void vostok::physics::bullet_character_controller::remove(btDynamicsWorld*)
void bullet_character_controller::remove( btDynamicsWorld* world )
{
	// LOCALS
	// btDynamicsWorld* 			world
	// ******

	// CALL SITE INFO
	// <0x584771> -> void <unknown>(btActionInterface*)
	// <0x584782> -> void <unknown>(btCollisionObject*)
	// ******

	// FUNCTION BODY
	// <1>
	// <0x584766>|0x000|0x000:'1165'
	// <0x584773>|0x00d|0x00d:'1166'
	// <0x584784>|0x01e|0x011:'1167'
	// <1>
	// <2>
	// <3>
	// <0x5847ac>|0x046|0x028:'1171'
	// <1>
	// <0x5847eb>|0x085|0x03f:'1173'
	// ******
}

// STATE[STUB]
// btTransform vostok::physics::bullet_character_controller::get_transform()
btTransform bullet_character_controller::get_transform( )
{
	return btTransform();
	// FUNCTION BODY
	// <0x5846a9>|0x000|0x000:'1178'
	// <1>
	// <0x584701>|0x058|0x058:'1180'
	// <1>
	// ******
}

// STATE[100%|DONE]
void bullet_character_controller::set_transform( btTransform const& transform )
{
	m_ghost_object->setWorldTransform( btTransform( transform.getRotation( ), transform.getOrigin( ) + m_shape_offset ) );	// <0x584f4c>|0x000|0x000:'1186'
	m_ghost_object->setInterpolationWorldTransform( m_ghost_object->getWorldTransform( ) );									// <0x584fb7>|0x06b|0x06b:'1187'
}

// STATE[STUB]
void bullet_character_controller::set_crouch( bool crouch )
{
	if ( crouch != m_in_crouch ) // <0x584b6a>|0x000|0x000:'1192'
	{
		setup_crouch_state( crouch );
		m_positions.pop_back( );
	}

	// FUNCTION BODY
	// <0x584b6a>|0x000|0x000:'1192'
	// <1>
	// <2>
	// <0x584b72>|0x008|0x008:'1195'
	// <1>
	// <0x584b76>|0x00c|0x004:'1197'
	// <0x584b78>|0x00e|0x002:'1198'
	// <1>
	// <2>
	// <0x584b7a>|0x010|0x002:'1201'
	// <1>
	// <0x584b81>|0x017|0x007:'1203'
	// ******
}

// STATE[STUB]
bool bullet_character_controller::can_stand( )
{
	return true; // <0x584510>|0x000|0x000:'1208'
	// FUNCTION BODY
	// <0x584510>|0x000|0x000:'1208'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// ******
}

} // namespace physics
} // namespace vostok