

#ifndef WINDOWS_SYS
#include <dirent.h>
#endif

#include "Pub_c.h"

namespace Pubc
{

/*!
* \brief  ��ȡȥ����չ�����ļ�����
*         
* \param 
*      pFileName�� �ļ�ȫ��
*      pRes     :  ȥ����չ�����ļ�����(���)
* \return
*      pRes
*/
char* getHeadName(const char * pFileName,char * pRes)
{
  int id;
  strcpy(pRes,pFileName);
  id=(int)strlen(pRes)-1;
  if(id>0)
  	{
  		while(id>=0&&pRes[id]!='.'&&pRes[id]!='/'&&pRes[id]!='\\') id--;
  		if(pRes[id]=='.')
    	pRes[id]=0;
  	}
  return pRes;
}

char* getHeadName(const char * pFileName)
{
	static char local_str[512];
	return  getHeadName(pFileName,local_str);
}



/*!
* \brief  ��ȡ�ļ���չ��
*         
* \param 
*      pFileName�� �ļ�ȫ��
*      pRes     :  ��չ��(���)
* \return
*      1:  SUCCESS
*      0:  FAILURE
*/
char * getExtName(const char * pFileName,char * pRes)
{
	int id;
  id=(int)strlen(pFileName)-1;
  if(id>=0)
  {
    while(id>=0&&pFileName[id]!='.'&&pFileName[id]!='/'&&pFileName[id]!='\\') id--;
  	if(pFileName[id]=='.')
  	{
    	strcpy(pRes,pFileName+id+1);
  	};
  }
  return pRes;
}

char * getExtName(const char * pFileName)
{
	static char local_str[512];
  return  getExtName(pFileName,local_str);
}

/*!
* \brief  �õ�������·�����ļ���
*         
* \param 
*      pFullName�� ȫ·���ļ���
*      pRes     :  ������·�����ļ���(���)
* \return
*      1:  SUCCESS
*      0:  FAILURE
*/
char * getShortName(const char * pFullName,char * pRes)
{
  int id;
  id=(int)strlen(pFullName)-1;
  while(id>=0&&pFullName[id]!='/'&&pFullName[id]!='\\') id--;
  if(pFullName[id]=='/' || pFullName[id]=='\\')
  	 strcpy(pRes,pFullName+id+1);
  else
  	 strcpy(pRes,pFullName);
  
  return pRes;
}

char * getShortName(const char * pFullName)
{
	static char local_str[512];
  return  getShortName(pFullName,local_str);
}




/*!
* \brief  �ж��ļ��Ƿ����
*         
* \param 
*      pFileName�� �ļ�ȫ��
*      
* \return
*      1:  ����
*      0:  ������
*/
bool isFileExist(const char * pFileName)
{
FILE  *fp;

  if((fp=fopen(pFileName,"rb"))!=NULL)
  {
    fclose(fp);
    return true;
  }
  else
    return false;
}



/*!
* \brief  ���ݾ��ȡ�ļ�����
*         
* \param 
*      handle   :  �ļ����
*      
* \return
*      >0:  �ļ�����
*      -1:  ʧ��
*/
int getFileSize(int handle)
{
struct stat statbuf;

  if(fstat(handle,&statbuf)!=0)
    return -1;
  else
    return statbuf.st_size;
}


/*!
* \brief  ���ݾ��ȡ�ļ�����
*         
* \param 
*      pFileName   :  �ļ�����
*      
* \return
*      >0:  �ļ�����
*      -1:  ʧ��
*/
int getFileSize(const char * pFileName)
{
struct stat statbuf;

  if(stat(pFileName,&statbuf)!=0)
    return -1;
  else
    return statbuf.st_size;
}


/*!
* \brief  �ж�·���Ƿ����
*         
* \param 
*      path    : ·����
*      
* \return
*      1:  ����
*      0:  ������
*/
#ifdef WINDOWS_SYS
#include <io.h>
#endif

bool isPathExist(const char * pPath)
{
#ifdef WINDOWS_SYS
  if((access( pPath, 0 )) != -1 )
      return true;
    return false;
#else
DIR * dirp;

  dirp=opendir(pPath);
  if(dirp==NULL)
  {
    return false;
  };
  closedir(dirp);
  return true;
#endif
}



// create a directory
/*!
* \brief  ����Ŀ¼
*         
* \param 
*      path    : Ŀ¼��,֧�ֶ༶Ŀ¼
*      
* \return
*      1:  �ɹ�
*      0:  ʧ��
*/
/*bool createDir(const char path[]) 
{
    char strRDir[256];
    mode_t mMode = 0777;

    if (isPathExist(path))
    {
        return true;
    }
    else
    {
        if(mkdir(path,mMode) == 0)
        {
            return true;
        }
        else
        {
            memset(strRDir,0,sizeof(strRDir));
            getShortName(path,strRDir);
            if(strcmp(strRDir,"/") == 0)
                return false;
            if(createDir(strRDir))
                return createDir(path);
            else
                return false;
        }
    }
}
*/
//-------------------------------------------------------------------------------------------

/*
�������ܣ����ַ�ת��Ϊ��д
���������
  src ��ת���ַ���
*/
char * upper(char * src)
{
  if(src==NULL) return src;
  char  *pos=src;
  while(*pos!=0)
  {
    if(*pos>='a'&&*pos<='z')
      *pos=(char)(*pos-'a'+'A');
    pos++;
  }
  return src;
}

/*
�������ܣ����ַ�ת��ΪСд
���������
  src ��ת���ַ���
*/
char * lower(char * src)
{
  if(src==NULL) return src;
  char  *pos=src;
  while(*pos!=0)
  {
    if(*pos>='A'&&*pos<='Z')
      *pos=(char)(*pos-'A'+'a');
    pos++;
  }
  return src;
}

char * firstUpper(char * p_str)
{
		lower(p_str);
    int len = strlen( p_str);
    bool Pre_is_Letter = false;
    for (int i=0; i < len ;i++ )
    {
        if (p_str[i]>='a' && p_str[i]<='z' )
        	{
        		if (Pre_is_Letter == false)
        		{
        			 p_str[i]=p_str[i]+'A' - 'a';
        			 Pre_is_Letter == true;
        		}
        	}
        else
        	{
        		 Pre_is_Letter == false;
        	}
    }    
    return p_str;
}

char * trimLeft(char * psrc)
{
	char * src,*tag;
  if(psrc==NULL) return psrc;
  tag=src=psrc;
  while(*src==' '||*src==0x09||*src==0x0a||*src==0x0d)  src++;
  if(tag!=src)
  {
  	while(*src!=0) *tag++=*src++;
  	*tag=0;
  }	
  return psrc;
}

char * trimRight(char * src)
{
  char *pos;

  if(src==NULL) return src;
  pos=src+strlen(src) -1 ;
  while(pos>=src)
  {
    if(*pos==' '||*pos==0x09||*pos==0x0a||*pos==0x0d)
    	*pos=0;
    else
    		break;
    pos--;
  };
  return src;
}

char * trim (char * src)
{
  trimLeft(src);
  trimRight(src);
  return src;
}

/*!
* \brief  ģ��ORACLE��like
*           ��ǰ֧��%���ַ�ͨ���,'_'���ַ�ͨ�����
*           '%'��ʾ��'\%','\'��ʾ��'\\','_'��ʾ��'\_'
*         
* \param 
*      src_str  :Դ�ַ���
*      pattern  :Ҫƥ��ĸ�ʽ
*      
* \return
*      1 :ƥ��
*      0 :��ƥ��
*     
*/
inline bool  likeHead(const char *src_str,const char*pattern)
{
    char *str=(char *)src_str;
    char *pa_str=(char *)pattern;
    char ch=0;
    while(*pa_str!='\0'&&*str!='\0')
    {
        ch=*pa_str++;
        switch(ch)
        {
            case '_':
                 str++;
                 break;
            case '\\':
               ch=*pa_str++;
            default :
               if(*str++!=ch)
             return false;
         break ;
        }
    }
    //�����ֶν���
    if(*pa_str!='\0') return false;
    return true;
    
}

bool  like(const char *src_str,const char*pattern)
{
    char pa_buf[1024];
    char *str=(char *)src_str;;
    char *pa_str=(char *)pattern;;
    char  ch=0;
    unsigned int   pa_len=0;

    // 1234%
    while(*pa_str!='\0'&&*str!='\0')
    {
        ch=*pa_str++;
        if(ch=='%') break;
        if(ch=='_') 
        {
        *str++;
        continue;
        }
        if(ch=='\\') ch=*pa_str++;
        if(*str++!=ch) return false;
    }
    
    //%......    
    while(*pa_str!='\0'&&*str!='\0')
    {
        pa_len=0;
        while(*pa_str!='\0'&&*pa_str!='%')
            {
                if(*pa_str=='\\') pa_str++;
                pa_buf[pa_len++]=*pa_str++;
            }   
        pa_buf[pa_len]=0;
    
        if(strlen(str) < pa_len ) return false;
        //%12111
        if(*pa_str=='\0')
        {
            str=str+strlen(str)- pa_len;
            return likeHead(str,pa_buf);
        }   
        //%121313%
        while(*str!='\0'&&pa_len >0)
        {
            if(strlen(str)< pa_len) return false;
            if(likeHead(str,pa_buf)) break;
             str++;
        }   
        //���ҵ�121313
        str+=pa_len;
        pa_str++;
    }
        
    return true;
}


/*!
* \brief  ���ַ����е�һ��������һ���滻,ע��strbuf�������㹻�Ŀռ����洢�滻��Ĵ�,��������ڴ����
*         
* \param 
*      strbuf : �ַ���(����/���)
*      src_str : Ŀ���ַ��Ӵ�
*      desc_str     : �滻����Ӵ�
* \return
*      �滻����ַ���
*     
*/
char * replace(char *strbuf,const char *src_str,const char *desc_str)
{
    
    char *pos,*pos1;
    
    if (strbuf==NULL||src_str==NULL||desc_str==NULL) return strbuf;
    
    if(strlen(src_str)==0) return strbuf;
    char *org=new char[strlen(strbuf)+1];
       
    strcpy(org,strbuf);
    pos=org;
    strbuf[0]=0;
    while(1)
    {
        pos1=strstr(pos,src_str);
        if(pos1==NULL)
        break;
        *pos1='\0';
        strcat(strbuf,pos);
        strcat(strbuf,desc_str);
        pos=pos1+strlen(src_str);       
    }
    strcat(strbuf,pos);
    delete org;
    return strbuf;
}


//--------------------------------------------------------------------------------------------
void uSleep(int num)
{
  struct timeval tm;

  tm.tv_sec = 0;
  if(num<=0) num=5;
  tm.tv_usec = num*1000;
  select(0, NULL, NULL, NULL, &tm);
};

//--------------------------------------------------------------------------------------------
void * loadDllFunc(const char *pLib,const char *pModule)
{
/*	 void * m_pHandle=NULL;
	 if(!pLib || !pModule) return NULL;
	 m_pHandle=dlopen(pLib,RTLD_LAZY);
	 if(NULL!=m_pHandle)
	   return (void *)dlsym(m_pHandle,pModule);
*/	 return NULL;	 
}
//--------------------------------------------------------------------------------------------
char * moneyToCapital(int amount,char * res_str)
{
char  upperdigital[][3]={"��","Ҽ","��","��","��","��","½","��","��","��"};
char  units[][3]={"��","��","Ԫ","ʰ","��","Ǫ","��","ʰ","��","Ǫ","��","ʰ","��","Ǫ"};
char  samount[32],upperres_str[64];
int   flag=0, pos;
static char loc_res_str[200];

  if(res_str==NULL) res_str=loc_res_str;

  memset(upperres_str,'\0',64);
  sprintf(samount,"%d.%02d",amount/100,amount%100);

  for(int digitalid=(int)strlen(samount)-1;digitalid>=0;digitalid--)
  {
    if(samount[(int)strlen(samount)- digitalid - 1]!='.')
    {
      pos=digitalid>2?digitalid-1:digitalid;
      if(samount[(int)strlen(samount)- digitalid - 1]!='0')
      {
        strcat(upperres_str,upperdigital[int(samount[(int)strlen(samount) - digitalid - 1]-'0')]);
        strcat(upperres_str,units[pos]);
        flag=1;
      }
      else
      {
        switch(pos)
        {
          case 10:/*�ڵ����⴦��*/
            if(flag)
            {
              if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)==0)/*ȥ����ǰ����*/
                upperres_str[(int)strlen(upperres_str)-2]='\0';
              strcat(upperres_str,"����");
            }
            break;
          case 6:/*������⴦��*/
            if(flag)
            {
              if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)==0)/*ȥ����ǰ����*/
                upperres_str[(int)strlen(upperres_str)-2]='\0';
              /*���ǰ�����ڣ������*/
              if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)==0)/*���û����λ������*/
                strcat(upperres_str,"��");
              else
                strcat(upperres_str,"����");
            }
            break;
          case 2:/*Ԫ�����⴦��*/
            if(flag)
            {
              if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)==0)/*ȥ��Ԫǰ����*/
                upperres_str[(int)strlen(upperres_str)-2]='\0';
                strcat(upperres_str,"Ԫ��");
            }
            else
                strcat(upperres_str,"��Ԫ");
            break;
          default:
            if(flag)
              if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)!=0)
                strcat(upperres_str,"��");
          break;
        }
      }
    }
  }
  /*���ᴦ��*/
  if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)==0)
    upperres_str[(int)strlen(upperres_str)-2]='\0';
  if(memcmp(upperres_str+(int)strlen(upperres_str)-2,"��",2)!=0)
    strcat(upperres_str,"��");
 
  strcpy(res_str,upperres_str);
  return res_str;
};

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
}
