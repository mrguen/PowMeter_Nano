#ifndef POWERMETERNANO_TYPEDEFS_h
#define POWERMETERNANO_TYPEDEFS_h

// ****************************************************************
// TYPEDEFS
// ****************************************************************


typedef enum
{
	SET_PERIOD								= 0,
	SET_DISPLAY								= 1,

	SET_USB_SOFT_ALERT				= 6,
	SET_VIN_SOFT_ALERT				= 7,

	SET_USB_MIN_VOLTAGE				= 10,
	SET_USB_MAX_VOLTAGE				= 11,
	SET_USB_MIN_CURRENT				= 12,
	SET_USB_MAX_CURRENT				= 13,

	SET_VIN_MIN_VOLTAGE				= 14,
	SET_VIN_MAX_VOLTAGE				= 15,
	SET_VIN_MIN_CURRENT				= 16,
	SET_VIN_MAX_CURRENT				= 17,

	SET_USB_HARD_ALERT				= 18,
	SET_VIN_HARD_ALERT				= 19,

	GET_USB_VOLTAGE						= 20,
	GET_USB_CURRENT						= 21,

	GET_VIN_VOLTAGE						= 22,
	GET_VIN_CURRENT						= 23,

  SET_POWER 								= 30,
  SET_I2C_ADDRESS 					= 31,

  SET_USB_CURRENT_OFFSET 		= 35,
  SET_VIN_CURRENT_OFFSET 		= 36

} Command;

typedef enum
{
   ALERT_OFF 				= 0,
   UNDER_VOLTAGE		= 1,
   OVER_VOLTAGE 		= 2,
   UNDER_CURRENT 		= 3,
   OVER_CURRENT 		= 4
} Alert;

typedef enum
{
   AUTO		= 0,
   USB 		= 1,
   VIN 		= 2,
   BOTH 	= 3
} Display;

typedef enum
{
   POWER_OFF 					= 0,
   POWER_ON 					= 1,
   MEASUREMENT_HALT 	= 2,
   OLED_OFF 					= 3,
   OLED_ON 						= 4
} Power;

typedef enum
{
  PERIOD_UNKNOWN 				= 0,
  DISPLAY_UNKNOWN 			= 1,
  ALERT_UNKNOWN					= 2,
  POWER_UNKNOWN 				= 3,
  INVALID_I2C_ADDRESS 	= 4,
  UNKNOWN_COMMAND 			= 5

} Error;


#endif