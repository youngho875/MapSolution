#include "pch.h"
#include "MapSolutionView.h"


void CMapSolutionView::InitializeDisplayParameter(CRect pClientRect)
{
	m_pCoordMgr->InitDisplayParameter(pClientRect);
}


void CMapSolutionView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect pClientRect;
	GetClientRect(pClientRect);


	//	Create DCs for Objects
	m_dcMain = new CDC;
	m_dcMainTemp = new CGDCGeo;
	m_dcMapLayer = new CDC;
	m_dcShpLayer = new CDC;
	m_dcDTEDLayer = new CDC;
	m_dcRasterLayer = new CDC;

	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	HDC dc = ::GetDC(this->GetSafeHwnd());
	m_dcMain->Attach(dc);

	m_dcMainTemp->CreateCompatibleDC((CGDCGeo*)m_dcMain);
	m_dcMapLayer->CreateCompatibleDC(m_dcMain);
	m_dcMapLayer->SetBkMode(TRANSPARENT);
	m_dcMapLayer->SelectObject(&m_brush_10_10_10);

	m_dcShpLayer->CreateCompatibleDC(m_dcMain);
	m_dcDTEDLayer->CreateCompatibleDC(m_dcMain);
	m_dcRasterLayer->CreateCompatibleDC(m_dcMain);

	InitializeDisplayParameter(pClientRect);
	int nWidth, nHeight;
	nWidth = m_pCoordMgr->m_nDisplayViewWidth;
	nHeight = m_pCoordMgr->m_nDisplayViewHeight;

	m_pBitmap4MainTemp.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Map.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Shape.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4DTED.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);
	m_pBitmap4Raster.CreateCompatibleBitmap(m_dcMain, nWidth, nHeight);

	m_pOldBitmap4MainTemp = m_dcMainTemp->SelectObject(&m_pBitmap4MainTemp);
	m_pOldBitmap4Map = m_dcMapLayer->SelectObject(&m_pBitmap4Map);
	m_pOldBitmap4Shape = m_dcShpLayer->SelectObject(&m_pBitmap4Shape);
	m_pOldBitmap4DTED = m_dcDTEDLayer->SelectObject(&m_pBitmap4DTED);
	m_pOldBitmap4Raster = m_dcRasterLayer->SelectObject(&m_pBitmap4Raster);

	m_pCoordMgr->m_fScale = 1.0;
	double x_coordinate, y_coordinate;
	CPoint pPoint;
	m_Coord.degrees_lat_long_to_x_y(37.0, 127.0, &x_coordinate, &y_coordinate);
	m_pCoordMgr->WorldXY2ScreenXY(x_coordinate, y_coordinate, &pPoint.x, &pPoint.y, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale);
	m_pCoordMgr->SetNewCenter((double)pPoint.x, (double)pPoint.y);

	m_pWMSEngine = new CWMSClient(this->GetSafeHwnd());


	AfxGetMainWnd()->SetFocus();
	m_bDCInitialized = TRUE;

	BufferDC.CreateCompatibleDC(m_dcMain);
	//bmpBuffer.CreateCompatibleBitmap(m_dcMain, rect.Width(), rect.Height());
	bmpBuffer.CreateCompatibleBitmap(m_dcMain, 2000, 2000);
	(CBitmap*)BufferDC.SelectObject(&bmpBuffer);

	//m_pWMSEngine = new CWMSClient(this->GetSafeHwnd());

	CRect rc;
	CGRectGP rect;

	double left, top, right, bottom;

	GetClientRect(&rc);

	m_pCoordMgr->ScreenXY2WorldXY(rc.left, rc.top, &left, &top);
	m_pCoordMgr->ScreenXY2WorldXY(rc.right, rc.bottom, &right, &bottom);
	m_Coord.x_y_to_degrees_lat_long(left, top, &rect.m_R.top, &rect.m_R.left);
	m_Coord.x_y_to_degrees_lat_long(right, bottom, &rect.m_R.bottom, &rect.m_R.right);

	CString leftTop, rightBottom;
	leftTop.Format("%fE%fN", rect.m_R.left, rect.m_R.top);
	rightBottom.Format("%fE%fN", rect.m_R.right, rect.m_R.bottom);

}