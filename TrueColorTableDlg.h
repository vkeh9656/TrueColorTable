
// TrueColorTableDlg.h: 헤더 파일
//

#pragma once


// CTrueColorTableDlg 대화 상자
class CTrueColorTableDlg : public CDialogEx
{
private:
	CImage m_color_image;
	unsigned int* mp_color_pattern, *mp_pick_pattern; // RGB를 한번에 묶어쓰기 위해 unsigned int형으로 선언
	
	CImage m_draw_image, m_pick_image;
	CDC m_draw_dc;

	CRect m_view_rect;

	CPoint m_prev_pos;
	unsigned char m_is_clicked = 0;

// 생성입니다.
public:
	CTrueColorTableDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRUECOLORTABLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};
