#include "WS_Relay.h"

bool Failure_Flag = 0;
uint32_t Channel_Pulse_Durations[8] = {500, 500, 500, 500, 500, 500, 500, 500};  // Default 500ms pulse duration for all channels  
uint32_t Channel_Pulse_Counts[8] = {2, 2, 2, 2, 2, 2, 2, 2};  // Default 2 pulse cycles for all channels
/*************************************************************  Relay I/O  *************************************************************/
bool Relay_Open(uint8_t CHx)
{
  if(!Set_EXIO(CHx, true)){
    printf("Failed to Open CH%d!!!\r\n", CHx);
    Failure_Flag = 1;
    return 0;
  }
  return 1;
}
bool Relay_Closs(uint8_t CHx)
{
  if(!Set_EXIO(CHx, false)){
    printf("Failed to Closs CH%d!!!\r\n", CHx);
    Failure_Flag = 1;
    return 0;
  }
  return 1;
}
bool Relay_CHx_Toggle(uint8_t CHx)
{
   if(!Set_Toggle(CHx)){
    printf("Failed to Toggle CH%d!!!\r\n", CHx);
    Failure_Flag = 1;
    return 0;
  }
  return 1;
}
bool Relay_CHx(uint8_t CHx, bool State)
{
  bool result = 0;
  if(State)
    result = Relay_Open(CHx);
  else
    result = Relay_Closs(CHx);
  if(!result)
    Failure_Flag = 1;
  return result;
}
bool Relay_CHxs_PinState(uint8_t PinState)
{
  if(!Set_EXIOS(PinState)){
    printf("Failed to set the relay status!!!\r\n");
    Failure_Flag = 1;
    return 0;
  }
  return 1;
}

void RelayFailTask(void *parameter) {
  while(1){
    if(Failure_Flag)
    {
      Failure_Flag = 0;
      printf("Error: Relay control failed!!!\r\n");
      RGB_Open_Time(60,0,0,5000,500);
      Buzzer_Open_Time(5000, 500);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelete(NULL);
}
void Relay_Init(void)
{
  TCA9554PWR_Init(0x00, 0x00);
  xTaskCreatePinnedToCore(
    RelayFailTask,    
    "RelayFailTask",   
    4096,                
    NULL,                 
    3,                   
    NULL,                 
    0                   
  );
}

/********************************************************  Data Analysis  ********************************************************/
bool Relay_Flag[8] = {0};       // Relay current status flag
void Relay_Analysis(uint8_t *buf,uint8_t Mode_Flag)
{
  uint8_t ret = 0;
  if(Mode_Flag == Bluetooth_Mode)
    printf("Bluetooth Data :\r\n");
  else if(Mode_Flag == WIFI_Mode)
    printf("WIFI Data :\r\n");
  else if(Mode_Flag == RS485_Mode)
    printf("RS485 Data :\r\n");  
  switch(buf[0])
  {
    case CH1: 
      ret = Relay_CHx_Toggle(GPIO_PIN_CH1);                                              //Toggle the level status of the GPIO_PIN_CH1 pin
      if(ret){
        Relay_Flag[0] =! Relay_Flag[0];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[0])
          printf("|***  Relay CH1 on  ***|\r\n");
        else
          printf("|***  Relay CH1 off ***|\r\n");
      }
      break;
    case CH2: 
      ret = Relay_CHx_Toggle(GPIO_PIN_CH2);                                             //Toggle the level status of the GPIO_PIN_CH2 pin
      if(ret){
        Relay_Flag[1] =! Relay_Flag[1];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[1])
          printf("|***  Relay CH2 on  ***|\r\n");
        else
          printf("|***  Relay CH2 off ***|\r\n");
      }
      break;
    case CH3:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH3);                                             //Toggle the level status of the GPIO_PIN_CH3 pin
      if(ret){
        Relay_Flag[2] =! Relay_Flag[2];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[2])
          printf("|***  Relay CH3 on  ***|\r\n");
        else
          printf("|***  Relay CH3 off ***|\r\n");
      }
      break;
    case CH4:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH4);                                             //Toggle the level status of the GPIO_PIN_CH4 pin
      if(ret){
        Relay_Flag[3] =! Relay_Flag[3];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[3])
          printf("|***  Relay CH4 on  ***|\r\n");
        else
          printf("|***  Relay CH4 off ***|\r\n");
      }
      break;
    case CH5:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH5);                                             //Toggle the level status of the GPIO_PIN_CH5 pin
      if(ret){  
        Relay_Flag[4] =! Relay_Flag[4];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[4])
          printf("|***  Relay CH5 on  ***|\r\n");
        else
          printf("|***  Relay CH5 off ***|\r\n");
      }
      break;
    case CH6:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH6);                                             //Toggle the level status of the GPIO_PIN_CH6 pin
      if(ret){
        Relay_Flag[5] =! Relay_Flag[5];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[5])
          printf("|***  Relay CH6 on  ***|\r\n");
        else
          printf("|***  Relay CH6 off ***|\r\n");
      }
      break;
    case CH7:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH7);                                             //Toggle the level status of the GPIO_PIN_CH6 pin
      if(ret){
        Relay_Flag[6] =! Relay_Flag[6];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[6])
          printf("|***  Relay CH7 on  ***|\r\n");
        else
          printf("|***  Relay CH7 off ***|\r\n");
      }
      break;
    case CH8:
      ret = Relay_CHx_Toggle(GPIO_PIN_CH8);                                             //Toggle the level status of the GPIO_PIN_CH6 pin
      if(ret){
        Relay_Flag[7] =! Relay_Flag[7];
        Buzzer_Open_Time(200, 0);
        if(Relay_Flag[7])
          printf("|***  Relay CH8 on  ***|\r\n");
        else
          printf("|***  Relay CH8 off ***|\r\n");
      }
      break;
    case ALL_ON:
      ret = Relay_CHxs_PinState(0xFF);                                              // Turn on all relay
      if(ret){
        memset(Relay_Flag,1, sizeof(Relay_Flag));
        printf("|***  Relay ALL on  ***|\r\n");
        Buzzer_Open_Time(500, 0);
      }

      break;
    case ALL_OFF:
      ret = Relay_CHxs_PinState(0x00);                                                // Turn off all relay
      if(ret){
        memset(Relay_Flag,0, sizeof(Relay_Flag));
        printf("|***  Relay ALL off ***|\r\n");
        Buzzer_Open_Time(500, 150); 
      }
      break;
    default:
      printf("Note : Non-instruction data was received !\r\n");
  }
}

void Relay_Immediate(uint8_t CHx, bool State, uint8_t Mode_Flag)
{
  if(!CHx || CHx > 8){
    printf("Relay_Immediate(function): Incoming parameter error!!!!\r\n");
    Failure_Flag = 1;
  }
  else{
    uint8_t ret = 0;
    if(Mode_Flag == DIN_Mode)
      printf("DIN Data :\r\n"); 
    else if(Mode_Flag == RTC_Mode)
      printf("RTC Data :\r\n");
    ret = Relay_CHx(CHx,State);                                               
    if(ret){
      Relay_Flag[CHx-1] = State;
      Buzzer_Open_Time(200, 0);
      if(Relay_Flag[0])
        printf("|***  Relay CH%d on  ***|\r\n",CHx);
      else
        printf("|***  Relay CH%d off ***|\r\n",CHx);
    }
  }
}
void Relay_Immediate_CHxn(Status_adjustment * Relay_n, uint8_t Mode_Flag)
{
  uint8_t ret = 0;
  if(Mode_Flag == DIN_Mode)
    printf("DIN Data :\r\n"); 
  else if(Mode_Flag == RTC_Mode)
    printf("RTC Data :\r\n");            
  for (int i = 0; i < 8; i++) {
    if(Relay_n[i] == STATE_Open || Relay_n[i] == STATE_Closs){
      Relay_Flag[i] = (bool)Relay_n[i];
      ret = Relay_CHx(i+1,Relay_n[i]);  
      if(Relay_n[i] == STATE_Open)
        printf("|***  Relay CH%d on  ***|\r\n",i+1);
      else if(Relay_n[i] == STATE_Closs)
        printf("|***  Relay CH%d off ***|\r\n",i+1);
    }
  }
  Buzzer_Open_Time(200, 0);
}

void Relay_Immediate_CHxs(uint8_t PinState, uint8_t Mode_Flag)
{
  uint8_t ret = 0;
  if(Mode_Flag == DIN_Mode)
    printf("DIN Data :\r\n"); 
  else if(Mode_Flag == RTC_Mode)
    printf("RTC Data :\r\n");
  for (int i = 0; i < 8; i++) {
    Relay_Flag[i] = (PinState >> i) & 0x01; // 提取每一位并赋值
  }
  ret = Relay_CHxs_PinState(PinState);                                               
  if(ret){
    for (int j = 0; j < 8; j++) {
      if(Relay_Flag[j])
        printf("|***  Relay CH%d on  ***|\r\n",j+1);
      else
        printf("|***  Relay CH%d off ***|\r\n",j+1);
    }
    Buzzer_Open_Time(200, 0);
  }
  else
  {
    printf("Relay_Immediate_CHxs(function): Relay control failure!!!!\r\n");
    Failure_Flag = 1;
  }
}

// Structure to pass parameters to pulse task
typedef struct {
  uint8_t CHx;
  uint32_t pulse_time_ms;
  uint32_t pulse_count;
} PulseTaskParams;

// Task function for relay pulse operation
void RelayPulseTask(void *parameter) {
  PulseTaskParams *params = (PulseTaskParams *)parameter;
  uint8_t CHx = params->CHx;
  uint32_t pulse_time_ms = params->pulse_time_ms;
  uint32_t pulse_count = params->pulse_count;
  
  printf("Starting pulse operation for CH%d with %dms timing and %d cycles\r\n", CHx, pulse_time_ms, pulse_count);
  
  // Perform the specified number of ON-OFF cycles
  for (uint32_t cycle = 0; cycle < pulse_count; cycle++) {
    // Turn relay ON
    if (Relay_CHx(CHx, true)) {
      Relay_Flag[CHx-1] = true;
      printf("|***  Relay CH%d pulse ON (cycle %d) ***|\r\n", CHx, cycle + 1);
    }
    vTaskDelay(pdMS_TO_TICKS(pulse_time_ms));
    
    // Turn relay OFF
    if (Relay_CHx(CHx, false)) {
      Relay_Flag[CHx-1] = false;
      printf("|***  Relay CH%d pulse OFF (cycle %d) ***|\r\n", CHx, cycle + 1);
    }
    vTaskDelay(pdMS_TO_TICKS(pulse_time_ms));
  }
  
  printf("Pulse operation completed for CH%d\r\n", CHx);
  Buzzer_Open_Time(200, 0);
  
  // Free the parameter memory and delete the task
  free(params);
  vTaskDelete(NULL);
}

void Relay_Pulse(uint8_t CHx, uint32_t pulse_time_ms)
{
  if (!CHx || CHx > 8) {
    printf("Relay_Pulse(function): Invalid channel number %d\r\n", CHx);
    Failure_Flag = 1;
    return;
  }
  
  if (pulse_time_ms < 10 || pulse_time_ms > 10000) {
    printf("Relay_Pulse(function): Invalid pulse time %dms (must be 10-10000ms)\r\n", pulse_time_ms);
    Failure_Flag = 1;
    return;
  }
  
  // Use the channel's configured pulse count
  uint32_t pulse_count = Channel_Pulse_Counts[CHx - 1];
  
  // Allocate memory for task parameters
  PulseTaskParams *params = (PulseTaskParams *)malloc(sizeof(PulseTaskParams));
  if (params == NULL) {
    printf("Relay_Pulse(function): Failed to allocate memory for task parameters\r\n");
    Failure_Flag = 1;
    return;
  }
  
  params->CHx = CHx;
  params->pulse_time_ms = pulse_time_ms;
  params->pulse_count = pulse_count;
  
  // Create a new FreeRTOS task for the pulse operation
  char taskName[20];
  sprintf(taskName, "PulseCH%d", CHx);
  
  BaseType_t result = xTaskCreatePinnedToCore(
    RelayPulseTask,
    taskName,
    4096,
    params,
    2,  // Priority lower than RelayFailTask (3) but higher than WebTask (1)
    NULL,
    0
  );
  
  if (result != pdPASS) {
    printf("Relay_Pulse(function): Failed to create pulse task for CH%d\r\n", CHx);
    free(params);
    Failure_Flag = 1;
  } else {
    printf("Pulse task created for CH%d with %dms timing and %d cycles\r\n", CHx, pulse_time_ms, pulse_count);
  }
}