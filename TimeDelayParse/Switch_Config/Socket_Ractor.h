#ifndef		_SOCKET_RACTOR_H_
#define		_SOCKET_RACTOR_H_

/**************************************************************************
 * \file    Socket_Ractor.h
 * \brief   
 *
 * Copyright (c) .
 * RCS: $Id: rate_sock.h,v 1.9 2008/08/06 10:39:17 leiq Exp $
 *
 * History
 *  2006/10/11 leiq     created
**************************************************************************/


#include "Socket.h"

#include <list>
#include <map>
#include <set>
#include <queue>


using namespace std;
//һ��1024����,�����޸��ں�����¿��ܴܺ�
const     int  SOCKET_MAXSIZE = FD_SETSIZE;


/**************************************************************************
  * class :Socket_Checker
  * brief :
         1��socket��Ӧ������������socket���н��м�⣬���¼�����ʱpush�����������У�
         		�ⲿ�߳�ʹ��pop_ReadyHandle�������л�ȡ��������������ⲿ�ٴηŻصȴ�����
         2��Ϊ������������������read set�ļ��
**************************************************************************/

class Socket_Checker 
{
	public:
		Socket_Checker(){};
		virtual bool  register_Handle(int handle)=0;
		virtual bool  remove_Handle(int handle)=0;
		//pop_ReadyHandle�ڲ����⣬����û������ʱ���뼶����,��ʱ����false
		virtual bool  pop_ReadyHandle(int &handle,int timeout_Millisecond=100)=0;
	protected:				
		virtual int check_io_once(int timeout_Millisecond=0)=0;//���һ��io���
};

class Socket_Set
{
public:
  Socket_Set();
  fd_set        *getSelectSet();//������ʱset,����select;���size=0���򷵻�NULL
	int						 getMaxHande();
	void					 reset();
  bool 					 setBit(int handle);
  bool 					 clrBit(int handle);
  bool					 isSet(int handle);
  int  	 				 getReadyHandle(queue<int> &lstHandle);
protected:
	bool					 isValidHandle(int handle);
	fd_set 		 		 m_set;
	fd_set 		 		 m_SelectSet;
	int       		 m_ArrayHandles[SOCKET_MAXSIZE+1];
	int						 m_MaxHande;
	int						 m_size;
};


/**************************************************************************
 * class :Socket_Selector
  * brief :
         1��Socket_Checker���Selectģʽʵ��
**************************************************************************/

class Socket_Selector:public Socket_Checker
{
	public:
		Socket_Selector(){};
		bool  register_Handle(int handle);
		bool  remove_Handle(int handle);
		bool  pop_ReadyHandle(int &handle,int timeout_Millisecond=100);
	private:
		int   										check_io_once(int timeout_Millisecond=0);
		Socket_Set								m_set;		
		queue<int>	 			        m_lstHandles_Ready;
		//����ʹ�û�����,��ֹ����Ҫ�ĵȴ�
		ThreadMutex  						  m_mutex_ready;
		ThreadMutex  						  m_mutex_select;
		ThreadMutex  						  m_mutex_set;
};

/**************************************************************************
 * class :Socket_Epoller
  * brief :
         1��Socket_Checker���epollģʽʵ�֣�epoll��ֻ֧��linux
**************************************************************************/
#ifdef   LINUX
#include <sys/epoll.h>

#define  PER_EPOLL_SIZE 50
class Socket_Epoller:public Socket_Checker
{
	public:
		Socket_Epoller();
		~Socket_Epoller();
		bool  register_Handle(int handle);
		bool  remove_Handle(int handle);
		bool  pop_ReadyHandle(int &handle,int timeout_Millisecond=1000);
	private:
		int   										check_io_once(int timeout_Millisecond=0);
		int												m_Epoll_Handle;		
		queue<int>	 			        m_lstHandles_Ready;
		set <int>                 m_setHandles;
		//����ʹ�û�����,��ֹ����Ҫ�ĵȴ�
		ThreadMutex  						  m_mutex_ready;
		ThreadMutex  						  m_mutex_poll;
};
#endif

//-------------------------------------------------------------------------------


class Socket_Svc_Handler : public Socket
{
public:
	 	/*
	  	�߼������������һ�ν���bufΪ40k(40960)
	  		����:
	   		 0  ok
	  		-1  �ر����ӣ��˳��߳�
	  */
	 virtual int      handle_process()=0;	 //READ_MASK ʱ����
	 virtual int      handle_connected();  //�����ӽ���ʱ����
	 virtual void     handle_close(); 		 //���ӹر�ʱ����
   
   void  lock(){m_mutex.lock();};
	 void  unlock(){m_mutex.unlock();};		 
private:	 
	 ThreadMutex  	 m_mutex;
};

/**************************************************************************
 * class :Socket_Ractor
  * brief :
    1��Ractorͬʱ���������̣߳��ֱ��Ǵ����̡߳�IO����߳�
    2����tcp�����Ӹ߲���������£����ܳ��ֶ˿ڲ������������Ĳ���
    sysctl -w net.ipv4.tcp_timestamps=1
    sysctl -w net.ipv4.tcp_tw_recycle=1
**************************************************************************/

class Socket_Ractor_Imp : public Thread_Base
{
public:
	Socket_Ractor_Imp();
	~Socket_Ractor_Imp();
	bool 	    open(int nPort,int process_num=5);
	int   		stop();
//	bool      wait();
protected:
	int  			run();//�����̺߳���
	static void * accept_client(void* pParam);
	Socket_Svc_Handler 	  				*getHandler(int handle);
	Socket_Svc_Handler 	  				*addHandler(int handle);
	virtual Socket_Checker        *getChecker()=0;
	virtual Socket_Svc_Handler    *newHandler()=0;
private:
	Socket_Checker							  *m_selector;
	Socket_Acceptor						     m_acceptor;
	map<int,Socket_Svc_Handler *>  m_mapHandlers;
	bool													 m_bExit;
	ThreadMutex  	                 m_Handlermutex;
};


template <class Checker_T,class Handler_T>
class Socket_Ractor : public Socket_Ractor_Imp
{
public:
	 Socket_Ractor(){};
protected:
	Socket_Checker        *getChecker() {return &m_selector;};
	Socket_Svc_Handler    *newHandler() {return new Handler_T();};
private:
	Checker_T											m_selector;
};


#endif

