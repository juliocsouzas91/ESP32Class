 /*Author: Julio Cesar Eduardo de Souza
Date: 27/10/2022
Task: Use queue to control global variables
Make 2 tasks and 2 queues
Task A {
  Print a new message from queue 2
  reads serial input from user and echo bach to serial terminal
  If command "delay XXX" is sent, send XXX to queue 1 that will monitored by task B  
  }
Task B{
  Update t with the time send to queue 1. Monitoring queue 1 for new values
  Update a local variable with the value read from queue 1. The local variable control the delay of a blinking LED
  Send message to queue 2 after LED blink 100 times
  }
*/
// Use only core 1 for demo purpose
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//_____________Pins_________________//
const int led_pin = 2;
//________Global Variables_________//
String Message_received = String(50);
 
//_____________Tasks_______________//
void task_a_eccho_message(void *parameters){
  while (1) {

    // Read characters from serial
    if (Serial.available() > 0) {
      Message_received = Serial.readString();
      Serial.println(Message_received);
      if(Message_received=="vai"){
        Serial.println("Foi");
      }
    }
  }
}

void task_b_blink_led(void *parameters){
  int time_delay = 1000;
  while(1){
    digitalWrite(led_pin,HIGH);
    vTaskDelay(time_delay/portTICK_PERIOD_MS);
    digitalWrite(led_pin,LOW);
    vTaskDelay(time_delay/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Inicalizando o programa\n");
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(      //Use xTaskCreate() in vanilla FreeRTOS
              task_b_blink_led,        //Function to be called
              "Blink LED",     //Name of the task
              1024,             //Stack size (byter in ESP32, words in FreeRTOS)
              NULL,             //Parameter to pass to function
              1,                //Task priority (0 to configMAX_PRIORITIES = 1)
              NULL,             //Task Handle (Is it the variable?)
              app_cpu);         //Run on one core for demo purposed (ESP32 only)
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              task_a_eccho_message,     // Function to be called
              "Eccho Message",  // Name of task
              1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,           // Parameter to pass
              1,              // Task priority (must be same to prevent lockup)
              NULL,           // Task handle
              app_cpu);       // Run on one core for demo purposes (ESP32 only)

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
