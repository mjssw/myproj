#ifndef _LOG4CXX_CREATER_H_
#define _LOG4CXX_CREATER_H_

#include <log4cxx/logger.h> 
#include <log4cxx/helpers/object.h> 
#include <log4cxx/helpers/pool.h> 
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/basicconfigurator.h> 
#include <log4cxx/PatternLayout.h> 
#include <log4cxx/DailyRollingFileAppender.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/filter/levelrangefilter.h>
#include <log4cxx/xml/domconfigurator.h>

using namespace log4cxx; 
using namespace log4cxx::helpers;

struct LogConfig
{
	LogConfig() : 
		m_bEnableFile(false), 
		m_bEnableConsole(false)
	{}

	bool m_bEnableFile;
	std::string m_File;
	std::string m_FileMaxLevel;
	std::string m_FileMinLevel;
	std::string m_FileLayout;

	bool m_bEnableConsole;
	std::string m_ConsoleMaxLevel;
	std::string m_ConsoleMinLevel;
	std::string m_ConsoleLayout;
};

class CLog4cxxCreater
{
public:
	static LoggerPtr GetLogger(const LogConfig &logCfg, const char *loggerName)
	{
		if( logCfg.m_bEnableFile )
		{
			log4cxx::filter::LevelRangeFilter *filter = new log4cxx::filter::LevelRangeFilter();
			filter->setLevelMin( GetLevel(logCfg.m_FileMinLevel) );
			filter->setLevelMax( GetLevel(logCfg.m_FileMaxLevel) );

			std::string dataPatten = "'.'yyyy-MM-dd";
			log4cxx::DailyRollingFileAppender *appender = new log4cxx::DailyRollingFileAppender(
				new log4cxx::PatternLayout( log4cxx::helpers::Transcoder::decode(logCfg.m_FileLayout.c_str()) ),
				log4cxx::helpers::Transcoder::decode(logCfg.m_File.c_str()),
				log4cxx::helpers::Transcoder::decode(dataPatten.c_str()) );
			log4cxx::helpers::Pool p; 
			appender->activateOptions( p );
			appender->addFilter( filter );

			log4cxx::BasicConfigurator::configure( log4cxx::AppenderPtr(appender) ); 
		}

		if( logCfg.m_bEnableConsole )
		{
			log4cxx::filter::LevelRangeFilter *filter = new log4cxx::filter::LevelRangeFilter();
			filter->setLevelMin( GetLevel(logCfg.m_ConsoleMinLevel) );
			filter->setLevelMax( GetLevel(logCfg.m_ConsoleMaxLevel) );

			log4cxx::ConsoleAppender *appender = new log4cxx::ConsoleAppender(
				new log4cxx::PatternLayout( 
					log4cxx::helpers::Transcoder::decode(logCfg.m_ConsoleLayout.c_str()) ));
			appender->addFilter( filter );
	
			log4cxx::BasicConfigurator::configure( log4cxx::AppenderPtr(appender) ); 
		}

		return log4cxx::Logger::getLogger( loggerName );
	}

private:
	static LevelPtr GetLevel(const std::string& strLevel)
	{
		if( strLevel == "DEBUG" )
		{
			return log4cxx::Level::getDebug();
		}
		else if( strLevel == "INFO" )
		{
			return log4cxx::Level::getInfo();
		}
		else if( strLevel == "ERROR" )
		{
			return log4cxx::Level::getError();
		}
		else if( strLevel == "FATAL" )
		{
			return log4cxx::Level::getFatal();
		}

		return log4cxx::Level::getDebug();
	}
};

#endif
