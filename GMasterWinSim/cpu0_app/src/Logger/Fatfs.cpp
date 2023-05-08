/*
 * Fatfs.cpp
 *
 *  Created on: 2019/12/06
 *      Author: imao_yusuke
 */

#include <Logger/Fatfs.h>
#include "Common.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include "xsdps.h"
#include "xil_printf.h"
#else
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstdarg>
#endif

static char tempBuf[128] = {'\0'};


Fatfs::Fatfs(Level level, const char *fileName)
: Logger(level), m_opened(false)
{
	if( !fileName || (strlen(fileName) >= 8) ) {
		strcpy_s(m_fileName, "01.log");
		return;
	}

	strcpy_s(m_fileName, fileName);
}

Fatfs::~Fatfs()
{
#if !defined(_WIN32) && !defined(_WIN64)
	f_close(&m_fil);
	f_mount(0, 0, 0);
#else
	fclose(m_fil);
#endif
}

void Fatfs::init()
{
#if !defined(_WIN32) && !defined(_WIN64)
	FRESULT res = f_mount(&m_fatfs, "0:/", 0);
	if( res ) {
		return;
	}

	char oldFile[32] = "_";
	strcat(oldFile, m_fileName);

	f_unlink(oldFile);
	f_rename(m_fileName, oldFile);


	res = f_open(&m_fil, m_fileName, FA_CREATE_ALWAYS | FA_WRITE);

	if( res ) {
		xil_printf("open falied.\r\n");
		return;
	}
#else
	char oldFile[32] = "_";
	strcpy_s(oldFile, m_fileName);
	
	rename(m_fileName, oldFile);

	m_fil = fopen(m_fileName,"w");
	if (!m_fil) {
		::printf("Failed to open log file file [%s].",m_fileName);
		return;
	}
#endif
	m_opened = true;

}

void Fatfs::error(const char *format, ...)
{
	if( !m_opened ) return;
	if( m_level < LevelError ) return;

	va_list va;
	va_start(va, format);
	vsprintf(tempBuf, format, va);
	va_end(va);

	fprintf("[ERROR]", tempBuf);
}

void Fatfs::warning(const char *format, ...)
{
	if( !m_opened ) return;
	if( m_level < LevelWarning ) return;

	va_list va;
	va_start(va, format);
	vsprintf(tempBuf, format, va);
	va_end(va);

	fprintf("[WARN]", tempBuf);
}

void Fatfs::info(const char *format, ...)
{
	if( !m_opened ) return;
	if( m_level < LevelInfo ) return;

	va_list va;
	va_start(va, format);
	vsprintf(tempBuf, format, va);
	va_end(va);

	fprintf("[INFO]", tempBuf);
}

void Fatfs::debug(const char *format, ...)
{
	if( !m_opened ) return;
	//if( m_level < LevelDebug ) return;

	va_list va;
	va_start(va, format);
	vsprintf(tempBuf, format, va);
	va_end(va);

	fprintf("[DEBUG]",tempBuf);
}

extern void output_func(const char * type, const char * msg);

void Fatfs::fprintf(const char * type, const char * msg)
{
#if !defined(_WIN32) && !defined(_WIN64)
	f_printf(&m_fil, "[%s] %s\r\n", type, msg);
	f_sync(&m_fil);
#else
	::fprintf(m_fil, "%s %s\n", type, msg);
	::fflush(m_fil);
	//::printf("[%s] %s\n", type, msg);

#if 1
	output_func(type,msg);
#endif

#endif
}