#include <Arduino.h>

#include "Button_CB.h"

void BTN_Handler( unsigned int event );

Button_CB btn;

#define btn_pin 	33

void setup() {
	Serial.begin(115200);

	pinMode( btn_pin, PULLUP );

	btn.begin( btn_pin, INPUT, false, BTN_Handler );
}

void loop() {
	
	btn.loop();

}

void BTN_Handler( unsigned int event )
{
	switch ( event )
	{
		case BTN_CLICK_EVENT:
			Serial.println( "Button Click" );
			break;

		case BTN_DOUBLECLICK_EVENT:
			Serial.println( "Button Doubleclick" );
			break;

		case BTN_MULTICLICK_EVENT:
			Serial.printf( "Button Multiclick: %i clicks\n", btn.getClickCount() );
			break;

		case BTN_LONGCLICK_DETECT_EVENT:
			Serial.println( "Button Longclick detect" );
			break;

		case BTN_LONGCLICK_PULSE_EVENT:
			Serial.println( "Button Longclick Pulse" );
			break;

		case BTN_LONGCLICK_EVENT:
			Serial.printf( "Button Longclick: %i ms\n", btn.getClickElapsedTime() );
			break;

	}
}
