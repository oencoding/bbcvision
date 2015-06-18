#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include "cJSON.h"
#include <map>
#include <deque>

#pragma comment(lib,"Ws2_32.lib")

#define MYMSG_EXIT WM_USER+1000
#define MYMSG_CREATE WM_USER+1002
#define MYMSG_PAUSE WM_USER+1004

const int TSLENTH = 188;
FILE *logfd = NULL;
const int BUFFNUM = 5;

typedef struct _MediaData
{
	int iPacketlen;
	char *cMediaData;
}MediaData;
//����tid
unsigned long CreateUnique()
{
	return GetTickCount();
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
char * replace(char *strbuf, const char *src_str, const char *desc_str)
{

	char *pos, *pos1;

	if (strbuf == NULL || src_str == NULL || desc_str == NULL) return strbuf;

	if (strlen(src_str) == 0) return strbuf;
	int ilen = strlen(strbuf);
	char *org = new char[ilen + 1];

	strcpy_s(org, ilen + 1, strbuf);
	pos = org;
	strbuf[0] = 0;
	while (1)
	{
		pos1 = strstr(pos, src_str);
		if (pos1 == NULL)
			break;
		*pos1 = '\0';
		strcat_s(strbuf, ilen+1, pos);
		strcat_s(strbuf, ilen+1, desc_str);
		pos = pos1 + strlen(src_str);
	}
	strcat_s(strbuf, ilen+1, pos);
	delete org;
	return strbuf;
}

//����PID
void AnalyzeTSPID(short sPID, unsigned int &iPid)
{
	//int old = sPID;
	sPID = sPID << 3;
	iPid = (unsigned int)(sPID >> 3);

}
//����PID
void AnalyzeTSPayload(unsigned char cPayload, bool &bPayload)
{
	bPayload =(bool)(cPayload & 0x40); //ȡ�ڶ�λ����
}

class SendMedia
{
public:
	char m_cFilePath[1024];
	char m_cClientIP[128];
	short m_sClientPort;
	short m_sServerPort;
	bool m_bStop;
	bool m_bPause;
	int m_iTid;
	unsigned m_threadID;
	unsigned m_GetThreadid;

	char TsPack[188] ;
	char PESPack[1024 * 1024];
	char SendPesPacket[1024 * 1024];
	char UDPSendBuff[1500];
	std::deque<MediaData*>m_dequeUseBuff;
	std::deque<MediaData*>m_dequeFreeBuff;
	CRITICAL_SECTION m_csData;
public:
	SendMedia(char* cfile = NULL, char* pIp = NULL, short sClientPort = 10000, short sSPort = 12000);
	~SendMedia();
	void GetMediaData();
	void SendMediaUDP();
	void SetMediaPause(bool bPause = true)
	{
		m_bPause = bPause;
	}
	void setMediaStop(bool bStop = true)
	{
		m_bStop = bStop;
	}

	static unsigned int CALLBACK SendMediaThread(void *param);
	static unsigned int CALLBACK GetMediaThread(void *param);
};

SendMedia::~SendMedia()
{
	m_threadID = 0;
}

SendMedia::SendMedia(char* cfile/* =NULL */, char* pIp/* =NULL */, short sClientPort/* =10000 */, short sSPort/* =12000 */)
{
	strcpy_s(m_cFilePath, 1024, cfile);
	strcpy_s(m_cClientIP, 128, pIp);
	m_sServerPort = sSPort;
	m_sClientPort = sClientPort;
	m_iTid = 0;
	m_bStop = false;
	m_bPause = false;
	m_threadID = 0;
	m_GetThreadid = 0;
	InitializeCriticalSection(&m_csData);
	m_dequeUseBuff.clear();
	int iloop = BUFFNUM;
	while (iloop--)
	{
		MediaData *tmp = new MediaData;
		tmp->iPacketlen = 0;
		char *UseData = new char[1024 * 1024];
		tmp->cMediaData = UseData;
		m_dequeFreeBuff.push_back(tmp);
	}
	printf("---------Free buff size =%d\n", m_dequeFreeBuff.size());

	if (m_cClientIP != NULL && m_cFilePath != NULL)
	{
		//���������߳�
		_beginthreadex(NULL, 0, &GetMediaThread, this, 0, &m_GetThreadid);
		Sleep(50);
		_beginthreadex(NULL, 0, &SendMediaThread, this, 0, &m_threadID);


	}
}

void SendMedia::GetMediaData()
{
	FILE *fp = NULL;

	// 	FILE *fptest = NULL;
	// 	fopen_s(&fptest, "e:\\test.ts", "w+b");

	fopen_s(&fp, m_cFilePath, "r+b");
	if (NULL == fp)
	{
		printf("%s,Read TS file error\n", m_cFilePath);
	}
	int iReadLen = 0;
	//fseek(fp, TSLENTH * 3, SEEK_SET);
	bool bPAT_PID = false;					//��ȡ��PAT��
	bool bPMT_PID = false;					//��ȡ��PMT��
	unsigned int uiPMT_PID = 0;					//PMT���PIDֵ
	unsigned int uiVideo_PID = 0;				//��Ƶ����PIDֵ
	unsigned int uiAudio_PID = 0;				//��Ƶ����PIDֵ
	int iPacketLen = 0;
	unsigned int iLastSendTime = 0;

	int ilastPframe = 0;
	int iSendTime = 0;
	while (true)
	{
		while (m_bPause)
		{
			Sleep(10);
		}
		if (m_bStop)
		{
			break;
		}
		iReadLen = fread(TsPack, 1, TSLENTH, fp);
		if (iReadLen <= 0)
		{
			break;
		}
		//printf("%0x len=%d\n", TsPack[0], iReadLen);
		unsigned short *sPID = (unsigned short*)(&TsPack[1]);
		unsigned short PID = ntohs(*sPID);
		//int *testPID = (int*)(&TsPack[1]);
		//unsigned char *cPayload = (unsigned char*)&TsPack[1];
		unsigned int iPID = 0;
		//bool bPayload = false;
		//AnalyzeTSPayload(*cPayload, bPayload);
		AnalyzeTSPID(PID, iPID);
		//printf("palyload = %d PID=%d\n", bPayload, iPID);
		if (iPID == 0 && TsPack[5] == 0x00 && !bPAT_PID)
		{
			//PAT����ȡ�� ��һ�ֽ�Ϊ0x00 table_id
			unsigned short *usPMTPID = (unsigned short*)(&TsPack[15]);
			AnalyzeTSPID(ntohs(*usPMTPID), uiPMT_PID);//�ҵ�PMT���PIDֵ
			bPAT_PID = true;
		}
		else if (uiPMT_PID == iPID && TsPack[5] == 0x02 && !bPMT_PID)
		{
			//PMT�� ��һ���ֽ�Ϊ0x02 table_id
			//ȡ19��20�ֽ� Ϊ��ƵPID
			unsigned short *usVideoPID = (unsigned short*)(&TsPack[18]);
			AnalyzeTSPID(ntohs(*usVideoPID), uiVideo_PID);//�ҵ���Ƶ��PIDֵ
			//ȡ 24��25�ֽ�Ϊ��ƵPID
			unsigned short *usAudioPID = (unsigned short*)(&TsPack[23]);
			AnalyzeTSPID(ntohs(*usAudioPID), uiAudio_PID);//�ҵ�P��Ƶ��PIDֵ
			bPMT_PID = true;
		}
		else if (bPMT_PID && uiVideo_PID == iPID)
		{
			//��Ƶ����
			bool bPayload = false;
			unsigned char *cPayload = (unsigned char*)&TsPack[1];
			AnalyzeTSPayload(*cPayload, bPayload);
			if (bPayload)
			{
				//int idelay = GetTickCount() - ilastPframe;
				//if ( idelay > 35)
				//	printf("===============over %dms \n",idelay);
				//ilastPframe = GetTickCount();

				//һ֡�Ŀ�ͷ�����һ֡���ݷ��ͳ�ȥ
				//�����ʱ��ͷ��ͳ�ȥ������ȴ�
				while (m_dequeFreeBuff.size() == 0)
					Sleep(2);
				EnterCriticalSection(&m_csData);
				if (m_dequeFreeBuff.size() > 0)
				{
					//�����ݷ���
					MediaData* cData = m_dequeFreeBuff.front();
					memcpy(cData->cMediaData, PESPack, iPacketLen);
					cData->iPacketlen = iPacketLen;
					m_dequeFreeBuff.pop_front();
					m_dequeUseBuff.push_back(cData);
					iPacketLen = 0;
					//LeaveCriticalSection(&m_csData);

				}
				LeaveCriticalSection(&m_csData);
#if 0
				EnterCriticalSection(&m_csData);
				int idesize = 0;
				idesize = m_dequeFreeBuff.size();
				if(idesize > 0)
				{
					//�����ݷ���
					MediaData* cData = m_dequeFreeBuff.front();
					memcpy(cData->cMediaData, PESPack, iPacketLen);
					cData->iPacketlen = iPacketLen;
					m_dequeFreeBuff.pop_front();
					m_dequeUseBuff.push_back(cData);
					iPacketLen = 0;
					LeaveCriticalSection(&m_csData);
				}
				else
				{
					LeaveCriticalSection(&m_csData);
					while (m_dequeFreeBuff.size() == 0)
						Sleep(2);
					EnterCriticalSection(&m_csData);
					if (m_dequeFreeBuff.size() > 0)
					{
						//�����ݷ���
						MediaData* cData = m_dequeFreeBuff.front();
						memcpy(cData->cMediaData, PESPack, iPacketLen);
						cData->iPacketlen = iPacketLen;
						m_dequeFreeBuff.pop_front();
						m_dequeUseBuff.push_back(cData);
						iPacketLen = 0;
						//LeaveCriticalSection(&m_csData);
						
					}
					LeaveCriticalSection(&m_csData);
				}
				//iPacketLen = 0;
#endif
			}
			//memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
			//iPacketLen += TSLENTH;
		}
		else if (bPMT_PID&& uiAudio_PID == iPID)
		{
			//��Ƶ����
			//memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
			//iPacketLen += TSLENTH;
		}
		memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
		iPacketLen += TSLENTH;

	}
	printf("over\n");
	m_GetThreadid = 0;
	fclose(fp);
}

unsigned int CALLBACK SendMedia::GetMediaThread(void *param)
{
	SendMedia* this0 = (SendMedia*)param;
	this0->m_GetThreadid = GetCurrentThreadId();
	this0->GetMediaData();

	return 0;
}

unsigned int CALLBACK SendMedia::SendMediaThread(void *param)
{
	printf(" --1 \n");
	SendMedia* this0 = (SendMedia*)param;
	this0->m_threadID = GetCurrentThreadId();
	//printf(" Udp Thread id =%d \n");
	this0->SendMediaUDP();

	return 0;
}

void SendMedia::SendMediaUDP()
{
	//char cFilePath[1024] = { 0 };
	//char cClientIP[128] = { 0 };
	//short sClientPort = 10000;
	//short sServerPort = 12000;

	//strcpy_s(cFilePath, "E:\\oldfile\\38.ts");

	//strcpy_s(cClientIP, "127.0.0.1");
	printf("create udpThread success\n");
	sockaddr_in sockAddr;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockAddr.sin_port = htons(m_sClientPort);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(m_cClientIP);

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(m_sServerPort);
	serverAddr.sin_addr.s_addr = ADDR_ANY;
	bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

	unsigned int iLastSendTime = 0;
	int iPacketLen = 0;
	int iSendovertime = 0;

	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);

	dfFreq = (double)litmp.QuadPart;// 	��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ

	while (true)
	{
		while (m_bPause)
		{
			Sleep(10);
		}
		if (m_bStop)
		{
			break;
		}
		MediaData *mSendDat = NULL;
		iPacketLen = 0;
		EnterCriticalSection(&m_csData);
		if (m_dequeUseBuff.size() > 0)
		{
			mSendDat = m_dequeUseBuff.front();
			memcpy(SendPesPacket, mSendDat->cMediaData, mSendDat->iPacketlen);
			iPacketLen = mSendDat->iPacketlen;
			m_dequeUseBuff.pop_front();
			m_dequeFreeBuff.push_back(mSendDat);
		}
		LeaveCriticalSection(&m_csData);

		do 
		{
			Sleep(5);
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;
			dfMinus = (double)(QPart2 - QPart1)*1000;
			dfTim = (dfMinus / dfFreq);
			//printf("-----%f\n", dfTim);
			
		} while (dfTim < 39);

		//printf("=======Sendover Time %f\n", dfTim);
		//while (!(GetTickCount() - iLastSendTime >= 32))
		//{
		//	Sleep(1);
		//}
		if (iPacketLen > 0)
		{
			iLastSendTime = GetTickCount();
			//send
			//ÿ�η��ͳ���Ϊ188*7
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// ��ó�ʼֵ

			//printf("getmedia Data len = %d\n", iPacketLen);
			if (iPacketLen > 0)
			{
				int iSendLen = 0;
				int iLen = 0;
				while (iSendLen < iPacketLen)
				{
					if ((iPacketLen - iSendLen) < TSLENTH * 7)
					{
						memcpy(UDPSendBuff, SendPesPacket + iSendLen, iPacketLen - iSendLen);
						iLen = (iPacketLen - iSendLen);

					}
					else
					{
						memcpy(UDPSendBuff, SendPesPacket + iSendLen, TSLENTH * 7);
						iLen = (TSLENTH * 7);
					}
					iSendLen += iLen;
					//��������
					//if (iSendTime++ > 7)
					//{
					//	Sleep(1);
					//	iSendTime = 0;
					//}
					int iret = sendto(sock, UDPSendBuff, iLen, 0, (sockaddr*)&sockAddr, sizeof(sockAddr));
					//fwrite(UDPSendBuff, 1, iLen, fptest);
				}
				//int idelay = GetTickCount() - iSendovertime;
				//if (idelay > 38) 
				//	printf("=======Sendover Time %d\n", idelay);
				//iSendovertime = GetTickCount();
				//printf("buff len = %d \n", iPacketLen);
				iPacketLen = 0;
			}
		}
		else
		{
			printf("===**************==Packet len = 0 no data \n");
		}

	}

	/*
	FILE *fp = NULL;

	// 	FILE *fptest = NULL;
	// 	fopen_s(&fptest, "e:\\test.ts", "w+b");

	fopen_s(&fp, m_cFilePath, "r+b");
	if (NULL == fp)
	{
		printf("%s,Read TS file error\n",m_cFilePath);
	}
	int iReadLen = 0;
	//fseek(fp, TSLENTH * 3, SEEK_SET);
	bool bPAT_PID = false;					//��ȡ��PAT��
	bool bPMT_PID = false;					//��ȡ��PMT��
	unsigned int uiPMT_PID = 0;					//PMT���PIDֵ
	unsigned int uiVideo_PID = 0;				//��Ƶ����PIDֵ
	unsigned int uiAudio_PID = 0;				//��Ƶ����PIDֵ
	int iPacketLen = 0;
	
	
	int ilastPframe = 0;
	int iSendTime = 0;
	while (true)
	{
		while(m_bPause)
		{
			Sleep(10);
		}
		if (m_bStop)
		{
			break;
		}
		iReadLen = fread(TsPack, 1, TSLENTH, fp);
		if (iReadLen <= 0)
		{
			break;
		}
		//printf("%0x len=%d\n", TsPack[0], iReadLen);
		unsigned short *sPID = (unsigned short*)(&TsPack[1]);
		unsigned short PID = ntohs(*sPID);
		//int *testPID = (int*)(&TsPack[1]);
		//unsigned char *cPayload = (unsigned char*)&TsPack[1];
		unsigned int iPID = 0;
		//bool bPayload = false;
		//AnalyzeTSPayload(*cPayload, bPayload);
		AnalyzeTSPID(PID, iPID);
		//printf("palyload = %d PID=%d\n", bPayload, iPID);
		if (iPID == 0 && TsPack[5] == 0x00 && !bPAT_PID)
		{
			//PAT����ȡ�� ��һ�ֽ�Ϊ0x00 table_id
			unsigned short *usPMTPID = (unsigned short*)(&TsPack[15]);
			AnalyzeTSPID(ntohs(*usPMTPID), uiPMT_PID);//�ҵ�PMT���PIDֵ
			bPAT_PID = true;
		}
		else if (uiPMT_PID == iPID && TsPack[5] == 0x02 && !bPMT_PID)
		{
			//PMT�� ��һ���ֽ�Ϊ0x02 table_id
			//ȡ19��20�ֽ� Ϊ��ƵPID
			unsigned short *usVideoPID = (unsigned short*)(&TsPack[18]);
			AnalyzeTSPID(ntohs(*usVideoPID), uiVideo_PID);//�ҵ���Ƶ��PIDֵ
			//ȡ 24��25�ֽ�Ϊ��ƵPID
			unsigned short *usAudioPID = (unsigned short*)(&TsPack[23]);
			AnalyzeTSPID(ntohs(*usAudioPID), uiAudio_PID);//�ҵ�P��Ƶ��PIDֵ
			bPMT_PID = true;
		}
		else if (bPMT_PID && uiVideo_PID == iPID)
		{
			//��Ƶ����
			bool bPayload = false;
			unsigned char *cPayload = (unsigned char*)&TsPack[1];
			AnalyzeTSPayload(*cPayload, bPayload);
			if (bPayload)
			{
				if (GetTickCount() - ilastPframe > 30)
					printf("===============over 30ms \n");
				ilastPframe = GetTickCount();

				//һ֡�Ŀ�ͷ�����һ֡���ݷ��ͳ�ȥ
				//�����ʱ��ͷ��ͳ�ȥ������ȴ�
				while (true)
				{
					if (GetTickCount() - iLastSendTime >= 39)
					{
						iLastSendTime = GetTickCount();
						//send
						//ÿ�η��ͳ���Ϊ188*7
						int iSendLen = 0;
						int iLen = 0;
						while (iSendLen < iPacketLen)
						{
							if ((iPacketLen - iSendLen) < TSLENTH * 7)
							{
								memcpy(UDPSendBuff, PESPack + iSendLen, iPacketLen - iSendLen);
								iLen = (iPacketLen - iSendLen);

							}
							else
							{
								memcpy(UDPSendBuff, PESPack + iSendLen, TSLENTH * 7);
								iLen = (TSLENTH * 7);
							}
							iSendLen += iLen;
							//��������
							//if (iSendTime++ > 7)
							//{
							//	Sleep(1);
							//	iSendTime = 0;
							//}
							int iret = sendto(sock, UDPSendBuff, iLen, 0, (sockaddr*)&sockAddr, sizeof(sockAddr));
							//fwrite(UDPSendBuff, 1, iLen, fptest);
						}
						//printf("buff len = %d \n", iPacketLen);

						iPacketLen = 0;
						break;
					}
					Sleep(2);
				}
			}
			//memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
			//iPacketLen += TSLENTH;
		}
		else if (bPMT_PID&& uiAudio_PID == iPID)
		{
			//��Ƶ����
			//memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
			//iPacketLen += TSLENTH;
		}
		memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
		iPacketLen += TSLENTH;

	}
	printf("over\n");

	fclose(fp);
	*/
	m_threadID = 0;
	closesocket(sock);
}

std::map<unsigned long, SendMedia*> mapTidObject;
unsigned int _stdcall ThreadClient(void* param)
{
	SOCKET sClient = *((SOCKET*)param);
	char cBuffRecv[2048] = { 0 };
	int iRecvLen = 0;
	
	cJSON *pItem = NULL;
	cJSON *pcmd = NULL;
	cJSON *pSerialNo = NULL;
	cJSON *pRet_root = NULL;

	char destIP[256];
	int destPort = 0;
	int iRate = 0;
	char cUrl_ts[1024];
	unsigned ThreadSend = 0;

	sockaddr_in clientaddr;
	int iAddrLen = sizeof(clientaddr);
	getpeername(sClient, (sockaddr*)&clientaddr, &iAddrLen);
	char szPeerAddress[16];

	memset((void *)szPeerAddress, 0, sizeof(szPeerAddress));
	strcpy_s(szPeerAddress, 16,inet_ntoa(clientaddr.sin_addr));
	printf("client %s ready \n", szPeerAddress);
	fprintf(logfd, "client IP is ");
	fwrite(szPeerAddress, 1, strlen(szPeerAddress), logfd);
	fprintf(logfd, "  \n");
	fflush(logfd);
	SendMedia *pSendObject=NULL;

	int iMeidaPause = 0;
	char cJsonBuff[1024 * 2];
	std::string ret_string;
	while (true)
	{
		//iRecvLen = recv(sClient, cBuffRecv, 1500, 0);
		int	i_rc = 0, i_count = 0;
		do
		{
			i_rc = recv(sClient,cBuffRecv + i_count, 2000 - i_count, 0);
			if (i_rc <= 0)break;//�쳣�ر�
			i_count += i_rc;
		} while (strstr(cBuffRecv, "XXEE") == NULL);
		iRecvLen = i_count;
		if (iRecvLen<=0) break;

		fwrite(cBuffRecv, 1, iRecvLen, logfd);
		fprintf(logfd, " \n");
		fflush(logfd);
		//������������
		replace(cBuffRecv, "XXEE", "");
		//test media
		//if (strcmp(cBuffRecv,"1")==0)
		//	pSendObject = new SendMedia("E:\\oldfile\\38.ts", "127.0.0.1");
		//else if (strcmp(cBuffRecv,"2")==0)
		//{
		//	iMeidaPause++;
		//	if (pSendObject)
		//	{
		//		iMeidaPause = iMeidaPause % 2;
		//		pSendObject->SetMediaPause(iMeidaPause);
		//	}
		//}
		//else if (strcmp(cBuffRecv, "3") == 0)
		//{
		//	pSendObject->setMediaStop(true);
		//}
		
		cJSON* pRoot = cJSON_Parse(cBuffRecv);
		
		if (pRoot)
		{
			pSerialNo = cJSON_GetObjectItem(pRoot, "serialno");
			pcmd = cJSON_GetObjectItem(pRoot, "cmd");
			if (pcmd)
			{
				//memset(strCmd, 0, sizeof(strCmd));
				//strcpy_s(strCmd, pcmd->valuestring, strlen(pcmd->valuestring));

				//�ж���������
				if (strcmp(pcmd->valuestring, "streamstart")==0)
				{
					//���󴴽�������
					//��ȡip port ts_url
					cJSON* pIP = cJSON_GetObjectItem(pRoot,"iip");
					memset(destIP, 0, sizeof(destIP));
					if(pIP) memcpy(destIP, pIP->valuestring, strlen(pIP->valuestring));
					cJSON* pPort = cJSON_GetObjectItem(pRoot, "iport");
					if(pPort) destPort = atoi(pPort->valuestring);
					cJSON* pRate = cJSON_GetObjectItem(pRoot, "rate");
					if(pRate) iRate = atoi(pRate->valuestring);
					cJSON* pUrl = cJSON_GetObjectItem(pRoot, "url");
					memset(cUrl_ts, 0, sizeof(cUrl_ts));
					if(pUrl) memcpy(cUrl_ts, pUrl->valuestring, strlen(pUrl->valuestring));

					//�ظ�
					pRet_root = cJSON_CreateObject();
					char * m_tmp;			

					char cTid[128] = { 0 };
					unsigned long ulTid = CreateUnique();
					sprintf_s(cTid, 128, "%d",ulTid);
					//�������������߳� 

					//std::map<unsigned long, SendMedia*>iterator::itFind = mapTidObject.find(ulTid);
					if (mapTidObject.find(ulTid) == mapTidObject.end())
					{
						pSendObject = NULL;
						pSendObject = new SendMedia(cUrl_ts, destIP, destPort, destPort + 1);
						//����map
						mapTidObject.insert(std::make_pair(ulTid, pSendObject));
						cJSON_AddStringToObject(pRet_root, "retcode", "0");
					}
					else
					{
						cJSON_AddStringToObject(pRet_root, "retcode", "-1");
					}
					cJSON_AddStringToObject(pRet_root, "cmd", "streamcontrol");
					
					cJSON_AddStringToObject(pRet_root, "tid", cTid); //
					cJSON_AddStringToObject(pRet_root, "rate", "37500"); //д���ˣ�û�����޸�

					//cJSON_AddItemToObject(pRet_root, "cmd", cJSON_CreateString("streamcontrol"));
					//cJSON_AddItemToObject(pRet_root, "retcode", cJSON_CreateString("0"));
					//cJSON_AddItemToObject(pRet_root, "tid", cJSON_CreateString("898899"));
					//cJSON_AddItemToObject(pRet_root, "rate", cJSON_CreateString("37500"));
					// 					cJSON_AddItemToObject(pRet_root, "cmd", pRoot);
					// 					m_tmp = cJSON_Print(pRet_root);
					cJSON_AddItemToObject(pRet_root, "serialno", pSerialNo);
					m_tmp = cJSON_Print(pRet_root);

					memset(cJsonBuff, 0, sizeof(cJsonBuff));
					sprintf_s(cJsonBuff, 1024 * 2, "%sXXEE", m_tmp);
					free(m_tmp);
					//ret_string = cJsonBuff;
					cJSON_Delete(pRet_root);
					send(sClient, cJsonBuff, strlen(cJsonBuff), 0);

					fwrite(cJsonBuff, 1, strlen(cJsonBuff), logfd);
					fprintf(logfd, " \n");
					fflush(logfd);
					//if (pSendObject != NULL)
					//{
					//	pSendObject->setMediaStop(true);
					//	delete pSendObject;
					//	pSendObject = NULL;
					//}
					//pSendObject = new SendMedia(cUrl_ts, destIP, destPort, destPort + 2);
				
				}
				else if (strcmp(pcmd->valuestring, "streamstop")==0)
				{
					//����ɾ��������
					cJSON* pIP = cJSON_GetObjectItem(pRoot, "iip");
					memset(destIP, 0, sizeof(destIP));
					if(pIP) memcpy(destIP, pIP->valuestring, strlen(pIP->valuestring));
					cJSON* pPort = cJSON_GetObjectItem(pRoot, "iport");
					if(pPort) destPort = atoi(pPort->valuestring);
					cJSON* pTid = cJSON_GetObjectItem(pRoot, "tid");
					unsigned long ulTid = 0;
					if(pTid)  ulTid = atoi(pTid->valuestring);
				
					//�ظ�
					pRet_root = cJSON_CreateObject();

					std::map<unsigned long, SendMedia*>::iterator itFind = mapTidObject.find(ulTid);
					if (itFind != mapTidObject.end())
					{
						pSendObject = NULL;
						pSendObject = itFind->second;

						if (pSendObject)
						{
							while (pSendObject->m_threadID != 0 || pSendObject->m_GetThreadid != 0)
							{
								pSendObject->setMediaStop(true);
								Sleep(2);
							}
							delete pSendObject;
							pSendObject = NULL;
						}
						cJSON_AddStringToObject(pRet_root, "retcode", "0");
					}
					else
					{
						cJSON_AddStringToObject(pRet_root, "retcode", "-1");
					}
					cJSON_AddStringToObject(pRet_root, "cmd", "streamstop");
					cJSON_AddItemToObject(pRet_root, "serialno", pSerialNo);
					char * m_tmp;
					m_tmp = cJSON_Print(pRet_root);
					memset(cJsonBuff, 0, sizeof(cJsonBuff));
					sprintf_s(cJsonBuff, 1024 * 2, "%sXXEE", m_tmp);
					free(m_tmp);
					//ret_string = cJsonBuff;
					cJSON_Delete(pRet_root);

					send(sClient, cJsonBuff, strlen(cJsonBuff), 0);
					fwrite(cJsonBuff, 1, strlen(cJsonBuff), logfd);
					fprintf(logfd, " \n");
					fflush(logfd);

					//break;
				}
				else if (strcmp(pcmd->valuestring, "streampause")==0)
				{
					//������ͣ������
					cJSON* pIP = cJSON_GetObjectItem(pRoot, "iip");
					memset(destIP, 0, sizeof(destIP));
					if(pIP) memcpy(destIP, pIP->valuestring, strlen(pIP->valuestring));
					cJSON* pPort = cJSON_GetObjectItem(pRoot, "iport");
					if(pPort) destPort = atoi(pPort->valuestring);
					cJSON* pTid = cJSON_GetObjectItem(pRoot, "tid");
					unsigned long ulTid = 0;
					if(pTid) ulTid = atoi(pTid->valuestring);

					//�ظ�
					pRet_root = cJSON_CreateObject();

					//iMeidaPause++;
					std::map<unsigned long, SendMedia*>::iterator itFind = mapTidObject.find(ulTid);
					if (itFind != mapTidObject.end())
					{
						pSendObject = NULL;
						pSendObject = itFind->second;

						if (pSendObject)
						{
							pSendObject->SetMediaPause(true/*iMeidaPause*/);
						}
						cJSON_AddStringToObject(pRet_root, "retcode", "0");
					}
					else
					{
						cJSON_AddStringToObject(pRet_root, "retcode", "-1");
					}
					cJSON_AddStringToObject(pRet_root, "cmd", "streampause");
				
					cJSON_AddItemToObject(pRet_root, "serialno", pSerialNo);
					char * m_tmp;
					m_tmp = cJSON_Print(pRet_root);
					memset(cJsonBuff, 0, sizeof(cJsonBuff));
					sprintf_s(cJsonBuff, 1024 * 2, "%sXXEE", m_tmp);
					free(m_tmp);
					ret_string = cJsonBuff;
					cJSON_Delete(pRet_root);
					send(sClient, cJsonBuff, strlen(cJsonBuff), 0);
					fwrite(cJsonBuff, 1, strlen(cJsonBuff), logfd);
					fprintf(logfd, " \n");
					fflush(logfd);
					//if (pSendObject)
					//{
					//	//iMeidaPause = iMeidaPause % 2;
					//	pSendObject->SetMediaPause(true/*iMeidaPause*/);
					//}
				}
				else if (strcmp(pcmd->valuestring, "streamresume") == 0)
				{
					//����ָ�������
					cJSON* pIP = cJSON_GetObjectItem(pRoot, "iip");
					memset(destIP, 0, sizeof(destIP));
					if (pIP) memcpy(destIP, pIP->valuestring, strlen(pIP->valuestring));
					cJSON* pPort = cJSON_GetObjectItem(pRoot, "iport");
					if (pPort) destPort = atoi(pPort->valuestring);
					cJSON* pTid = cJSON_GetObjectItem(pRoot, "tid");
					unsigned long ulTid = 0;
					if (pTid) ulTid = atoi(pTid->valuestring);
					//�ظ�
					pRet_root = cJSON_CreateObject();

					std::map<unsigned long, SendMedia*>::iterator itFind = mapTidObject.find(ulTid);
					if (itFind != mapTidObject.end())
					{
						pSendObject = NULL;
						pSendObject = itFind->second;

						if (pSendObject)
						{
							pSendObject->SetMediaPause(false/*iMeidaPause*/);
						}
						cJSON_AddStringToObject(pRet_root, "retcode", "0");
					}
					else
					{
						cJSON_AddStringToObject(pRet_root, "retcode", "-1");
					}

					cJSON_AddStringToObject(pRet_root, "cmd", "streamresume");
					
					cJSON_AddItemToObject(pRet_root, "serialno", pSerialNo);
					char * m_tmp;
					m_tmp = cJSON_Print(pRet_root);
					memset(cJsonBuff, 0, sizeof(cJsonBuff));
					sprintf_s(cJsonBuff, 1024 * 2, "%sXXEE", m_tmp);
					free(m_tmp);
					ret_string = cJsonBuff;
					cJSON_Delete(pRet_root);
					send(sClient, cJsonBuff, strlen(cJsonBuff), 0);
					fwrite(cJsonBuff, 1, strlen(cJsonBuff), logfd);
					fprintf(logfd, " \n");
					fflush(logfd);
					//if (pSendObject)
					//{
					//	pSendObject->SetMediaPause(false);
					//}

					//iMeidaPause++;
					//if (pSendObject)
					//{
					//	iMeidaPause = iMeidaPause % 2;
					//	pSendObject->SetMediaPause(iMeidaPause);
					//}
				}
	
				//pRet_root = cJSON_CreateObject();

			}
		}
	}
	std::map<unsigned long, SendMedia*>::iterator itFind = mapTidObject.begin();
	while (mapTidObject.size() > 0)
	{
		pSendObject = itFind->second;
		if (pSendObject)
		{
			while (pSendObject->m_threadID != 0)
			{
				pSendObject->setMediaStop(true);
				Sleep(2);
			}
			delete pSendObject;
			pSendObject = NULL;
		}
		mapTidObject.erase(itFind);
		itFind = mapTidObject.begin();
	}
	closesocket(sClient);
	printf("one client quit!\n");
	return 0;
}

// ����3 �������� �ֱ�Ϊfilepath clientIP clientPort
int main(int argc, char *argv[])
{


#if 0
	if (argc != 4)
	{
		printf("must input 4 paramters!\n");
		return -1;
	}

	strcpy_s(cFilePath,argv[1]);

	strcpy_s(cClientIP, argv[2]);

	sClientPort = atoi(argv[3]);

	printf("*******************************\n");
	printf("FilePath=%s\n", cFilePath);
	printf("ClientIP=%s\n", cClientIP);
	printf("ClientPort=%d\n", sClientPort);
	printf("*******************************\n");
#endif

	WSADATA wsaData;
	int Ret;
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup Failed \n");
		return -2;
	}

	short sTCPServerPort = 6869;
	SOCKET sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in TCPaddr;
	TCPaddr.sin_family = AF_INET;
	TCPaddr.sin_port = htons(sTCPServerPort);
	TCPaddr.sin_addr.s_addr = ADDR_ANY;
	int ret = 0;
	ret = bind(sockTCP, (sockaddr*)&TCPaddr, sizeof(TCPaddr));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sockTCP); //�ر��׽���  
		WSACleanup();
		//return 0;  
	}
	listen(sockTCP, 5);
	printf("Waiting for client connecting!\n");
	sockaddr_in clientAddr;
	int iAddrLen = sizeof(clientAddr);
	
	fopen_s(&logfd, "D:\\logTs.txt", "w+");
	while (true)
	{
		SOCKET socktmp = accept(sockTCP, (sockaddr*)&clientAddr, &iAddrLen);
		if (socktmp == INVALID_SOCKET)
		{
			printf("accept() faild! code:%d\n", WSAGetLastError());
			closesocket(sockTCP); //�ر��׽���  
			WSACleanup();
			return 0;
		}
		printf("one client connect \n");
		//�������߳���socketͨ��
		unsigned Threadid = 0;
		HANDLE hdthread = (HANDLE)_beginthreadex(NULL, 0, &ThreadClient, &socktmp, 0, &Threadid);
		//WaitForSingleObject(hdthread,INFINITE);
	}

	//getchar();
	return 0;
}