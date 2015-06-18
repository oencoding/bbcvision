#ifndef _UTIL_TIMER_H
#define _UTIL_TIMER_H

#include "Thread.h"
#include "Singleton.h"
#include "DateTime.h"
#include <list>

//��ʱ����Ļ��࣬Timer�ڶ�ʱ����ʱ����handle_timeout��������ʣ�����=0ʱ������close����
//����Ĭ�ϵ�close������delete�Լ��������ϣ����Timerɾ����������븲��close����
class TaskTimer_Handler
{
public:
	 virtual int      Timer_timeout()=0;
	 virtual void     Timer_close(){delete this;};
};


class Timer_Handler
{
public:
	 Timer_Handler(TaskTimer_Handler * handler,
	 							 DateTime				    &expire_date,
	 							 int								 count,
	 							 int								 interval_second,
	 							 int								 id):
	 							 m_Handler(handler),
	 							 mExpire_Date(expire_date),
	 							 mLeftCount(count),
	 							 mInterval(interval_second),
	 							 mTimerId(id){};
public:
	 TaskTimer_Handler *m_Handler;
	 DateTime						mExpire_Date;
	 int								mLeftCount;
	 int								mInterval;
	 int								mTimerId;
private:
	 Timer_Handler(){};
};


class  TaskTimer : public Thread_Base
{
public:
	 TaskTimer();
	 ~TaskTimer();
	 //��expire_dateʱ�俪ʼִ�У���ִ��count�Σ�ÿ��֮����interval��
	 //����ֵ�� >=0.��ȷ���أ�Ϊ���������ID,������ȡ��
	 //					<0  ʧ��           
	 int    registerTimer(TaskTimer_Handler *handler,
	 										DateTime				  &expire_date,
	 										int								 count=1,
	 										int								 interval_second=0);
	 void   removeTimer (int timer_id);
	 int    stop() {m_bExit=true;}; 
protected:
	 int   run();
private:
	 int   process_timeout(int &nextsecond);
	 void	 pushTimer(Timer_Handler *th);
	 list<Timer_Handler *> m_lstHanders;
 	 bool  						m_bExit;
 	 int   						m_maxTimerId;
 	 ThreadCondition  m_condTimer;
};

//���嵥��Timer
typedef Singleton<TaskTimer>  TaskTimer_Singleton;

#endif

