/****************************************************************************
** ** Program: This file is part of the Util                 
** Author:  leiqiang  
** Date  :  2006-02-01
**
** RCS: $Id: sss.h,v 1.1.1.1 2006/09/25 01:28:12 sss Exp $
**
****************************************************************************/


#ifndef Util_Thread_H
#define Util_Thread_H

#include <pthread.h>
#include <list>
#include <iostream>
#include "Pub_c.h"


class ThreadMutex
{
public:
    ThreadMutex()    { pthread_mutex_init(&m_mutex, 0); };
    ~ThreadMutex()  { pthread_mutex_destroy(&m_mutex); };
    void lock()     { pthread_mutex_lock(&m_mutex); };
    void unlock()   { pthread_mutex_unlock(&m_mutex); };
    int try_lock()   { return pthread_mutex_trylock(&m_mutex); };
private:
    pthread_mutex_t m_mutex;
};


class ThreadLocker
{
public:
    ThreadLocker(ThreadMutex& mutex) : m_mutex(mutex)
    {
        m_mutex.lock();
    };    
    ~ThreadLocker() { m_mutex.unlock(); };
private:
    ThreadMutex& m_mutex;
};



class ThreadCondition
{
public:
	ThreadCondition();
	~ThreadCondition();
	void signal();
	void broadcast();	
	bool wait();
	bool wait(int ms);
private:
	pthread_cond_t m_event;
	pthread_mutex_t m_mutex;
	bool  m_bSignaled;
};

/*
   Thread ������������֧������ģʽ���̻߳��࣬�ʺ���ͬ�߼��Ķ��̲߳�����
   1�����߳���Ϊ1ʱ����ͬ�ڴ�ͳ�ļ�Thread��װ,�����߳�ģʽ����detachģʽ 
   2������ͳһ�������ǣ��߳��˳�����run��������ģʽ����stop�������     
*/
class Thread_Base  
{
public:
	Thread_Base();
	virtual ~Thread_Base();
protected:
	//��Ҫ����run/stop����
	virtual int  run() =0;
	virtual bool initialize(pthread_t id);  
	virtual void uninitialize(pthread_t id);
	
public:
  bool 		start(int num=1,pthread_t * thread_ids = NULL);
	virtual int  stop() {return 0;};  
  virtual bool wait();
	int     count()
	{
		return m_ThreadLists.size();
	};
	
  void   setName(const char *p_name)
  {
  	if(NULL!=p_name) 	
  		strncpy(m_sName,p_name,190);
  };
  
  char  *getName()
  {
  	 return m_sName;
  };
protected:
	ThreadMutex  						 m_mutex;
	std::list<pthread_t>     m_ThreadLists;
private:
	static void * 		       startThread(void* pParam);
	ThreadCondition		       m_cond;
	char 										 m_sName[200];
};

#endif 
