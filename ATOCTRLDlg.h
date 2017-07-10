// ATOCTRLDlg.h : header file
//

#if !defined(AFX_ATOCTRLDLG_H__5049E3F6_4F8C_4A1D_8A80_7BB33E11955F__INCLUDED_)
#define AFX_ATOCTRLDLG_H__5049E3F6_4F8C_4A1D_8A80_7BB33E11955F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CATOCTRLDlg dialog

class CATOCTRLDlg : public CDialog
{
// Construction
public:
	void DrawLED(CPaintDC &dc);
	BOOL JudgeBit(byte m_bytedata , byte m_bytebit);
	CString FindMC(byte m_mcbyte);
	void FrameParse(byte * m_byte);
	CATOCTRLDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL m_bSendOnTime;
	//ATO自动折返指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	int m_natoautoreturnled;
	CRgn m_rgnatoautoreturnled;
	int m_nreturnatoautoreturnled;
	//ATO启动按钮指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	int m_natostartled;
	CRgn m_rgnatostartled;
	int m_nreturnatostartled;
	//MA模式指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	int m_nmamodeled;
	CRgn m_rgnmamodeled;
	int m_nreturnmamodeled;
	//ATO故障指示灯，00=熄灭，01=1Hz频率的闪烁，10=常亮。
	int m_natofaultled;
	CRgn m_rgnatofaultled;
	int m_nreturnatofaultled;
	//ATP模式输出00b：0模式，10b：RM 11b：CM，01b：无效
	int m_natpmodeout;
	CRgn m_rgnatpmodeoutled;
	int m_nreturnatpmodeout;

	CRgn	m_rgnReturnMAModeStatus;
	CRgn	m_rgnReturnATOStartStatus;
	CRgn	m_rgnReturnCancelAutoReturnStatus;
	CRgn	m_rgnReturnATOAutoReturnEffect;
	CRgn	m_rgnReturnATOAutoReturnStatus;
	CRgn	m_rgnReturnATOModeStatus;
	CRgn	m_rgnReturnATOModeEffect;

	BOOL	m_bReturnMAModeStatus;
	BOOL	m_bReturnATOStartStatus;
	BOOL	m_bReturnCancelAutoReturnStatus;
	BOOL	m_bReturnATOAutoReturnEffect;
	BOOL	m_bReturnATOAutoReturnStatus;
	BOOL	m_bReturnATOModeStatus;
	BOOL	m_bReturnATOModeEffect;


	//串口相关变量和函数开始
	CString m_strshowRX;
	void InitFrame();
	void Init();
	LRESULT OnThreadRXMessage(WPARAM wParam,LPARAM lParam);
	//串口相关变量和函数结束

// Dialog Data
	//{{AFX_DATA(CATOCTRLDlg)
	enum { IDD = IDD_ATOCTRL_DIALOG };
	CButton	m_COnTimeSend;
	CEdit	m_CReturnMC;
	CEdit	m_CReturnMCATOFaceUnit;
	CEdit	m_CReturnMAModeStatus;
	CEdit	m_CReturnATOStartStatus;
	CEdit	m_CReturnCancelAutoReturnStatus;
	CEdit	m_CReturnATOAutoReturnEffect;
	CEdit	m_CReturnATOAutoReturnStatus;
	CEdit	m_CReturnATOModeStatus;
	CEdit	m_CReturnATOModeEffect;
	CButton	m_CReturnGuardre;
	CButton	m_CReturnHighAcere;
	CButton	m_CReturnLeftOpenre;
	CButton	m_CReturnLeftSelectre;
	CButton	m_CReturnRightOpenre;
	CButton	m_CReturnRightSelectre;
	CButton	m_CReturnSlopeStartre;
	CEdit	m_CReturnATPModeOutLed;
	CEdit	m_CReturnATOFaultLed;
	CEdit	m_CReturnMAModeLed;
	CEdit	m_CReturnATOStartLed;
	CEdit	m_CReturnATOAutoReturnLed;
	CButton	m_CReturnATOMode;
	CButton	m_CReturnAutoReturnRalayEnable;
	CButton	m_CReturnRightSelect;
	CButton	m_CReturnRightOpen;
	CButton	m_CReturnLeftSelect;
	CButton	m_CReturnLeftOpen;
	CButton	m_CReturnGuard;
	CButton	m_CReturnHighAce;
	CButton	m_CReturnSlopeStart;
	CEdit	m_CMCOutReturn;
	CButton	m_CATPModeCM;
	CButton	m_CATPModeNo;
	CButton	m_CATPModeRM;
	CButton	m_CATPModeZero;
	CButton	m_CATOFaultLight;
	CButton	m_CATOFaultNoLight;
	CButton	m_CATOFaultGlint;
	CButton	m_CMAModeLight;
	CButton	m_CMAModeNoLight;
	CButton	m_CMAModeGlint;
	CButton	m_CATOStartLedLight;
	CButton	m_CATOStartLedGlint;
	CButton	m_CATOStartLedNoLight;
	CButton	m_CATOAutoReturnLedLight;
	CButton	m_CATOAutoReturnLedGlint;
	CButton	m_CATOAutoReturnLedNoLight;
	CButton	m_CATOMode;
	CButton	m_CATOAutoReturn;
	CButton	m_CRightSelect;
	CButton	m_CRightOpen;
	CButton	m_CLeftOpen;
	CButton	m_CLeftSelect;
	CButton	m_CGuard;
	CButton	m_CHighAcc;
	CButton	m_CSlopeStart;
	CSliderCtrl	m_CMCLeval;
	CRichEditCtrl	m_CTX;
	CButton	m_CcomButton;
	CComboBox	m_Ccomportnumber;
	CRichEditCtrl	m_CRX;
	CEdit	m_CRXCounterByte;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATOCTRLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CATOCTRLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtoncom();
	afx_msg void OnSelchangeCombocomportnumber();
	afx_msg void OnButtonsend();
	afx_msg void OnCheckreturnlednolight();
	afx_msg void OnCheckreturnglint();
	afx_msg void OnCheckreturnlight();
	afx_msg void OnCheckatostartlednolight();
	afx_msg void OnCheckatostartglint();
	afx_msg void OnCheckatostartlight();
	afx_msg void OnCheckmamodelednolight();
	afx_msg void OnCheckmamodeglint();
	afx_msg void OnCheckmamodelight();
	afx_msg void OnCheckatofaultlednolight();
	afx_msg void OnCheckatofaultglint();
	afx_msg void OnCheckatofaultlight();
	afx_msg void OnCheckzeromode();
	afx_msg void OnCheckrmmode();
	afx_msg void OnCheckcmmode();
	afx_msg void OnChecknomode();
	afx_msg void OnButtonontimesend();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnReceiveAComPort(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnWriteComPortMSG(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATOCTRLDLG_H__5049E3F6_4F8C_4A1D_8A80_7BB33E11955F__INCLUDED_)
