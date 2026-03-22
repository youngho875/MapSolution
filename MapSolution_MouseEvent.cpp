#include "pch.h"
#include "MapSolutionView.h"


#define		CENTERLAT			"370000N"
#define		CENTERLONG			"1270000E"



char		globalLat[10] = CENTERLAT;
char		globalLong[10] = CENTERLONG;

void CMapSolutionView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);

	UpdateStatusBar(point);

	if (m_bPanning && nFlags == MK_LBUTTON)
	{
		m_bMapMove = TRUE;

		if (m_ptPrevPoint == point)
			return;
		 
		//CKB 메인 DC만 그리도록 해서 성능 확보  
		m_ptPrevPoint.x = point.x - m_ptZoomWindowLT.x;
		m_ptPrevPoint.y = point.y - m_ptZoomWindowLT.y;

		m_pWMSEngine->MoveMap(m_ptPrevPoint.x, m_ptPrevPoint.y);

		draw_maindc_only(m_ptPrevPoint);

		return;
	}

/*
	if (m_bPanning && m_pWMSEngine)
	{
		// 픽셀 변화량 계산
		int dx = point.x - m_lastPoint.x;
		int dy = point.y - m_lastPoint.y;

		m_pWMSEngine->MoveMap(dx, dy);

		// 변화량을 위경도로 변환하여 엔진 업데이트
		// (dx * resolution) 만큼 경도 이동, (dy * resolution) 만큼 위도 이동
		// 화면 Y는 아래로 갈수록 커지므로 위도는 뺍니다.
		double dLon = dx * m_pWMSEngine->GetResolution();
		double dLat = dy * m_pWMSEngine->GetResolution();

		//double Lon, Lat;
		//m_Coord.x_y_to_degrees_lat_long(dLon, dLat, &Lat, &Lon);
		double newLon = m_pWMSEngine->GetCenterLon() - dLon;
		double newLat = m_pWMSEngine->GetCenterLat() + dLat;

		double fS = 1.0 / m_pWMSEngine->m_resolution;
		double x_coordinate, y_coordinate;
		CPoint pPoint;
		m_Coord.degrees_lat_long_to_x_y(newLat, newLon, &x_coordinate, &y_coordinate);
		m_pCoordMgr->WorldXY2ScreenXY(x_coordinate, y_coordinate, &pPoint.x, &pPoint.y, m_pCoordMgr->m_fCenterX, m_pCoordMgr->m_fCenterY, m_pCoordMgr->m_fScale * fS);
		m_pCoordMgr->SetNewCenter((double)pPoint.x, (double)pPoint.y);

		m_pWMSEngine->SetCenter(newLon, newLat);

		//m_lastPoint = point;

		Invalidate(FALSE); // 화면 갱신
	}
*/
}


void CMapSolutionView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bPanning = TRUE;

	m_ptPrevPoint = point;
	m_ptMapMoveFirstPoint = point;
	//m_lastPoint = point;

	if (m_bPanning)
	{
		m_ptZoomWindowLT = m_ptZoomWindowBR = point;
		m_bMapMove = TRUE;
		SetCapture();
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMapSolutionView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnLButtonUp(nFlags, point);

	ReleaseCapture();

	m_bPanning = FALSE;

	if (m_bMapMove && point != m_ptMapMoveFirstPoint)
	{
		CPoint ptNewPoint;
		ptNewPoint.x = -point.x + m_ptMapMoveFirstPoint.x;
		ptNewPoint.y = -point.y + m_ptMapMoveFirstPoint.y;
		m_pCoordMgr->m_fCenterX -= ptNewPoint.x;
		m_pCoordMgr->m_fCenterY -= ptNewPoint.y;

		DrawSuit(TRUE);

		m_bMapMove = FALSE;
	}


	double fWorldX, fWorldY;
	m_pCoordMgr->ScreenXY2WorldXY(point.x, point.y, &fWorldX, &fWorldY);
	m_Coord.x_y_to_degrees_lat_long_str(fWorldX, fWorldY, globalLat, globalLong);

	// 점 그리기
	CBrush pBrush, * pOldBrush;
	pBrush.CreateSolidBrush(RGB(255, 0, 0));
	pOldBrush = m_dcMain->SelectObject(&pBrush);
	m_dcMain->Ellipse(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
	m_dcMain->SelectObject(pOldBrush);
}


BOOL CMapSolutionView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

/*
	if (!m_pWMSEngine)
		return FALSE;

	// 휠 위치를 뷰 좌표로 변환
	ScreenToClient(&pt);

	// 1. 현재 줌 레벨 조정
	int curZoom = m_pWMSEngine->GetZoom();
	if (zDelta > 0)
		curZoom = min(18, curZoom + 1);
	else
		curZoom = max(0, curZoom - 1);

	// 2. 엔진의 줌 업데이트 (내부적으로 UpdateResolution 및 가시범위 재계산 수행)
	// 마우스 위치 중심 줌을 구현하고 싶다면 엔진에 pt를 전달하거나
	// 위에서 설명한 중심점 보정 로직을 여기서 실행합니다.
	m_pWMSEngine->SetZoom(curZoom);

	Invalidate(FALSE);
*/

	if ((zDelta > 0 && m_pCoordMgr->m_fScale < -100.0) ||
		(zDelta < 0 && m_pCoordMgr->m_fScale > 2500.0))
		return FALSE;

	if (zDelta == 0)
		return FALSE;

	double geo_min_x, geo_max_x, geo_min_y, geo_max_y;
	double scr_min_x, scr_max_x, scr_min_y, scr_max_y;
	
	CRect rect;
	GetClientRect(&rect);
	m_pCoordMgr->ScreenXY2WorldXY(rect.left, rect.top, &geo_min_x, &geo_min_y);
	m_pCoordMgr->ScreenXY2WorldXY(rect.right, rect.bottom, &geo_max_x, &geo_max_y);

	m_fPrevScale = m_pCoordMgr->m_fScale;

	double	fXCoord, fYCoord;
	m_pCoordMgr->ScreenXY2WorldXY(m_pCoordMgr->m_nDisplayViewWidth / 2, m_pCoordMgr->m_nDisplayViewHeight / 2, &fXCoord, &fYCoord);

	double	fZoomRatio = (zDelta / abs(zDelta) * (-1)) * m_pCoordMgr->m_fScale / 3.0;
	m_pCoordMgr->m_fScale = m_pCoordMgr->m_fScale + fZoomRatio;

	if (m_pCoordMgr->m_fScale > 2500)
		m_pCoordMgr->m_fScale = 2500;

	long nCenterX, nCenterY;
	m_pCoordMgr->WorldXY2ScreenXY(fXCoord, fYCoord, &nCenterX, &nCenterY);
	m_pCoordMgr->SetNewCenter(nCenterX, nCenterY);

	m_pCoordMgr->WorldXY2ScreenXY(geo_min_x, geo_min_y, &scr_min_x, &scr_min_y, nCenterX, nCenterY, m_pCoordMgr->m_fScale);
	m_pCoordMgr->WorldXY2ScreenXY(geo_max_x, geo_max_y, &scr_max_x, &scr_max_y, nCenterX, nCenterY, m_pCoordMgr->m_fScale);

	//m_pWMSEngine->ZoomControl();
	m_pWMSEngine->CalculateVisibleRange(rect);

	Invalidate(FALSE);

	/*
		ScreenToClient(&pt);

		// 줌 직전의 마우스 위치 위경도 저장 (이 지점을 고정할 것임)
		double mouseLon, mouseLat;
		double x, y;
		m_pCoordMgr->ScreenXY2WorldXY(pt.x, pt.y, &x, &y);
		m_Coord.x_y_to_degrees_lat_long(x, y, &mouseLat , &mouseLon );

		// 줌 레벨 변경 및 해상도 업데이트
		int oldLevel = m_pWMSEngine->GetZoom();
		if (zDelta > 0)
			m_pWMSEngine->SetZoom(oldLevel + 1);
		else
			m_pWMSEngine->SetZoom(oldLevel - 1);

		// 최소 레벨 제한 (화면 전체를 채우기 위한 방어 코드)
		if (m_pWMSEngine->GetZoom() < 0) m_pWMSEngine->SetZoom(0);

		// 해상도 갱신 (반드시 먼저 해야함)
		m_pWMSEngine->UpdateResolution();

		// [중요] 마우스가 가리켰던 위경도가 새로운 레벨에서도
		// 동일한 화면 좌표(pt)에 오도록 중심점(m_center) 역계산
		// 새로운 해상도에서 마우스 좌표(pt)가 mouseLon/Lat이 되기 위한 중심점 찾기
		double newCenterLon = mouseLon - (pt.x - m_pCoordMgr->m_nScreenWidth / 2.0) * m_pWMSEngine->m_resolution;
		double newCenterLat = mouseLat + (pt.y - m_pCoordMgr->m_nScreenHeight / 2.0) * m_pWMSEngine->m_resolution;

		m_pWMSEngine->SetCenter(newCenterLon, newCenterLat);

		// 가시 범위 재계산 및 화면 갱신
		m_pWMSEngine->CalculateVisibleRange();

		m_pWMSEngine->CleanUpWorkQueue(); // 이전 레벨의 불필요한 요청 제거

		Invalidate(FALSE);
	*/
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}