/*
 * Fatfs.h
 *
 *  Created on: 2019/12/06
 *      Author: imao_yusuke
 */

#ifndef SRC_LOGGER_FATFS_H_
#define SRC_LOGGER_FATFS_H_

#include <Logger/Logger.h>
#include <stdio.h>


#if !defined(_WIN32) && !defined(_WIN64)
#include "ff.h"
#endif

class Fatfs: public Logger
{
public:
	Fatfs(Level level, const char *fileName);
	~Fatfs();

	void init();

	void error(const char *format, ...);
	void warning(const char *format, ...);
	void info(const char *format, ...);
	void debug(const char *format, ...);

private:
#if !defined(_WIN32) && !defined(_WIN64)
	FATFS m_fatfs;
	FIL   m_fil;
#else
	FILE * m_fil;
#endif
	char  m_fileName[16];
	bool  m_opened;

	void fprintf(const char * type, const char * text);
};

#endif /* SRC_LOGGER_FATFS_H_ */
