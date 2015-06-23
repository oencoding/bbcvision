// BBCV-PlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BBCV-Player.h"
#include "BBCV-PlayerDlg.h"
#include ".\bbcv-playerdlg.h"
#include <stdio.h>
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int INTERVAL0 = 1000;
const int INTERVAL1 = 500;
const int INTERVAL2 = 100;
const int INTERVAL3 = 50;
const int INTERVAL4 = 10;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBBCVPlayerDlg �Ի���



CBBCVPlayerDlg::CBBCVPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBBCVPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBBCVPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DISPALY, m_cDisplay_Control);
	DDX_Control(pDX, IDC_EDIT1, m_cUDPPort);
	DDX_Control(pDX, IDC_EDIT2, m_showLogWnd);
	DDX_Control(pDX, IDC_SLIDER1, m_cAudioVolctrl);
	DDX_Control(pDX, IDCANCEL, m_MediaMatch);
	DDX_Control(pDX, IDC_CHECK1, m_cCheckMatch);
	DDX_Control(pDX, IDC_CHECK2, m_cSmooth);
	DDX_Control(pDX, IDOK, m_cPlayButton);
	DDX_Control(pDX, IDC_BUTTON1, m_cStopButton);
	DDX_Control(pDX, IDC_BUTTON2, m_cPauseStatus);
	DDX_Control(pDX, IDC_STATIC_SHOW,  m_showInfo);
	DDX_Control(pDX, IDC_COMBO1, m_cbExamble);

	DDX_Control(pDX, IDC_RADIO_KEY,m_cSendKeyctrl);
	DDX_Control(pDX,IDC_IPADDRESS1,m_cIPRSMctrl);
	DDX_Control(pDX,IDC_EDIT3,m_cPortRSMctrl);
	DDX_Control(pDX,IDC_EDIT4,m_cLoginNumberctrl);
	DDX_Control(pDX,IDC_EDIT5,m_cControlStreamIndexctrl);
	DDX_Control(pDX,IDC_EDIT6,m_cStreamPortbeginctrl);
	DDX_Control(pDX,IDC_EDIT7,m_cControlVNCportctrl);
	DDX_Control(pDX,IDC_EDIT8,m_cURLctrl);
	DDX_Control(pDX,IDC_IPADDRESS2,m_cRecvTSIPtctrl);
	
	DDX_Control(pDX, IDC_CHECK3, m_cSaveVideoData);
	DDX_Control(pDX, IDC_CHECK4, m_cNeedLog);
	DDX_Control(pDX, IDC_CHECK5, m_cDelayTimeModel);
	DDX_Control(pDX, IDC_CHECK6, m_cStreamtoClientctrl);
	DDX_Control(pDX, IDC_CHECK7, m_cStreamtoLocalctrl);
}

BEGIN_MESSAGE_MAP(CBBCVPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnNMReleasedcaptureSlider1)

	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnBnClickedCheck5)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELENDOK(IDC_COMBO1, OnCbnSelendokCombo1)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_BN_CLICKED(IDC_CHECK6, OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_RADIO_KEY, OnBnClickedRadioKey)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
END_MESSAGE_MAP()


// CBBCVPlayerDlg ��Ϣ�������

BOOL CBBCVPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
//	m_lDisplayport = 0;
//	AMP_Display_GetHandle(m_lDisplayport);

//	AMP_Display_InitDisplay(m_lDisplayport,m_cDisplay_Control.GetSafeHwnd());

	m_pInstance = NULL;
	m_bPlaying = false;
	m_fpLog = NULL;
	m_iCurrentLineNum = 0;
	m_bAutoMatch = true;
	m_bSmoothPlay = false;
	m_rtsptheClient = NULL;
	m_bPauseStatus = true;
	m_bDelayTimeModel = false;
	m_bNeedLog = true;
	m_bSavevideodata = false;
	m_bStreamtoClient = true;

	//m_cStreamtoClientctrl.SetCheck(m_bStreamtoClient);//Ĭ���ǵ��ͻ���
	//����
	m_cStreamtoClientctrl.ShowWindow(false);
	m_cStreamtoLocalctrl.ShowWindow(false);
	m_cControlStreamIndexctrl.ShowWindow(false);
	m_cDelayTimeModel.ShowWindow(false);
	m_cNeedLog.ShowWindow(false);

	m_cSendKeyctrl.SetCheck(false); //Ĭ�ϲ��������ͼ�ֵ
	m_bSendKey = false;

	m_cCheckMatch.SetCheck(m_bAutoMatch);
	m_MediaMatch.SetCheck(m_bAutoMatch);
	m_cAudioVolctrl.SetRange(0,100);
	m_cStopButton.EnableWindow(FALSE);
	//����Ϊ��Ҫ��־ ����Ҫ������Ƶ���� ����ʱģʽ�ر�
	m_cNeedLog.SetCheck(m_bNeedLog);
	m_cSaveVideoData.SetCheck(m_bSavevideodata);
	m_cDelayTimeModel.SetCheck(m_bDelayTimeModel);

	char streamInfo[1024]={0};
	sprintf(streamInfo,"average rate(kbps): %d        I FrameSize(byte): %d        time-delay(ms):%d",
			0,0,0);
	m_showInfo.SetWindowText(streamInfo);
	
	m_cbExamble.AddString("1000");
	m_cbExamble.AddString("500");
	m_cbExamble.AddString("100");
	m_cbExamble.AddString("50");
	m_cbExamble.AddString("10");

	m_btnhelp.SubclassDlgItem(IDC_BUTTON10,this);
	m_btnhelp.LoadBitmaps(IDB_BITMAP1);
	m_btnhelp.SizeToContent();

	m_cbExamble.SetCurSel(0); //���õ�nIndex��Ϊ��ʾ������

	m_cIPRSMctrl.SetWindowText("192.168.200.182");
	m_cPortRSMctrl.SetWindowText("25000");
	m_cLoginNumberctrl.SetWindowText("1");
	m_cRecvTSIPtctrl.SetWindowText("192.168.60.104");
	m_cStreamPortbeginctrl.SetWindowText("30028");
	m_cURLctrl.SetWindowText("http://192.168.100.31/cisco_test11/tv2.0352.html");

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CBBCVPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBBCVPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(m_pInstance)
		{
			int iRate = 0;
			Get_tsRate(m_pInstance,&iRate);
			char streamInfo[1024]={0};
			sprintf(streamInfo,"average rate(kbps): %d        I FrameSize(kb): %d        time-delay(ms):%d",
				iRate,0,0);
			m_showInfo.SetWindowText(streamInfo);
		}


		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CBBCVPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//play
void CBBCVPlayerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(m_bPlaying)
	{
		AfxMessageBox("���ȹر����ڲ��ŵ�����");
		return;
	}
	m_ifileType= tsUDP;
	char strURL[1024]={0};
	m_cUDPPort.GetWindowText(strURL,1024);

	int port = 0;
	char cUDPPort[8] = {0};

	char pURL[1024] = {0};
	strcpy(pURL,strURL);
	char *pFind1 = strstr(pURL,"udp:");
	char *pFind2 = strstr(pURL,"rtsp:");

			//  udp://@:14000 rtsp://192.168.20.131:8554/sd.ts
	if(pFind1 != NULL)
	{
		//��udp��ʾ ��ʾts��
		m_ifileType = tsUDP;
		
		char* pfindport1 = strstr(pURL,"//");
		if(pfindport1 != NULL)
		{
			char* pfindport2 = strstr(pfindport1,":");
			if(pfindport2 != NULL)
			{
				port = atoi(pfindport2+1);
				OutputDebugString(pfindport2+1);
			}
			
		}	
	}
	else if(pFind2 != NULL)
	{
		//rtsp ��
		m_ifileType = tsRTSP;

		//��ȡ ip �˿� ��Դ
/*		char* pfindport1 = strstr(pURL,"//");
		if(pfindport1 != NULL)
		{
			
			char* pfindport2 = strstr(pfindport1,":");
			if(pfindport2)
			{
				char strPorttmp[1024] ={0};
				strcpy(strPorttmp,pfindport2);
				char strRtspIP[256] ={0};

				int ilen = pfindport2 - pfindport1;
				*(pfindport1+ilen) = '\0';
				strcpy(strRtspIP,pfindport1+2);
				OutputDebugString(strRtspIP);


				char* pfingport3 = strstr(strPorttmp,"/");
				if(pfingport3)
				{
					*pfingport3 = '\0';

					port = atoi(strPorttmp+1);
					OutputDebugString(strPorttmp+1);
					
				}
			}

		}
*/

	}

	if(port == 0 && m_ifileType!=tsRTSP)
	{
		char *ptmpDigit = strURL;
		int ilen = 0;
		while(ilen<strlen(strURL)) 
		{
			if(!isdigit(*ptmpDigit))
				break;
			ptmpDigit++;
			++ilen;
		}
		if(ilen >= strlen(strURL))
		{
			port = atoi(strURL);
		}
	}

	if(port == 0 && m_ifileType!=tsRTSP)
	{
		AfxMessageBox("��������ȷ�ĸ�ʽ,�磺ֱ������˿ں� ���� udp://:1234 ���� rtsp://192.168.100.100:8888/filename");
		return ;
	}


	if(m_ifileType == tsUDP )
	{
		//m_cUDPPort.GetWindowText(cUDPPort,8);
		
		sprintf(cUDPPort,"%d",port);
		if(strcmp(cUDPPort,"")==0 || atoi(cUDPPort) < 0 || atoi(cUDPPort) > 65535)
		{
			AfxMessageBox("ǰ�˶˿ںŲ���Ϊ����Ϊ0��65535֮�������");
			return ;
		}

		if(m_pInstance)
			uninit_TS_Decoder(m_pInstance);
		m_pInstance = NULL;

		char filename[512] ={0};
		sprintf(filename,"udp://@:%d",atoi(cUDPPort));
		m_bPlaying = true;
		
		m_cPlayButton.EnableWindow(FALSE);
		m_cStopButton.EnableWindow(TRUE);
		//begin decoder
		m_pInstance = init_TS_Decoder(filename,m_cDisplay_Control.GetSafeHwnd(),m_bAutoMatch,m_bSmoothPlay);

	}
	else if(m_ifileType == tsRTSP)
	{
		//m_cUDPPort.GetWindowText(cUDPPort,8);
		if(m_rtsptheClient)
			delete m_rtsptheClient;
		m_rtsptheClient = NULL;
		m_rtsptheClient = new CRTSPclient;

		HANDLE ThreadHandle = (HANDLE)_beginthreadex(NULL, 0, PlayerStatusCheck, (LPVOID)this, 0, &udp_recv_thread);
		CloseHandle (ThreadHandle);
		Sleep(10);
		m_rtsptheClient->SetCheckThreadID(udp_recv_thread);

		if(m_rtsptheClient->Connect(strURL) != 0)
		{
			return ;
		}

		port = m_rtsptheClient->GetRtspPort();

		
		sprintf(cUDPPort,"%d",port);
		if(strcmp(cUDPPort,"")==0 || atoi(cUDPPort) < 0 || atoi(cUDPPort) > 65535)
		{
			AfxMessageBox("ǰ�˶˿ںŲ���Ϊ����Ϊ0��65535֮�������");
			return ;
		}

		if(m_pInstance)
			uninit_TS_Decoder(m_pInstance);
		m_pInstance = NULL;

		char filename[1024] ={0};
		sprintf(filename,"udp://@:%d",atoi(cUDPPort));
		m_bPlaying = true;

		m_cPlayButton.EnableWindow(FALSE);
		m_cStopButton.EnableWindow(TRUE);
		//begin decoder
		m_pInstance = init_TS_Decoder(filename,m_cDisplay_Control.GetSafeHwnd(),m_bAutoMatch,m_bSmoothPlay,true);

	}
	if(m_pInstance)
		Set_tsDecoder_SaveStream(m_pInstance,m_bSavevideodata);
	//Set_tsDecoder_stat(m_pInstance,m_bAutoMatch);
	if(NULL == m_fpLog)
	{
		char Path[512] = {0};
		GetCurrentDirectory(sizeof(Path),Path);
		strcat(Path,"\\\BBCV-Play.log");
		m_fpLog = fopen(Path,"r");
	}
	Set_tsDecoder_Volume(m_pInstance,80);
	OnCbnSelchangeCombo1();
	if(m_pInstance)
		Set_tsRate_period(m_pInstance,1000);


	 SetTimer(1, 1000, NULL);   
	  SetTimer(2, 1000, NULL);   
//	OnOK();
}

//quit
void CBBCVPlayerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
		m_bPlaying = false;
	KillTimer(1);
	if(m_fpLog)
		fclose(m_fpLog);
	m_fpLog= NULL;
	uninit_TS_Decoder(m_pInstance);

	if(m_rtsptheClient != NULL&& m_ifileType==tsRTSP)
	{
		m_rtsptheClient->Disconnect();
	}
	delete m_rtsptheClient;
	m_rtsptheClient = NULL;
	
	//KillTimer(2);
	m_pInstance = NULL;


	OnCancel();
}

unsigned int _stdcall CBBCVPlayerDlg::PlayerStatusCheck(void* param)
{
	CBBCVPlayerDlg* this0 = (CBBCVPlayerDlg*)param;
	
	this0->udp_recv_thread = GetCurrentThreadId();

	MSG Msg;
	PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE);

	//SetTimer(NULL, 0, 10, NULL);
	BOOL bRet;
	bool fisrt = true;
	while ((bRet = GetMessage(&Msg, NULL, 0, 0)) != 0)
	{
		if(!this0->m_bPlaying)
		{
			break;
		}
		if((bRet == -1))
		{
			continue;
		}
		if(fisrt)
		{
			printf(" =========== Decode GetMessage First \n");
			fisrt = false;
		}
		switch(Msg.message)
		{
		case MESS_PLAY_STATUS_BYE:
			{
				this0->OnBnClickedButton1();
				return 0;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

void CBBCVPlayerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
//	AMP_Display_StopDisplay(m_lDisplayport);
	m_bPlaying = false;
	KillTimer(1);
	if(m_fpLog)
		fclose(m_fpLog);
	m_fpLog= NULL;
	uninit_TS_Decoder(m_pInstance);
	if(m_rtsptheClient != NULL&& m_ifileType==tsRTSP)
	{
		m_rtsptheClient->Disconnect();
	}
	delete m_rtsptheClient;
	m_rtsptheClient = NULL;
	//KillTimer(2);


	m_pInstance = NULL;

	m_cPlayButton.EnableWindow(TRUE);
	m_cStopButton.EnableWindow(FALSE);


//	CRect rt;
//	m_cDisplay_Control.GetClientRect(&rt);
//	m_cDisplay_Control.InvalidateRect(&rt);
}

void CBBCVPlayerDlg::show_Log()
{

	return;
}

void CBBCVPlayerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	int iVolume = 0;
   if(m_pInstance != NULL && Get_tsDecoder_Volume(m_pInstance,iVolume))
   {
	   m_cAudioVolctrl.SetPos(iVolume);
   }

	switch(nIDEvent)
	{
	case 2:
		{
			if(m_pInstance)
			{
				int iRate = 0;
				Get_tsRate(m_pInstance,&iRate);
				int iFrameSize = 0;
				Get_tsIFrame_size(m_pInstance,&iFrameSize);
				int iDelaytime = 0;
				char streamInfo[1024]={0};
				sprintf(streamInfo,"average rate(kbps): %d        I FrameSize(byte): %d        time-delay(ms): %d",
					iRate,iFrameSize,iDelaytime);
				m_showInfo.SetWindowText(streamInfo);
			}
			break;
		}
	case 1:
		{
			if(m_fpLog)
			{
				char buf[1024]={0};
				int count = 0;
				char* iret = NULL;
		/*		while ( m_iCurrentLineNum >=count)
				{
					iret = getline (&buf, 1024, m_fpLog);
					if(iret == -1)
						break;
					count++;
				}
				m_iCurrentLineNum = count;
		*/
						//show
				while(fgets(buf, 1024, m_fpLog))
				{
					m_iCurrentLineNum++;
					int ilen=strlen(buf);
					buf[ilen] = '\n';
					m_showLogWnd.SetSel(-1);
					m_showLogWnd.ReplaceSel(buf);

					//CString str="\n";
				//	m_showLogWnd.SetSel(-1);
				//	m_showLogWnd.ReplaceSel(str);
					memset(buf,0,1024);
				}

			}
			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CBBCVPlayerDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int ivolume = m_cAudioVolctrl.GetPos();

	Set_tsDecoder_Volume(m_pInstance,ivolume);

	*pResult = 0;
}

void CBBCVPlayerDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
//	m_bAutoMatch = m_bAutoMatch ? false:true;

	//m_MediaMatch.SetCheck(m_bAutoMatch);
//	m_cCheckMatch.SetCheck(m_bAutoMatch);
	
}

void CBBCVPlayerDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	m_bAutoMatch = m_bAutoMatch ? false:true;

	//m_MediaMatch.SetCheck(m_bAutoMatch);
	m_cCheckMatch.SetCheck(m_bAutoMatch);
}

void CBBCVPlayerDlg::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	m_bSmoothPlay = m_bSmoothPlay ? false:true;
	m_cSmooth.SetCheck(m_bSmoothPlay);

}

void CBBCVPlayerDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	if(m_bPauseStatus)
	{
		//��Ҫ��ͣ ����״̬����Ϊ�ָ�
		m_cPauseStatus.SetWindowText("resume");

		if(m_rtsptheClient != NULL && m_ifileType==tsRTSP)
		{
//			m_rtsptheClient->Pause();
		}
		m_bPauseStatus = false;
	}
	else
	{
		//��Ҫ�ָ� ��״̬����Ϊ��ͣ
		m_cPauseStatus.SetWindowText("pause");
		if(m_rtsptheClient != NULL && m_ifileType==tsRTSP)
		{
//			m_rtsptheClient->Resume();
		}
		m_bPauseStatus = true;
	}
}

void CBBCVPlayerDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CString strFile;

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.ts)|*.h264|All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	
	if(m_pInstance)
		uninit_TS_Decoder(m_pInstance);
	m_pInstance = NULL;

	char strFileName[1024] ={};
	sprintf(strFileName,"%s",strFile);
	m_cPlayButton.EnableWindow(FALSE);
	m_cStopButton.EnableWindow(TRUE);
	//begin decoder
	m_pInstance = init_TS_Decoder(strFileName,m_cDisplay_Control.GetSafeHwnd(),m_bAutoMatch,m_bSmoothPlay);

}

void CBBCVPlayerDlg::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	//save stream
	m_bSavevideodata = m_bSavevideodata ? false:true;
	m_cSaveVideoData.SetCheck(m_bSavevideodata);
	if(m_pInstance)
		Set_tsDecoder_SaveStream(m_pInstance,m_bSavevideodata);
}

void CBBCVPlayerDlg::OnBnClickedCheck4()
{
	// TODO: Add your control notification handler code here
	m_bNeedLog = m_bNeedLog ? false:true;
	m_cNeedLog.SetCheck(m_bNeedLog);
}

void CBBCVPlayerDlg::OnBnClickedCheck5()
{
	// TODO: Add your control notification handler code here
	m_bDelayTimeModel = m_bDelayTimeModel ? false:true;
	m_cDelayTimeModel.SetCheck(m_bDelayTimeModel);
}

void CBBCVPlayerDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_cbExamble.GetCurSel(); //��ǰѡ�е���
	int iInterval = INTERVAL0;
	switch(nIndex)
	{
	case 0:
		iInterval = INTERVAL0;
		break;
	case 1:
		iInterval = INTERVAL1;
		break;
	case 2:
		iInterval = INTERVAL2;
		break;
	case 3:
		iInterval = INTERVAL3;
		break;
	case 4:
		iInterval = INTERVAL4;
		break;
	}
	if(m_pInstance)
		Set_tsRate_period(m_pInstance,iInterval);

}

void CBBCVPlayerDlg::OnCbnSelendokCombo1()
{
	// TODO: Add your control notification handler code here
	//int nIndex = m_cbExamble.GetCurSel(); //��ǰѡ�е���
}

void CBBCVPlayerDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	//��ʼ���� 
	//����ѡ�� rsm ip ,port ,�����ͻ��˵���ʼ�˿ڣ�ip��
	//�õ�rsm��ip���˿ڣ�·����
	char strRSMIP[256]={0};
	m_cIPRSMctrl.GetWindowText(strRSMIP,256);
	char strRSMPort[256]={0};
	m_cPortRSMctrl.GetWindowText(strRSMPort,256);
	char strLoginNum[256]={0};
	m_cLoginNumberctrl.GetWindowText(strLoginNum,256);
	char strBeginPort[256]={0};
	m_cStreamPortbeginctrl.GetWindowText(strBeginPort,256);
	char strRecvTsIP[256]={0};
	m_cRecvTSIPtctrl.GetWindowText(strRecvTsIP,256);
	char strURL[1024]={0};
	m_cURLctrl.GetWindowText(strURL,1024);

	memset(m_strRsmIP,0,256);
	strcpy(m_strRsmIP,strRSMIP);
	memset(m_strRecvTSIP,0,256);
	strcpy(m_strRecvTSIP,strRecvTsIP);
	m_iRSMPort = atoi(strRSMPort);
	m_iLoginNumber = atoi(strLoginNum);
	m_iStreamPortBegin = atoi(strBeginPort);
	
	int ret  =m_rsmjsonobj.init(m_strRsmIP,m_iRSMPort);
	if(ret < 0)
	{
		AfxMessageBox("���ڵ�½�У����Ժ����ԣ�");
		return ;
	}
	//���͵�½����
	ret = m_rsmjsonobj.login(strURL,m_strRecvTSIP,strBeginPort,m_iLoginNumber);
	if(ret > 0)
	{
		AfxMessageBox("�������ڵ�ǰĿ¼��login_***.log��");
	}

}

void CBBCVPlayerDlg::OnBnClickedCheck6()
{
	// TODO: Add your control notification handler code here
	//����Ϊ�Ƿ��������ͻ��� ��ѡ���������ͻ��ˣ���������������
	m_bStreamtoClient = true;
	//m_cStreamtoClientctrl.SetCheck(m_bStreamtoClient);
}

void CBBCVPlayerDlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	//����Ϊ����������������
	m_bStreamtoClient = false;
	//m_cStreamtoLocalctrl.SetCheck(!m_bStreamtoClient);
}

int CBBCVPlayerDlg::SendKey(int iKeyID)
{
	//��ȡvncm��ip��key�˿ڣ������ǵڼ�·��
	//��Ҫ�Ȼ�ȡip��vncms�˿�

	char strRSMIP[256]={0};
	m_cIPRSMctrl.GetWindowText(strRSMIP,256);
	char strVncPort[256]={0};
	m_cControlVNCportctrl.GetWindowText(strVncPort,256);
//	char strStreamIndex[256]={0};
//	m_cControlStreamIndexctrl.GetWindowText(strStreamIndex,256);
	//int iIndex = atoi(strStreamIndex);
	
	if(strcmp(strVncPort,"")==0 || atoi(strVncPort) < 0 || atoi(strVncPort) > 65535)
	{
		AfxMessageBox("ǰ�˶˿ںŲ���Ϊ����Ϊ0��65535֮�������");
		return -1;
	}
	int ivncmsPort = atoi(strVncPort);
	//���ӵ�vncms������key

	 BlcYunKeyIrrMsg irrmsg;
	 irrmsg.head.dev_type = 1001;
	 irrmsg.body.key_value = iKeyID;
	 irrmsg.body.key_status = 2;

	Stream ptmpRequest;
	ptmpRequest.ConnectServer_udp(strRSMIP,ivncmsPort);
	ptmpRequest.Send_str_udp((char*)&irrmsg, sizeof(BlcYunKeyIrrMsg));
	irrmsg.body.key_status = 3; //̧����Ϣ
	ptmpRequest.Send_str_udp((char*)&irrmsg, sizeof(BlcYunKeyIrrMsg));

	return 0;
}

void CBBCVPlayerDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	//key up 82
	int ret = -1;
	if(m_bSendKey)
		ret = SendKey(82);
	if(ret < 0)
	{
		//AfxMessageBox("");
	}

}

void CBBCVPlayerDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	//key left 80
	int ret = -1;
	if(m_bSendKey)
		ret = SendKey(80);
	if(ret < 0)
	{
		//AfxMessageBox("");
	}
}

void CBBCVPlayerDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	//key down 81
	int ret = -1;
	if(m_bSendKey)
		ret = SendKey(81);
	if(ret < 0)
	{
		//AfxMessageBox("");
	}
}

void CBBCVPlayerDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	//key ok 40
	int ret = -1;
	if(m_bSendKey)
		ret = SendKey(40);
	if(ret < 0)
	{
		//AfxMessageBox("");
	}
}

void CBBCVPlayerDlg::OnBnClickedRadioKey()
{
	// TODO: Add your control notification handler code here
	//select sendkey
	m_bSendKey = !m_bSendKey;
	m_cSendKeyctrl.SetCheck(m_bSendKey);
}


BOOL CBBCVPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//�ж��Ƿ�Ϊ������Ϣ
	//if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST && m_bSendKey)
	if (WM_KEYDOWN == pMsg->message && m_bSendKey)
	{
		//�ж��Ƿ��¼���Enter��
		switch(pMsg->wParam)
		{
			case VK_RETURN:
			{
				//ok
				int ret = SendKey(40);
				if(ret < 0)
				{
					//AfxMessageBox("");
				}
				return true;
			}
			case VK_LEFT:
			{
				int ret = SendKey(80);
				if(ret < 0)
				{
					//AfxMessageBox("");
				}
				return true;
			}
			case VK_RIGHT:
			{
				int ret = SendKey(79);
				if(ret < 0)
				{
					//AfxMessageBox("");
				}
				return true;
			}
			case VK_UP:
			{
				int ret = SendKey(82);
				if(ret < 0)
				{
					//AfxMessageBox("");
				}
				return true;
			}
			case VK_DOWN:
			{
				int ret = SendKey(81);
				if(ret < 0)
				{
					//AfxMessageBox("");
				}
				return true;
			}

		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CBBCVPlayerDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	//right
	int ret = -1;
	if(m_bSendKey)
		ret = SendKey(79);
	if(ret < 0)
	{
		//AfxMessageBox("");
	}
}

void CBBCVPlayerDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("BBCV-Player Version 1.0  \n 2015 by yyd");
}
