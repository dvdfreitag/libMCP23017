#ifndef MCP23017_H_
#define MCP23017_H_

#include <avr/io.h>

#define MCP23017_PORTA 0x00
#define MCP23017_PORTB 0x01

void MCP23017_Init(TWI_t *twi, uint8_t address);

uint8_t MCP23017_GetIODIR(uint8_t *iodir, uint8_t port);
uint8_t MCP23017_SetIODIR(uint8_t iodir, uint8_t port);
uint8_t MCP23017_GetIPOL(uint8_t *ipol, uint8_t port);
uint8_t MCP23017_SetIPOL(uint8_t ipol, uint8_t port);
uint8_t MCP23017_GetGPINTEN(uint8_t *gpinten, uint8_t port);
uint8_t MCP23017_SetGPINTEN(uint8_t gpinten, uint8_t port);
uint8_t MCP23017_GetDEFVAL(uint8_t *defval, uint8_t port);
uint8_t MCP23017_SetDEFVAL(uint8_t defval, uint8_t port);
uint8_t MCP23017_GetINTCON(uint8_t *intcon, uint8_t port);
uint8_t MCP23017_SetINTCON(uint8_t intcon, uint8_t port);
uint8_t MCP23017_GetIOCON(uint8_t *iocon);
uint8_t MCP23017_SetIOCON(uint8_t iocon);
uint8_t MCP23017_GetGPPU(uint8_t *gppu, uint8_t port);
uint8_t MCP23017_SetGPPU(uint8_t gppu, uint8_t port);
uint8_t MCP23017_GetINTF(uint8_t *intf, uint8_t port);
uint8_t MCP23017_SetINTF(uint8_t intf, uint8_t port);
uint8_t MCP23017_GetINTCAP(uint8_t *intcap, uint8_t port);
uint8_t MCP23017_SetINTCAP(uint8_t intcap, uint8_t port);
uint8_t MCP23017_GetGPIO(uint8_t *gpio, uint8_t port);
uint8_t MCP23017_SetGPIO(uint8_t gpio, uint8_t port);
uint8_t MCP23017_GetOLAT(uint8_t *olat, uint8_t port);
uint8_t MCP23017_SetOLAT(uint8_t olat, uint8_t port);

#endif
