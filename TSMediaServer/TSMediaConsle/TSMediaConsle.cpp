#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <process.h>

#pragma comment(lib,"Ws2_32.lib")

const int TSLENTH = 188;


//����PID
void AnalyzeTSPID(unsigned short sPID, unsigned int &iPid)
{
	//int old = sPID;
// 	unsigned int x = 511;
// 	iPid = sPID & 511;
	sPID = sPID << 3;
	iPid = (unsigned int)(sPID >> 3);

}
//����PID
void AnalyzeTSPayload(unsigned char cPayload, bool &bPayload)
{
	bPayload = (bool)(cPayload & 0x40); //ȡ�ڶ�λ����
}

class SendMedia
{
private:
	char m_cFilePath[1024];
	char m_cClientIP[128];
	short m_sClientPort;
	short m_sServerPort;
	bool m_bStop;
	bool m_bPause;
	int m_iTid;
	unsigned m_threadID;

	unsigned char TsPack[188];
	//int m_icurrentIndex;
	char PESPack[1024*10224];
	char UDPSendBuff[1500];
public:
	SendMedia(char* cfile = NULL, char* pIp = NULL, short sClientPort = 10000, short sSPort = 12000);
	~SendMedia();
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

};

SendMedia::~SendMedia()
{
	delete[] PESPack;
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
	//m_icurrentIndex = 0;
// 	PESPack[0] = new char[1024 * 1024];
// 	PESPack[1] = new char[1024 * 1024];
	if (m_cClientIP != NULL && m_cFilePath != NULL)
	{
		//���������߳�
		_beginthreadex(NULL, 0, &SendMediaThread, this, 0, &m_threadID);
	}
}

unsigned int CALLBACK SendMedia::SendMediaThread(void *param)
{
	SendMedia* this0 = (SendMedia*)param;
	this0->m_threadID = GetCurrentThreadId();
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

	FILE *fp = NULL;

	// 	FILE *fptest = NULL;
	// 	fopen_s(&fptest, "e:\\test.ts", "w+b");

	fopen_s(&fp, m_cFilePath, "r+b");
	if (NULL == fp)
	{
		printf("Read TS file error\n");
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
			//�ҵ����ȥ��4���ֽ� �ٺ������ֽ�
			unsigned char *cFlag = &TsPack[TSLENTH-1];
			while ((*cFlag) == 0xFF)
			{
				--cFlag;
			}
			cFlag = cFlag - (2 + 3);
			//cFlag++;	//�ڶ����ֽ�
			unsigned short *usPMTPID = (unsigned short*)(cFlag);
			//printf("0x 0x \n", *cFlag, *(cFlag + 1));
			AnalyzeTSPID(ntohs(*usPMTPID), uiPMT_PID);//�ҵ�PMT���PIDֵ
			bPAT_PID = true;
		}
		else if (uiPMT_PID == iPID && TsPack[5] == 0x02 && !bPMT_PID)
		{
			//PMT�� ��һ���ֽ�Ϊ0x02 table_id
			// Ϊ��ƵPID �ҵ���Ч���ݣ�ȥ����4���ֽڣ�Ȼ��5���ֽھ�����Ƶ��2-3����PID����5���ֽ�����ƵPID
			unsigned char *cFlag = &TsPack[TSLENTH - 1];
			while ((*cFlag) == 0xFF)
			{
				--cFlag;
			}
			//���� 4���ֽ� + 5����Ƶ��Ϣ�ֽ�
			cFlag = cFlag - (5+3);
			cFlag++;	//�ڶ����ֽ�
			unsigned short *usAudioPID = (unsigned short*)(cFlag);
			AnalyzeTSPID(ntohs(*usAudioPID), uiAudio_PID);//�ҵ�P��Ƶ��PIDֵ
			//����1 + 5 ����Ƶ��Ϣ
			cFlag = cFlag - (1 + 5);
			cFlag++;	//�ڶ����ֽ�
			unsigned short *usVideoPID = (unsigned short*)(cFlag);
			AnalyzeTSPID(ntohs(*usVideoPID), uiVideo_PID);//�ҵ���Ƶ��PIDֵ
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
							if (iSendTime++ > 7)
							{
								Sleep(1);
								iSendTime = 0;
							}
							int iret = sendto(sock, UDPSendBuff, iLen, 0, (sockaddr*)&sockAddr, sizeof(sockAddr));
							//fwrite(UDPSendBuff, 1, iLen, fptest);
						}
						//printf("buff len = %d \n", iPacketLen);

						iPacketLen = 0;
						break;
					}
					//iSleepTime = iSleepTime > 39 || iSleepTime < 2 ? 37:iSleepTime-2;
					//printf("sleep time = %d \n", iSleepTime);
					Sleep(3);
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
		if (iPacketLen > 1024 * 1024)
		{
			break;
		}
		if (bPMT_PID)
		{
			memcpy(PESPack + iPacketLen, TsPack, TSLENTH);
			iPacketLen += TSLENTH;
		}


	}
	m_threadID = 0;
	fclose(fp);
	closesocket(sock);
}


// ����3 �������� �ֱ�Ϊfilepath clientIP clientPort
int main(int argc, char *argv[])
{
	char cFilePath[1024];
	char cClientIP[128];
	short sClientPort;
	short sServerPort;

	if (argc != 4)
	{
		printf("must input 4 paramters!\n");
		return -1;
	}

	strcpy_s(cFilePath, argv[1]);

	strcpy_s(cClientIP, argv[2]);

	sClientPort = atoi(argv[3]);

	printf("*******************************\n");
	printf("FilePath=%s\n", cFilePath);
	printf("ClientIP=%s\n", cClientIP);
	printf("ClientPort=%d\n", sClientPort);
	printf("*******************************\n");

	WSADATA wsaData;
	int Ret;
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup Failed \n");
		return -2;
	}

	SendMedia *pSendMedia = new SendMedia(cFilePath, cClientIP, sClientPort);
	while (1)
	{
		Sleep(100);
	}

}