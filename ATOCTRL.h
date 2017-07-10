// ATOCTRL.h : main header file for the ATOCTRL application
//

#if !defined(AFX_ATOCTRL_H__F2AB9CCB_65FD_4CC1_89CE_02F015DC5F0A__INCLUDED_)
#define AFX_ATOCTRL_H__F2AB9CCB_65FD_4CC1_89CE_02F015DC5F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CATOCTRLApp:
// See ATOCTRL.cpp for the implementation of this class
//

class CATOCTRLApp : public CWinApp
{
public:
	CATOCTRLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATOCTRLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CATOCTRLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATOCTRL_H__F2AB9CCB_65FD_4CC1_89CE_02F015DC5F0A__INCLUDED_)
