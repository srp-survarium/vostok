// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_SCALEFORM_API_H_INCLUDED
#define VOSTOK_SCALEFORM_API_H_INCLUDED

#ifndef VOSTOK_SCALEFORM_API
#	define VOSTOK_SCALEFORM_API
#endif // #ifndef VOSTOK_SCALEFORM_API

// no module entry points: the shipped vostok/scaleform module is pure glue
// (survarium:: flash wrappers + the Scaleform D3D11 render HAL fork); its
// world is owned by game (scaleform_engine::initialize/destroy in factory.cpp)

#endif // #ifndef VOSTOK_SCALEFORM_API_H_INCLUDED
