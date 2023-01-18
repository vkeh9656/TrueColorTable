
// TrueColorTable.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "TrueColorTable.h"
#include "TrueColorTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTrueColorTableApp

BEGIN_MESSAGE_MAP(CTrueColorTableApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTrueColorTableApp 생성

CTrueColorTableApp::CTrueColorTableApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTrueColorTableApp 개체입니다.

CTrueColorTableApp theApp;


// CTrueColorTableApp 초기화

BOOL CTrueColorTableApp::InitInstance()
{
	CWinApp::InitInstance();

	CTrueColorTableDlg dlg;
	dlg.DoModal();
	return FALSE;
}

