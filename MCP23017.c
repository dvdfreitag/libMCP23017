#include <avr/io.h>

#include "TWI.h"

#define MCP23017_OPCODE 0x40U

#define IODIRA_B0 	0x00U
#define IODIRB_B0 	0x01U
#define IPOLA_B0 	0x02U
#define IPOLB_B0 	0x03U
#define GPINTENA_B0 0x04U
#define GPINTENB_B0 0x05U
#define DEFVALA_B0 	0x06U
#define DEFVALB_B0 	0x07U
#define INTCONA_B0 	0x08U
#define INTCONB_B0 	0x09U
#define IOCONA_B0 	0x0AU
#define IOCONB_B0 	0x0BU
#define GPPUA_B0 	0x0CU
#define GPPUB_B0 	0x0DU
#define INTFA_B0 	0x0EU
#define INTFB_B0 	0x0FU
#define INTCAPA_B0 	0x10U
#define INTCAPB_B0 	0x11U
#define GPIOA_B0 	0x12U
#define GPIOB_B0 	0x13U
#define OLATA_B0 	0x14U
#define OLATB_B0 	0x15U

#define IODIRA_B1 	0x00U
#define IODIRB_B1 	0x10U
#define IPOLA_B1 	0x01U
#define IPOLB_B1 	0x11U
#define GPINTENA_B1 0x02U
#define GPINTENB_B1 0x12U
#define DEFVALA_B1 	0x03U
#define DEFVALB_B1 	0x13U
#define INTCONA_B1 	0x04U
#define INTCONB_B1 	0x14U
#define IOCONA_B1 	0x05U
#define IOCONB_B1 	0x15U
#define GPPUA_B1 	0x06U
#define GPPUB_B1 	0x16U
#define INTFA_B1 	0x07U
#define INTFB_B1 	0x17U
#define INTCAPA_B1 	0x08U
#define INTCAPB_B1 	0x18U
#define GPIOA_B1 	0x09U
#define GPIOB_B1 	0x19U
#define OLATA_B1 	0x0AU
#define OLATB_B1 	0x1AU

// The following indexing scheme works based on the truthy/falsey 
//   value, Bank, as well as the passed truthy/falsey port. 
//   Each function starts with a base index of the PORTA address
//   and uses the passed port value to offset to the PORTB address
//   if necessary. From there, the current Bank value is used to
//   determine the index for the second dimension, which gives us
//   the bank to use. In the end there is a slight performance/speed 
//   gain, but if you only use a few functions there is a big bulky 
//   44 byte array for almost no reason. The alternative is a set of 
//   nested if/else logic in each function. That makes using one or 
//   two functions use less space, but it's obviously slower and 
//   scales poorly. The result is that if you end up using almost 
//   every function, the maximum size is quite small.

uint8_t Bank_Addr[22][2] = {   // Bank = 0    Bank = 1
                               { IODIRA_B0,   IODIRA_B1   },
                               { IODIRB_B0,   IODIRB_B1   },
                               { IPOLA_B0,    IPOLA_B1    },
                               { IPOLB_B0,    IPOLB_B1    },
                               { GPINTENA_B0, GPINTENA_B1 },
                               { GPINTENB_B0, GPINTENB_B1 },
                               { DEFVALA_B0,  DEFVALA_B1  },
                               { DEFVALB_B0,  DEFVALB_B1  },
                               { INTCONA_B0,  INTCONA_B1  },
                               { INTCONB_B0,  INTCONB_B1  },
                               { IOCONA_B0,   IOCONA_B1   },
                               { IOCONB_B0,   IOCONB_B1   },
                               { GPPUA_B0,    GPPUA_B1    },
                               { GPPUB_B0,    GPPUB_B1    },
                               { INTFA_B0,    INTFA_B1    },
                               { INTFB_B0,    INTFB_B1    },
                               { INTCAPA_B0,  INTCAPA_B1  },
                               { INTCAPB_B0,  INTCAPB_B1  },
                               { GPIOA_B0,    GPIOA_B1    },
                               { GPIOB_B0,    GPIOB_B1    },
                               { OLATA_B0,    OLATA_B1    },
                               { OLATB_B0,    OLATB_B1    }
                           };

TWI_t *TWI;
uint8_t Address;
uint8_t Bank;

uint8_t value_write(uint8_t addr, uint8_t value)
{	// Sanity check
	if (!TWI) return TWI_NACK;

	TWI_Start(TWI);

	if (TWI_WriteByte(MCP23017_OPCODE | Address) != TWI_ACK) goto error;
	if (TWI_WriteByte(addr) != TWI_ACK) goto error;
	if (TWI_WriteByte(value) != TWI_ACK) goto error;

	TWI_Stop();
	return TWI_ACK;

error:
	TWI_Stop();
	return TWI_NACK;
}

uint8_t value_read(uint8_t addr, uint8_t *value)
{	// Sanity check
	if (!TWI || !value) return TWI_NACK;

	TWI_Start(TWI);

	if (TWI_WriteByte(MCP23017_OPCODE | Address) != TWI_ACK) goto error;
	if (TWI_WriteByte(addr) != TWI_ACK) goto error;

	TWI_Restart();

	if (TWI_WriteByte(MCP23017_OPCODE | Address | 0x01) != TWI_ACK) goto error;
	uint8_t retval = TWI_ReadByte(TWI_NACK);

	TWI_Stop();
	return retval;

error:
	TWI_Stop();
	return TWI_NACK;
}

void MCP23017_Init(TWI_t *twi, uint8_t address)
{
	TWI = twi;
	Address = (address & 0x07) << 1;
}

uint8_t MCP23017_GetIODIR(uint8_t *iodir, uint8_t port)
{
	return value_read(Bank_Addr[IODIRA_B0 + (port != 0)][(Bank != 0)], iodir);
}

uint8_t MCP23017_SetIODIR(uint8_t iodir, uint8_t port)
{
	return value_write(Bank_Addr[IODIRA_B0 + (port != 0)][(Bank != 0)], iodir);
}

uint8_t MCP23017_GetIPOL(uint8_t *ipol, uint8_t port)
{
	return value_read(Bank_Addr[IPOLA_B0 + (port != 0)][(Bank != 0)], ipol);
}

uint8_t MCP23017_SetIPOL(uint8_t ipol, uint8_t port)
{
	return value_write(Bank_Addr[IPOLA_B0 + (port != 0)][(Bank != 0)], ipol);
}

uint8_t MCP23017_GetGPINTEN(uint8_t *gpinten, uint8_t port)
{
	return value_read(Bank_Addr[GPINTENA_B0 + (port != 0)][(Bank != 0)], gpinten);
}

uint8_t MCP23017_SetGPINTEN(uint8_t gpinten, uint8_t port)
{
	return value_write(Bank_Addr[GPINTENA_B0 + (port != 0)][(Bank != 0)], gpinten);
}

uint8_t MCP23017_GetDEFVAL(uint8_t *defval, uint8_t port)
{
	return value_read(Bank_Addr[DEFVALA_B0 + (port != 0)][(Bank != 0)], defval);
}

uint8_t MCP23017_SetDEFVAL(uint8_t defval, uint8_t port)
{
	return value_write(Bank_Addr[DEFVALA_B0 + (port != 0)][(Bank != 0)], defval);
}

uint8_t MCP23017_GetINTCON(uint8_t *intcon, uint8_t port)
{
	return value_read(Bank_Addr[INTCONA_B0 + (port != 0)][(Bank != 0)], intcon);
}

uint8_t MCP23017_SetINTCON(uint8_t intcon, uint8_t port)
{
	return value_write(Bank_Addr[INTCONA_B0 + (port != 0)][(Bank != 0)], intcon);
}

uint8_t MCP23017_GetIOCON(uint8_t *iocon)
{
	return value_read(Bank_Addr[IOCONA_B0][(Bank != 0)], iocon);
}

uint8_t MCP23017_SetIOCON(uint8_t iocon)
{
	return value_write(Bank_Addr[IOCONA_B0][(Bank != 0)], iocon);

	Bank = iocon & 0x80; // Update Bank when setting IOCON
}

uint8_t MCP23017_GetGPPU(uint8_t *gppu, uint8_t port)
{
	return value_read(Bank_Addr[GPPUA_B0 + (port != 0)][(Bank != 0)], gppu);
}

uint8_t MCP23017_SetGPPU(uint8_t gppu, uint8_t port)
{
	return value_write(Bank_Addr[GPPUA_B0 + (port != 0)][(Bank != 0)], gppu);
}

uint8_t MCP23017_GetINTF(uint8_t *intf, uint8_t port)
{
	return value_read(Bank_Addr[INTFA_B0 + (port != 0)][(Bank != 0)], intf);
}

uint8_t MCP23017_SetINTF(uint8_t intf, uint8_t port)
{
	return value_write(Bank_Addr[INTFA_B0 + (port != 0)][(Bank != 0)], intf);
}

uint8_t MCP23017_GetINTCAP(uint8_t *intcap, uint8_t port)
{
	return value_read(Bank_Addr[INTCAPA_B0 + (port != 0)][(Bank != 0)], intcap);
}

uint8_t MCP23017_SetINTCAP(uint8_t intcap, uint8_t port)
{
	return value_write(Bank_Addr[INTCAPA_B0 + (port != 0)][(Bank != 0)], intcap);
}

uint8_t MCP23017_GetGPIO(uint8_t *gpio, uint8_t port)
{
	return value_read(Bank_Addr[GPIOA_B0 + (port != 0)][(Bank != 0)], gpio);
}

uint8_t MCP23017_SetGPIO(uint8_t gpio, uint8_t port)
{
	return value_write(Bank_Addr[GPIOA_B0 + (port != 0)][(Bank != 0)], gpio);
}

uint8_t MCP23017_GetOLAT(uint8_t *olat, uint8_t port)
{
	return value_read(Bank_Addr[OLATA_B0 + (port != 0)][(Bank != 0)], olat);
}

uint8_t MCP23017_SetOLAT(uint8_t olat, uint8_t port)
{
	return value_write(Bank_Addr[OLATA_B0 + (port != 0)][(Bank != 0)], olat);
}
