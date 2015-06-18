/****************************************************************************
** ** Program: This file is part of the Util                 
** Author:  leiqiang  
** Date  :  2006-02-01
**
** RCS: $Id: sss.h,v 1.1.1.1 2006/09/25 01:28:12 sss Exp $
**
****************************************************************************/

#ifndef pub_c_h
#define pub_c_h

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

#include "Global_Macros.h"

namespace Pubc
{

  //�ļ������ຯ��,ע�ⲿ�ֺ����漰static���������̲߳���ȫ��
	extern char* getHeadName(const char * pFileName,char * pRes);  //��ȡȥ����չ������ļ�����
	extern char* getHeadName(const char * pFileName);  						 //ͬ�ϣ��̲߳���ȫ
	extern char* getExtName(const char * pFileName,char * pRes);   //��ȡ�ļ���չ��
  extern char* getExtName(const char * pFileName);               //ͬ�ϣ��̲߳���ȫ
	extern char* getShortName(const char * pFullName,char * pRes); //�õ�������·�����ļ���
	extern char* getShortName(const char * pFullName);             //ͬ�ϣ��̲߳���ȫ
	extern bool  isFileExist(const char * pFileName);                   //�ж��ļ��Ƿ����	
	extern int   getFileSize(int fd);                                   //���ݾ��ȡ�ļ�����
	extern int   getFileSize(const char * pFileName);                   //�����ļ�����ȡ�ļ�����
	extern bool  isPathExist(const char * pPath);                       //�ж�·���Ƿ����




  //�ַ���������
  extern char * upper(char * pStr);     		 	//ת��Ϊ��д�ַ���,�ƻ�ԭ������
	extern char * lower(char *pStr);      		 	//ת��ΪСд�ַ���,�ƻ�ԭ������
	extern char * firstUpper(char* szIn);      //ת��Ϊ����ĸСд�ַ���,�ƻ�ԭ������
  extern char * trimLeft(char *pStr);       	//�ü����ַ�����ߵĿո�,�ƻ�ԭ������
	extern char * trimRight(char *pStr);      	//�ü����ַ����ұߵĿո�,�ƻ�ԭ������
	extern char * trim(char *pStr);           	//�ü����ַ�����β��ͷ�Ŀո�,�ƻ�ԭ������
	
	/*ģ��ORACLE��like�� ��ǰ֧��%���ַ�ͨ���, '_'���ַ�ͨ���,'%'��ʾ��'\%','\'��ʾ��'\\','_'��ʾ��'\_' 	*/
	extern bool like(const char * pSrc, const char * pTag);
	//�ô�pDescStrr���滻pStrBuf���е�pSrcSt�Ӵ�,pStrBuf�����㹻�󣬲���ȫ
	extern char * replace(char *pStrBuf,const char *pSrcStr,const char *pDescStr);

  //���뼶����
  extern void uSleep(int num);
  //��̬װ�����ӿ��еĺ���,��֧��linux/unixƽ̨��ע�Ⲣδ����close������������������open��Ͳ���close�ĳ���
  extern void * loadDllFunc(const char *pLib,const char *pModule);
  //���ת���ɺ���
  extern char * moneyToCapital(int amount,char * res_str=NULL);
  //�ַ�ת��
	//extern char * strToHex(const char * p_str,char * res_buf,int size);
	//extern char * hexToStr(const char * p_hex,char * res_buf,int size);

}

#endif

