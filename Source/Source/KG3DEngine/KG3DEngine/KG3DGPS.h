////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DGPS.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2007-4-19 11:41:46
//  Comment     : �������Ҫ�������������д������λ�õ����⡣ԭ����Բο�
/*KG3DTimeClock����������������Ա������������Accept��Ȼ����ݲ��Ե���
ע����ʹ����KG3DLifeTimeDependedClassBase����������
*/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DGPS_H_
#define _INCLUDE_KG3DGPS_H_


////////////////////////////////////////////////////////////////////////////////
struct KG3DGPSBase : public KG3D_CUSTOM_HELPERS::KG3DLifeTimeDependedClassBase<1>
{
	virtual ~KG3DGPSBase() = 0{}
	virtual HRESULT Init(D3DXVECTOR3 vecPos) = 0;
	virtual HRESULT AdjustPos(D3DXVECTOR3 vecPos) = 0;
	virtual HRESULT GetPos(D3DXVECTOR3& RefPos) = 0;
	virtual HRESULT	GetRegionIndexByPos(INT& nXIndex, INT& nZIndex)= 0;
	
};
//////////////////////////////////////////////////////////////////////////
class KG3DGPS_Dummy : public KG3DGPSBase
{
private:
	D3DXVECTOR3 m_vecCamLookAtPos;
public:
	KG3DGPS_Dummy();
public:
	virtual HRESULT Init(D3DXVECTOR3 vecPos);
	virtual HRESULT AdjustPos(D3DXVECTOR3 vecPos);
	virtual HRESULT GetPos(D3DXVECTOR3& RefPos);
	virtual HRESULT	GetRegionIndexByPos(INT& nXIndex, INT& nZIndex);
};
#endif //_INCLUDE_KG3DGPS_H_
