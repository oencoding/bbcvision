#ifndef		_RATE_SOCK_H_
#define		_RATE_SOCK_H_

/**************************************************************************
 * \file    socket.h
 * \brief   
 *
 * Copyright (c) .
 * RCS: $Id: rate_sock.h,v 1.9 2008/08/06 10:39:17 leiq Exp $
 *
 * History
 *  2006/10/11 leiq     created
**************************************************************************/


#ifdef WINDOW_SYS
    #include <windows.h>
    #include <winbase.h>
#else
    #include	<netdb.h>
    #include	<unistd.h>
    #include	<arpa/inet.h>
    #include	<sys/socket.h>
	#include	<fcntl.h>
#endif
#include <string>
#include <iostream>
#include <queue>

#include "Thread.h"

using namespace std;

#define  BACKLOG 128 //listen backlog

/**************************************************************************
 * class :Socket
  * brief :
         1��������socket����
         2��ע��Socket���ṩ������װ���������κδ���������������£��������������ر�����
**************************************************************************/
class Socket
{
public:
	Socket():m_nSock(-1){};
	Socket( int nSock ) { setHandle( nSock );};
	Socket(Socket &sock){ setHandle(sock.getHandle());};
	~Socket(){};
public:
	void		setHandle(int nSock){m_nSock=nSock;};
	int			getHandle(){	return m_nSock;};
	//���ض�ȡ�ֽ�����������ʱ�����ض�ȡ�ֽ���������ʱ����-1���������ر�����
	int	    read(char* pBuf, int p_iMax_Len,int p_iMin_Len=0,int usec_time_out=5);
	int	    write( const char* pBuf, int nLen,int timeout_sec=30);

	//���ػ������ɶ����ֽ���,Ч�ʱȽϲ�������Ƶ�ȵ���
	int     readyLength();
	void 		close();

	bool    is_Valid(){return m_nSock>0;};
	bool    is_Bad();
	bool    get_peer(string &host,int &port);
	
	
	Socket  &operator = (Socket &sock)
	{
		if(this != &sock)
			setHandle(sock.getHandle());
		return *this;
	};
	
protected:
	int	    m_nSock;
};

/**************************************************************************
 * class :Socket_Connector
  * brief :
         1���ͻ���������,Ĭ������SO_KEEPALIVEΪtrue
**************************************************************************/
class Socket_Connector
{
public:
  Socket_Connector(){};
  static bool   connect(Socket &new_sock,const char* host,int nPort);
};


/**************************************************************************
 * class :Socket_Acceptor
  * brief :
     1�����ӽ�����,���timeout_sec=0��Ϊ����ģʽ,���ڱȽϼ򵥵ĳ���
**************************************************************************/
class Socket_Acceptor
{
public:
	Socket_Acceptor():m_nSock(-1) {};
	bool 	    open(int nPort); 
	int				getHandle(){ return m_nSock;};
	bool		  accept(Socket &new_sock,int timeout_sec=0);
	void 			close();
private:
	int	    	m_nSock;
};

/*
**************************************************************************
 * class :Socket_Server_TPC
  * brief :
     1����Ϊ�򵥵�socket serverģʽ��Thread per Connecttion
**************************************************************************
*/
class Socket_Server_TPC: public Thread_Base
{
public:
	Socket_Server_TPC();
protected:
	/*
	  �߼������������һ�ν���bufΪ40k(40960)
	  ����:
	   0  ok
	  -1  �ر����ӣ��˳��߳�
	   1  ������ȡ����,�����data 
	*/
	virtual int      process(string &data,string &return_date)=0;
public:
	int     				 stop() {m_bExit=true;};
	void 						 setPort(int port){m_Port=port;};
protected:
	int							 run();
  virtual int      worker(int sock_id);
private:
	int              accept();
	int 						 m_Port;
	bool             m_bExit;
	bool						 m_bListened;
	queue<int>       m_queueSock;
};


#endif

