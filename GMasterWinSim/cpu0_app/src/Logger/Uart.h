/*
 * Uart.h
 *
 *  Created on: 2019/12/06
 *      Author: imao_yusuke
 */

#ifndef SRC_LOGGER_UART_H_
#define SRC_LOGGER_UART_H_

#include <Logger/Logger.h>

class Uart: public Logger
{
public:
	Uart(Level level);
	~Uart();

	void init(){};

	void error(const char *format, ...);
	void warning(const char *format, ...);
	void info(const char *format, ...);
	void debug(const char *format, ...);

private:
	void printf(const char * type, const char * msg);

};

#endif /* SRC_LOGGER_UART_H_ */
