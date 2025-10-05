////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet_character_controller.h"

#include "bullet_include.h"

namespace vostok {
namespace physics_2 {

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
btVector3 getNormalizedVector( btVector3 const& v )
{
	// FUNCTION BODY
	// <0x584e69>|0x000|0x000:'61'
	// <0x584ed2>|0x069|0x069:'62'


	// <0x584f14>|0x0ab|0x042:'65'
	// ******
}

// STATE[STUB]
btVector3 computeReflectionDirection( btVector3 const& direction, btVector3 const& normal )
{
	// FUNCTION BODY
	// <0x584ae6>|0x000|0x000:'75'
	// ******
}

// STATE[STUB]
btVector3 parallelComponent( btVector3 const& direction, btVector3 const& normal ) {
	// FUNCTION BODY
	// <0x584640>|0x000|0x000:'83'
	// <0x584674>|0x034|0x034:'84'
	// ******
}

// STATE[STUB]
btVector3 perpindicularComponent( btVector3 const& direction, btVector3 const& normal )
{
	// FUNCTION BODY
	// <0x584a66>|0x000|0x000:'92'
	// ******
}

// STATE[STUB]
void setup_game_material_groups( u16 const* game_material_groups, u16 game_materials_count )
{
	// FUNCTION BODY

	// <0x584530>|0x000|0x000:'201'
	// ******
}

class character_move_test_callback : btCollisionWorld::ClosestConvexResultCallback, boost::noncopyable {
public:
	character_move_test_callback( btCollisionObject* self, btVector3 const& up_vector, float minSlopeDot );

	virtual float addSingleResult(btCollisionWorld::LocalConvexResult&, bool) override {}

	virtual ~character_move_test_callback() {};

private:
	/* offset 0x0000 */ /* fields for btCollisionWorld::ClosestConvexResultCallback */
	/* offset 0x0060 */ /* fields for boost::noncopyable */
	/* offset 0x0060 */ btVector3                           m_up_vector;
	/* offset 0x0070 */ btCollisionObject*                  m_self;
	/* offset 0x0074 */ float                               m_minSlopeDot;
}; // class character_move_test_callback

namespace {
	typedef char size_assert[
		sizeof(character_move_test_callback) == 0x80 ? 1 : -1
	];
}

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
	// FUNCTION BODY
	// <0xde4c6>|0x000|0x000:'238'
	// <0xde4d6>|0x010|0x010:'239'


	// <0xde4e3>|0x01d|0x00d:'242'

	// <0xde4ea>|0x024|0x007:'244'
	// <0xde4fa>|0x034|0x010:'245'


	// <0xde4ff>|0x039|0x005:'248'













	// <0xde5a3>|0x0dd|0x0a4:'262'
	// <0xde5ce>|0x108|0x02b:'263'
	// <0xde5dc>|0x116|0x00e:'264'

	// <0xde5e5>|0x11f|0x009:'266'
	// ******
}
*/
// STATE[STUB]
// vostok::physics::bullet_character_controller::bullet_character_controller(btPairCachingGhostObject*, vostok::math::float2 const&, vostok::math::float2 const&, short, short)
bullet_character_controller::bullet_character_controller( btPairCachingGhostObject* ghost_object, float2 const& stand_shape_dim, float2 const& crouch_shape_dim ):
	m_shape	( btCapsuleShape( 0.0f, 0.0f ) )
{
	// LOCALS
	// s16                             collisionFilterGroup
	// s16                             collisionFilterMask
	// ******

	// FUNCTION BODY
	// <0x584e4b>|0x000|0x000:'310'
	// ******
}

// STATE[STUB]
bullet_character_controller::~bullet_character_controller( )
{
}

// STATE[STUB]
btVector3 bullet_character_controller::updateTargetPositionBasedOnCollision( btVector3 const& hitNormal, btVector3 const& target_pos, float __formal, float normalMag )
{
	// LOCALS
	// float                           movement_length
	// btVector3                       movementDirection
	// btVector3                       reflectDir
	// ******

	// FUNCTION BODY
	// <0x585470>|0x000|0x000:'319'
	// <0x5854ac>|0x03c|0x03c:'320'
	// <0x5854b1>|0x041|0x005:'321'
	// <0x5854dc>|0x06c|0x02b:'322'

	// <0x585526>|0x0b6|0x04a:'324'

	// <0x585539>|0x0c9|0x013:'326'

	// <0x585541>|0x0d1|0x008:'328'
	// <0x5855b3>|0x143|0x072:'329'


	// <0x585607>|0x197|0x054:'332'









	// <0x585659>|0x1e9|0x052:'342'
	// <0x585677>|0x207|0x01e:'343'


	// <0x58569f>|0x22f|0x028:'346'
	// ******
}

// STATE[STUB]
void bullet_character_controller::updateAction( btCollisionWorld* collisionWorld, float deltaTime )
{
	// FUNCTION BODY




	// <0x5863f9>|0x000|0x000:'356'
	// <0x58643a>|0x041|0x041:'357'

	// <0x586446>|0x04d|0x00c:'359'


	// <0x586534>|0x13b|0x0ee:'362'
	// <0x586539>|0x140|0x005:'363'
	// <0x586546>|0x14d|0x00d:'364'

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
	// btTransform                     new_transform
	// btVector3                       step_up_correction
	// ******

	// STATICS
	// static bool                     use_shape_size = <0x10000>;
	// ******

	// FUNCTION BODY
	// <0x58622d>|0x000|0x000:'374'


	// <0x586263>|0x036|0x036:'377'
	// <0x5862a3>|0x076|0x040:'378'



	// <0x5862fe>|0x0d1|0x05b:'382'

	// <0x586319>|0x0ec|0x01b:'384'

	// <0x58631d>|0x0f0|0x004:'386'


	// <0x586328>|0x0fb|0x00b:'389'

	// <0x586331>|0x104|0x009:'391'
	// <0x58633a>|0x10d|0x009:'392'


	// <0x586341>|0x114|0x007:'395'

	// <0x58634a>|0x11d|0x009:'397'






	// <0x58635b>|0x12e|0x011:'404'
	// <0x5863a3>|0x176|0x048:'405'
	// <0x5863b3>|0x186|0x010:'406'


















	// ******
}

// STATE[STUB]
float bullet_character_controller::recover_from_penetration( )
{
	// LOCALS
	// float                           shape_y
	// float                           maxPen
	// btAlignedObjectArray<btPersistentManifold *> manifold_array
	// s32                             i
	// s32                             j
	// btVector3                       pos_on_shape
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5740c3 }, type_index: TypeIndex(0x1386d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x574123 }, type_index: TypeIndex(0x15032) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5741a7 }, type_index: TypeIndex(0x15030) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5741c4 }, type_index: TypeIndex(0x1385b) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x574350 }, type_index: TypeIndex(0x15032) })
	// ******

	// FUNCTION BODY
	// <0x58506d>|0x000|0x000:'429'



	// <0x5850a6>|0x039|0x039:'433'

	// <0x5850c5>|0x058|0x01f:'435'


	// <0x5850df>|0x072|0x01a:'438'

	// <0x5850e8>|0x07b|0x009:'440'

	// <0x585108>|0x09b|0x020:'442'

	// <0x585131>|0x0c4|0x029:'444'

	// <0x585192>|0x125|0x061:'446'

	// <0x5851b0>|0x143|0x01e:'448'
	// <0x5851b5>|0x148|0x005:'449'


	// <0x5851c6>|0x159|0x011:'452'

	// <0x5851f0>|0x183|0x02a:'454'
	// <0x5851f7>|0x18a|0x007:'455'
	// <0x585206>|0x199|0x00f:'456'

	// <0x585217>|0x1aa|0x011:'458'



	// <0x585230>|0x1c3|0x019:'462'

	// <0x585235>|0x1c8|0x005:'464'




	// <0x585241>|0x1d4|0x00c:'469'

	// <0x585250>|0x1e3|0x00f:'471'

	// <0x585256>|0x1e9|0x006:'473'



	// <0x585264>|0x1f7|0x00e:'477'
	// <0x5852bd>|0x250|0x059:'478'

	// <0x5852dc>|0x26f|0x01f:'480'





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


	// <0x585c06>|0x000|0x000:'496'


	// <0x585c10>|0x00a|0x00a:'499'


	// <0x585c32>|0x02c|0x022:'502'

	// <0x585c5e>|0x058|0x02c:'504'

	// <0x585ca0>|0x09a|0x042:'506'
	// <0x585cc4>|0x0be|0x024:'507'









































	// ******
}

// STATE[STUB]
void bullet_character_controller::step_forward_and_strafe( btVector3 const& walkMove )
{
	// LOCALS
	// btVector3                       target_pos
	// btTransform                     start
	// btTransform                     end
	// float                           fraction
	// character_move_test_callback    callback
	// btVector3                       sweepDirNegative
	// btVector3                       currentDir
	// ******

	// FUNCTION BODY
	// <0x585786>|0x000|0x000:'553'


	// <0x5857c7>|0x041|0x041:'556'

	// <0x5857ca>|0x044|0x003:'558'



	// <0x5857dc>|0x056|0x012:'562'
	// <0x585852>|0x0cc|0x076:'563'
	// <0x585940>|0x1ba|0x0ee:'564'

	// <0x585946>|0x1c0|0x006:'566'

	// <0x585962>|0x1dc|0x01c:'568'

	// <0x58596d>|0x1e7|0x00b:'570'



	// <0x585972>|0x1ec|0x005:'574'



	// <0x5859f1>|0x26b|0x07f:'578'

	// <0x585a18>|0x292|0x027:'580'
	// <0x585a40>|0x2ba|0x028:'581'

	// <0x585a42>|0x2bc|0x002:'583'


	// <0x585a71>|0x2eb|0x02f:'586'

	// <0x585a8a>|0x304|0x019:'588'




	// <0x585a9b>|0x315|0x011:'593'
	// <0x585ac5>|0x33f|0x02a:'594'
	// <0x585aec>|0x366|0x027:'595'
	// <0x585b12>|0x38c|0x026:'596'

	// <0x585b27>|0x3a1|0x015:'598'

	// ******
}

// STATE[STUB]
void bullet_character_controller::step_down( float dt, bool change_size_only, btVector3 const& pos_up_correction )
{
	// LOCALS
	// character_move_test_callback    callback
	// btTransform                     start
	// btTransform                     finish
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5751dc }, type_index: TypeIndex(0x81ca) })
	// ******

	// FUNCTION BODY
	// <0x585cdc>|0x000|0x000:'627'


	// <0x585d1c>|0x040|0x040:'630'
	// <0x585d1f>|0x043|0x003:'631'

	// <0x585d43>|0x067|0x024:'633'

	// <0x585dcd>|0x0f1|0x08a:'635'
	// <0x585de3>|0x107|0x016:'636'



	// <0x585de6>|0x10a|0x003:'640'



	// <0x585de9>|0x10d|0x003:'644'



	// <0x585eca>|0x1ee|0x0e1:'648'

	// <0x585eef>|0x213|0x025:'650'
	// <0x585f19>|0x23d|0x02a:'651'

	// <0x585f1b>|0x23f|0x002:'653'


	// <0x585f44>|0x268|0x029:'656'

	// <0x585f5a>|0x27e|0x016:'658'
	// <0x585f6c>|0x290|0x012:'659'

	// <0x585fc0>|0x2e4|0x054:'661'

	// <0x586039>|0x35d|0x079:'663'
	// <0x5860ae>|0x3d2|0x075:'664'
	// <0x5860c2>|0x3e6|0x014:'665'
	// <0x5860c9>|0x3ed|0x007:'666'
	// <0x5860d0>|0x3f4|0x007:'667'

	// <0x5860d5>|0x3f9|0x005:'669'


	// <0x586188>|0x4ac|0x0b3:'672'


	// <0x58618d>|0x4b1|0x005:'675'












































	// <0x5861a9>|0x4cd|0x01c:'720'
	// <0x5861d5>|0x4f9|0x02c:'721'
	// <0x5861e1>|0x505|0x00c:'722'

	// ******
}

// STATE[STUB]
void bullet_character_controller::set_desired_walk_vector( btVector3 const& walk_vector )
{
	// FUNCTION BODY
	// <0x584fde>|0x000|0x000:'728'
	// <0x584fe5>|0x007|0x007:'729'

	// <0x584ffb>|0x01d|0x016:'731'
	// <0x585027>|0x049|0x02c:'732'


	// ******
}

// STATE[STUB]
void bullet_character_controller::pre_step( float __formal )
{
	// FUNCTION BODY
	// <0x5856c0>|0x000|0x000:'739'
	// <0x5856f7>|0x037|0x037:'740'







	// <0x58570f>|0x04f|0x018:'748'

	// <0x585726>|0x066|0x017:'750'
	// <0x585727>|0x067|0x001:'751'





	// <0x58573c>|0x07c|0x015:'757'

	// ******
}

// STATE[STUB]
bool bullet_character_controller::can_jump( ) const
{
	// FUNCTION BODY
	// <0x5845f1>|0x000|0x000:'1077'
	// ******
}

// STATE[STUB]
void bullet_character_controller::jump( )
{
	// FUNCTION BODY
	// <0x5849f1>|0x000|0x000:'1082'






	// <0x584a2c>|0x03b|0x03b:'1089'
	// ******
}

// STATE[STUB]
void bullet_character_controller::end_jump( )
{
	// FUNCTION BODY
	// <0x584520>|0x000|0x000:'1094'
	// ******
}

// STATE[STUB]
bool bullet_character_controller::on_ground( ) const
{
	// FUNCTION BODY
	// <0x5845b1>|0x000|0x000:'1105'
	// ******
}

// STATE[STUB]
void bullet_character_controller::setup_shape_dim( float2 const& shape_dim )
{
	// FUNCTION BODY

	// <0x584549>|0x000|0x000:'1111'
	// ******
}

// STATE[STUB]
void bullet_character_controller::setup_crouch_state( bool crouch )
{
	// LOCALS
	// btVector3                       prev_shape_offset
	// btVector3                       orign
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5739bc }, type_index: TypeIndex(0x1397d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5739e1 }, type_index: TypeIndex(0x14fb0) })
	// ******

	// FUNCTION BODY






	// <0x584809>|0x000|0x000:'1122'
	// <0x584896>|0x08d|0x08d:'1123'
	// <0x58489e>|0x095|0x008:'1124'

	// <0x5848a0>|0x097|0x002:'1126'
	// <0x5848ac>|0x0a3|0x00c:'1127'
	// <0x5848fc>|0x0f3|0x050:'1128'





	// <0x58490f>|0x106|0x013:'1134'
	// <0x584986>|0x17d|0x077:'1135'
	// <0x58499c>|0x193|0x016:'1136'

	// <0x5849aa>|0x1a1|0x00e:'1138'

	// <0x5849be>|0x1b5|0x014:'1140'
	// <0x5849c5>|0x1bc|0x007:'1141'






	// ******
}

// STATE[STUB]
void bullet_character_controller::insert( btDynamicsWorld* world )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573bde }, type_index: TypeIndex(0x162ec) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573be9 }, type_index: TypeIndex(0x1604b) })
	// ******

	// FUNCTION BODY
	// <0x584bb4>|0x000|0x000:'1152'
	// <0x584bb8>|0x004|0x004:'1153'
	// <0x584bbf>|0x00b|0x007:'1154'
	// <0x584be0>|0x02c|0x021:'1155'
	// <0x584beb>|0x037|0x00b:'1156'



	// ******
}

// STATE[STUB]
void bullet_character_controller::remove( btDynamicsWorld* world )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573771 }, type_index: TypeIndex(0x1604b) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573782 }, type_index: TypeIndex(0x162d0) })
	// ******

	// FUNCTION BODY

	// <0x584766>|0x000|0x000:'1165'
	// <0x584773>|0x00d|0x00d:'1166'
	// <0x584784>|0x01e|0x011:'1167'



	// <0x5847ac>|0x046|0x028:'1171'

	// <0x5847eb>|0x085|0x03f:'1173'
	// ******
}

// STATE[STUB]
btTransform bullet_character_controller::get_transform( )
{
	// FUNCTION BODY
	// <0x5846a9>|0x000|0x000:'1178'

	// <0x584701>|0x058|0x058:'1180'

	// ******
}

// STATE[STUB]
void bullet_character_controller::set_transform( btTransform const& transform )
{
	// FUNCTION BODY
	// <0x584f4c>|0x000|0x000:'1186'
	// <0x584fb7>|0x06b|0x06b:'1187'
	// ******
}

// STATE[STUB]
void bullet_character_controller::set_crouch( bool crouch )
{
	// FUNCTION BODY
	// <0x584b6a>|0x000|0x000:'1192'


	// <0x584b72>|0x008|0x008:'1195'

	// <0x584b76>|0x00c|0x004:'1197'
	// <0x584b78>|0x00e|0x002:'1198'


	// <0x584b7a>|0x010|0x002:'1201'

	// <0x584b81>|0x017|0x007:'1203'
	// ******
}

// STATE[STUB]
bool bullet_character_controller::can_stand( )
{
	// FUNCTION BODY
	// <0x584510>|0x000|0x000:'1208'


























	// ******
}

} // namespace physics
} // namespace vostok