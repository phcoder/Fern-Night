/*==============================================================================
Copyright (C) Andrzej Adamczyk (at https://blackdev.org/). All rights reserved.
==============================================================================*/

uint64_t driver_rtc_time() {
	// retrieve weekday
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_weekday );
	uint64_t current = driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve year
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_year );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve month
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_month );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve day
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_day_of_month );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve hour
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_hour );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve minutes
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_minutes );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// retrieve seconds
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_REGISTER_seconds );
	current = (current << STATIC_MOVE_AL_TO_HIGH_shift) | driver_port_in_byte( DRIVER_RTC_PORT_data );

	// return current time
	return current;
}

void driver_rtc_init() {
	// state of real-time controller register
	uint8_t register_status = EMPTY;

	// controller is idle?
	do {
		// get state of register A
		driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_STATUS_REGISTER_A );
		register_status = driver_port_in_byte( DRIVER_RTC_PORT_data );
	} while( register_status & DRIVER_RTC_STATUS_REGISTER_A_update_in_progress );

	// put controller into modification mode of register A
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_STATUS_REGISTER_A | 0x80 );

	// set calling frequency to 1024 Hz
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_STATUS_REGISTER_A );
	driver_port_out_byte( DRIVER_RTC_PORT_data, DRIVER_RTC_STATUS_REGISTER_A_divider | DRIVER_RTC_STATUS_REGISTER_A_rate );

	// put controller into modification mode of register B
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_STATUS_REGISTER_B | 0x80 );

	// get current state of registry flags
	register_status = driver_port_in_byte( DRIVER_RTC_PORT_data );

	// set registry flags
	register_status |= DRIVER_RTC_STATUS_REGISTER_B_24_hour_mode;
	register_status |= DRIVER_RTC_STATUS_REGISTER_B_data_mode_binary;
	register_status |= DRIVER_RTC_STATUS_REGISTER_B_periodic_interrupt;
	register_status &= ~DRIVER_RTC_STATUS_REGISTER_B_update_ended_interrupt;
	register_status &= ~DRIVER_RTC_STATUS_REGISTER_B_alarm_interrupt;
	driver_port_out_byte( DRIVER_RTC_PORT_data, register_status );

	// receive pending interrupt of real-time controller
	driver_port_out_byte( DRIVER_RTC_PORT_command, DRIVER_RTC_STATUS_REGISTER_C );
	driver_port_in_byte( DRIVER_RTC_PORT_data );
}