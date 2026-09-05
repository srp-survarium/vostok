// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_SCALEFORM_API_H_INCLUDED
#define VOSTOK_SCALEFORM_API_H_INCLUDED

#ifndef VOSTOK_SCALEFORM_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_SCALEFORM_API
#	else
#		ifdef VOSTOK_SCALEFORM_BUILDING
#			define VOSTOK_SCALEFORM_API VOSTOK_DLL_EXPORT
#		else
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_SCALEFORM_API VOSTOK_DLL_IMPORT
#			else
#				define VOSTOK_SCALEFORM_API VOSTOK_DLL_EXPORT
#			endif
#		endif
#	endif
#endif // #ifndef VOSTOK_SCALEFORM_API

// no module entry points: the shipped vostok/scaleform module is pure glue
// (survarium:: flash wrappers + the Scaleform D3D11 render HAL fork); its
// world is owned by game (scaleform_engine::initialize/destroy in factory.cpp)

#endif // #ifndef VOSTOK_SCALEFORM_API_H_INCLUDED
