/***********************************
 * @file    Button_CB.h
 * @author  Gfy63 (mrgoofy@gmx.net)
 * @brief   Class for button control. Click, DoubleClick, LongClick, LongClickPulse.
 *          A event is raise on clicks.
 * @version 0.1.2
 * @date 2024-01-26
 * 
 * Licence: MIT
 * 
 * @copyright 2024-26
 **********************************/

#ifndef BUTTON_CB_H
#define BUTTON_CB_H

/*----------------------------------
	INCLUDE
----------------------------------*/

#include <Arduino.h>

/*----------------------------------
	DEBUG HELPER
----------------------------------*/

// #define BTN_DEBUG
#ifdef BTN_DEBUG
	#define btnDbg_print(x)		Serial.print(x)
	#define btnDbg_println(x)	Serial.println(x)
	#define btnDbg_printf(...)	Serial.printf(__VA_ARGS__)
#else
	#define btnDbg_print(x)
	#define btnDbg_println(x)
	#define btnDbg_printf(...)
#endif

/*----------------------------------
	CONSTANT
----------------------------------*/

// Button_CB events type.
enum eventType {
	BTN_NO_EVENT,					// No Clicks jet.
	BTN_CLICK_EVENT,				// Click
	BTN_DOUBLECLICK_EVENT,			// DoubleClick
	BTN_MULTICLICK_EVENT,			// MultiClick, getClickCount() for the count.
	BTN_LONGCLICK_DETECT_EVENT,		// LongClick detect, pulse start.
	BTN_LONGCLICK_PULSE_EVENT,		// LongClick pulse.
	BTN_LONGCLICK_EVENT				// LongClick (end)
};


class Button_CB
{
	protected:

	public:

		#define PIN_UNDEFIIND	255		// Pin not defined.

		/*----------------------------------
			STRUCT & TYPEDEF
		----------------------------------*/

		typedef void( *CallbackFunction)(unsigned int event);

		// Click detection cycle states.
		enum cycleType {
			WAIT_BUTTON_PRESSED,			// Wait for the button to be pressed. Cycle (Click) start.
			WAIT_DEBOUNCE_PRESSED,			// Wait for elapse of debounce time on pressed button.
			WAIT_BUTTON_RELEASED,			// Wait for the button to be released. Click ends.
			WAIT_DEBOUNCE_RELEASED,			// Wait for elapse of debounce time on released button.
			WAIT_IF_DCLICK_START,			// Wait if DoubleClick start.
			LCLICK_PROGRESS,				// LongClick in progress.
			WAIT_LCICK_DEBOUNCE_RELEASE		// Wait for elapse time of debounce on LongClick button release.
		};

		/*----------------------------------
			CONSTRUCTOR & BEGIN
		----------------------------------*/

		Button_CB( byte pin, byte btnMode, bool activeLow, CallbackFunction cb );
		Button_CB( byte pin, byte btnMode, CallbackFunction cb );
		Button_CB( byte pin, CallbackFunction cb );
		Button_CB();

		void begin( byte pin, byte btnMode , bool activeLow, CallbackFunction cb );
		void begin( byte pin, byte btnMode, CallbackFunction cb );
		void begin( byte pin, CallbackFunction cb );
		
		/*----------------------------------
			PUBLIC FUNCTIONS PROTOTYPE
		----------------------------------*/

		void loop( void );
		int getClickCount( void );
		unsigned int getClickElapsedTime( void );
		int getEvent( void );
		void LongClick_Abort( void );
		void setDebounceDelay( unsigned int debounce );
		unsigned int getDebounceDelay( void );
		void setDoubleClickDelay( unsigned int dclickDelay );
		unsigned int getDoubleClickDelay( void );
		void setLongClickStartDelay( unsigned int lclickStartDelay );
		unsigned int getLongClickStartDelay( void );
		void setLongClickPulseDelay( unsigned int lclickPulseDelay );
		unsigned int getLongClickPulseDelay( void );

	private:
	
		/*----------------------------------
			PRIVATE VARIABELS
		----------------------------------*/

		CallbackFunction _cb = NULL;	// Callback for events.
		byte _pin = PIN_UNDEFIIND;
		byte _btnMode = INPUT_PULLUP;
		bool _activeLow = true;				// Button active mode. (default=true)
		byte _buttonPressed = LOW;			// Button is pressed state. (default=LOW)

		// Timing in mSec.
		unsigned int _debounce_delay = 30;			// Debounce delay. (in mSec)
		unsigned int _dclick_delay = 150;			// Delay between to clicks to be DoubleClick. (in mSec)
		unsigned int _lclick_start_delay = 300;		// Delay to start the LongClick. (in mSec)
		unsigned int _lclick_pulse_delay = 250;		// Delay beween to LongClickPulses. (in mSec)

		int _event = BTN_NO_EVENT;					// Last event.
		int _click_cnt = 0;							// Nummbers of clicks done in click cycle.
		unsigned int _clickElapseTime;				// Total elapsed time since Click start.
		bool _lclick_abort = false;					// Stop callback BTN_LONGCLICK_PULSE_EVENT and BTN_LONGCLICK_EVENT.
													// use if longClick time is reached.

		int _cycle = WAIT_BUTTON_PRESSED;			// Click cycle start.

		/*----------------------------------
			PRIVATE FUNCTIONS PROTOTYPE
		----------------------------------*/

		bool WaitDebounceElapse( unsigned long debounceStart );
		void FireCallback( int event );
		bool IsBtnPressed( void );
		
};

#endif

// End of 'Button_CB.h'.