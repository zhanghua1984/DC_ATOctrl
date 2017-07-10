// ATOCTRLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATOCTRL.h"
#include "ATOCTRLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//串口相关变量
#include "Includes.h"
#include "COMPORT.h"

BOOL	m_bWorking;
BOOL	m_bHighSpeed;
extern BOOL	m_bThreadRXrunning;
extern BOOL	m_bThreadTXrunning;
extern HANDLE hCom; //串口
extern  BYTE	m_byteRXbuffer[BUFFERLENTH];
BYTE	m_byteFrame[24];
extern BYTE	m_byteWriteFrame1[24];
extern BYTE	m_byteWriteFrame2[24];
extern BYTE	m_byteWriteFrame3[24];
extern BYTE	m_byteWriteFrame4[24];
BOOL	m_bConnectCom;	//串口是否连接
extern BOOL	m_bSendPackage;
#define MAXQSIZE BUFFERLENTH

/*利用循环队列实现循环缓冲*/

typedef BYTE QElemType ; //定义新类型名 

typedef struct
{
	QElemType *base;  //循环队列的存储空间 （ 用数组指针指示存储实际长度10字节的字符串) 
	int front;  //队头元素下标（取元素） 
	int rear;  //队尾元素下标（插入元素） 
}SqQueue;            

//建立循环队列（循环缓冲区）
int InitQueue(SqQueue &Q)
{
    Q.base=(QElemType *)malloc(MAXQSIZE*sizeof(QElemType));
    if(!Q.base) 
		return -1;  //创建失败，返回-1 
    Q.front=0;
    Q.rear=0;
    return 0;    //创建成功，返回0 
}    

//插入元素（入队列）
int EnQueue(SqQueue &Q,QElemType e)
{
    if((Q.rear+1)%MAXQSIZE==Q.front)	//下一个是否和读取指针重回
		return -1;  //队满
    //memcpy(Q.base[Q.rear],e);  //赋值
	Q.base[Q.rear]=e;
    Q.rear=(Q.rear+1)%MAXQSIZE;  //下标加一 
    return 0;  //入队成功
}

//取元素(数组指针，指向取到的值） 
int DeQueue(SqQueue &Q,QElemType &e)
{
    if(Q.rear==Q.front)
        return -1;   //队空
    //memcpy(e,Q.base[Q.front]);
	e=Q.base[Q.front];
    Q.front=(Q.front+1)%MAXQSIZE; //下标加一 
    return 0;  //成功
}  

int QueueLenth(SqQueue Q)
{
	return (Q.rear-Q.front+MAXQSIZE)%MAXQSIZE;
}

SqQueue q;  //数据结构 环形缓冲区队列
int flag; 
QElemType strIn,strOut;


//串口变量结束


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATOCTRLDlg dialog

CATOCTRLDlg::CATOCTRLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATOCTRLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATOCTRLDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATOCTRLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATOCTRLDlg)
	DDX_Control(pDX, IDC_BUTTONONTIMESEND, m_COnTimeSend);
	DDX_Control(pDX, IDC_EDITRETURNMC, m_CReturnMC);
	DDX_Control(pDX, IDC_RETURNMCATOFACEUNIT, m_CReturnMCATOFaceUnit);
	DDX_Control(pDX, IDC_RETURNMAMODESTATUS, m_CReturnMAModeStatus);
	DDX_Control(pDX, IDC_RETURNATOSTARTSTATUS, m_CReturnATOStartStatus);
	DDX_Control(pDX, IDC_RETURNCANCELAUTORETURNSTATUS, m_CReturnCancelAutoReturnStatus);
	DDX_Control(pDX, IDC_EDITRETURNATOAUTORETURNEFFECT, m_CReturnATOAutoReturnEffect);
	DDX_Control(pDX, IDC_EDITRETURNATOAUTORETURNSTATUS, m_CReturnATOAutoReturnStatus);
	DDX_Control(pDX, IDC_EDITRETURNATOMODESTATUS, m_CReturnATOModeStatus);
	DDX_Control(pDX, IDC_EDITRETURNATOMODEEFFECT, m_CReturnATOModeEffect);
	DDX_Control(pDX, IDC_CHECKRETURNGUARDRE, m_CReturnGuardre);
	DDX_Control(pDX, IDC_CHECKRETURNHIGHACCRE, m_CReturnHighAcere);
	DDX_Control(pDX, IDC_CHECKRETURNLEFTOPENRE, m_CReturnLeftOpenre);
	DDX_Control(pDX, IDC_CHECKRETURNLEFTSELECTRE, m_CReturnLeftSelectre);
	DDX_Control(pDX, IDC_CHECKRETURNRIGHTOPENRE, m_CReturnRightOpenre);
	DDX_Control(pDX, IDC_CHECKRETURNRIGHTSELECTRE, m_CReturnRightSelectre);
	DDX_Control(pDX, IDC_CHECKRETURNSLOPESTARTRE, m_CReturnSlopeStartre);
	DDX_Control(pDX, IDC_EDITRETURNATPMODEOUT, m_CReturnATPModeOutLed);
	DDX_Control(pDX, IDC_EDITRETURNATOFAULTLED, m_CReturnATOFaultLed);
	DDX_Control(pDX, IDC_EDITRETURNMAMODELED, m_CReturnMAModeLed);
	DDX_Control(pDX, IDC_EDITRETURNATOSTART, m_CReturnATOStartLed);
	DDX_Control(pDX, IDC_EDITRETURNATOAUTORETURNLED, m_CReturnATOAutoReturnLed);
	DDX_Control(pDX, IDC_CHECKRETURNATOMODERALAYENABLE, m_CReturnATOMode);
	DDX_Control(pDX, IDC_CHECKRETURNATOAUTORETURN, m_CReturnAutoReturnRalayEnable);
	DDX_Control(pDX, IDC_CHECKRETURNRIGHTSELECT, m_CReturnRightSelect);
	DDX_Control(pDX, IDC_CHECKRETURNRIGHTOPEN, m_CReturnRightOpen);
	DDX_Control(pDX, IDC_CHECKRETURNLEFTSELECT, m_CReturnLeftSelect);
	DDX_Control(pDX, IDC_CHECKRETURNLEFTOPEN, m_CReturnLeftOpen);
	DDX_Control(pDX, IDC_CHECKRETURNGUARD, m_CReturnGuard);
	DDX_Control(pDX, IDC_CHECKRETURNHIGHACC, m_CReturnHighAce);
	DDX_Control(pDX, IDC_CHECKRETURNSLOPESTART, m_CReturnSlopeStart);
	DDX_Control(pDX, IDC_EDITRETURNMCOUT, m_CMCOutReturn);
	DDX_Control(pDX, IDC_CHECKCMMODE, m_CATPModeCM);
	DDX_Control(pDX, IDC_CHECKNOMODE, m_CATPModeNo);
	DDX_Control(pDX, IDC_CHECKRMMODE, m_CATPModeRM);
	DDX_Control(pDX, IDC_CHECKZEROMODE, m_CATPModeZero);
	DDX_Control(pDX, IDC_CHECKATOFAULTLIGHT, m_CATOFaultLight);
	DDX_Control(pDX, IDC_CHECKATOFAULTLEDNOLIGHT, m_CATOFaultNoLight);
	DDX_Control(pDX, IDC_CHECKATOFAULTGLINT, m_CATOFaultGlint);
	DDX_Control(pDX, IDC_CHECKMAMODELIGHT, m_CMAModeLight);
	DDX_Control(pDX, IDC_CHECKMAMODELEDNOLIGHT, m_CMAModeNoLight);
	DDX_Control(pDX, IDC_CHECKMAMODEGLINT, m_CMAModeGlint);
	DDX_Control(pDX, IDC_CHECKATOSTARTLIGHT, m_CATOStartLedLight);
	DDX_Control(pDX, IDC_CHECKATOSTARTGLINT, m_CATOStartLedGlint);
	DDX_Control(pDX, IDC_CHECKATOSTARTLEDNOLIGHT, m_CATOStartLedNoLight);
	DDX_Control(pDX, IDC_CHECKRETURNLIGHT, m_CATOAutoReturnLedLight);
	DDX_Control(pDX, IDC_CHECKRETURNGLINT, m_CATOAutoReturnLedGlint);
	DDX_Control(pDX, IDC_CHECKRETURNLEDNOLIGHT, m_CATOAutoReturnLedNoLight);
	DDX_Control(pDX, IDC_CHECKATOMODE, m_CATOMode);
	DDX_Control(pDX, IDC_CHECKATOAUTORETURN, m_CATOAutoReturn);
	DDX_Control(pDX, IDC_CHECKRIGHTSELECT, m_CRightSelect);
	DDX_Control(pDX, IDC_CHECKRIGHTOPEN, m_CRightOpen);
	DDX_Control(pDX, IDC_CHECKLEFTOPEN, m_CLeftOpen);
	DDX_Control(pDX, IDC_CHECKLEFTSELECT, m_CLeftSelect);
	DDX_Control(pDX, IDC_CHECKGUARD, m_CGuard);
	DDX_Control(pDX, IDC_CHECKHIGHACC, m_CHighAcc);
	DDX_Control(pDX, IDC_CHECKSLOPESTART, m_CSlopeStart);
	DDX_Control(pDX, IDC_SLIDERMCLEVAL, m_CMCLeval);
	DDX_Control(pDX, IDC_RICHEDITTX, m_CTX);
	DDX_Control(pDX, IDC_BUTTONCOM, m_CcomButton);
	DDX_Control(pDX, IDC_COMBOCOMPORTNUMBER, m_Ccomportnumber);
	DDX_Control(pDX, IDC_RICHEDITRX, m_CRX);
	DDX_Control(pDX, IDC_EDITRX, m_CRXCounterByte);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATOCTRLDlg, CDialog)
	//{{AFX_MSG_MAP(CATOCTRLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCOM, OnButtoncom)
	ON_CBN_SELCHANGE(IDC_COMBOCOMPORTNUMBER, OnSelchangeCombocomportnumber)
	ON_BN_CLICKED(IDC_BUTTONSEND, OnButtonsend)
	ON_BN_CLICKED(IDC_CHECKRETURNLEDNOLIGHT, OnCheckreturnlednolight)
	ON_BN_CLICKED(IDC_CHECKRETURNGLINT, OnCheckreturnglint)
	ON_BN_CLICKED(IDC_CHECKRETURNLIGHT, OnCheckreturnlight)
	ON_BN_CLICKED(IDC_CHECKATOSTARTLEDNOLIGHT, OnCheckatostartlednolight)
	ON_BN_CLICKED(IDC_CHECKATOSTARTGLINT, OnCheckatostartglint)
	ON_BN_CLICKED(IDC_CHECKATOSTARTLIGHT, OnCheckatostartlight)
	ON_BN_CLICKED(IDC_CHECKMAMODELEDNOLIGHT, OnCheckmamodelednolight)
	ON_BN_CLICKED(IDC_CHECKMAMODEGLINT, OnCheckmamodeglint)
	ON_BN_CLICKED(IDC_CHECKMAMODELIGHT, OnCheckmamodelight)
	ON_BN_CLICKED(IDC_CHECKATOFAULTLEDNOLIGHT, OnCheckatofaultlednolight)
	ON_BN_CLICKED(IDC_CHECKATOFAULTGLINT, OnCheckatofaultglint)
	ON_BN_CLICKED(IDC_CHECKATOFAULTLIGHT, OnCheckatofaultlight)
	ON_BN_CLICKED(IDC_CHECKZEROMODE, OnCheckzeromode)
	ON_BN_CLICKED(IDC_CHECKRMMODE, OnCheckrmmode)
	ON_BN_CLICKED(IDC_CHECKCMMODE, OnCheckcmmode)
	ON_BN_CLICKED(IDC_CHECKNOMODE, OnChecknomode)
	ON_BN_CLICKED(IDC_BUTTONONTIMESEND, OnButtonontimesend)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RX,OnThreadRXMessage)
	ON_MESSAGE(WM_FINDAVACOMPORT,OnReceiveAComPort)
	ON_MESSAGE(WM_UCOM_WRITE,OnWriteComPortMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATOCTRLDlg message handlers

BOOL CATOCTRLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	Init();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATOCTRLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CATOCTRLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		DrawLED(dc);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CATOCTRLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// 串口相关函数开始

LRESULT CATOCTRLDlg::OnThreadRXMessage(WPARAM wParam,LPARAM lParam)		//接收串口消息
{
	static int m_snc=0;
	m_snc+=wParam;
	CString m_temp;
	m_temp.Format("%d",m_snc);
	m_CRXCounterByte.SetWindowText(m_temp);

	static bool m_binitsq=TRUE;
	if (m_binitsq==TRUE)
	{
		m_binitsq=FALSE;
		
		if (InitQueue(q)==-1)
		{
			AfxMessageBox("创建缓冲区失败!");
		}
	}
	//将接收的串口数，全部入队
	for (int i=0;i<(int)wParam;i++)
	{
		EnQueue(q,m_byteRXbuffer[i]);
	}
	//判断缓冲区内是否够一帧数据
	for (int m_nc=QueueLenth(q);m_nc>=24;m_nc--)
	{
		//出队 检查是否收到0xfc
		if(DeQueue(q,m_byteFrame[0])!=0)
		{
			AfxMessageBox("读缓冲区失败");
		}
		if (m_byteFrame[0]==0xfc)
		{
			m_strshowRX+="FC";
			for (int j=1;j<24;j++)
			{
				DeQueue(q,m_byteFrame[j]);
				m_temp.Format("%02X",m_byteFrame[j]);
				m_strshowRX+=m_temp;
			}
			//命令解析
			FrameParse(m_byteFrame);
			m_strshowRX+="\r\n";
			m_CRX.SetSel(-1, -1);
			if (m_byteFrame[FRAMEPOS]==0xE0)
			{
				m_CRX.ReplaceSel( (LPCTSTR)m_strshowRX);
				m_CRX.PostMessage(WM_VSCROLL, SB_BOTTOM,0);
			}
			m_nc-=23;
			m_strshowRX.Empty();
		}
	}

	if (m_strshowRX.GetLength()>1024)
	{
		m_strshowRX.Empty();
	}

	return 0;
}

void CATOCTRLDlg::Init()//初始化数据
{
	CCOMPORT m_comport;
	m_comport.InitPort();	//串口初始化
	InitFrame();
	m_CMCLeval.EnableWindow(TRUE);
	m_CMCLeval.SetRange(1,14);
	m_CMCLeval.SetPos(1);
	//ATO自动折返指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	m_natoautoreturnled=0;
	//ATO启动按钮指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	m_natostartled=0;
	//MA模式指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	m_nmamodeled=0;
	//ATO故障指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	m_natofaultled=0;
	//ATP模式输出00b：0模式，10b：RM 11b：CM，01b：无效
	m_natpmodeout=0;

	m_bSendOnTime=FALSE;
	OnButtonontimesend();

	//m_rectatoautoreturnled.SetRect(490,432,510,446);

	POINT m_point;
	m_point.x=500;
	m_point.y=440;
	m_rgnatoautoreturnled.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=490;
	m_rgnReturnCancelAutoReturnStatus.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=515;
	m_rgnReturnATOStartStatus.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);

	m_point.y=465;
	m_rgnatostartled.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.x=700;
	m_point.y=515;
	m_rgnmamodeled.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=440;
	m_rgnReturnATOModeEffect.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=465;
	m_rgnReturnATOModeStatus.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=490;
	m_rgnReturnMAModeStatus.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);

	m_point.x=920;
	m_point.y=490;
	m_rgnatofaultled.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=515;
	m_rgnatpmodeoutled.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=440;
	m_rgnReturnATOAutoReturnEffect.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);
	m_point.y=465;
	m_rgnReturnATOAutoReturnStatus.CreateEllipticRgn(m_point.x-TEN,m_point.y-TEN,m_point.x+TEN,m_point.y+TEN);

	m_nreturnatoautoreturnled=0;
	m_nreturnatostartled=0;
	m_nreturnmamodeled=0;
	m_nreturnatofaultled=0;
	m_nreturnatpmodeout=0;

	m_bReturnMAModeStatus=FALSE;
	m_bReturnATOStartStatus=FALSE;
	m_bReturnCancelAutoReturnStatus=FALSE;
	m_bReturnATOAutoReturnEffect=FALSE;
	m_bReturnATOAutoReturnStatus=FALSE;
	m_bReturnATOModeStatus=FALSE;
	m_bReturnATOModeEffect=FALSE;

	SetTimer(2,40,NULL);
}
LRESULT CATOCTRLDlg::OnReceiveAComPort(WPARAM wParam, LPARAM lParam)	//找到串口并添加
{
	static int m_nindex=0;
	CString m_strport;
	m_strport.Format("COM%d",lParam);
	m_Ccomportnumber.InsertString(m_nindex,m_strport);
	m_nindex++;
	m_Ccomportnumber.SetCurSel(0);
	return 0;
}
void CATOCTRLDlg::InitFrame()//帧值初始化
{
	m_byteWriteFrame1[0x00]=FRAME_HEAD1;
	m_byteWriteFrame2[0x00]=FRAME_HEAD1;
	m_byteWriteFrame3[0x00]=FRAME_HEAD1;
	m_byteWriteFrame4[0x00]=FRAME_HEAD1;
	
	m_byteWriteFrame1[0x01]=FRAME_HEAD2;
	m_byteWriteFrame2[0x01]=FRAME_HEAD2;
	m_byteWriteFrame3[0x01]=FRAME_HEAD2;
	m_byteWriteFrame4[0x01]=FRAME_HEAD2;
	
	m_byteWriteFrame1[0x02]=LOCAL_ADDRESS;
	m_byteWriteFrame2[0x02]=LOCAL_ADDRESS;
	m_byteWriteFrame3[0x02]=LOCAL_ADDRESS;
	m_byteWriteFrame4[0x02]=LOCAL_ADDRESS;
	
	m_byteWriteFrame1[0x03]=ATP_MPM_A;
	m_byteWriteFrame2[0x03]=ATP_MPM_A;
	m_byteWriteFrame3[0x03]=ATP_MPM_A;
	m_byteWriteFrame4[0x03]=ATP_MPM_A;


	m_byteWriteFrame1[0x05]=FRAME_SEQUENCE_70;
	m_byteWriteFrame2[0x05]=FRAME_SEQUENCE_E0;
	m_byteWriteFrame3[0x05]=FRAME_SEQUENCE_E1;
	m_byteWriteFrame4[0x05]=FRAME_SEQUENCE_E2;
	
}

LRESULT CATOCTRLDlg::OnWriteComPortMSG(WPARAM wParam, LPARAM lParam)//串口信息发送显示
{
	//显示发送信息
	CString m_str;
	CString m_strf1,m_strf2,m_strf3,m_strf4;
	for (int i=0;i<24;i++)
	{
		m_str.Format("%02X",m_byteWriteFrame1[i]);
		m_strf1+=m_str;
		m_str.Format("%02X",m_byteWriteFrame2[i]);
		m_strf2+=m_str;
		m_str.Format("%02X",m_byteWriteFrame3[i]);
		m_strf3+=m_str;
		m_str.Format("%02X",m_byteWriteFrame4[i]);
		m_strf4+=m_str;
	}
	m_strf1+="\r\n";
	m_strf2+="\r\n";
	m_strf3+="\r\n";
	m_strf4+="\r\n";
	
	m_str=m_strf1+m_strf2+m_strf3+m_strf4;
	//文件写入记录
	m_CTX.ReplaceSel(m_str);
	m_CTX.PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	return 0;
}


//串口相关函数结束

void CATOCTRLDlg::OnButtoncom() 
{
	// TODO: Add your control notification handler code here
	if (m_bConnectCom==TRUE)	//串口关闭
	{
		m_CcomButton.SetWindowText("打开串口");
		OnSelchangeCombocomportnumber();
	} 
	else
	{
		m_CcomButton.SetWindowText("关闭串口");
		m_bConnectCom=TRUE;
		CString m_strsel;
		int  m_nselcom=m_Ccomportnumber.GetCurSel();
		m_Ccomportnumber.GetLBText(m_nselcom,m_strsel);
		m_strsel.Delete(0,3);
		CCOMPORT m_comport;
		m_comport.OpenPort(atoi(m_strsel));
	}
}

void CATOCTRLDlg::OnSelchangeCombocomportnumber() 
{
	// TODO: Add your control notification handler code here
	//结束线程
	if (m_bConnectCom==TRUE)
	{
		m_bThreadRXrunning=FALSE;
		m_bThreadTXrunning=FALSE;
		CloseHandle(hCom);
		m_bConnectCom=FALSE;
		m_CcomButton.SetWindowText("打开串口");
	}
}

void CATOCTRLDlg::OnButtonsend() 
{
	// TODO: Add your control notification handler code here
	//档位设置
	switch (m_CMCLeval.GetPos())
	{
	case 1:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_EB;
			break;
		}
	case 2:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B7;
			break;
		}
	case 3:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B6;
			break;
		}
	case 4:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B5;
			break;
		}
	case 5:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B4;
			break;
		}
	case 6:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B3;
			break;
		}
	case 7:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B2;
			break;
		}
	case 8:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_B1;
			break;
		}
	case 9:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_P0;
			break;
		}
	case 10:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_P1;
			break;
		}
	case 11:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_P2;
			break;
		}
	case 12:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_P3;
			break;
		}
	case 13:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_P4;
			break;
		}
	case 14:
		{
			m_byteWriteFrame2[DATASTART]=MCLEVAL_ERR;
			break;
		}
	}

	//坡起
	if (m_CSlopeStart.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xBF;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x40;
	}
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xBF;
	}

	//高加速
	if (m_CHighAcc.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xDF;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x20;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xDF;
	}
	
	//警惕
	if (m_CGuard.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xEF;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x10;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xEF;
	}

	//左门门选
	if (m_CLeftSelect.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xF7;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x08;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xF7;
	}

	//开左门
	if (m_CLeftOpen.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFB;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x04;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFB;
	}

	//右门门选
	if (m_CRightSelect.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFD;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x02;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFD;
	}

	//开右门
	if (m_CRightOpen.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFE;
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]|0x01;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+1]=m_byteWriteFrame2[DATASTART+1]&0xFE;
	}

	//ATO自动折返继电器使能
	if (m_CATOAutoReturn.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0x7F;
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x80;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0x7F;
	}

	//ATO模式继电器使能
	if (m_CATOMode.GetCheck())
	{
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xBF;
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x40;
	} 
	else
	{
		m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xBF;
	}

	//ATO自动折返指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	switch (m_natoautoreturnled)
	{
	case 0:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xCF;
			break;
		}
	case 0x1F:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xCF;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x1F;
			break;
		}
	case 0x2F:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xCF;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x2F;
			break;
		}
	}

	//ATO启动按钮指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。

	switch (m_natostartled)
	{
	case 0:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xF3;
			break;
		}
	case 0x04:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xF3;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x04;
			break;
		}
	case 0x08:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xF3;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x08;
			break;
		}
	}
	//MA模式指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	switch (m_nmamodeled)
	{
	case 0:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xFC;
			break;
		}
	case 0x01:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xFC;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x01;
			break;
		}
	case 0x02:
		{
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]&0xFC;
			m_byteWriteFrame2[DATASTART+2]=m_byteWriteFrame2[DATASTART+2]|0x02;
			break;
		}
	}

	//ATO故障指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。

	switch (m_natofaultled)
	{
	case 0:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xF3;
			break;
		}
	case 0x04:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xF3;
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]|0x04;
			break;
		}
	case 0x08:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xF3;
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]|0x08;
			break;
		}
	}
	//ATP模式输出00b：0模式，10b：RM 11b：CM，01b：无效
	switch (m_natpmodeout)
	{
	case 0:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xFC;
			break;
		}
	case 0x01:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xFC;
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]|0x01;
			break;
		}
	case 0x02:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xFC;
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]|0x02;
			break;
		}
	case 0x03:
		{
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]&0xFC;
			m_byteWriteFrame2[DATASTART+3]=m_byteWriteFrame2[DATASTART+3]|0x03;
			break;
		}
	}
	
	m_bSendPackage=TRUE;

}

void CATOCTRLDlg::OnCheckreturnlednolight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOAutoReturnLedNoLight.GetCheck())
	{
		m_CATOAutoReturnLedGlint.SetCheck(0);
		m_CATOAutoReturnLedLight.SetCheck(0);
		m_natoautoreturnled=0;
	} 
	else
	{
		
	}
}

void CATOCTRLDlg::OnCheckreturnglint() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOAutoReturnLedGlint.GetCheck())
	{
		m_CATOAutoReturnLedNoLight.SetCheck(0);
		m_CATOAutoReturnLedLight.SetCheck(0);
		m_natoautoreturnled=0x1F;
	} 
	else
	{
		m_natoautoreturnled=0;
	}
}

void CATOCTRLDlg::OnCheckreturnlight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOAutoReturnLedLight.GetCheck())
	{
		m_CATOAutoReturnLedNoLight.SetCheck(0);
		m_CATOAutoReturnLedGlint.SetCheck(0);
		m_natoautoreturnled=0x2F;
	} 
	else
	{
		m_natoautoreturnled=0;
	}
}

void CATOCTRLDlg::OnCheckatostartlednolight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOStartLedNoLight.GetCheck())
	{
		m_CATOStartLedGlint.SetCheck(0);
		m_CATOStartLedLight.SetCheck(0);
		m_natostartled=0;
	} 
	else
	{
	}
}

void CATOCTRLDlg::OnCheckatostartglint() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOStartLedGlint.GetCheck())
	{
		m_CATOStartLedNoLight.SetCheck(0);
		m_CATOStartLedLight.SetCheck(0);
		m_natostartled=0x04;
	} 
	else
	{
		m_natostartled=0;
	}
}

void CATOCTRLDlg::OnCheckatostartlight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOStartLedLight.GetCheck())
	{
		m_CATOStartLedGlint.SetCheck(0);
		m_CATOStartLedNoLight.SetCheck(0);
		m_natostartled=0x08;
	} 
	else
	{
		m_natostartled=0;
	}
}

void CATOCTRLDlg::OnCheckmamodelednolight() 
{
	// TODO: Add your control notification handler code here
	if (m_CMAModeNoLight.GetCheck())
	{
		m_CMAModeGlint.SetCheck(0);
		m_CMAModeLight.SetCheck(0);
		m_nmamodeled=0;
	} 
	else
	{
		m_nmamodeled=0;
	}
}

void CATOCTRLDlg::OnCheckmamodeglint() 
{
	// TODO: Add your control notification handler code here
	if (m_CMAModeGlint.GetCheck())
	{
		m_CMAModeNoLight.SetCheck(0);
		m_CMAModeLight.SetCheck(0);
		m_nmamodeled=0x01;
	} 
	else
	{
		m_nmamodeled=0;
	}
}

void CATOCTRLDlg::OnCheckmamodelight() 
{
	// TODO: Add your control notification handler code here
	if (m_CMAModeLight.GetCheck())
	{
		m_CMAModeNoLight.SetCheck(0);
		m_CMAModeGlint.SetCheck(0);
		m_nmamodeled=0x02;
	} 
	else
	{
		m_nmamodeled=0;
	}
}

void CATOCTRLDlg::OnCheckatofaultlednolight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOFaultNoLight.GetCheck())
	{
		m_CATOFaultGlint.SetCheck(0);
		m_CATOFaultLight.SetCheck(0);
		m_natofaultled=0;
	} 
	else
	{
		m_natofaultled=0;
	}
}

void CATOCTRLDlg::OnCheckatofaultglint() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOFaultGlint.GetCheck())
	{
		m_CATOFaultNoLight.SetCheck(0);
		m_CATOFaultLight.SetCheck(0);
		m_natofaultled=0x04;
	} 
	else
	{
		m_natofaultled=0;
	}
}

void CATOCTRLDlg::OnCheckatofaultlight() 
{
	// TODO: Add your control notification handler code here
	if (m_CATOFaultLight.GetCheck())
	{
		m_CATOFaultGlint.SetCheck(0);
		m_CATOFaultNoLight.SetCheck(0);
		m_natofaultled=0x08;
	} 
	else
	{
		m_natofaultled=0;
	}
}

void CATOCTRLDlg::OnCheckzeromode() 
{
	// TODO: Add your control notification handler code here
	if (m_CATPModeZero.GetCheck())
	{
		m_CATPModeCM.SetCheck(0);
		m_CATPModeNo.SetCheck(0);
		m_CATPModeRM.SetCheck(0);
		m_natpmodeout=0;
	} 
	else
	{
		m_natpmodeout=0;
	}
}

void CATOCTRLDlg::OnCheckrmmode() 
{
	// TODO: Add your control notification handler code here
	if (m_CATPModeRM.GetCheck())
	{
		m_CATPModeCM.SetCheck(0);
		m_CATPModeNo.SetCheck(0);
		m_CATPModeZero.SetCheck(0);
		m_natpmodeout=0x02;
	} 
	else
	{
		m_natpmodeout=0;
	}
}

void CATOCTRLDlg::OnCheckcmmode() 
{
	// TODO: Add your control notification handler code here
	if (m_CATPModeCM.GetCheck())
	{
		m_CATPModeRM.SetCheck(0);
		m_CATPModeNo.SetCheck(0);
		m_CATPModeZero.SetCheck(0);
		m_natpmodeout=0x03;
	} 
	else
	{
		m_natpmodeout=0;
	}
}

void CATOCTRLDlg::OnChecknomode() 
{
	// TODO: Add your control notification handler code here
	if (m_CATPModeNo.GetCheck())
	{
		m_CATPModeRM.SetCheck(0);
		m_CATPModeCM.SetCheck(0);
		m_CATPModeZero.SetCheck(0);
		m_natpmodeout=0x01;
	} 
	else
	{
		m_natpmodeout=0;
	}
}

void CATOCTRLDlg::FrameParse(byte *m_byte)
{
	//控车档位输出回采
	if (m_byteFrame[FRAMEPOS]==0xE0)
	{
		m_CMCOutReturn.SetWindowText(FindMC(m_byteFrame[DATASTART]));
		//坡起
		m_CReturnSlopeStart.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],7));
		//高加速
		m_CReturnHighAce.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],6));
		//警惕
		m_CReturnGuard.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],5));
		//左门门选
		m_CReturnLeftSelect.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],4));
		//开左门
		m_CReturnLeftOpen.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],3));
		//右门门选
		m_CReturnRightSelect.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],2));
		//开右门
		m_CReturnRightOpen.SetCheck(JudgeBit(m_byteFrame[DATASTART+1],1));

		//ATO自动折返继电器使能
		m_CReturnAutoReturnRalayEnable.SetCheck(JudgeBit(m_byteFrame[DATASTART+2],8));
		//ATO模式继电器使能
		m_CReturnATOMode.SetCheck(JudgeBit(m_byteFrame[DATASTART+2],7));
		//ATO自动折返指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
		BYTE m_bytetmp;
		m_bytetmp=m_byteFrame[DATASTART+2]&0x30;
		m_nreturnatoautoreturnled=m_bytetmp;
		switch (m_bytetmp)
		{
		case 0:
			{
				m_CReturnATOAutoReturnLed.SetWindowText("熄灭");
				break;
			}
		case 0x10:
			{
				m_CReturnATOAutoReturnLed.SetWindowText("闪烁");
				break;
			}
		case 0x20:
			{
				m_CReturnATOAutoReturnLed.SetWindowText("常亮");
				break;
			}
		}
		
		//ATO启动按钮指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
		m_bytetmp=m_byteFrame[DATASTART+2]&0x0C;
		m_nreturnatostartled=m_bytetmp;
		switch (m_bytetmp)
		{
		case 0:
			{
				m_CReturnATOStartLed.SetWindowText("熄灭");
				break;
			}
		case 0x04:
			{
				m_CReturnATOStartLed.SetWindowText("闪烁");
				break;
			}
		case 0x08:
			{
				m_CReturnATOStartLed.SetWindowText("常亮");
				break;
			}
		}
		//MA模式指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
		m_bytetmp=m_byteFrame[DATASTART+2]&0x03;
		m_nreturnmamodeled=m_bytetmp;
		switch (m_bytetmp)
		{
		case 0:
			{
				m_CReturnMAModeLed.SetWindowText("熄灭");
				break;
			}
		case 0x01:
			{
				m_CReturnMAModeLed.SetWindowText("闪烁");
				break;
			}
		case 0x02:
			{
				m_CReturnMAModeLed.SetWindowText("常亮");
				break;
			}
		}
		//ATO故障指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
		m_bytetmp=m_byteFrame[DATASTART+3]&0x0C;
		m_nreturnatofaultled=m_bytetmp;
		switch (m_bytetmp)
		{
		case 0:
			{
				m_CReturnATOFaultLed.SetWindowText("熄灭");
				break;
			}
		case 0x04:
			{
				m_CReturnATOFaultLed.SetWindowText("闪烁");
				break;
			}
		case 0x08:
			{
				m_CReturnATOFaultLed.SetWindowText("常亮");
				break;
			}
		}
		//ATP模式输出00b：0模式，10b：RM 11b：CM，01b：无效
		m_bytetmp=m_byteFrame[DATASTART+3]&0x0C;
		m_nreturnatpmodeout=m_bytetmp;
		switch (m_bytetmp)
		{
		case 0:
			{
				m_CReturnATPModeOutLed.SetWindowText("0模式");
				break;
			}
		case 0x01:
			{
				m_CReturnATPModeOutLed.SetWindowText("无效");
				break;
			}
		case 0x02:
			{
				m_CReturnATPModeOutLed.SetWindowText("RM");
				break;
			}
		case 0x03:
			{
				m_CReturnATPModeOutLed.SetWindowText("CM");
				break;
			}
		}

		//坡起
		m_CReturnSlopeStartre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],7));
		//高加速
		m_CReturnHighAcere.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],6));
		//警惕
		m_CReturnGuardre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],5));
		//左门门选
		m_CReturnLeftSelectre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],4));
		//开左门
		m_CReturnLeftOpenre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],3));
		//右门门选
		m_CReturnRightSelectre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],2));
		//开右门
		m_CReturnRightOpenre.SetCheck(JudgeBit(m_byteFrame[DATASTART+4],1));

		//ATO模式继电器回采有效位1：有效 0：故障
		if (JudgeBit(m_byteFrame[DATASTART+5],7))
		{
			m_CReturnATOModeEffect.SetWindowText("有效");
			m_bReturnATOModeEffect=TRUE;
		} 
		else
		{
			m_CReturnATOModeEffect.SetWindowText("故障");
			m_bReturnATOModeEffect=FALSE;
		}
		//ATO模式继电器回采状态位1：励磁0：失磁
		if (JudgeBit(m_byteFrame[DATASTART+5],6))
		{
			m_CReturnATOModeStatus.SetWindowText("有效");
			m_bReturnATOModeStatus=TRUE;
		} 
		else
		{
			m_CReturnATOModeStatus.SetWindowText("故障");
			m_bReturnATOModeStatus=FALSE;
		}
		//自动折返继电器回采有效位1：有效	0：故障
		if (JudgeBit(m_byteFrame[DATASTART+5],5))
		{
			m_CReturnATOAutoReturnEffect.SetWindowText("有效");
			m_bReturnATOAutoReturnEffect=TRUE;
		} 
		else
		{
			m_CReturnATOAutoReturnEffect.SetWindowText("故障");
			m_bReturnATOAutoReturnEffect=FALSE;
		}
		//自动折返继电器回采状态位1：励磁	0：失磁

		if (JudgeBit(m_byteFrame[DATASTART+5],4))
		{
			m_CReturnATOAutoReturnStatus.SetWindowText("有效");
			m_bReturnATOAutoReturnStatus=TRUE;
		} 
		else
		{
			m_CReturnATOAutoReturnStatus.SetWindowText("故障");
			m_bReturnATOAutoReturnStatus=FALSE;
		}
		//取消自动折返按钮状态位1：按下 0：抬起
		if (JudgeBit(m_byteFrame[DATASTART+5],3))
		{
			m_CReturnCancelAutoReturnStatus.SetWindowText("按下");
			m_bReturnCancelAutoReturnStatus=TRUE;
		} 
		else
		{
			m_CReturnCancelAutoReturnStatus.SetWindowText("抬起");
			m_bReturnCancelAutoReturnStatus=FALSE;
		}
		//ATO启动按钮状态1：按下 0：抬起
		if (JudgeBit(m_byteFrame[DATASTART+5],2))
		{
			m_CReturnATOStartStatus.SetWindowText("按下");
			m_bReturnATOStartStatus=TRUE;
		} 
		else
		{
			m_CReturnATOStartStatus.SetWindowText("抬起");
			m_bReturnATOStartStatus=FALSE;
		}
		//MA模式按钮状态位	1：按下 0：抬起
		if (JudgeBit(m_byteFrame[DATASTART+5],1))
		{
			m_CReturnMAModeStatus.SetWindowText("按下");
			m_bReturnMAModeStatus=TRUE;
		} 
		else
		{
			m_CReturnMAModeStatus.SetWindowText("抬起");
			m_bReturnMAModeStatus=FALSE;
		}
		m_CReturnMCATOFaceUnit.SetWindowText(FindMC(m_byteFrame[DATASTART+6]));
		m_CReturnMC.SetWindowText(FindMC(m_byteFrame[DATASTART+7]));
	}
}

CString CATOCTRLDlg::FindMC(byte m_mcbyte)
{
	CString m_str;
	switch(m_mcbyte)
	{
	case MCLEVAL_ERR:
		{
			m_str="ERR";
			break;
		}
	case MCLEVAL_B1:
		{
			m_str="B1";
			break;
		}
	case MCLEVAL_B2:
		{
			m_str="B2";
			break;
		}
	case MCLEVAL_B3:
		{
			m_str="B3";
			break;
		}
	case MCLEVAL_B4:
		{
			m_str="B4";
			break;
		}
	case MCLEVAL_B5:
		{
			m_str="B5";
			break;
		}
	case MCLEVAL_B6:
		{
			m_str="B6";
			break;
		}
	case MCLEVAL_B7:
		{
			m_str="B7";
			break;
		}
	case MCLEVAL_P0:
		{
			m_str="P0";
			break;
		}
	case MCLEVAL_P1:
		{
			m_str="P1";
			break;
		}
	case MCLEVAL_P2:
		{
			m_str="P2";
			break;
		}
	case MCLEVAL_P3:
		{
			m_str="P3";
			break;
		}
	case MCLEVAL_P4:
		{
			m_str="P4";
			break;
		}
	case MCLEVAL_EB:
		{
			m_str="EB";
			break;
		}
	}
	return m_str;
}

BOOL CATOCTRLDlg::JudgeBit(byte m_bytedata, byte m_bytebit)
{
	BYTE m_byte=0;
	m_bytebit--;
	m_byte=(m_bytedata>>m_bytebit) & 0x01;
	return	m_byte;
}

void CATOCTRLDlg::OnButtonontimesend() 
{
	// TODO: Add your control notification handler code here
	if (m_bSendOnTime==FALSE)
	{
		m_bSendOnTime=TRUE;
		SetTimer(1,10,NULL);
		m_COnTimeSend.SetWindowText("关闭定时发送");
	} 
	else
	{
		m_bSendOnTime=FALSE;
		KillTimer(1);
		m_COnTimeSend.SetWindowText("开启定时发送");
	}
}

void CATOCTRLDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
		{
			if (m_bSendOnTime==TRUE)
			{
				OnButtonsend();
				m_bSendPackage=TRUE;
			} 
			else
			{
				m_bSendPackage=FALSE;
			}
			break;
		}
	case 2:
		{
			InvalidateRgn(&m_rgnatoautoreturnled,FALSE);
			InvalidateRgn(&m_rgnatostartled,FALSE);
			InvalidateRgn(&m_rgnmamodeled,FALSE);
			InvalidateRgn(&m_rgnatofaultled,FALSE);
			InvalidateRgn(&m_rgnatpmodeoutled,FALSE);
			InvalidateRgn(&m_rgnReturnATOModeEffect,FALSE);
			InvalidateRgn(&m_rgnReturnATOModeStatus,FALSE);
			InvalidateRgn(&m_rgnReturnATOAutoReturnEffect,FALSE);
			InvalidateRgn(&m_rgnReturnATOAutoReturnStatus,FALSE);
			InvalidateRgn(&m_rgnReturnATOStartStatus,FALSE);
			InvalidateRgn(&m_rgnReturnMAModeStatus,FALSE);
			InvalidateRgn(&m_rgnReturnCancelAutoReturnStatus,FALSE);
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CATOCTRLDlg::DrawLED(CPaintDC &dc)
{
	CBrush m_BrushGreen(RGB( 0, 255, 127));
	CBrush m_BrushBlack(RGB( 190, 190, 190));
	CBrush m_BrushBlue(RGB(0,191,255));
	CBrush m_BrushYellow(RGB(0,238,238));
	//ATO自动折返指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。

	switch (m_nreturnatoautoreturnled)
	{
	case 0:
		{
			dc.FillRgn(&m_rgnatoautoreturnled,&m_BrushBlack);
			break;
		}
	case 0x10:
		{
			dc.FillRgn(&m_rgnatoautoreturnled,&m_BrushBlue);
			break;
		}
	case 0x20:
		{
			dc.FillRgn(&m_rgnatoautoreturnled,&m_BrushGreen);
			break;
		}
		
	}

	switch (m_nreturnatostartled)
	{
	case 0:
		{
			dc.FillRgn(&m_rgnatostartled,&m_BrushBlack);
			break;
		}
	case 0x04:
		{
			dc.FillRgn(&m_rgnatostartled,&m_BrushBlue);
			break;
		}
	case 0x08:
		{
			dc.FillRgn(&m_rgnatostartled,&m_BrushGreen);
			break;
		}
	}
	

	switch (m_nreturnmamodeled)
	{
	case 0:
		{
			dc.FillRgn(&m_rgnmamodeled,&m_BrushBlack);
			break;
		}
	case 0x01:
		{
			dc.FillRgn(&m_rgnmamodeled,&m_BrushBlue);
			break;
		}
	case 0x02:
		{
			dc.FillRgn(&m_rgnmamodeled,&m_BrushGreen);
			break;
		}
	}

	
	switch (m_nreturnatofaultled)
	{
	case 0:
		{
			dc.FillRgn(&m_rgnatofaultled,&m_BrushBlack);
			break;
		}
	case 0x04:
		{
			dc.FillRgn(&m_rgnatofaultled,&m_BrushBlue);
			break;
		}
	case 0x08:
		{
			dc.FillRgn(&m_rgnatofaultled,&m_BrushGreen);
			break;
		}
	}

	
	switch (m_nreturnatpmodeout)
	{
	case 0:
		{
			dc.FillRgn(&m_rgnatpmodeoutled,&m_BrushBlack);
			break;
		}
	case 0x01:
		{
			dc.FillRgn(&m_rgnatpmodeoutled,&m_BrushBlue);
			break;
		}
	case 0x02:
		{
			dc.FillRgn(&m_rgnatpmodeoutled,&m_BrushGreen);
			break;
		}
	case 0x03:
		{
			dc.FillRgn(&m_rgnatpmodeoutled,&m_BrushYellow);
			break;
		}
	}

		//ATO模式继电器回采有效位1：有效 0：故障
		if (m_bReturnATOModeEffect==TRUE)
		{
			dc.FillRgn(&m_rgnReturnATOModeEffect,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnATOModeEffect,&m_BrushBlack);
		}
		//ATO模式继电器回采状态位1：励磁0：失磁
		if (m_bReturnATOModeStatus==TRUE)
		{
			dc.FillRgn(&m_rgnReturnATOModeStatus,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnATOModeStatus,&m_BrushBlack);
		}
		//自动折返继电器回采有效位1：有效	0：故障
		if (m_bReturnATOAutoReturnEffect==TRUE)
		{
			
			dc.FillRgn(&m_rgnReturnATOAutoReturnEffect,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnATOAutoReturnEffect,&m_BrushBlack);
		}
		//自动折返继电器回采状态位1：励磁	0：失磁
		if (m_bReturnATOAutoReturnStatus==TRUE)
		{
			dc.FillRgn(&m_rgnReturnATOAutoReturnStatus,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnATOAutoReturnStatus,&m_BrushBlack);
		}
		//取消自动折返按钮状态位1：按下 0：抬起
		if (m_bReturnCancelAutoReturnStatus==TRUE)
		{
			dc.FillRgn(&m_rgnReturnCancelAutoReturnStatus,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnCancelAutoReturnStatus,&m_BrushBlack);
		}
		//ATO启动按钮状态1：按下 0：抬起
		if (m_bReturnATOStartStatus==TRUE)
		{
			dc.FillRgn(&m_rgnReturnATOStartStatus,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnATOStartStatus,&m_BrushBlack);
		}
		//MA模式按钮状态位	1：按下 0：抬起
		if (m_bReturnMAModeStatus==TRUE)
		{
			dc.FillRgn(&m_rgnReturnMAModeStatus,&m_BrushGreen);
		} 
		else
		{
			dc.FillRgn(&m_rgnReturnMAModeStatus,&m_BrushBlack);
		}
		/*
		InvalidateRgn(&m_rgnatoautoreturnled,FALSE);
		InvalidateRgn(&m_rgnatostartled,FALSE);
		InvalidateRgn(&m_rgnmamodeled,FALSE);
		InvalidateRgn(&m_rgnatofaultled,FALSE);
		InvalidateRgn(&m_rgnatpmodeoutled,FALSE);
		InvalidateRgn(&m_rgnReturnATOModeEffect,FALSE);
		InvalidateRgn(&m_rgnReturnATOModeStatus,FALSE);
		InvalidateRgn(&m_rgnReturnATOAutoReturnEffect,FALSE);
		InvalidateRgn(&m_rgnReturnATOAutoReturnStatus,FALSE);
		InvalidateRgn(&m_rgnReturnATOStartStatus,FALSE);
		InvalidateRgn(&m_rgnReturnMAModeStatus,FALSE);
		InvalidateRgn(&m_rgnReturnCancelAutoReturnStatus,FALSE);
		*/

}
