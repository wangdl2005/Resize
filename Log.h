#ifndef LOG_H
#define LOG_H

#include <fstream.h>
#include <time.h>
#include <stdio.h>

class Log
{
public:
	static void toLog(const char * message)
	{
		//CTime nowTime = CTime::GetCurrentTime();
		time_t timeep;
		struct tm *p;
		time(&timeep);
		p = localtime(&timeep);
		char tempString[50];
		/*
		wsprintf(tempString,"%d年%d月%d日 %d:%d:%d %s",nowTime.GetYear(),nowTime.GetMonth()
			,nowTime.GetDay(),nowTime.GetHour(),nowTime.GetMinute(),nowTime.GetSecond(),message);*/
		sprintf(tempString,"%d年%d月%d日 %d:%d:%d %s",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday
			,p->tm_hour,p->tm_min,p->tm_sec,message);
		ofstream outFile;
		outFile.open("D:\\codes\\C++\\Projects\\Resize\\Debug\\log.txt",ios::out|ios::app);
		outFile<<tempString<<"\n";
		outFile.close();
	}
};

#endif