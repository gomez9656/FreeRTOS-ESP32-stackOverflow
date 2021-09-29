//stack overflow example

//use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//task: perfom some task
void testTask(void *parameter) {

  while (1) {
    int a = 1;
    int b[100];

    //do something with the array so it's not optimized by the compiler
    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    //print out remaining stack memory (words)
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    //print outnumber of free heap memory bytes before malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));

    //to prevent heap overflow is to check malloc output
    if ( ptr == NULL) {
      Serial.print("Not enough heap");
    }
    else {
      //Do something so the compiler doesn't optimize it
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }



    //print outnumber of free heap memory bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    //free up our allocated memory
    vPortFree(ptr);

    vTaskDelay(100 / portTICK_PERIOD_MS);

  }
}

void setup() {

  Serial.begin(9600);

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.println("FreeRTOS example");

  //start the only task
  xTaskCreatePinnedToCore(testTask,
                          "Test task",
                          1500, //an array of 100 * 4 is 400 byest. if use 1024 bytes, aprox 768 is overhead, so it won't work
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  //delete setup and loop task
  vTaskDelete(NULL);


}

void loop() {
  // put your main code here, to run repeatedly:

}
