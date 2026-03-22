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

// MapSolutionView.cpp: CMapSolutionView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MapSolution.h"
#endif

#include "MapSolutionDoc.h"
#include "MapSolutionView.h"
#include "../generalinclude/Useful.h"
//#include "GPoint.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapSolutionView

IMPLEMENT_DYNCREATE(CMapSolutionView, CView)

BEGIN_MESSAGE_MAP(CMapSolutionView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapSolutionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_TILE_DOWNLOAD_COMPLETE, OnTileDownloaded)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMapSolutionView 생성/소멸

CMapSolutionView::CMapSolutionView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

	m_pCoordMgr = CCoordMgr::GetInst();
	m_Coord.initialize_adaptation_data();

	m_dcMain = NULL;
	m_dcMainTemp = NULL;
	m_dcMapLayer = NULL;
	m_dcShpLayer = NULL;
	m_dcDTEDLayer = NULL;
	m_dcRasterLayer = NULL;

	m_pOldBitmap4MainTemp = NULL;
	m_pOldBitmap4Map = NULL;
	m_pOldBitmap4Shape = NULL;
	m_pOldBitmap4DTED = NULL;
	m_pOldBitmap4Raster = NULL;
	m_pOldBitmap4Sym = NULL;

	m_brush_10_10_10.CreateSolidBrush(RGB(10, 10, 10));

	m_bDCInitialized = FALSE;

	m_pWMSEngine = NULL;

	m_bPanning = FALSE;
	m_bMapMove = FALSE;
	m_fPrevScale = 0.0;

}

CMapSolutionView::~CMapSolutionView()
{
	m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);

	m_pBitmap4MainTemp.DeleteObject();
	m_pBitmap4Map.DeleteObject();
	m_pBitmap4Shape.DeleteObject();
	m_pBitmap4DTED.DeleteObject();
	m_pBitmap4Raster.DeleteObject();
	//m_pBitmap4Sym.DeleteObject();

	//m_dcSymLayer->DeleteDC();
	m_dcRasterLayer->DeleteDC();
	m_dcDTEDLayer->DeleteDC();
	m_dcShpLayer->DeleteDC();
	m_dcMapLayer->DeleteDC();
	m_dcMainTemp->DeleteDC();

	::ReleaseDC(this->GetSafeHwnd(), m_dcMain->m_hDC);

	SAFE_DELETE(m_dcRasterLayer);
	SAFE_DELETE(m_dcDTEDLayer);
	SAFE_DELETE(m_dcShpLayer);
	SAFE_DELETE(m_dcMapLayer);
	SAFE_DELETE(m_dcMainTemp);
	SAFE_DELETE(m_dcMain);
	SAFE_DELETE(m_pWMSEngine);

}

BOOL CMapSolutionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapSolutionView 그리기

void CMapSolutionView::OnDraw(CDC* /*pDC*/)
{
	CMapSolutionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

}


// CMapSolutionView 인쇄


void CMapSolutionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMapSolutionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapSolutionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapSolutionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMapSolutionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapSolutionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapSolutionView 진단

#ifdef _DEBUG
void CMapSolutionView::AssertValid() const
{
	CView::AssertValid();
}

void CMapSolutionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapSolutionDoc* CMapSolutionView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapSolutionDoc)));
	return (CMapSolutionDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapSolutionView 메시지 처리기


void CMapSolutionView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CRect rec = m_recSave;
	GetWindowRect(&m_recSave);

	CRect pRectView;
	pRectView.top = 0;
	pRectView.left = 0;
	pRectView.right = cx;
	pRectView.bottom = cy;

	double x, y;
	m_pCoordMgr->GetDisplayCenter(&x, &y);
	m_pCoordMgr->m_fCenterX = x + rec.left - m_recSave.left;
	m_pCoordMgr->m_fCenterY = y + rec.top - m_recSave.top;

	m_pCoordMgr->m_nDisplayViewWidth = pRectView.Width();
	m_pCoordMgr->m_nDisplayViewHeight = pRectView.Height();
	m_pCoordMgr->m_fHorVerRatio = (double)(pRectView.Width()) / (double)(pRectView.Height());

	m_pCoordMgr->InitDisplayParameter(pRectView);

	if (m_pWMSEngine && cx > 0 && cy > 0)
	{
		// 엔진에 현재 View의 실제 크기를 전달
		m_pWMSEngine->OnSize(m_pCoordMgr->m_nDisplayViewWidth, m_pCoordMgr->m_nDisplayViewHeight);
		Invalidate(FALSE);
	}

	GetWindowRect(pRectView);
	if (m_bDCInitialized)
	{
		ReInitializeDCs();
	}
	
}


void CMapSolutionView::ReInitializeDCs()
{
	int nWidth, nHeight;
	nWidth = m_pCoordMgr->m_nDisplayViewWidth;
	nHeight = m_pCoordMgr->m_nDisplayViewHeight;

	// 투명도 디바이스 컨텍스트에게 현재 view정보를 전달한다.
	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);

	m_pBitmap4MainTemp.DeleteObject();
	m_pBitmap4Map.DeleteObject();
	m_pBitmap4Shape.DeleteObject();
	m_pBitmap4DTED.DeleteObject();
	m_pBitmap4Raster.DeleteObject();

	m_pBitmap4MainTemp.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Map.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Shape.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4DTED.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Raster.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);

	m_dcMainTemp->SelectObject(m_pOldBitmap4MainTemp);
	m_dcMapLayer->SelectObject(m_pOldBitmap4Map);
	m_dcShpLayer->SelectObject(m_pOldBitmap4Shape);
	m_dcDTEDLayer->SelectObject(m_pOldBitmap4DTED);
	m_dcRasterLayer->SelectObject(m_pOldBitmap4Raster);

	m_pOldBitmap4MainTemp = m_dcMainTemp->SelectObject(&m_pBitmap4MainTemp);
	m_pOldBitmap4Map = m_dcMapLayer->SelectObject(&m_pBitmap4Map);
	m_pOldBitmap4Shape = m_dcShpLayer->SelectObject(&m_pBitmap4Shape);
	m_pOldBitmap4DTED = m_dcDTEDLayer->SelectObject(&m_pBitmap4DTED);
	m_pOldBitmap4Raster = m_dcRasterLayer->SelectObject(&m_pBitmap4Raster);
}


BOOL CMapSolutionView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}


void CMapSolutionView::draw_maindc_only(CPoint ptNewPoint)
{
	int fWidth = m_pCoordMgr->m_nDisplayViewWidth;
	int fHeight = m_pCoordMgr->m_nDisplayViewHeight;

	GetClientRect(&m_rect);
	BufferDC.FillSolidRect(m_rect, RGB(15, 15, 15));
	BufferDC.BitBlt(ptNewPoint.x, ptNewPoint.y, fWidth, fHeight, m_dcMainTemp, 0, 0, SRCCOPY);

	m_dcMain->BitBlt(0, 0, fWidth, fHeight, &BufferDC, 0, 0, SRCCOPY);
}


//LRESULT CMapSolutionView::OnTileDownloaded(WPARAM wParam, LPARAM lParam)
//{
//	// 배경을 지우지 않고 다시 그리기 (깜빡임 방지)
//	Invalidate(FALSE);
//	return 0;
//}

LRESULT CMapSolutionView::OnTileDownloaded(WPARAM wp, LPARAM lp) 
{
	TileRequest* pReq = (TileRequest*)wp;
	CWMSClient* pEngine = (CWMSClient*)lp;

	// 엔진에게 이미지 변환 및 캐시 저장 요청
	pEngine->OnTileDownloaded(pReq);

	// 화면 다시 그리기 (이때 엔진의 Draw 함수는 캐시된 이미지를 찾아 그림)
	Invalidate(FALSE);
	return 0;
}

void CMapSolutionView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pDC->SetMapMode(MM_ANISOTROPIC);                                                               

	CView::OnPrepareDC(pDC, pInfo);
}


