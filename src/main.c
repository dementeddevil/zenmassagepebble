#include <pebble.h>
#include "main.h"

#define PH_DATA_OFFSET         0
#define PE_DATA_SIZE           3

#define PE_NAME_INDEX          0
#define PE_MONTH_INDEX         1
#define PE_DAY_INDEX           2

typedef struct trigger_event_t {
  char name[PERSIST_DATA_MAX_LENGTH];
  int triggerMonth;
  int triggerDay;

  void *previous_event;
  void *next_event;
} TriggerEvent;

static Window *window = 0;
static TextLayer *title_layer = 0;
static TextLayer *text_layer = 0;

static bool isSettingSessionLength = false;
static bool canDoubleClickFor30 = true;
static bool canTripleClickFor60 = true;

static int sessionLength = 0;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(isSettingSessionLength){
    switch(click_number_of_clicks_counted(recognizer)) {
      case 1:
        ++sessionLength;
        break;
      case 2:
        sessionLength += 30;
        break;
      case 3:
        sessionLength += 60;
        break;
    }    
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(isSettingSessionLength){
    switch(click_number_of_clicks_counted(recognizer)) {
      case 1:
        sessionLength = Max(0, sessionLength - 1);
        break;
      case 2:
        sessionLength = Max(0, sessionLength - 30);
        break;
      case 3:
        sessionLength = Max(0, sessionLength - 60);
        break;
    }    
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 500, up_click_handler);
  window_multi_click_subscribe(BUTTON_ID_UP, 2, 400, true, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 500, down_click_handler);
  window_multi_click_subscribe(BUTTON_ID_DOWN, 2, 400, true, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  title_layer = text_layer_create((GRect) { .origin = { 0, 52 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(title_layer));
  
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(title_layer);
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	  .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}