////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2009
//	Author		: Igor Lobanchikov
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/device.h>
#include "com_utils.h"

#if USE_DX10 
#	pragma warning( push )
#	pragma warning( disable : 4995 )
#	include <d3dx10.h>
#	pragma warning( pop )
#endif // #if !USE_DX10 

vostok::command_line::key	g_debug_render_device("debug_dx", "", "render", "create d3d device with D3D_CREATE_DEVICE_DEBUG flag");

namespace vostok {
namespace render {

// REMAINDER: setup_states/get_device_state (mostly commented-out DX9 era,
// canonical class dropped them) + vid/render mode list shims (target replaced
// with real monitor enumeration).

inline void	fill_vid_mode_list			( const device* const) {;}
inline void	free_vid_mode_list			() {;}

inline void	fill_render_mode_list		() {;}
inline void	free_render_mode_list		() {;}












// Not sure if we need this in DX10 ========
//
// u32 device::select_gpu() const
// {
// 	if ( m_caps.force_gpu_sw) return D3DCREATE_SOFTWARE_VERTEXPROCESSING;
// 
// 	D3DCAPS9	caps;
// 	m_d3d->GetDeviceCaps( m_dev_adapter, m_dev_type, &caps);
// 
// 	if( caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
// 	{
// 		if ( m_caps.force_gpu_non_pure)	
// 			return D3DCREATE_HARDWARE_VERTEXPROCESSING;
// 		else 
// 		{
// 			if ( caps.DevCaps&D3DDEVCAPS_PUREDEVICE) 
// 				return D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_PUREDEVICE;
// 			else 
// 				return D3DCREATE_HARDWARE_VERTEXPROCESSING;
// 		}
// 	} else return D3DCREATE_SOFTWARE_VERTEXPROCESSING;
// }

//bool device::support( DXGI_FORMAT fmt, DWORD usage)
//{
//	//	TODO:DX10: implement stub for this code.
//	//	ASSERT( !"Implement CHW::support");
//	u32 res;
//	m_device->CheckFormatSupport( fmt, &res);
//
//	return res == ( res&usage);
//}


//bool device::begin_frame()
//{
//	//	TODO: SYSTEM
//	//RCache.OnFrameBegin		();
//	//if ( HW.m_caps.SceneMode)	overdrawBegin	();
//
//	return true;
//}

//void device::end_frame()
//{
//	m_render_output->present();
//	++m_frame;
//
//
//#ifndef DEDICATED_SERVER
//
////	if ( dwPrecacheFrame)
////	{
////		::Sound->set_master_volume	( 0.f);
////		dwPrecacheFrame	--;
////		pApp->load_draw_internal	();
////		if ( 0==dwPrecacheFrame)
////		{
////			m_pRender->updateGamma();
////
////			if( precache_light) precache_light->set_active	( false);
////			if( precache_light) precache_light.destroy		();
////			::Sound->set_master_volume						( 1.f);
////			pApp->destroy_loading_shaders					();
////
////			m_pRender->ResourcesDestroyNecessaryTextures	();
////			Memory.mem_compact								();
////			Msg												( "* MEMORY USAGE: %d K",Memory.mem_usage()/1024);
////			Msg												( "* End of synchronization A[%d] R[%d]",b_is_Active, b_is_Ready);
////
////#ifdef FIND_CHUNK_BENCHMARK_ENABLE
////			g_find_chunk_counter.flush();
////#endif // FIND_CHUNK_BENCHMARK_ENABLE
////
////			CheckPrivilegySlowdown							();
////
////			if( g_pGamePersistent->GameType()==1)//haCk
////			{
////				WINDOWINFO	wi;
////				GetWindowInfo( m_hWnd,&wi);
////				if( wi.dwWindowStatus!=WS_ACTIVECAPTION)
////					Pause( TRUE,TRUE,TRUE,"application start");
////			}
////		}
////	}
//
//	// end scene
//	//	Present goes here, so call OA Frame end.
//	//if ( g_SASH.IsBenchmarkRunning())
//	//	g_SASH.DisplayFrame( Device.fTimeGlobal);
//
//
//	//if ( HW.m_caps.SceneMode)	overdrawEnd();
//
//	//RCache.OnFrameEnd	();
//	//Memory.dbg_check		();
//
//#endif
//
//}

void device::setup_states() const
{
	// General Render States
	//mView.identity			();
	//mProject.identity		();
	//mFullTransform.identity	();
	//vCameraPosition.set		( 0,0,0);
	//vCameraDirection.set	( 0,0,1);
	//vCameraTop.set			( 0,1,0);
	//vCameraRight.set		( 1,0,0);

//	device::ref().update_caps();

	// --Porting to DX10_
	// setup_states_platform();
}

//void device::test_corrporate_level()
//{
//}

// This may be removed for DX10 
device::device_state device::get_device_state() const
{
// 	HRESULT	_hr		= pdevice->TestCooperativeLevel();
// 	if (FAILED(_hr))
// 	{
// 		// If the device was lost, do not render until we get it back
// 		if		(D3DERR_DEVICELOST==_hr)
// 			return ds_lost;
// 
// 		// Check if the device is ready to be reset
// 		if		(D3DERR_DEVICENOTRESET==_hr)
// 			return ds_need_reset;
// 	}
// 
 	return ds_ok;
}

// ID3DRenderTargetView * device::get_base_render_target()
// {
// 	return m_render_output.m_base_rt;
// }
// 
// ID3DDepthStencilView * device::get_depth_stencil_target()
// {
// 	return m_render_output.m_base_zb;
// }

//size_t device::video_memory_size	()
//{
//	return m_adapter_desc.DedicatedVideoMemory;
//}



} // namespace render 
} // namespace vostok 
