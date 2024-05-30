/***********************************
 * @file    Button_CB.h
 * @author  Gfy63 (mrgoofy@gmx.net)
 * @brief   Class for button control. Click, DoubleClick, LongClick, LongClickPulse.
 *          A event is raise on clicks.
 * @version 0.1.1
 * @date 2024-01-26
 * 
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * @copyright 2024
 **********************************/

#ifndef BUTTON_CB_H
#define BUTTONCB_H

// #include 
#include <Arduino.h>

/**
 * --- DEBUG HELPER ---
*/
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
		/**
		 * --- #DEFINE ---
		*/

		#define PIN_UNDEFIIND	255		// Pin not defined.

		/**
		 * --- STRUCT & TYPEDEF ---
		*/

  		// typedef std::function<void(int event)> CallbackFunction;
		typedef void( *CallbackFunction)(unsigned int event);

		// // Button_CB events type.
		// enum eventType {
		// 	NO_EVENT,					// No Clicks jet.
		// 	CLICK_EVENT,				// Click
		// 	DOUBLECLICK_EVENT,			// DoubleClick
		// 	MULTICLICK_EVENT,			// MultiClick, getClicks() for the count.
		// 	LONGCLICK_DETECT_EVENT,		// LongClick detect, pulse start.
		// 	LONGCLICK_PULSE_EVENT,		// LongClick pulse.
		// 	LONGCLICK_EVENT				// LongClick (end)
		// };

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

		/**
		 * --- CONSTRUCTOR & BEGIN ---
		*/

		/**
		 * @brief Constructors.
		 * @param   pin Button pin.
		 * @param   btnMode Button mode.INPUT or INPUT_PULLUP. (default: INPUT_PULLUP) (optional)
		 * @param	activeLow True or false. (default: true) (optional)
		 * @param   cb Callback function for Event calls.
		 */
		Button_CB( byte pin, byte btnMode, bool activeLow, CallbackFunction cb );
		Button_CB( byte pin, byte btnMode, CallbackFunction cb );
		Button_CB( byte pin, CallbackFunction cb );
		Button_CB();

		/**
		 * @brief Start with the configuration.
		 * @param   pin Button pin.
		 * @param   btnMode Button mode.INPUT or INPUT_PULLUP. (default: INPUT_PULLUP) (optional)
		 * @param	activeLow True or false. (default: true) (optional)
		 * @param   cb Callback function for Event calls.
		 */
		void begin( byte pin, byte btnMode , bool activeLow, CallbackFunction cb );
		void begin( byte pin, byte btnMode, CallbackFunction cb );
		void begin( byte pin, CallbackFunction cb );
		
		/**
		 * --- PUBLIC FUNCTIONS ---
		*/

		/**
		 * @brief Button master loop.
		*/
		void loop( void );

		/**
		 * @brief get the Click count if event is MULTICLICK_EVENT.
		 * @return	Click count.
		*/
		int getClickCount( void );

		/**
		 * @brief get the duration of the running or last click.
		 * @return	Click duration.
		*/
		unsigned int getClickElapsedTime( void );

		/**
		 * @brief get last event.
		 * @return	Event.
		*/
		int getEvent( void );

		/**
		 * @brief Stop callback BTN_LONGCLICK_PULSE_EVENT and BTN_LONGCLICK_EVENT.
		 *			Use if longClick time is reacht.
		*/
		void LongClick_Abort( void );

		/**
		 * @brief Set/get the button debounce delay.
		 * @param debounce Debounce delay in mSec.
		*/
		void setDebounceDelay( unsigned int debounce );
		/**
		 * @brief 	Set/get the button debounce delay.
		 * @return	Debounce delay in mSec.
		*/
		unsigned int getDebounceDelay( void );

		/**
		 * @brief Set/get the DoubleClick delay. Time from release to next press of the button.
		 * @param dclickDelay DoubleClick delay in mSec.
		*/
		void setDoubleClickDelay( unsigned int dclickDelay );
		/**
		 * @brief 	Set/get the DoubleClick delay. Time from release to next press of the button.
		 * @return	DoubleClick delay in mSec.
		*/
		unsigned int getDoubleClickDelay( void );
	
		/**
		 * @brief Set/get the LongClick start delay, from ther on the button press is a LongClick.
		 * @param lclickStartDelay LongClick start delay in mSec.
		*/
		void setLongClickStartDelay( unsigned int lclickStartDelay );
		/**
		 * @brief Set/get the LongClick start delay, from ther on the button press is a LongClick.
		 * @return	LongClick start delay in mSec.
		*/
		unsigned int getLongClickStartDelay( void );

		/**
		 * @brief Set/get the delay beween to LongClickPulses.
		 * @param lclickPulseDelay LongClick pulse delay in mSec.
		*/
		void setLongClickPulseDelay( unsigned int lclickPulseDelay );
		/**
		 * @brief Set/get the delay beween to LongClickPulses.
		 * @return	LongClick pulse delay in mSec.
		*/
		unsigned int getLongClickPulseDelay( void );

	private:
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
													// use if longClick time is reacht.

		int _cycle = WAIT_BUTTON_PRESSED;			// Click cycle start.

		/**
		 * @brief 	Wait for debounce time to elapse.
		 * @param	debounceStart Starttime for debounce.
		 * @return	True - Debounce time elapse.
		 */
		bool WaitDebounceElapse( unsigned long debounceStart );

		/**
		 * @brief 	Fire the callback function.
		 * @param	event Event to send with.
		 */
		void FireCallback( int event );

		/**
		 * @brief 	Is the button pressed?
		 * @return	true if pressed.
		 */
		bool IsBtnPressed( void );

};

#endif

// End of 'Button_BC.h'.