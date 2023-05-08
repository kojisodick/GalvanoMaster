/*
 * Uart.cpp
 *
 *  Created on: 2019/12/06
 *      Author: imao_yusuke
 */

#include <Logger/Uart.h>
#include <string.h>
#include <stdio.h>

#ifdef __arm__
#include "xil_printf.h"
#else
#include <iostream>
#include <cstdarg>
#endif

static char tempBuf[128] = {'\0'};

Uart::Uart(Level level): Logger(level)
{
}

Uart::~Uart()
{
}

void Uart::error(const char *format, ...)
{
	if( m_level < LevelError ) return;

	va_list va;
	va_start(va, format);
	vsprintf_s(tempBuf, format, va);
	va_end(va);

	printf("[ERROR] %s\r\n", tempBuf);
}

void Uart::warning(const char *format, ...)
{
	if( m_level < LevelWarning ) return;

	va_list va;
	va_start(va, format);
	vsprintf_s(tempBuf, format, va);
	va_end(va);

	printf("[WARN ] %s\r\n", tempBuf);
}

void Uart::info(const char *format, ...)
{
	if( m_level < LevelInfo ) return;

	va_list va;
	va_start(va, format);
	vsprintf_s(tempBuf, format, va);
	va_end(va);

	printf("[INFO ] %s\r\n", tempBuf);
}

void Uart::debug(const char *format, ...)
{
	//if( m_level < LevelDebug ) return;

	va_list va;
	va_start(va, format);
	vsprintf_s(tempBuf, format, va);
	va_end(va);

	printf("[DEBUG] %s\r\n", tempBuf);
}

void Uart::printf(const char * type, const char * msg)
{
#if !defined(_WIN32) && !defined(_WIN64)
	xil_printf("[%s] %s\r\n", type, msg);
#else
	::printf("[%s] %s\r\n", type, msg);
#endif
}