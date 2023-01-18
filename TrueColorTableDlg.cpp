
// TrueColorTableDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TrueColorTable.h"
#include "TrueColorTableDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTrueColorTableDlg 대화 상자



CTrueColorTableDlg::CTrueColorTableDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRUECOLORTABLE_DIALOG, pParent), m_view_rect(0, 0, 160, 128)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTrueColorTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrueColorTableDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTrueColorTableDlg 메시지 처리기

BOOL CTrueColorTableDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_color_image.Create(4096, 4096, 32);
	m_draw_image.Create(1920, 1080, 32);
	m_pick_image.Create(16, 16, 32);

	m_draw_dc.Attach(m_draw_image.GetDC());

	
	BITMAP bmp_info;
	GetObject((HBITMAP)m_color_image, sizeof(BITMAP), &bmp_info);
	mp_color_pattern = (unsigned int*)m_color_image.GetBits();
	mp_color_pattern += bmp_info.bmWidthBytes / sizeof(unsigned int) - 1;

	
	GetObject((HBITMAP)m_pick_image, sizeof(BITMAP), &bmp_info);
	mp_pick_pattern = (unsigned int*)m_pick_image.GetBits();
	mp_pick_pattern += bmp_info.bmWidthBytes / sizeof(unsigned int) - 1;

	unsigned int* p = mp_color_pattern;
	int x = 0, y = 0, step = 0;
	for (int i = 0; i < 4096 * 4096; i++)
	{
		*(p - y * 4096 - x - step * 256) = i;
		x = (x + 1) % 256;
		if (!x) y++;
		if (y >= 4096) 
		{
			y = 0;
			step++;
		}
		/**p-- = i;*/
	}

	unsigned int* p_src = mp_color_pattern;
	unsigned int* p_dest = mp_pick_pattern;

	for (int i = 0; i < 256; i++)
	{
		if (i && !(i % 16)) p_src -= 4096 * 255;
		*p_dest = *p_src;

		p_src -= 256;
		p_dest--;
	}

	m_color_image.Draw(m_draw_dc, 0, 0, 512, 512);
	m_color_image.Draw(m_draw_dc, 512, 0, 256 * 5, 256 * 4, 0, 0, 256 * 5, 256 * 4);
	m_pick_image.Draw(m_draw_dc, 0, 512, 512, 512);

	m_draw_dc.SelectStockObject(NULL_BRUSH);
	m_draw_dc.SelectStockObject(WHITE_PEN);
	m_draw_dc.Rectangle(m_view_rect);

	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTrueColorTableDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{	

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_draw_image.Draw(dc, 0, 0);

		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTrueColorTableDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTrueColorTableDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_view_rect.PtInRect(point))
	{
		m_is_clicked = 1;
		m_prev_pos = point;
		SetCapture();
	}
	else if (CRect(512, 0, 512 + 256, 256).PtInRect(point))
	{
		m_is_clicked = 2;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTrueColorTableDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_is_clicked ==1) ReleaseCapture();
	
	m_is_clicked = 0;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CTrueColorTableDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_is_clicked == 1) 
	{
		CPoint pos = point - m_prev_pos;
		m_view_rect += pos;
		m_prev_pos = point;

		if (m_view_rect.left < 0)
		{
			m_view_rect.right -= m_view_rect.left;
			m_view_rect.left = 0;
		}
		else if (m_view_rect.right > 512)
		{
			m_view_rect.right = 512;
			m_view_rect.left = m_view_rect.right - 160;
		}

		if (m_view_rect.top < 0)
		{
			m_view_rect.bottom -= m_view_rect.top;
			m_view_rect.top = 0;
		}
		else if (m_view_rect.bottom > 512)
		{
			m_view_rect.bottom = 512;
			m_view_rect.top = m_view_rect.bottom - 128;
		}

		m_color_image.Draw(m_draw_dc, 0, 0, 512, 512);
		m_color_image.Draw(m_draw_dc, 512, 0, 256 * 5, 256 * 4, m_view_rect.left << 3, m_view_rect.top << 3, 256 * 5, 256 * 4);
		m_pick_image.Draw(m_draw_dc, 0, 512, 512, 512);

		m_draw_dc.SelectStockObject(NULL_BRUSH);
		m_draw_dc.SelectStockObject(WHITE_PEN);
		m_draw_dc.Rectangle(m_view_rect);
		
		CClientDC dc(this);
		m_draw_image.Draw(dc, 0, 0);
	}

	else if (m_is_clicked == 2 && CRect(512, 0, 512 + 256, 256).PtInRect(point))
	{
		unsigned int* p_src = mp_color_pattern - point.y * 4096 
							- (255 - (point.x - 512));
		unsigned int* p_dest = mp_pick_pattern;

		for (int i = 0; i < 256; i++)
		{
			if (i && !(i % 16)) p_src -= 4096 * 255;

			*p_dest = *p_src;
			p_src -= 256;
			p_dest--;
		}

		m_pick_image.Draw(m_draw_dc, 0, 512, 512, 512);
		CClientDC dc(this);
		m_draw_image.Draw(dc, 0, 0);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CTrueColorTableDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
}
