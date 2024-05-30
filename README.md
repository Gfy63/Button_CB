# Button_CB
Class for button control. Click, DoubleClick, LongClick, LongClickPulse raise a event.

## INSTALLATION & DOWNLOAD
Install via download from [github](https://github.com/Gfy63/Button_CB.git)

## BASIC USAGE

### INSTANTIATE

```cpp
#include <Button_CB.h>

#define _pin        4

void BTN_Handler( unsigned int event );

Button_CB btn;
```

### Setup
```cpp
btn.begin( _pin, INPUT, false, BTN_Handler );
```

### Loop
```cpp
btn.loop();
```

### Constructor
```cpp
Button_CB( byte pin, byte btnMode, bool activeLow, CallbackFunction cb );
Button_CB( byte pin, byte btnMode, CallbackFunction cb );
Button_CB( byte pin, CallbackFunction cb );
Button_CB();
```

### Functions
```cpp
void begin( byte pin, byte btnMode , bool activeLow, CallbackFunction cb );
void begin( byte pin, byte btnMode, CallbackFunction cb );
void begin( byte pin, CallbackFunction cb );

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
```

### Events

```cpp
BTN_CLICK_EVENT,				// Click
BTN_DOUBLECLICK_EVENT,			// DoubleClick
BTN_MULTICLICK_EVENT,			// MultiClick, getClickCount() for the count.
BTN_LONGCLICK_DETECT_EVENT,		// LongClick detect, pulse start.
BTN_LONGCLICK_PULSE_EVENT,		// LongClick pulse.
BTN_LONGCLICK_EVENT				// LongClick (end)
```


### Callback function
```cpp
void BTN_Handler( unsigned int event )
{
	switch ( event )
	{
		case BTN_CLICK_EVENT:
			break;

		case BTN_DOUBLECLICK_EVENT:
			break;

		case BTN_MULTICLICK_EVENT:
			break;

		case BTN_LONGCLICK_DETECT_EVENT:
			break;

		case BTN_LONGCLICK_PULSE_EVENT:
			break;

		case BTN_LONGCLICK_EVENT:
			break;
	}
}
```


