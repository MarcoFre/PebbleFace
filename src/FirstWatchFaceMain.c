
	//Include files from SDK
	#include "pebble_os.h"
	#include "pebble_app.h"
	#include "pebble_fonts.h"

	//Define Universally Unique Identifier 
	//	88a97cfd-2377-463a-84bd-b85eb4964063
	#define UUID { 0x88, 0xa9, 0x7c, 0xfd, 0x23, 0x77, 0x46, 0x3a, 0x84, 0xbd, 0xb8, 0x5e, 0xb4, 0x96, 0x40, 0x63 }

	//Declare structures for watch face elements
	Window window;
	TextLayer timeLayer;

	//Declare variable for storing time string
	static char hourText[] = "00:00";

	//Set app info
	PBL_APP_INFO(UUID, "First Watch Face", "Chris Lewis", 0, 1, DEFAULT_MENU_ICON, APP_INFO_WATCH_FACE);
							 
	//UUID, app name, author, minor version, major version,
	//use default icon, tell compiler this is a face

	/**
		* Function to set the time and date features on the TextLayers
		*/
	void setTime(PblTm *t) {
		
		//If user selects '24hr' in Settings on the watch
		if(clock_is_24h_style())
			string_format_time(hourText, sizeof(hourText), "%H:%M", t);
		else
			string_format_time(hourText, sizeof(hourText), "%I:%M", t);
	
		//Set the TextLayer text
		text_layer_set_text(&timeLayer, hourText);
	}

	/**
		* Handle function called every minute
		*/
	void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
		(void)ctx;
	
		PblTm time;	//Structure to store time info
		get_time(&time);	//Fill the structure with current time	
		setTime(t->tick_time);	//Change time on 'zero seconds' mark
	}
	
	/**
		* Handle function called every second
		*/
	void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
		(void)ctx;
		
		PblTm time;	//Structure to store time info
		get_time(&time);	//Fill the structure with current time
		
		int seconds = time.tm_sec;	//Get the current number of seconds
		
		if(seconds == 0)
			setTime(t->tick_time);	//Change the time on the 'zero seconds' mark
	}

	/**
		* Watch face initialisation handle function
		*/
	void handle_init(AppContextRef ctx) {
		(void)ctx;	//This is not needed. Convert to void (nothing)

		//Initialise window
		window_init(&window, "Window Name");
		window_stack_push(&window, true);
		window_set_background_color(&window, GColorBlack);

		//Initialise TextLayers
		text_layer_init(&timeLayer, GRect(30, 30, 150, 50));
		text_layer_set_background_color(&timeLayer, GColorClear);
		text_layer_set_text_color(&timeLayer, GColorWhite);
		text_layer_set_font(&timeLayer, 
												fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
		text_layer_set_text_alignment(&timeLayer, GTextAlignmentLeft);
		
		//Add to window
		layer_add_child(&window.layer, &timeLayer.layer);
	
		//Set initial time so display isn't blank
		PblTm time;
		get_time(&time);
		setTime(&time); 
	}

	/**
		* Main Pebble app loop
		*/
	void pbl_main(void *params) {
		PebbleAppHandlers handlers = {
			.init_handler = &handle_init,	//Register initialisation function

			.tick_info = {
				.tick_handler = &handle_minute_tick,	//Register tick function
				.tick_units = MINUTE_UNIT	//Specify to call every minute
			}
		};
		app_event_loop(params, &handlers);	//Continue from there!
	}

