/*
 * 01_Echo_Tx
 * 
 * This project is the receiver side of the Echo project,
 * The nrf24 radio is configurd to transmit 1 byte and receive the same byte via
 * ACK with payload from the receiver radio.
 * The data sent is the char received with the UART component, when the receiver
 * radio send the char back, the transmitter radio will print it on the UART.
 */
#include "project.h"
#include <stdbool.h>

volatile bool irq_flag = false;

// This handler is executed when we get an interrupt from
// the IRQ pin of the radio
CY_ISR(IRQ_Handler)
{
    irq_flag = true;
    IRQ_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable;
    
    const uint8_t RX_ADDR[5]= {0xBA, 0xAD, 0xC0, 0xFF, 0xEE};
    
    // Set the Handler for the IRQ interrupt
    isr_IRQ_StartEx(IRQ_Handler);
    
    nRF24_start();
    nRF24_setRxPipe0Address(RX_ADDR, 5);

    while (1) {
        
        if (true == irq_flag) {
            
            // Get and clear the flag that caused the IRQ interrupt
            NrfIRQ flag = nRF24_getIRQFlag();
            nRF24_clearIRQFlag(flag);
            
            unsigned char data;
            
            // get the data from the transmitter
            nRF24_getRxPayload(&data, 1);

            // Send the data + ACK
            nRF24_rxWritePayload(0, &data, 1);
            
            irq_flag = false;
        }
        
    }
}

/* [] END OF FILE */