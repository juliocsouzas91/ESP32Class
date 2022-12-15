/*Author: Julio Cesar Eduardo de Souza
Date: 15/10/2022
Task: Blink the LED in port 2 with two different Tasks
      The output makes the LED blink in two frequencies.
*/

// Use only core 1 for demo purpose
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//Pins
static const int led_pin = 2;

//Our task: blink an LED
void toggleLED(void *parameter){
  
  while(1){
    digitalWrite(led_pin,HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }

}

void toggleLED2(void *parameter){
  
  while(1){
    digitalWrite(led_pin,HIGH);
    vTaskDelay(200/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(200/portTICK_PERIOD_MS);
  }

}
void setup() {
  pinMode(led_pin, OUTPUT);

  xTaskCreatePinnedToCore(      //Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,        //Function to be called
              "Toogle LED",     //Name of the task
              1024,             //Stack size (byter in ESP32, words in FreeRTOS)
              NULL,             //Parameter to pass to function
              1,                //Task priority (0 to configMAX_PRIORITIES = 1)
              NULL,             //Task Handle (Is it the variable?)
              app_cpu);         //Run on one core for demo purposed (ESP32 only)

  xTaskCreatePinnedToCore(      //Use xTaskCreate() in vanilla FreeRTOS
              toggleLED2,        //Function to be called
              "Toogle LED2",     //Name of the task
              1024,             //Stack size (byter in ESP32, words in FreeRTOS)
              NULL,             //Parameter to pass to function
              1,                //Task priority (0 to configMAX_PRIORITIES = 1)
              NULL,             //Task Handle
              app_cpu);         //Run on one core for demo purposed (ESP32 only)
  //If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  //main after setting up your task
}

void loop() {

}
