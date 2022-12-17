/**
 * FreeRTOS LED Demo
 * 
 * One task flashes an LED at a rate specified by a value set in another task.
 * 
 * Date: December 4, 2020
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Needed for atoi()
#include <stdlib.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
xTaskHandle TaskHandle_1;

// Pins
static const int led_pin = LED_BUILTIN;
int ChangeInfo = 0;

// Globals
String teststr;
static int led_delay = 500;   // ms
//*****************************************************************************
// Tasks

// Task: Blink LED at rate set by global variable
void writeSerial(void *parameter) {
 while(1){    
    Serial.print(teststr);
    vTaskSuspend(TaskHandle_1);
    ChangeInfo = 0;   
 }
}

// Task: Read from serial terminal
// Feel free to use Serial.readString() or Serial.parseInt(). I'm going to show
// it with atoi() in case you're doing this in a non-Arduino environment. You'd
// also need to replace Serial with your own UART code for non-Arduino.
void readSerial(void *parameters) {

  //Loop Forever
  while (1) {

    // Read characters from serial
    if (Serial.available() > 0) {
      teststr = Serial.readString();
      vTaskResume(TaskHandle_1);
    }
  }
}

//*****************************************************************************
// Main

void setup() {

  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Configure serial and wait a second
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Multi-task Ecco Demo");
  Serial.println("Enter a string.");

  // Start serial Write task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            writeSerial,      // Function to be called
            "Write Serial",   // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority
            &TaskHandle_1,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)
            
  // Start serial read task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            readSerial,     // Function to be called
            "Read Serial",  // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority (must be same to prevent lockup)
            NULL,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)
  vTaskSuspend(TaskHandle_1);

}

void loop() {
  // Execution should never get here
}