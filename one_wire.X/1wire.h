
#ifndef ONEWIRE_H
#define	ONEWIRE_H

    void drive_OW_low (void);
    void drive_OW_high (void);
    int read_OW (void);
    int OW_reset_pulse(void);
    void OW_write_bit (unsigned char write_bit);
    int OW_read_bit (void);
    void OW_write_byte (unsigned char write_data);
    int OW_read_byte (void);
    int Detect_Slave_Device(void);
    void onewire_reset();
    
#endif	/* 1WIRE_H */

