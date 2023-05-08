/*
 * Logger.h
 *
 *  Created on: 2019/12/06
 *      Author: imao_yusuke
 */

#ifndef SRC_LOGGER_LOGGER_H_
#define SRC_LOGGER_LOGGER_H_

class Logger
{
public:
	enum Level
	{
		LevelError   = 0,
		LevelWarning,
		LevelInfo,
		LevelDebug,
	};

	Logger(Level level): m_level(level){};

	virtual void init() = 0;

	virtual void error(const char *format, ...)   = 0;
	virtual void warning(const char *format, ...) = 0;
	virtual void info(const char *format, ...)    = 0;
	virtual void debug(const char *format, ...)   = 0;

protected:

	Level m_level;
};



#endif /* SRC_LOGGER_LOGGER_H_ */
