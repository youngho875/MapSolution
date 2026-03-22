// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해
// 추가적으로 제공되는 내용입니다.
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MapSolutionView.h: CMapSolutionView 클래스의 인터페이스
//

#pragma once

#include "MapSolutionDoc.h"
#include "MainFrm.h"



struct MAP_STATUS_INFO
{
	BOOL bUseStatusBar;			// 사용 여부
	CString strStatusScale;		// 상태바 축척 정보
	CString strStatusCoord;		// 상태바 좌표 정보

	// 상태바 마우스 위치, 고도 정보
	double pGeo_x;
	double pGeo_y;
	double pGeo_z;

	MAP_STATUS_INFO()
	{
		bUseStatusBar = FALSE;
		strStatusScale = _T("");
		strStatusCoord = _T("");
		pGeo_x = 0.0;
		pGeo_y = 0.0;
		pGeo_z = 0.0;
	}
};

class CMapSolutionView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapSolutionView() noexcept;
	DECLARE_DYNCREATE(CMapSolutionView)

// 특성입니다.
public:
	CMapSolutionDoc* GetDocument() const;

// 작업입니다.
public:
	BOOL m_bDCInitialized;

	CDC* m_dcMain;
	CGDCGeo* m_dcMainTemp;
	CDC* m_dcMapLayer;
	CDC* m_dcShpLayer;
	CDC* m_dcDTEDLayer;
	CDC* m_dcRasterLayer;

	CCoordMgr* m_pCoordMgr;
	CCoord m_Coord;

	CBitmap	m_pBitmap4MainTemp;
	CBitmap	m_pBitmap4Map;
	CBitmap	m_pBitmap4Shape;
	CBitmap m_pBitmap4DTED;
	CBitmap m_pBitmap4Raster;
	CBitmap	m_pBitmap4Sym;

	CBitmap* m_pOldBitmap4MainTemp;
	CBitmap* m_pOldBitmap4Map;
	CBitmap* m_pOldBitmap4Shape;
	CBitmap* m_pOldBitmap4DTED;
	CBitmap* m_pOldBitmap4Raster;
	CBitmap* m_pOldBitmap4Sym;

	CBrush m_brush_10_10_10;

	CDC BufferDC;
	CBitmap bmpBuffer;
	
	CRect m_rect;
	CRect m_recSave;
	double 	m_fPrevScale;

	MAP_STATUS_INFO	m_pStatusInfo; /**< 상태바 정보 */

public:
	//CWMSEngine* m_pWMSEngine;
	CWMSClient* m_pWMSEngine;

	void ReInitializeDCs();
	void SetStatusBarTxt(CString strMsg);
	void UpdateStatusBar(CPoint point);
	void draw_maindc_only(CPoint ptNewPoint);
	void InitializeDisplayParameter(CRect pClientRect);

	void DrawSuit(CPoint pStartPoint);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapSolutionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL    m_bPanning;
	BOOL	m_bMapMove;
	POINT   m_lastPoint;

	CPoint m_ptMapMoveFirstPoint;
	CPoint m_ptPrevPoint;
	CPoint m_ptZoomWindowLT, m_ptZoomWindowBR;


// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	LRESULT OnTileDownloaded(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // MapSolutionView.cpp의 디버그 버전
inline CMapSolutionDoc* CMapSolutionView::GetDocument() const
   { return reinterpret_cast<CMapSolutionDoc*>(m_pDocument); }
#endif

