////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BODY_PART_PARAMETERS_H_INCLUDED
#define BODY_PART_PARAMETERS_H_INCLUDED

namespace stalker2 {

class damage_protector;

// @TODO: Should those enums be moved somewhere else?
// Make sure their size is __int32
enum hit_affects_type_enum
{
  affects_type_death = 0x0,
  affects_type_bleeding = 0x1,
  affects_type_concussion = 0x2,
  affects_type_hand_damage = 0x3,
  affects_type_leg_damage = 0x4,
  affects_type_critical_poisoning = 0x5,
  affects_type_poisoning = 0x6,
  affects_type_radiation_sickness = 0x7,
  affects_type_blindness = 0x8,
  affect_types_count = 0x9,
};

enum affects_applying_type_enum
{
  type_apply_directly = 0x0,
  type_read_only = 0x1,
};

enum affect_event_type_enum
{
  affect_applying = 0x0,
  affect_recalling = 0x1,
  affect_canceling = 0x2,
};

//


class body_part_parameters : public boost::noncopyable {
public:
	body_part_parameters();

	void hit_by_type(
		const char *hit_type,
		unsigned int time_in_ms,
		float amount,
		float armor_piercing,
		bool formal,
		damage_protector *prot
		);

public:
	body_part_parameters* next;
	// vostok::intrusive_list

}; // class body_part_parameters 

} // namespace stalker2

#endif // #ifndef BODY_PART_PARAMETERS_H_INCLUDED