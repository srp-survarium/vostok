

#include <vostok/game_core/affect_event_type_enum.h>
#include <vostok/game_core/affects_applying_type_enum.h>
#include <vostok/game_core/hit_affects_type_enum.h>

#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/body_part_parameters.h>
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/damage_protector.h>
#include <vostok/game_core/hit_type_parameters.h>

namespace survarium
{

class IncludeAll
{
public:
	IncludeAll();
};


class Callback1
{
public:
	Callback1();

	void complete ( char const * name );
	void partial  ( char const * name, int value );

public:
	typedef boost::function< void ( char const * ) > fn;
	
	fn m_complete;
	fn m_partial;
};

class Callback2
{
public:
	Callback2();

	void complete ( char const * name, int value );
	void partial  ( char const * name, int value, float precision );

public:
	typedef boost::function< void ( char const *, int ) > fn;	

	fn m_complete;
	fn m_partial;
};

}
