////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : pickdropboxmgr.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2006-2-23 17:13:50
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_PICKDROPBOXMGR_H_
#define _INCLUDE_PICKDROPBOXMGR_H_

#include "KG3DEngine/KG3DInterface.h"

namespace UI
{

enum KPICKDROPBOXIMAGES
{
	PICKDROPBOX_IMAGE_BEGIN = 0,
	DISABLE_IMAGE = PICKDROPBOX_IMAGE_BEGIN, //������
	UNEQUIPABLE_IMAGE, //����װ��
	COOLDOWN_IMAGE, //��ȴ
	SELECTED_IMAGE, //ѡ��
	PRESSED_IMAGE, //����
	NO_OBJECT_MOUSEOVER_IMAGE, //û����Ʒʱ���������
	MOUSEOVER_IMAGE, //���������
	PICKDROPBOX_IMAGE_END,
	PICKDROPBOX_IMAGE_COUNT = PICKDROPBOX_IMAGE_END,
};

class KPickDropBoxMgr
{
private:
	KPickDropBoxMgr();
	~KPickDropBoxMgr();
public:
	static bool Init();
	static void Exit();
	static KPickDropBoxMgr &GetSelf();

	void DrawBoxAddOnLayer(float fX, float fY, float fWidth, float fHeight, DWORD dwBoxStatus, float fCoolingPercentage);

	void DrawBoxMouseOverLayer(float fX, float fY, float fWidth, float fHeight);

	int AppendSparkingAnimate();
	int RemoveSparkingAnimate(int nAnimateIndex);

	int AppendInUseAnimate();
	int RemoveInUseAnimate(int nAnimateIndex);

	int AppendStaringAnimate();
	int RemoveStaringAnimate(int nAnimateIndex);

private:
	int LoadSource();
	int FillImageData(int nIndex, int nFrame);
	int UnloadSource();

	int FillCollingImageData();

protected:
private:
	int m_nImageIndex;
	KG3DUIImage2 m_aImage[PICKDROPBOX_IMAGE_COUNT];
	char m_szImageFileName[MAX_PATH];
	char m_szAnimateFileName[MAX_PATH];
	int m_nSparkingAnimateGroup;
	int m_nInUseAnimateGroup;
	int m_nStaringAnimateGroup;

	KG3DUIImagePart2 m_CoolingImage;

	static KPickDropBoxMgr *ms_pSelf;
};

} //namespace UI


#endif //_INCLUDE_PICKDROPBOXMGR_H_
