#include "pch.h"
#include "MapSolutionView.h"
#include "GPoint.h"


void CMapSolutionView::SetStatusBarTxt(CString strMsg)
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame) return;

	CMFCRibbonStatusBar& wndStatus = pFrame->m_wndStatusBar;
	CMFCRibbonBaseElement* pStatusElement = wndStatus.GetElement(wndStatus.GetCount() - 1);

	if (pStatusElement)
	{
		pStatusElement->SetText(strMsg);

		// 1. 텍스트 너비 계산
		CClientDC dc(&wndStatus);
		CFont* pOldFont = dc.SelectObject(wndStatus.GetFont());
		CSize size = dc.GetTextExtent(strMsg);
		dc.SelectObject(pOldFont);

		int nNewWidth = size.cx + 30; // 여백 포함

		// 2. 현재 위치 정보를 가져와서 너비만 변경
		CRect rectElem = pStatusElement->GetRect();

		// SetRect를 사용하여 영역을 강제로 업데이트합니다.
		// 위치는 유지하고 너비(right)만 조정합니다.
		pStatusElement->SetRect(CRect(rectElem.left, rectElem.top, rectElem.left + nNewWidth, rectElem.bottom));

		// 3. UI 갱신
		wndStatus.RecalcLayout(); // 엘리먼트들이 겹치지 않게 재배치
		pStatusElement->Redraw();
		wndStatus.Invalidate();
		wndStatus.UpdateWindow();
	}
}


void CMapSolutionView::UpdateStatusBar(CPoint point)
{
	CCoordMgr* CoordMgr = CCoordMgr::GetInst();
	CCoord		Coord;
	GPoint		ptWorldPoint;


	double resolution;
	char strUTM[20], strGeoref[10], strMGRS[20], strGARS[9];
	CString strScale = _T("");

	resolution = m_pWMSEngine->getResolution();

	CoordMgr->ScreenXY2WorldXY(point.x, point.y, &ptWorldPoint.x, &ptWorldPoint.y, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale * (1.0 / resolution));
	//Coord.x_y_to_degrees_lat_long_str(ptWorldPoint.x, ptWorldPoint.y, strLat, strLong);
	//Coord.x_y_to_degrees_lat_long(ptWorldPoint.x, ptWorldPoint.y, &dLat, &dLong);
	Coord.x_y_to_georef_str(ptWorldPoint.x, ptWorldPoint.y, strGeoref);
	Coord.x_y_to_utm_str(ptWorldPoint.x, ptWorldPoint.y, strMGRS);
	Coord.x_y_to__other_utm_str(ptWorldPoint.x, ptWorldPoint.y, strUTM);

	Coord.x_y_to_degrees_lat_long(ptWorldPoint.x, ptWorldPoint.y, &m_pStatusInfo.pGeo_y, &m_pStatusInfo.pGeo_x);

	CString strStatus = "";
	CString strGeo = "", strrMGR = "", strrUTM = "", strGeoRef = "", strrGARS = "", strHeight = "";
	CString strMGRSZone = _T("");
	CString strPoint = _T("");

	strPoint.Format(" (WorldX: %f, WorldY: %f |)", ptWorldPoint.x, ptWorldPoint.y);
	strStatus += strPoint;

	int DoX, MinX;	double SecX;
	int DoY, MinY;  double SecY;

	Coord.DegreeToDmsLon(m_pStatusInfo.pGeo_x, &DoX, &MinX, &SecX);
	Coord.DegreeToDmsLat(m_pStatusInfo.pGeo_y, &DoY, &MinY, &SecY);

	strGeo.Format(" Lon:%3d° %2d'%7.3f\", Lat:%3d° %2d'%7.3f\" (X:%.6f, Y:%.6f) |", DoX, MinX, SecX, DoY, MinY, SecY, m_pStatusInfo.pGeo_x, m_pStatusInfo.pGeo_y);
	strStatus += strGeo;

	strrMGR.Format(" MGRS: %s |", strMGRS);
	strStatus += strrMGR;

	strrUTM.Format(" UTM: %s |", strUTM);
	strStatus += strrUTM;

	strGeoRef.Format(" GEOREF: %s |", strGeoref);
	strStatus += strGeoRef;

	m_Coord.degree_lat_lon_to_gars(m_pStatusInfo.pGeo_y, m_pStatusInfo.pGeo_x, 5, strGARS);
	strrGARS.Format(" GARS: %s |", strGARS);
	strStatus += strrGARS;

	strScale.Format(" SCALE: %f |", CoordMgr->m_fScale);
	strStatus += strScale; 
	

	//double height = 0.0f;
	//height = mHeights->get_Heights(m_pStatusInfo.pGeo_y, m_pStatusInfo.pGeo_x);
	//strHeight.Format(" Alt: %fm", height);
	//strStatus += strHeight;

	m_pStatusInfo.strStatusCoord = strStatus;

	SetStatusBarTxt(m_pStatusInfo.strStatusCoord);

	//if (m_pStatusInfo.strStatusCoord.IsEmpty())
	//	SetStatusBar2Txt(_T("Disconnected"));
	//else
	//	SetStatusBar2Txt(_T("Connected"));
	
}