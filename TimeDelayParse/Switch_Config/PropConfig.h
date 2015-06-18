/****************************************************************************
** ** Program: This file is part of the Util                 
** Author:  leiqiang  
** Date  :  2006-02-01
**
** RCS: $Id: sss.h,v 1.1.1.1 2006/09/25 01:28:12 sss Exp $
**
****************************************************************************/




#ifndef PropConfig_H
#define PropConfig_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>

using namespace std;


class Property
{
public:
	 string key;
	 string value;
	 Property& operator=(const Property& p)
		{
			if( this != &p )
			{
				key = p.key;
				value = p.value;
			}
		return *this;
	  }
};

/*================Example====================*
#############################################################
#                  For Kado Code Debug Logger
#############################################################
log4j.logger.com.wasu=DEBUG, C
log4j.appender.C=org.apache.log4j.RollingFileAppender 
log4j.appender.C.File=logs/gp-kado-code.log 
log4j.appender.C.MaxFileSize=5MB
log4j.appender.C.MaxBackupIndex=12
*================Example====================
ע�⣬ֻ�п�ͷΪ'#'���Ϊע�ͣ������κεط�����'#'Ҳ���Ϊ����
log4j.appender�ɿ���log4j.appender.C�ĸ��ڵ�
*/

class PropConfig
{
    public:
        PropConfig(){};
        bool    init(const char* pConfFile);  
        //��key��ȡValue������log4j.appender.C.File
				string  getValue(const char * key);
				string  getValue(const string &key);
				//��key��ǰ׺����ȡ����ǰ׺������ֵ������log4j.appender
				bool    find(const char *front_str,list<Property>  &props);
				bool    find(const string &front_str,list<Property>  &props);
    private:
        map<string,Property>  Props;
        bool     parse(const char * buf,Property &prop);
};


#endif

