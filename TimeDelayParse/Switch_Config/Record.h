#ifndef	_RECORD_H_
#define	_RECORD_H_


using namespace std;

#include <string>
#include <vector>
#include <iostream>
#include "Pub_c.h"


/**************************************************************************
 * class :Record
 * brief :
         1���ṩ��¼�Ĵ洢����ҵ���޹�
**************************************************************************/
class Record
{
public:
    Record(){};
    Record(int num){resize(num);};
    Record(const Record&rec){m_listValues=rec.m_listValues;};
    virtual ~Record(){ clear(); };
public:
		bool                set(int index,string &value);
		bool                set(int index,const char* szValue);
		bool                get(int index,string &value) const;
		bool                get(int index,int    &value) const;
		bool								isvalid_addr(string &value) const;
		bool                issHexNumber(const char *pBuf);
public:
    void                clear(){ m_listValues.clear();};
    int                 size() const {return m_listValues.size(); };
    void                resize(int size){m_listValues.resize(size);};
    /*!
    * \brief: ��ָ���ָ�������ַ���
    * \param: 
    *      str      : �ַ���
    *      strflag  : �ָ��
    * \return:
    *      true     -success
    *      false    �Cfailed
    */
    bool                from_String(const char *str,const char* strflag);
    string							to_string();
public:    
    Record&             operator=(const Record&rec);
    vector<string>      m_listValues;
};
    


#endif //_UP_DR_H_

