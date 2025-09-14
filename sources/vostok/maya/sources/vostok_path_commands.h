////////////////////////////////////////////////////////////////////////////
//	Created		: 07.09.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PATH_COMMANDS_H_INCLUDED
#define VOSTOK_PATH_COMMANDS_H_INCLUDED

class vostok_path_cmd : public MPxCommand 
{
public:
	static const MString	Name;
public:

	virtual MStatus			doIt		( const MArgList&);
	static void*			creator		( )					{return CRT_NEW(vostok_path_cmd);}
	virtual bool			isUndoable	( ) const			{return false;} 
	static MSyntax			newSyntax	( );
}; // class vostok_path_cmd

#endif // #ifndef VOSTOK_PATH_COMMANDS_H_INCLUDED