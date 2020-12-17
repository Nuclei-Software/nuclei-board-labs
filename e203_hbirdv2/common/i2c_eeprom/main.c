// See LICENSE for license details.
#include <stdio.h>
#include "hbird_sdk_hal.h"

#define I2C_PRESCALER 0x1F //(soc_freq/(5*i2cfreq)) -1   i2cfreq = 100Khz
#define I2C_ID        I2C0 

void gpio_config();
int i2c_eeprom_wr_test();

int main(void)
{

    printf("I2C Test\r\n");
   
    /* GPIO Config */
    gpio_config();
   
    /* I2C Config */
    i2c_setup(I2C_ID, I2C_PRESCALER, I2C_CTR_EN);
   
    /* EEPROM Write & Read Test */  
    i2c_eeprom_wr_test();
    
    return 0;
}

/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config()
{
    // GPIO Init
    // Set GPIOA[14] as I2C SCL, GPIOA[15] as I2C SDA
    gpio_iof_config(GPIOA, IOF_I2C_MASK);
}


/**
    \brief      Write and Read Data from EEPROM
    \param[in]  none
    \param[out] none
    \retval     none
*/
int i2c_eeprom_wr_test()
{
    // Write data into EEPROM
    // Send Control Byte
    i2c_send_data(I2C_ID, 0xA0); // Control Byte: 0xA0
    i2c_send_command(I2C_ID, I2C_START_WRITE); //do a start bit and send data

    if(!i2c_get_ack(I2C_ID)) {
      printf("No ack received from EEPROM, Device Control, Write data\n");
      return 0;
    }

    // Send Write Address
    i2c_send_data(I2C_ID, 0x00); // Write Addr: 0x00
    i2c_send_command(I2C_ID, I2C_WRITE); //do a start bit and send data

    if(!i2c_get_ack(I2C_ID)) {
      printf("No ack received from EEPROM, Address\n");
      return 0;
    }

    // Send Write Data
    for (int i = 0; i < 16; i++) {
      i2c_send_data(I2C_ID, i);            //write i-th byte into fifo
      i2c_send_command(I2C_ID, I2C_WRITE); //send data on the i2c bus
      i2c_get_ack(I2C_ID);                 //wait for ack
    }

    i2c_send_command(I2C_ID, I2C_STOP);      //do a stop bit, initiate eeprom write
    while(i2c_busy(I2C_ID));

    printf("I2C Write data done \n");

    // Acknowledge Polling
    do {
      i2c_send_data(I2C_ID, 0xA0);
      i2c_send_command(I2C_ID, I2C_START_WRITE);
    } while (!i2c_get_ack(I2C_ID));

    // Send Write Address, set current addr to 0x00
    i2c_send_data(I2C_ID, 0x00); //Addr: 0x00 
    i2c_send_command(I2C_ID, I2C_WRITE); //send data

    if(!i2c_get_ack(I2C_ID)) {
      printf("No ack received from EEPROM, Address\n");
      return 0;
    }

    i2c_send_command(I2C_ID, I2C_STOP);      //do a stop bit, initiate eeprom write
    while(i2c_busy(I2C_ID));

    // Read data from EEPROM
    // Send Control Byte
    i2c_send_data(I2C_ID, 0xA1); // Control Byte: 0xA0
    i2c_send_command(I2C_ID, I2C_START_WRITE); //do a start bit and send data

    if(!i2c_get_ack(I2C_ID)) {
      printf("No ack received from EEPROM, Device Control, Read data\n");
      return 0;
    }

    int value;

    // Read Data
    for (int i = 0; i < 16; i++) {
      if (i == 15)
        i2c_send_command(I2C_ID, I2C_STOP_READ); //do a stop bit and read data
      else
        i2c_send_command(I2C_ID, I2C_READ);      // read data

      if (!i2c_get_ack(I2C_ID)) {
        printf("No ack received from EEPROM for byte %d\n", i);
        return 0;
      }

      value = i2c_get_data(I2C_ID);
      printf("Received %d expecting %d\n", value, i);
      if (value != i) { 
         printf("I2C W&R data Fail \n");
      }
    }

    printf("I2C Test Pass!!!\n");
}
