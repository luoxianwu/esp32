/* Basic Multi Threading Arduino Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
// Please read file README.md in the folder containing this example.

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define ANALOG_INPUT_PIN A0

#ifndef LED_BUILTIN
#define LED_BUILTIN 13  // Specify the on which is your LED
#endif

// Define two tasks for Blink & AnalogRead.
void TaskBlink(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
TaskHandle_t analog_read_task_handle;  // You can (don't have to) use this to be able to manipulate a task from somewhere else.
// Define a queue handle
QueueHandle_t xQueue;

// The setup function runs once when you press reset or power on the board.
void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.printf("ANALOG_INPUT_PIN = %d\n",ANALOG_INPUT_PIN);
  Serial.printf("LED_BUILTIN = %d\n", LED_BUILTIN );
  
  // Create a queue that can hold 10 integer items
  xQueue = xQueueCreate(100, sizeof(int));
  
  // Set up two tasks to run independently. 
  uint32_t blink_delay = 10;  // Delay between changing state on LED pin
  xTaskCreate(
    TaskBlink, "Task Blink"  // A name just for humans
    ,
    2048  // The stack size can be checked by calling `uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);`
    ,
    (void *)&blink_delay  // Task parameter which can modify the task behavior. This must be passed as pointer to void.
    ,
    1  // Priority
    ,
    NULL  // Task handle is not used here - simply pass NULL
  );

  // This variant of task creation can also specify on which core it will be run (only relevant for multi-core ESPs)
  xTaskCreatePinnedToCore(
    TaskAnalogRead, "Analog Read", 2048  // Stack size
    ,
    NULL  // When no parameter is used, simply pass NULL
    ,
    1  // Priority
    ,
    &analog_read_task_handle  // With task handle we will be able to manipulate with this task.
    ,
    ARDUINO_RUNNING_CORE  // Core on which the task will run
  );



  Serial.printf("Basic Multi Threading Arduino Example\n");
  Serial.println("-----------------------------------------------");
  Serial.printf("use ADC input value to determine LED blink rate\n");
  Serial.printf("ADC value 0x0---0xfff coresponds 0ms---5000ms\n");
  Serial.println("-----------------------------------------------");
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
/*  
  // use task handle to delete task, after a period of time
  if (analog_read_task_handle != NULL) {   // Make sure that the task actually exists
    delay(100000);
    vTaskDelete(analog_read_task_handle);  // Delete task
    analog_read_task_handle = NULL;        // prevent calling vTaskDelete on non-existing task
  }
*/  
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters) {  // This is a task.
  uint32_t blink_delay = *((uint32_t *)pvParameters);
  Serial.printf("initial blink_delay = %dms\n", blink_delay);
  /*
  Blink LED, the blink rate is determinded by ADC input. When input is 0, LED stay on
  */

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) {                          // A Task shall never return or exit.
    
    // Receive the message from the queue. the last take effect
    while (xQueueReceive(xQueue, &blink_delay, 0) == pdTRUE) {
      // Message received successfully
      Serial.printf("blink_delay = %dms\n", blink_delay); 
    }

    if (blink_delay == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      continue;
    }
    
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    delay(blink_delay);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    delay(blink_delay);
  }
}

void TaskAnalogRead(void *pvParameters) {  // This is a task.
  (void)pvParameters;
  // Check if the given analog pin is usable - if not - delete this task
  if (digitalPinToAnalogChannel(ANALOG_INPUT_PIN) == -1) {
    Serial.printf("TaskAnalogRead cannot work because the given pin %d cannot be used for ADC - the task will delete itself.\n", ANALOG_INPUT_PIN);
    analog_read_task_handle = NULL;  // Prevent calling vTaskDelete on non-existing task
    vTaskDelete(NULL);               // Delete this task
  }

  /*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/
  int previous_sensorValue = 0;
  for (;;) {
    // read the input on analog pin:
    int sensorValue = analogRead(ANALOG_INPUT_PIN);
    int blinkRate = sensorValue * 5000 /0xfff;
    int blinkDelay = blinkRate / 2;
        
    if (abs(previous_sensorValue - sensorValue) > 100) {
      previous_sensorValue = sensorValue;
      // Send the message to the queue
      if (xQueueSend(xQueue, &blinkDelay, portMAX_DELAY) == pdPASS) {
        // Message sent successfully
        Serial.printf("sent Queue. value = %dms\n",blinkDelay);
      }
      else {
        Serial.println("xQueueSend() failed!");
      }
    }
    // print out the value you read:
    Serial.printf("sensorValue = %x\n", sensorValue);
    Serial.printf("blinkRate = %d\n", blinkRate);
    delay(100);  // 100ms delay
  }
}


/*
ets Jul 29 2019 12:21:46

rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:1448
load:0x40078000,len:14844
ho 0 tail 12 room 4
load:0x40080400,len:4
load:0x40080404,len:3356
entry 0x4008059c
ANALOG_INPUT_PIN = 36
LED_BUILTIN = 2
initial blink_delay = 10ms
Basic Multi Threading Arduino Example
-----------------------------------------------
use ADC input value to determine LED blink rate
ADC value 0x0---0xfff coresponds 0ms---5000ms
-----------------------------------------------
blink_delay = 1327ms
sent Queue. value = 1327ms
sensorValue = 87e
blinkRate = 2654
sensorValue = 877
blinkRate = 2645
sensorValue = 82f
blinkRate = 2557
sensorValue = 833
blinkRate = 2562
sensorValue = 855
blinkRate = 2604
sent Queue. value = 1257ms
sensorValue = 80c
blinkRate = 2515
sensorValue = 7fa
blinkRate = 2493
sensorValue = 828
blinkRate = 2549
sensorValue = 828
blinkRate = 2549
sensorValue = 81e
blinkRate = 2537
sensorValue = 824
blinkRate = 2544
sensorValue = 84b
blinkRate = 2592
sensorValue = 865
blinkRate = 2623
sensorValue = 861
blinkRate = 2619
sent Queue. value = 1342ms
sensorValue = 897
blinkRate = 2684
sent Queue. value = 1260ms
sensorValue = 810
blinkRate = 2520
sensorValue = 810
blinkRate = 2520
sensorValue = 810
blinkRate = 2520
sensorValue = 86f
blinkRate = 2636
sensorValue = 867
blinkRate = 2626
sent Queue. value = 1325ms
sensorValue = 87b
blinkRate = 2650
sensorValue = 826
blinkRate = 2547
sensorValue = 83e
blinkRate = 2576
sent Queue. value = 1240ms
sensorValue = 7f0
blinkRate = 2481
sent Queue. value = 1327ms
sensorValue = 87f
blinkRate = 2655
sensorValue = 82a
blinkRate = 2551
sensorValue = 86a
blinkRate = 2630
blink_delay = 1257ms
blink_delay = 1342ms
blink_delay = 1260ms
blink_delay = 1325ms
blink_delay = 1240ms
blink_delay = 1327ms
sensorValue = 865
blinkRate = 2623
sensorValue = 865
blinkRate = 2623
sensorValue = 8a1
blinkRate = 2697
sensorValue = 87b
blinkRate = 2650
*/
