// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "flash_external_handler.h"
#include "flash_external_handler_impl.h"
#include "flash_function_handler.h"
#include "flash_function_handler_impl.h"
#include "flash_function_handler_params.h"
#include "flash_movie.h"
#include "flash_value.h"

#include "GFx.h"

namespace survarium {

// flash_value stores a GFx::Value in its opaque body buffer.
Scaleform::GFx::Value& gfx( flash_value const& v )
{
	return *( Scaleform::GFx::Value* )( v.body );
}

Scaleform::GFx::Value* gfx( flash_value const* v )
{
	return ( Scaleform::GFx::Value* )( v->body );
}

flash_value::flash_value( )
{
	new ( body ) Scaleform::GFx::Value( );
}

flash_value::~flash_value( )
{
	gfx( this )->~Value( );
}

void flash_value::SetBoolean( bool value )
{
	gfx( this )->SetBoolean( value );
}

// claude@NOTE: the target inlines the string assignments; this compiler island keeps them out of line.
void flash_value::SetString( pcstr value )
{
	Scaleform::GFx::Value vv( value );
	*gfx( this ) = vv;
}

void flash_value::SetStringW( wchar_t const* value )
{
	Scaleform::GFx::Value vv( value );
	*gfx( this ) = vv;
}

void flash_value::SetInt( s32 value )
{
	gfx( this )->SetInt( value );
}

void flash_value::SetUInt( u32 value )
{
	gfx( this )->SetUInt( value );
}

void flash_value::SetNumber( float value )
{
	gfx( this )->SetNumber( value );
}

float flash_value::GetNumber( )
{
	return ( float )gfx( this )->GetNumber( );
}

bool flash_value::GetBool( )
{
	return gfx( this )->GetBool( );
}

void flash_value::SetMember( pcstr member_name, flash_value& value )
{
	gfx( this )->SetMember( member_name, gfx( value ) );
}

void flash_value::GetMember( pcstr member_name, flash_value* value )
{
	gfx( this )->GetMember( member_name, gfx( value ) );
}

void flash_value::SetElement( u32 idx, flash_value& value )
{
	gfx( this )->SetElement( idx, gfx( value ) );
}

// claude@NOTE: the target passes the string argument in ESI through a custom LTCG convention.
void flash_value::SetElement( u32 idx, pcstr value )
{
	gfx( this )->SetElement( idx, Scaleform::GFx::Value( value ) );
}

void flash_value::PushBack( flash_value& value )
{
	gfx( this )->PushBack( gfx( value ) );
}

void flash_value::GetElement( u32 index, flash_value* value )
{
	gfx( this )->GetElement( index, gfx( value ) );
}

u32 flash_value::GetArraySize( )
{
	return gfx( this )->GetArraySize( );
}

pcstr flash_value::GetString( ) const
{
	return gfx( this )->GetString( );
}

void flash_value::SetConvertStringW( )
{
	gfx( this )->SetConvertStringW( );
}

wchar_t const* flash_value::GetStringW( ) const
{
	return gfx( this )->GetStringW( );
}

s32 flash_value::GetInt( ) const
{
	return gfx( this )->GetInt( );
}

u32 flash_value::GetUInt( ) const
{
	return gfx( this )->GetUInt( );
}

void flash_movie::SetVariable( pcstr path, pcstr value )
{
	m_movie->SetVariable( path, value );
}

void flash_movie::SetVariable( pcstr path, flash_value const& value )
{
	m_movie->SetVariable( path, gfx( value ) );
}

void flash_movie::GetVariable( flash_value* value, pcstr path )
{
	m_movie->GetVariable( gfx( value ), path );
}

void flash_external_handler_impl::Callback(
		Scaleform::GFx::Movie*			movie,
		pcstr							method_name,
		Scaleform::GFx::Value const*	args,
		u32								args_count
	)
{
	flash_movie*	flash	= ( flash_movie* )movie->GetUserData( );
	owner.callback(
		flash,
		method_name,
		( flash_value const* )args,
		args_count
	);
}

bool flash_movie::Invoke( pcstr method_name, flash_value* result, flash_value const* args, u32 args_count )
{
	return m_movie->Invoke( method_name, gfx( result ), gfx( args ), args_count );
}

void flash_movie::CreateArray( flash_value* value )
{
	m_movie->CreateArray( gfx( value ) );
}

void flash_movie::CreateObject( flash_value* value )
{
	m_movie->CreateObject( gfx( value ) );
}

void flash_movie::SetExternalInterface( flash_external_handler* handler )
{
	m_movie->SetExternalInterface( handler->impl );
}

void flash_movie::CreateFunction( flash_value* value, flash_function_handler* handler )
{
	m_movie->CreateFunction( gfx( value ), handler->impl );
}

flash_external_handler::flash_external_handler( )
{
	impl = SF_NEW flash_external_handler_impl( *this );
}

flash_external_handler::~flash_external_handler( )
{
	delete impl;
}

flash_function_handler_impl::flash_function_handler_impl( flash_function_handler& o )
	:	owner	( o )
{
}

void flash_function_handler_impl::Call( Scaleform::GFx::FunctionHandler::Params const& params )
{
	flash_function_handler_params	p;
	p.pRetVal	= ( flash_value* )params.pRetVal;
	p.pArgs		= ( flash_value* )params.pArgs;
	p.ArgCount	= params.ArgCount;
	p.pMovie	= ( flash_movie* )params.pMovie->GetUserData( );

	owner.call( p );
}

flash_function_handler::flash_function_handler( )
{
	impl = SF_NEW flash_function_handler_impl( *this );
}

flash_function_handler::~flash_function_handler( )
{
	delete impl;
}

} // namespace survarium
