#include "WS_CAN.h"

static bool driver_installed = false;

void CAN_Init(void)
{                                // Initializing serial port
  // Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)TXD1, (gpio_num_t)RXD1, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();  //Look in the api-reference for other speed sets.
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    printf("Driver installed\r\n");
  } else {
    printf("Failed to install driver\r\n");
    return;
  }

  // Start TWAI driver
  if (twai_start() == ESP_OK) {
    printf("Driver started\r\n");
  } else {
    printf("Failed to start driver\r\n");
    return;
  }

  // Reconfigure alerts to detect TX alerts and Bus-Off errors
  uint32_t alerts_to_enable = TWAI_ALERT_RX_DATA | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR | TWAI_ALERT_RX_QUEUE_FULL | TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED;
  if (twai_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    printf("CAN Alerts reconfigured\r\n");
  } else {
    printf("Failed to reconfigure alerts\r\n");
    return;
  }

  // TWAI driver is now successfully installed and started
  driver_installed = true;

  xTaskCreatePinnedToCore(
    CANTask,    
    "CANTask",   
    4096,                
    NULL,                 
    3,                   
    NULL,                 
    0                   
  );
}

static void send_message_Test(void) {
  // Send message
  // Configure message to transmit
  twai_message_t message;
  message.identifier = 0x0F6;
  message.data_length_code = 4;
  for (int i = 0; i < 4; i++) {
    message.data[i] = i;
  }

  // Queue message for transmission
  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    printf("Message queued for transmission\n");
  } else {
    printf("Failed to queue message for transmission\n");
  }
}
// Standard frames ID: 0x000 to 0x7FF
// Extended frames ID: 0x00000000  to 0x1FFFFFFF
// Frame_type : 1：Extended frames   0：Standard frames
void send_message(uint32_t CAN_ID, uint8_t* Data, uint8_t Data_length, bool Frame_type) {
  // Send message
  // Configure message to transmit
  twai_message_t message;
  message.identifier = CAN_ID;
  message.rtr = 0;                              // Disable remote frame
  if(CAN_ID > 0x7FF){
    if(!Frame_type)
      printf("The frame type is set incorrectly and data will eventually be sent as an extended frame!!!!\r\n");
    message.extd = 1;
  }
  else
    message.extd = Frame_type;
  if(Data_length > 8){
    uint16_t Frame_count = (Data_length / 8);
    for (int i = 0; i < Frame_count; i++) {
      message.data_length_code = 8;
      for (int j = 0; j < 8; j++) {
        message.data[j] = Data[j + (i * 8)];
      }
      // Queue message for transmission
      if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        printf("Message queued for transmission\n");
      } else {
        printf("Failed to queue message for transmission\n");
      }
    }
    if(Data_length % 8){
      uint8_t Data_length_Now = Data_length % 8;
      message.data_length_code = Data_length_Now;
      for (int k = 0; k < Data_length_Now; k++) {
        message.data[k] = Data[k + (Data_length - Data_length_Now)];
      }
      // Queue message for transmission
      if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        printf("Message queued for transmission\n");
      } else {
        printf("Failed to queue message for transmission\n");
      }
    }
  }
  else{
    message.data_length_code = Data_length;
    for (int i = 0; i < Data_length; i++) {
      message.data[i] = Data[i];
    }
    // Queue message for transmission
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
      printf("Message queued for transmission\n");
    } else {
      printf("Failed to queue message for transmission\n");
    }
  }
}



static void handle_rx_message(twai_message_t &message) {
  // Process received message
  if (message.extd) {
    printf("Message is in Extended Format\r\n");
  } else {
    printf("Message is in Standard Format\r\n");
  }
  printf("ID: %lx\nByte:", message.identifier);
  if (!(message.rtr)) {
    if (message.data_length_code > 0) {
      printf(" Data: ");
      for (int i = 0; i < message.data_length_code; i++) {
        printf("%02x ", message.data[i]);
      }
      printf("\r\n");
      // printf("Send back the received data!\r\n");
      // send_message(message.identifier,  message.data, message.data_length_code, message.extd);
    } else {
      printf(" No data available\r\n");
    }
  } else {
    printf("This is a Remote Transmission Request (RTR) frame.\r\n");
  }
}

unsigned long previousMillis = 0;  // will store last time a message was send
#if Communication_failure_Enable
  static unsigned long previous_bus_error_time = 0; // To store the last time a BUS_ERROR was printed
#endif
void CAN_Loop(void)
{
  if(driver_installed){
    // Check if an alert happened
    uint32_t alerts_triggered;
    twai_read_alerts(&alerts_triggered, pdMS_TO_TICKS(POLLING_RATE_MS));
    twai_status_info_t twaistatus;
    twai_get_status_info(&twaistatus);

    // Handle alerts
    if (alerts_triggered & TWAI_ALERT_ERR_PASS) {
      printf("Alert: TWAI controller has become error passive.\r\n");
    }
    if (alerts_triggered & TWAI_ALERT_BUS_ERROR) {
      // printf("Alert: A (Bit, Stuff, CRC, Form, ACK) error has occurred on the bus.\r\n");
      // printf("Bus error count: %ld\n", twaistatus.bus_error_count);
      #if Communication_failure_Enable
        unsigned long currentMillis = millis();
        // Only print the message if more than 2 seconds have passed since the last time it was printed
        if (currentMillis - previous_bus_error_time >= BUS_ERROR_INTERVAL_MS) {
          printf("Note if there are other devices on the CAN bus (other devices must be present) and that the rate of the device is the same as set in this program\r\n");
          previous_bus_error_time = currentMillis; // Update the last print time
        }
      #endif
    }
    if (alerts_triggered & TWAI_ALERT_RX_QUEUE_FULL) {
      printf("Alert: The RX queue is full causing a received frame to be lost.\r\n");
      printf("RX buffered: %ld\t", twaistatus.msgs_to_rx);
      printf("RX missed: %ld\t", twaistatus.rx_missed_count);
      printf("RX overrun %ld\n", twaistatus.rx_overrun_count);
    }
    if (alerts_triggered & TWAI_ALERT_TX_FAILED) {
      printf("Alert: The Transmission failed.\r\n");
      printf("TX buffered: %ld\t", twaistatus.msgs_to_tx);
      printf("TX error: %ld\t", twaistatus.tx_error_counter);
      printf("TX failed: %ld\n", twaistatus.tx_failed_count);
    }
    if (alerts_triggered & TWAI_ALERT_TX_SUCCESS) {
      printf("Alert: The Transmission was successful.\r\n");
      printf("TX buffered: %ld\t \r\n", twaistatus.msgs_to_tx);
    }

    // Receive messages if any are available
    if (alerts_triggered & TWAI_ALERT_RX_DATA) {
      // One or more messages received. Handle all.
      twai_message_t message;                             // This is the structure used to store the received CAN message.
      while (twai_receive(&message, 0) == ESP_OK) {
        handle_rx_message(message);                       // This function will process the received message.
      }
    }
  }
}

void CANTask(void *parameter) {
  // send_message_Test();
  // uint8_t Data[27]={0x80, 0x2A, 0xC3, 0x58, 0x17, 0x11, 0x4D, 0x3F, 0x3B, 0xCE, 0x0F, 0xFF, 0x79, 0x20, 0xB4, 0x40, 0x5D, 0x29, 0x05, 0x49, 0xE6, 0x12, 0x57, 0x0E, 0x6D, 0xC9, 0xAE};
  // send_message(0x079,Data,27);
  while(1){
    CAN_Loop();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelete(NULL);
}