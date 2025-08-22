////////////////////////////////////////////////////////////////////////////
//	Created		: 07.05.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "collision_node.h"
#include "create_collision_cmd.h"
#include "build_collision_cmd.h"
#include "body_node.h"
#include "body_node_manip.h"
#include "joint_node.h"
#include "anim_export_util.h"
#include "skeleton_export_util.h"
#include "create_body_cmd.h"
#include "create_joint_cmd.h"
#include "physics_exporter.h"
#include "solid_visual_exporter.h"
#include "skin_exporter.h"
#include "fake_drawing_locator.h"
#include "vostok_path_commands.h"
//#include "vostok_material.h"
#include <vostok/core/core.h>
#include <vostok/command_line_extensions.h>
#include <vostok/core/simple_engine.h>
#include "maya_engine.h"
#include <vostok/maya_animation/world.h>
#include <vostok/maya_animation/api.h>
#include <vostok/os_include.h>
#include <vostok/animation/api.h>
#include <vostok/resources_fs.h>
#include <vostok/resources.h>
#include <vostok/fs/simple_synchronous_interface.h>

vostok::maya::allocator_type			vostok::maya::g_allocator(vostok::memory::thread_id_const_false);
vostok::maya::maya_engine				core_engine;
vostok::maya::maya_engine&			vostok::maya::g_maya_engine = core_engine;

vostok::uninitialized_reference< vostok::fs_new::simple_synchronous_interface >	fs_devices;
static vostok::resources::mount_ptr	s_resources_mount;

static bool						s_on_resources_mounted_called	= false;

static void on_resources_mounted		( vostok::resources::mount_ptr * out_mount, vostok::resources::mount_ptr result )
{
	CHECK_OR_EXIT						( result, "Cannot mount resources" );
	* out_mount							= result;
	s_on_resources_mounted_called		= true;
}

static void scene_loading	( void* )
{
	vostok::maya::g_maya_engine.m_is_scene_loading = true;
}

static void scene_loaded	( void * )
{
	vostok::maya::g_maya_engine.m_is_scene_loading = false;
}

VOSTOK_DLL_EXPORT MStatus initializePlugin( MObject obj )
{
	vostok::core::preinitialize	(
		& core_engine,
		vostok::logging::no_log, 
		"-enable_crt_memory_allocator -no_memory_usage_stats -no_fs_watch -floating_point_control_disabled -fpe_disabled -log_to_stdout -max_resources_size=0 -debug_allocator=64",
		vostok::command_line::contains_application_false,
		"maya",
		__DATE__
	);

	MStatus stat						= MStatus::kSuccess;

	LOG_INFO( "initialize vostok maya plugin!" );

	vostok::maya::g_allocator.do_register	( 50 * vostok::Mb, "maya" );
	vostok::memory::allocate_region		( );

	pstr lua_config_device_folder_to_save_to = 0;
	STR_JOINA							( lua_config_device_folder_to_save_to, core_engine.get_resources_path(), "/exported/" );
	vostok::core::initialize				( lua_config_device_folder_to_save_to, "maya plugin", vostok::core::perform_debug_initialization );

	VOSTOK_CONSTRUCT_REFERENCE			( fs_devices, vostok::fs_new::simple_synchronous_interface ) 
											( vostok::fs_new::watcher_enabled_true );

	vostok::core::initialize_resources	( fs_devices->hdd_async, fs_devices->dvd_async, vostok::resources::enable_fs_watcher_false );
	vostok::resources::start_resources_threads	( );

	vostok::logging::push_filter			( "", vostok::logging::warning, &vostok::maya::g_allocator );
	
	vostok::logging::push_filter			( "maya", vostok::logging::trace, &vostok::maya::g_allocator );

	vostok::animation::set_memory_allocator( vostok::maya::g_allocator );

	core_engine.animation_world = vostok::animation::create_world( core_engine, 0, 0, 0 );
	ASSERT( core_engine.animation_world );
	
	vostok::maya_animation::set_memory_allocator( vostok::maya::g_allocator );
	core_engine.maya_animation_world = vostok::maya_animation::create_world( core_engine );

	ASSERT( core_engine.maya_animation_world );


	MFnPlugin plugin	( obj, "GSC Game World", "1.0", "Any");

	build_sphere		(stat);
	CHK_STAT_R			( stat );

	build_cylinder		(stat);
	CHK_STAT_R			( stat );

	stat = plugin.registerTransform(	collision_node::typeName,
										collision_node::typeId,
										&collision_node::creator, 
										&collision_node::initialize, 
										MPxTransformationMatrix::creator, 
										MPxTransformationMatrix::baseTransformationMatrixId);

	CHK_STAT_R			( stat );

	stat = plugin.registerTransform(	body_node::typeName, 
										body_node::typeId, 
										&body_node::creator, 
										&body_node::initialize, 
										MPxTransformationMatrix::creator, 
										MPxTransformationMatrix::baseTransformationMatrixId);
	CHK_STAT_R			( stat );

	stat = plugin.registerTransform(	joint_node::typeName,
										joint_node::typeId,
										&joint_node::creator, 
										&joint_node::initialize, 
										MPxTransformationMatrix::creator, 
										MPxTransformationMatrix::baseTransformationMatrixId);

	CHK_STAT_R			( stat );

	stat = plugin.registerNode(			fake_drawing_locator::typeName,
										fake_drawing_locator::typeId,
										&fake_drawing_locator::creator, 
										&fake_drawing_locator::initialize, 
										MPxNode::kLocatorNode );

	CHK_STAT_R			( stat );

	stat = plugin.registerNode(			body_node_manip::typeName,
										body_node_manip::typeId, 
										&body_node_manip::creator,
										&body_node_manip::initialize, 
										MPxNode::kManipContainer );

	CHK_STAT_R			( stat );

	stat = plugin.registerCommand( createBodyCmd::Name, createBodyCmd::creator, createBodyCmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand( createCollisionCmd::Name, createCollisionCmd::creator, createCollisionCmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand( createJointCmd::Name, createJointCmd::creator, createJointCmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand( build_collision_cmd::Name, build_collision_cmd::creator, build_collision_cmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerFileTranslator("vostok_ph_exporter", NULL, &physics_exporter::creator);
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand(	vostok::maya::solid_visual_exporter::Name, 
									vostok::maya::solid_visual_exporter::creator, 
									vostok::maya::solid_visual_exporter::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand(vostok_path_cmd::Name, vostok_path_cmd::creator, vostok_path_cmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand(anim_export_cmd::Name, anim_export_cmd::creator, anim_export_cmd::newSyntax );
	CHK_STAT_R			( stat );

	stat = plugin.registerCommand(skeleton_export_cmd::Name, skeleton_export_cmd::creator, skeleton_export_cmd::newSyntax );
	CHK_STAT_R			( stat );


	//const MString UserClassify( "shader/surface" );

	//stat = plugin.registerNode( vostok_material_node::Name, 
	//							vostok_material_node::id, 
	//							vostok_material_node::creator, 
	//							vostok_material_node::initialize,
	//							MPxNode::kDependNode, 
	//							&UserClassify );
	//CHK_STAT_R		( stat );

	//const MString& swatchName =	MHWShaderSwatchGenerator::initialize();
	//const MString UserClassify_hw( "shader/surface/utility/:swatch/"+swatchName );

	//stat = plugin.registerNode( "hwPhongShader", hwPhongShader::id, 
	//		                      hwPhongShader::creator, hwPhongShader::initialize,
	//							  MPxNode::kHwShaderNode, &UserClassify_hw );
	//CHK_STAT_R		( stat );

	//plugin.registerDragAndDropBehavior("hwPhongShaderBehavior", 
	//								   hwPhongShaderBehavior::creator);

	//
	//MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	//command			+= UserClassify;
	//command			+= "\");}\n";

	//MGlobal::executeCommand( command );

	//command = ( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	//command			+= UserClassify_hw;
	//command			+= "\");}\n";

	//MGlobal::executeCommand( command );

	stat = plugin.registerCommand(	vostok::maya::skinned_visual_exporter::Name, 
									vostok::maya::skinned_visual_exporter::creator, 
									vostok::maya::skinned_visual_exporter::newSyntax );
	CHK_STAT_R			( stat );
	
	
	MCallbackId id;
	id					= MSceneMessage::addCallback( MSceneMessage::kBeforeOpen, scene_loading, NULL, &stat );
	CHK_STAT_R			( stat );
	vostok::maya::g_maya_engine.register_callback_id( id );
	id					= MSceneMessage::addCallback( MSceneMessage::kAfterOpen, scene_loaded, NULL, &stat );
	CHK_STAT_R			( stat );
	vostok::maya::g_maya_engine.register_callback_id( id );
	//id					= MSceneMessage::addCallback( MSceneMessage::kAfterPluginLoad, plugin_loaded, NULL, &stat );
	//CHK_STAT_R			( stat );
	//vostok::maya::g_maya_engine.register_callback_id( id );


	
	vostok::resources::query_mount			( "resources", 
											boost::bind(& on_resources_mounted, & s_resources_mount, _1), 
											&vostok::maya::g_allocator );

	vostok::timing::timer	timer;
	timer.start			( );
	while ( !s_on_resources_mounted_called )
	{
		vostok::resources::dispatch_callbacks();
		if ( vostok::threading::g_debug_single_thread )
			vostok::resources::tick				( );

		if ( timer.get_elapsed_sec() > 10 && !vostok::debug::is_debugger_present() )
			break;
	}
	
	vostok::resources::wait_and_dispatch_callbacks( true );


	if ( !s_on_resources_mounted_called || !s_resources_mount )
		return					MStatus::kFailure;

	LOG_INFO("Initialized VOSTOK maya plugin!");

	return stat;
}

VOSTOK_DLL_EXPORT MStatus uninitializePlugin( MObject obj)
{
	s_resources_mount	=	NULL;
	LOG_INFO("uninitializePlugin called");
	MStatus			stat;
	MFnPlugin		plugin( obj );

	stat = plugin.deregisterNode( collision_node::typeId );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterNode( body_node::typeId );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterNode( body_node_manip::typeId );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterNode( fake_drawing_locator::typeId );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterNode( joint_node::typeId );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( createBodyCmd::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( createCollisionCmd::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( createJointCmd::Name );
	CHK_STAT_R			( stat );
	
	stat = plugin.deregisterCommand( build_collision_cmd::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterFileTranslator("vostok_ph_exporter");
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( vostok::maya::solid_visual_exporter::Name );
	CHK_STAT_R			( stat );
	

	stat = plugin.deregisterCommand( vostok::maya::skinned_visual_exporter::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( vostok_path_cmd::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( anim_export_cmd::Name );
	CHK_STAT_R			( stat );

	stat = plugin.deregisterCommand( skeleton_export_cmd::Name );
	CHK_STAT_R			( stat );

	//CHK_STAT_R ( plugin.deregisterNode( vostok_material_node::id ) );
	//CHK_STAT_R ( plugin.deregisterNode( hwPhongShader::id ) );
	//plugin.deregisterDragAndDropBehavior("hwPhongShaderBehavior");

	//const MString UserClassify( "shader/surface" );
	//MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );
	//command			+= UserClassify;
	//command			+= "\");}\n";
	//MGlobal::executeCommand( command );

	vostok::maya_animation::destroy_world	( core_engine.maya_animation_world );
	vostok::animation::destroy_world		( core_engine.animation_world );

	for(int i=0; i<20; ++i)
	{
		vostok::resources::dispatch_callbacks		( );
		vostok::threading::yield					(50);
	}

	vostok::resources::finish_resources_threads	( );
	vostok::core::finalize_resources		( );
	vostok::core::finalize				( );

	VOSTOK_DESTROY_REFERENCE				( fs_devices );

	return stat;
}