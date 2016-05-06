#include <xc.h>
#include <sys/attribs.h>
#include "timer.h"
#include "1wire.h"

#define TRIS_OW         TRISGbits.TRISG1
#define OW_WRITE_PIN    LATGbits.LATG1   
#define OW_READ_PIN     PORTGbits.RG1

void drive_OW_low (void)   //Working
 {
  TRIS_OW       = 0;
  OW_WRITE_PIN  = 0;
 }
 /**********************************************************************
 * Function:        void drive_OW_high (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     None 
 * Overview:     Configure the OW_PIN as Output and drive the OW_PIN HIGH. 
 ***********************************************************************/
 void drive_OW_high (void) //Working
 {
  TRIS_OW = 0;
  OW_WRITE_PIN = 1; 
 }
 /**********************************************************************
 * Function:        unsigned char read_OW (void)
 * PreCondition:    None
 * Input:     None 
 * Output:     Return the status of OW pin. 
 * Overview:     Configure as Input pin and Read the status of OW_PIN  
 ***********************************************************************/
 int read_OW (void)
 {
  unsigned char read_data=0;
  
    TRIS_OW = 1;
    if (OW_READ_PIN==1)
    read_data = 1;
    else  
    read_data = 0;
   
    return read_data;
 }
 /**********************************************************************
 * Function:        unsigned char OW_reset_pulse(void)
 * PreCondition:    None
 * Input:     None 
 * Output:     Return the Presense Pulse from the slave. 
 * Overview:     Initialization sequence start with reset pulse.
 *       This code generates reset sequence as per the protocol
 ***********************************************************************/
 int OW_reset_pulse(void)  //Working
 {
    unsigned char presence_detect;

    drive_OW_low();     // Drive the bus low

    delay_us(480);      // delay 480 microsecond (us) 
    drive_OW_high ();      // Release the bus

    delay_us(70);    // delay 70 microsecond (us)

    presence_detect = read_OW(); //Sample for presence pulse from slave
    delay_us(410);      // delay 410 microsecond (us)


    drive_OW_high ();       // Release the bus

    return presence_detect;
 } 
 /**********************************************************************
 * Function:        void OW_write_bit (unsigned char write_data)
 * PreCondition:    None
 * Input:     Write a bit to 1-wire slave device.
 * Output:     None
 * Overview:     This function used to transmit a single bit to slave device.
 *       
 ***********************************************************************/
 void OW_write_bit (unsigned char write_bit)
 {
   
  if (write_bit) {
    drive_OW_low();  //writing a bit '1'
    delay_us(6);    // delay 6 microsecond (us)
    drive_OW_high();      // Release the bus
    delay_us(64);    // delay 64 microsecond (us)
    return;
  } else {
  drive_OW_low();       //writing a bit '0' // Drive the bus low
    delay_us(60);        // delay 60 microsecond (us)
    drive_OW_high();    // Release the bus
    delay_us(10);        // delay 10 microsecond for recovery (us)
  }
 } 
 
 /**********************************************************************
 * Function:        unsigned char OW_read_bit (void)
 * PreCondition:    None
 * Input:     None
 * Output:     Return the status of the OW PIN
 * Overview:     This function used to read a single bit from the slave device.
 *       
 ***********************************************************************/
 int OW_read_bit (void)
 {
 
    //reading a bit 
    drive_OW_low();       // Drive the bus low
    delay_us(6);
    drive_OW_high();  // Release the bus
    TRIS_OW = 1; 
    delay_us(6);
    delay_us(6);
    // Toggle();  // delay 9 microsecond (us)
    if (OW_READ_PIN==0)
    {
        delay_us(55);
        return 0;
    }
    if (OW_READ_PIN==1)
    {
        delay_us(55);
        return 1;
    }
  
   return 0;
 }
 /**********************************************************************
 * Function:        void OW_write_byte (unsigned char write_data)
 * PreCondition:    None
 * Input:     Send byte to 1-wire slave device
 * Output:     None
 * Overview:     This function used to transmit a complete byte to slave device.
 *       
 ***********************************************************************/
 void OW_write_byte (unsigned char write_data)
 {
  unsigned char loop;
  
  for (loop = 0; loop < 8; loop++)
  {
   OW_write_bit(write_data & 0x01);  //Sending LS-bit first
   write_data >>= 1;     // shift the data byte for the next bit to send
  } 
 } 
 /**********************************************************************
 * Function:        unsigned char OW_read_byte (void)
 * PreCondition:    None
 * Input:     None
 * Output:     Return the read byte from slave device
 * Overview:     This function used to read a complete byte from the slave device.
 *       
 ***********************************************************************/
 int OW_read_byte (void)
 {
  unsigned char loop, result=0;
  
  for (loop = 0; loop < 8; loop++)
  {
   
   result >>= 1;     // shift the result to get it ready for the next bit to receive
   if (OW_read_bit())
   result |= 0x80;    // if result is one, then set MS-bit
  }
  return result;     
 } 
 
 /**********************************************************************
 * Function:        unsigned char Detect_Slave_Device(void)
 * PreCondition:    None
 * Input:     None 
 * Output:     0 - Not Present   1 - Present  
 * Overview:       To check the presence of slave device.    
 ***********************************************************************/
 int Detect_Slave_Device(void)
 {
  if (!OW_reset_pulse())
   return 1;
  else   
   return 0;
 } 
 /********************************************************************************************
                   E N D     O F     1 W I R E . C  
 *********************************************************************************************/

 void onewire_reset()
 {
     drive_OW_low();
     delay_us(500);
     drive_OW_high();
     delay_us(500);
     drive_OW_high();
 }