/***********************************
 * @file    Button_CB.cpp
 * @author  Gfy63 (mrgoofy@gmx.net)
 * 
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * @copyright 2024
 **********************************/

#include "Button_CB.h"

/**
 * --- PUBLIC FUNCTIONS ---
*/

//--- Constructor ---

Button_CB::Button_CB() {}
Button_CB::Button_CB( byte pin, CallbackFunction cb )
{
	begin( pin, cb );

} // Button_CB()

Button_CB::Button_CB( byte pin, byte btnMode, CallbackFunction cb )
{
	begin( pin, btnMode, cb );

} // Button_CB()

Button_CB::Button_CB( byte pin, byte btnMode, bool activeLow, CallbackFunction cb )
{
	begin( pin, btnMode, activeLow, cb );

} // Button_CB()

////////////////////////////////////

void Button_CB::begin( byte pin, CallbackFunction cb )
{
	begin( pin, INPUT_PULLUP, true, cb );

} // begin()

void Button_CB::begin( byte pin, byte btnMode, CallbackFunction cb )
{
	begin( pin, btnMode, true, cb );

} // begin()

void Button_CB::begin( byte pin, byte btnMode, bool activeLow, CallbackFunction cb )
{
	// Copy data to private.
	_cb = cb;
	_pin = pin;
	_btnMode = btnMode;
	_buttonPressed = ( activeLow)  ? LOW : HIGH;

	pinMode( _pin, _btnMode );

	btnDbg_printf( "pb: - %ul - Pin: %i - Mode: %i - activeLow: %i\n", millis(), pin, btnMode, activeLow );

} // begin()

////////////////////////////////////

void Button_CB::loop( void )
{

	static unsigned long _click_start_time;			// Start time a click cycle. (mSec)
	static unsigned long _dclick_check_start;		// Start time for DounbleClick check. (mSec)
	static unsigned long _debounceStart;			// Start time for debounce. (mSec)
	static unsigned long _pulsStart;				// LongClickPuls start time. (mSec)

	int _pinRead;
	int event;

	if( _pin == PIN_UNDEFIIND ) return;

	_pinRead = digitalRead( _pin );

	switch( _cycle )
	{
		case WAIT_BUTTON_PRESSED:		// Wait for the button to be pressed. Cycle (Click) start.

			if( _pinRead == _buttonPressed )
			{
				// Button get pressed. New button press cyclus start.
				_click_start_time = millis();
				_debounceStart = millis();
				_cycle = WAIT_DEBOUNCE_PRESSED;

				btnDbg_printf( "pb: - %u - WAIT_BUTTON_PRESSED Button get Pressed.\n", millis() );
			}
			break;

		case WAIT_DEBOUNCE_PRESSED:		// Wait for elapse of debounce time on pressed button.

			if( WaitDebounceElapse( _debounceStart ) )
			{
				_cycle = WAIT_BUTTON_RELEASED;
			
				// btnDbg_printf( "pb: - %u - WAIT_DEBOUNCE_PRESSED - elapsed.\n", millis() );
			}
			break;

		case WAIT_BUTTON_RELEASED:		// Wait for the button to be released. Click ends.

			if( _pinRead != _buttonPressed )
			{
				// Button released.
				_click_cnt++;
				_dclick_check_start = millis();
				_debounceStart = millis();
				
				_clickElapseTime = millis() - _click_start_time;

				_cycle = WAIT_DEBOUNCE_RELEASED;

				btnDbg_printf( "pb: - %u - WAIT_BUTTON_RELEASED. Click time: %u Count: %i\n", millis(), millis()-_click_start_time, _click_cnt );
				break;
			}

			if( millis() - _click_start_time > _lclick_start_delay && _click_cnt == 0 )
			{
				// Long click detected.
				_pulsStart = millis();
				_cycle = LCLICK_PROGRESS;

				btnDbg_printf( "pb: - %u - WAIT_BUTTON_RELEASED. LongClick start.\n", millis() );
								
				FireCallback( BTN_LONGCLICK_DETECT_EVENT );
			}
			break;

		case WAIT_DEBOUNCE_RELEASED:	// Wait for elapse of debounce time on released button.

			if( WaitDebounceElapse( _debounceStart ) )
			{
				_cycle = WAIT_IF_DCLICK_START;

				// btnDbg_printf( "pb: - %u - WAIT_DEBOUNCE_RELEASED elapsed.\n", millis() );
			}
			break;

		case WAIT_IF_DCLICK_START:		// Wait if DoubleClick start.

			if( _pinRead == _buttonPressed )
			{
				// Next click.
				// Button get pressed. New button press cyclus start.
				_debounceStart = millis();
				_cycle = WAIT_DEBOUNCE_PRESSED;

				btnDbg_printf( "pb: - %u - WAIT_IF_DCLICK_START DoubleClick start.\n", millis() );
				break;
			}

			if( millis() - _dclick_check_start > _dclick_delay )
			{
				// Click end.
				// Call Click callback


				switch( _click_cnt )
				{
					case 1:
						// Click.
						event = BTN_CLICK_EVENT;
						btnDbg_printf( "pb: - %u - Click.", millis() );
						break;
					
					case 2:
						// DoubleClick.
						event = BTN_DOUBLECLICK_EVENT;
						btnDbg_printf( "pb: - %u - DoubleClick.", millis() );

						break;

					default:
						// MultiClick.
						event = BTN_MULTICLICK_EVENT;
						btnDbg_printf( "pb: - %u - MultiClick.", millis() );
						break;

				}

				btnDbg_printf( " last Clicktime: %u Clicks: %i\n", _dclick_check_start - _click_start_time, _click_cnt );
				FireCallback( event );

				_click_cnt = 0;
				_cycle = WAIT_BUTTON_PRESSED;		// Restart next cycle.
				// btnDbg_printf( "pb: - %u - WAIT_IF_DCLICK_START elapsed. Simple Click\n", millis() );
			}
			break;

		case LCLICK_PROGRESS:			// LongClick in pregress.

			if( millis() - _pulsStart > _lclick_pulse_delay )
			{
				// LongClickPuls fire.
				_pulsStart = millis();		// For next pulse.
				_clickElapseTime = millis() - _click_start_time;

				btnDbg_printf( "pb: - %u - LCLICK_PROGRESS. LongClickPulse fires.\n", millis() );

				FireCallback( BTN_LONGCLICK_PULSE_EVENT );
			}

			if( _pinRead != _buttonPressed )
			{
				// LongClick ends.
				_debounceStart = millis();
				_clickElapseTime = millis() - _click_start_time;
				_cycle = WAIT_LCICK_DEBOUNCE_RELEASE;

				btnDbg_printf( "pb: - %u - LCLICK_PROGRESS. LongClick time: %u\n", millis(), _clickElapseTime );

				// Call LongClick callback
				FireCallback( BTN_LONGCLICK_EVENT );
			}
			break;

		case WAIT_LCICK_DEBOUNCE_RELEASE:	// Wait for elapse of debounce time on LongClick released button.
			if( WaitDebounceElapse( _debounceStart ) )
			{
				_lclick_abort = false;
				_cycle = WAIT_BUTTON_PRESSED;
			}
			break;

		default:
			btnDbg_printf( "pb: - %ul - default: cycle= %i\n", millis(), _cycle );
			break;
	}
} // loop()

////////////////////////////////////

int Button_CB::getClickCount( void )
{
	return _click_cnt;
	
} // ClickCount()

////////////////////////////////////

unsigned int Button_CB::getClickElapsedTime( void )
{
	return _clickElapseTime;
	
} // ClickCount()

////////////////////////////////////

int Button_CB::getEvent( void )
{
	return _event;
	
} // ClickCount()

////////////////////////////////////

void Button_CB::LongClick_Abort( void )
{
	_lclick_abort = true;

} // LongClick_Abort()

////////////////////////////////////

void Button_CB::setDebounceDelay( unsigned int debounce )
{
	_debounce_delay = debounce;

} // setDebounceDelay()

unsigned int Button_CB::getDebounceDelay( void )
{
	return _debounce_delay;
	
} // getDebounceDelay()

////////////////////////////////////

void Button_CB::setDoubleClickDelay( unsigned int dclickDelay )
{
	_dclick_delay = dclickDelay;

} // setDoubleClickDelay()

unsigned int Button_CB::getDoubleClickDelay( void )
{
	return _dclick_delay;

} // getDoubleClickDelay()

////////////////////////////////////

void Button_CB::setLongClickStartDelay( unsigned int lclickStartDelay )
{
	_lclick_start_delay = lclickStartDelay;

} // setLongClickStartDelay()

unsigned int Button_CB::getLongClickStartDelay( void )
{
	return _lclick_start_delay;

} // getLongClickStartDelay()

////////////////////////////////////

void Button_CB::setLongClickPulseDelay( unsigned int lclickPulseDelay )
{
	_lclick_pulse_delay = lclickPulseDelay;

} // setLongClickPulseDelay()

unsigned int Button_CB::getLongClickPulseDelay( void )
{
	return _lclick_start_delay;

} // getLongClickPulseDelay()

////////////////////////////////////

/**
 * --- PRIVATE FUNCTIONS ---
*/

bool Button_CB::WaitDebounceElapse( unsigned long debounceStart )
{
	// btnDbg_printf( "pb: - %ul - WaitDebounceElapse() Delay: %i > _debounce_delay %i\n", millis(), millis()-debounceStart, _debounce_delay );
	if( millis() - debounceStart > _debounce_delay ) return true;

	return false;

} // WaitDebounceElapse()

////////////////////////////////////

void Button_CB::FireCallback( int event )
{
	if( _lclick_abort && ( event == BTN_LONGCLICK_PULSE_EVENT || BTN_LONGCLICK_EVENT ) ) return;

	_event = event;
	if( _cb != NULL ) _cb( event );

} // FireCallback()

////////////////////////////////////

bool Button_CB::IsBtnPressed( void )
{
	return  digitalRead( _pin ) == _buttonPressed;

} // IsBtnPressed()

// End of 'Button_CB.cpp'.