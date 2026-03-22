#include "pch.h"
#include "MapSolutionView.h"



void CMapSolutionView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
	
	DrawSuit(TRUE);
}


void CMapSolutionView::DrawSuit(CPoint pStartPoint)
{
	if (m_bMapMove == TRUE)
		return;

	if (m_dcMainTemp == NULL)
		return;


	int fWidth = m_pCoordMgr->m_nDisplayViewWidth;
	int fHeight = m_pCoordMgr->m_nDisplayViewHeight;
	m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	((CDC*)m_dcMainTemp)->FillSolidRect(0, 0, fWidth, fHeight, RGB(10, 10, 10));
	//COLORREF colorOldBack4DTED;
	//COLORREF colorOldBack4Raster;
	//COLORREF colorOldBack4Shp;

	// TRANSPARENT Color Setting
	//colorOldBack4DTED = m_dcDTEDLayer->SetBkColor(RGB(0, 0, 0));
	//colorOldBack4Raster = m_dcRasterLayer->SetBkColor(RGB(0, 0, 0));
	//colorOldBack4Shp = m_dcShpLayer->SetBkColor(RGB(0, 0, 0));

	/*************************************************************************************************************************/
	//
	//		이곳에 지도 그리는 함수 
	//
	/*************************************************************************************************************************/
	//if (!(m_pCoordMgr->m_bFlat))
	//{
	//	double scx, scy, x, y;
	//	scx = m_pCoordMgr->m_nDisplayViewWidth / 2;
	//	scy = m_pCoordMgr->m_nDisplayViewHeight / 2;

	//	m_pCoordMgr->ScreenXY2WorldXY((int)scx, (int)scy, &x, &y);
	//	m_pCoordMgr->m_bFlat = 1;
	//	m_pCoordMgr->WorldXY2ScreenXY(x, y, &scx, &scy);
	//	m_pCoordMgr->SetNewCenter(scx, scy);
	//}

	m_pWMSEngine->OnDrawTile(m_dcMainTemp);


	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcMapLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));

	//m_pOverlayThread->Redraw(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y);
	//::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcShpLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	//::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcDTEDLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	//::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcRasterLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));
	//::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcSymLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));


	//투명도보다 위에 전시하려 했으나 투명도에서 알파값을 조정 후 복구를 안시켜버림.
	//추후 해결 후 투명도보다 위에 전시하도록 순서 변경 필요.	
	//m_pOverlayThread->Redraw(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y);
	//m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX - pStartPoint.x;
	//m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY - pStartPoint.y;
	//m_dcMainTemp->SetCoordMgr(m_pCoordMgr);
	//DrawArmyTroop_Etc(m_dcMainTemp);


	// HJYOO : 투명도를 그린후 좌표값을 원상태로 복구한다.
	//m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX + pStartPoint.x;
	//m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY + pStartPoint.y;
	//m_dcMainTemp->SetCoordMgr(m_pCoordMgr);


	/*
		DrawArmyTroop();
	*/
	//	::TransparentBlt(m_dcMainTemp->m_hDC, -pStartPoint.x, -pStartPoint.y, fWidth, fHeight, m_dcMapLayer->m_hDC, 0, 0, fWidth, fHeight, RGB(0, 0, 0));

		/*
			DrawOverlay();
		*/


		//m_pCoordMgr->m_fCenterX = m_pCoordMgr->m_fCenterX + pStartPoint.x;
		//m_pCoordMgr->m_fCenterY = m_pCoordMgr->m_fCenterY + pStartPoint.y;
		//m_dcMainTemp->SetCoordMgr(m_pCoordMgr);

	m_dcMain->BitBlt(0, 0, fWidth, fHeight, m_dcMainTemp, 0, 0, SRCCOPY);
	//m_dcDTEDLayer->SelectObject(&colorOldBack4DTED);
	//m_dcRasterLayer->SelectObject(&colorOldBack4Raster);
	//m_dcShpLayer->SelectObject(&colorOldBack4Shp);

	//m_bIsDrawSituationInProcess = FALSE;


	CPen pen, * pPen;

	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pPen = m_dcMain->SelectObject(&pen);
	m_dcMain->SelectStockObject(NULL_BRUSH);
	CRect client_rect;
	GetClientRect(&client_rect);
	int center_x = client_rect.right / 2;
	int center_y = client_rect.bottom / 2;
	m_dcMain->MoveTo(center_x - 10, center_y);
	m_dcMain->LineTo(center_x + 10, center_y);
	m_dcMain->MoveTo(center_x, center_y - 10);
	m_dcMain->LineTo(center_x, center_y + 10);
	m_dcMain->SelectObject(pPen);

/*
	CPen pen, * p_pen;
	pen.CreateStockObject(WHITE_PEN);
	p_pen = m_dcMain->SelectObject(&pen);
	CRect client_rect;
	GetClientRect(&client_rect);
	int center_x = client_rect.right / 2;
	int center_y = client_rect.bottom / 2;
	m_dcMain->MoveTo(center_x - 10, center_y);
	m_dcMain->LineTo(center_x + 10, center_y);
	m_dcMain->MoveTo(center_x, center_y - 10);
	m_dcMain->LineTo(center_x, center_y + 10);
	m_dcMain->SelectObject(p_pen);
*/
}