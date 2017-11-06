///////////////////////////////////////////////////////////
//   UART Task v1.0                                      //
//   ==============                                      //
//   Copyright© 2017 by Felix Knobl, FH Technikum Wien   //
//   Partial codeparts by Martin Horauer                 //
///////////////////////////////////////////////////////////

/**
   @mainpage XMC Interface (UART/SPI/I2C) Template

   @section Usage

   This project is intended to be used along with Eclipse or from the CMD-Line
   using a Makefile. The latter provides the following commands:

   @verbatim
   make                        # build the project
   make debug                  # build the project and start the debugger
   make flash                  # flash the application
   make doc                    # generate the documentation
   make clean                  # delete intermediate files
   @endverbatim

   @section Folders

   The code is organized in various folders - checkout the comments below:

   @verbatim
   Makefile          # top level Makefile used for the command-line variant
   doxygen           # doxygen configuration file
   inc/              # the place for your header files
   src/              # the place for your source files
   bin/              # output folder - here you will find listings and binaries
   Debug/            # this fiolder is only used by Eclipse
   doc/              # here the generated documentation is found
   system/           # CMSIS library, Infineon header files, linker script, etc.
   xmclib/           # low-level XMC library
   @endverbatim

   @file main.c
   @date 08 Sep, 2015
   @version 1.0.0

   @brief XMC4500 Relax Interface Template


   UART communication via USB-to-Serial cable
   UART_TX P1_5 -> CABLE_RX (WHITE)
   UART_RX P1_4 -> CABLE_TX (GREEN)

   History <br>

   Version 1.0.0 Initial <br>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xmc4500_uart_lib.h>
#include <debug_lib.h>

#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

//#define DEBUG

int main (void)
{
	char rx_buff[RX_BUFFER_SIZE] = {0};
	char tx_buff[TX_BUFFER_SIZE] = {0};

	initRetargetSwo();

	_init_uart0_ch0();

	#if defined DEBUG
		_uart_printf ("UART ready. Waiting for data...\r\n");
	#endif

	while (1)
	{
		// Empty RX buffer
		memset(&rx_buff, 0, sizeof(rx_buff));
		memset(&tx_buff, 0, sizeof(tx_buff));

		// Get buffer from UART
		_uart_get_string (rx_buff);

		#if defined DEBUG
			_uart_printf("INFO: User input: '%s'\r\n", rx_buff);
		#endif

		// Check min length, '#' and ','
		if (strlen(rx_buff) < 5 || rx_buff[0] != '#' || rx_buff[2] != ',')
		{
			_uart_printf("NACK\n");
			continue;
		}

		switch (rx_buff[1])
		{
			case 'r':
			{
				char *pFromNumber = strtok(rx_buff + 3, ":");

				// Token found?
				if (pFromNumber == NULL)
				{
					_uart_printf ("NACK\n");
					continue;
				}

				char *str = NULL;
				int32_t fromNumber = strtol(pFromNumber, &str, 10);

				if (str == pFromNumber || *str != '\0')
				{
					_uart_printf ("NACK\n");

					#if defined DEBUG
						_uart_printf ("Error converting FromNumber\r\n");
					#endif

					continue;
				}

				char *pToNumber = strtok(NULL, "\r\n");

				// Token found?
				if (pToNumber == NULL)
				{
					_uart_printf ("NACK\n");
					continue;
				}

				str = NULL;
				int32_t toNumber = strtol(pToNumber, &str, 10);

				if (str == pToNumber || *str != '\0')
				{
					_uart_printf ("NACK\n");

					#if defined DEBUG
						_uart_printf ("Error converting ToNumber\r\n");
					#endif

					continue;
				}

				if (toNumber < fromNumber)
				{
					_uart_printf ("NACK\n");

					#if defined DEBUG
						_uart_printf ("ERROR: toNumber < fromNumber\r\n");
					#endif

					continue;
				}

				int32_t randomNumber = rand() % (toNumber + 1 - fromNumber) + fromNumber;

				_uart_printf("ACK\n#a,%i\n", randomNumber);

				#if defined DEBUG
					_uart_printf("From: '%d', To: '%d', Random: '%d'\r\n", fromNumber, toNumber, randomNumber);
				#endif

				break;

			}

			default:
				_uart_printf("NACK\n");
				break;
		}
	}
}
