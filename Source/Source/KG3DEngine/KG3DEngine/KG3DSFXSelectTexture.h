//////////////////////////////////////////////////////////////////////////
// File : KG3DSFXSelectTexture.h
// Creator : FengBo
// Date : 2007-10-10
// Desc : Ϊ�ر���������һ����ͼ�������磺����������NPCѡ���
//			 ���߱�ը�����Ĵ�Χ��ͼ��
//////////////////////////////////////////////////////////////////////////

#ifndef __KG3DSFX_SELECT_TEXTURE_H__
#define __KG3DSFX_SELECT_TEXTURE_H__

#include "KG3DTexture.h"
#include "IEKG3DSFX.h"
enum KGFOOTPRINT
{
	DEFAULT_FOOT,
	LEFT_FOOT,
	RIGHT_FOOT
};
class KG3DSFX;
class KG3DSFXSelectTexture : public IEKG3DSFXSelectTexture 
{
public:
	KG3DSFXSelectTexture();
	KG3DSFXSelectTexture(KG3DSFX* pSFX);
	~KG3DSFXSelectTexture();
public:
	/**
	* @func : Update
	* @param : pVecPosCenter [in] Ҫ���ӵ���ͼ���ĵ�����
	* @return : HRESULT---S_OK or E_FAIL���Ƿ���³ɹ�
	* @brief : �˺�������ͼ��������Ϊ������������ͼ���Ƿ�Χ��ʹ
	*			  ���Ƿ�Χ�ڵ�������ر�Ⱦ�����һ����ͼ
	*/
    void Update();

	HRESULT ShutDown();

	/**
	* @func : GetTexture
	* @param : none
	* @return : ��ǰʹ�õ���ͼ����
	*/
	IEKG3DTexture* GetTexture();

	/**
	* @func : LoadTexture
	* @param : szTextureName [in] ��ȡ�����ļ����ļ�����
	* @return : HRESULT S_OK or E_FAIL���Ƿ��ȡ�ɹ�
	* @brief: �˺������ⲿ�ļ�����һ��������Ϊ�˴���ͼ������
	*/
	HRESULT LoadTexture(const char* szTextureFileName);

	/**
	* @func : SetTexture
	* @param : pTexture [in] 
	* @return : HRESULT S_OK
	* @brief : ����һ���Ѵ��ڵ�KG3DTexture���͵�����Ϊ������
	*/
	HRESULT SetTexture(IEKG3DTexture* pTexture);

	/**
	* @func :SetTexturePos
	* @param : pNewPos [in] �µ�����λ��
	* @return : HRESULT S_OK or E_FAIL
	* @brief : �ֹ�ָ����ǰ��ͼ��λ������
	*/
	HRESULT SetTexturePos(D3DXVECTOR3* pNewPos);

	/**
	* @func :getTexturePos
	* @param : pPos [out] ��ǰ�����������ڵ�����λ��
	* @return : HRESULT S_OK or E_FAIL
	* @brief : none
	*/
	HRESULT GetTexturePos(D3DXVECTOR3* pPos);

	/**
	* @func :GetTextureWidth
	* @param : none
	* @return : UINT 
	* @brief : ���ص�ǰʹ����ͼ�Ŀ��
	*/
	DWORD GetTextureWidth();
	void  SetTextureWidth(DWORD dwWidth);
	/**
	* @func :GetTextureHeight
	* @param : none
	* @return : UINT 
	* @brief : ���ص�ǰʹ����ͼ�ĸ߶�
	*/
	DWORD GetTextureHeight();
	void  SetTextureHeight(DWORD dwHeight);

	HRESULT GetRect(D3DXVECTOR2& vLeftBottom,D3DXVECTOR2& vRightTop);
public:
	float GetAlpha();
	void  SetAlpha(float fAlpha);

	HRESULT SetSFX(KG3DSFX* pSFX);
private:
	/**
	* @func :GetPickPos
	* @param : pVecPos [Out] ��ǰ����3D����Ľ���(��һ��)
	* @return : HRESULT S_OK or E_FAIL 
	* @brief : ��������ֻȡ��һ��
	*/
	//HRESULT GetPickPos(OUT D3DXVECTOR3* pVecPos);

private:
	KG3DTexture* m_pTexture;		// ��ǰʹ�õ�������ͼ
	KG3DSFX*     m_pSFX;            // ָ�����ڵ���Ч

private:
	float         m_fAlpha;
	DWORD         m_nTextureWidth;
	DWORD		  m_nTextureHeight;
	D3DXVECTOR3   m_vec3PosCenter;
public:
	KGFOOTPRINT   m_enFoot;
	D3DXMATRIX    m_Matrix;
public:
	//vector<POINT> m_vecIncludeRegionIndex; //��¼��ǰͶӰ��ͼ����Region������.
};

//////////////////////////////////////////////////////////////////////////
// Inline Functions
//////////////////////////////////////////////////////////////////////////

class KG3DSFXSelectMark : public IEKG3DSFXSelectMark,public KG3DTransformation
{
public:
	//KGFOOTPRINT   m_enFoot;                 //��������ţ��ҽţ����ǲ������ҽ�
	DWORD         m_dwTerraType;            //���ڵر����͡���ʱ�ر�û�����͡�Ĭ��Ϊ0
private:
	DWORD         m_nTextureWidth;
	DWORD		  m_nTextureHeight;
	DWORD         m_dwLastTime;
	KG3DTexture*  m_pTexture;		// ��ǰʹ�õ�������ͼ

	LPDIRECT3DVERTEXBUFFER9 m_lpVB;
	LPDIRECT3DINDEXBUFFER9  m_lpIB;
	//D3DXVECTOR3   m_vDirection;
	D3DXMATRIX m_matSaveSFXWorld;
public:
	KG3DSFXSelectMark(KG3DSFX* pSFX);
	~KG3DSFXSelectMark();

private:
	HRESULT CreatVertexBuffer();
	void FillVertexBuffer(VFormat::_Faces_Diffuse_Texture1* pData);
public:
	HRESULT RenderTexture();
	
	HRESULT LoadTexture(const char* szTextureFileName);
	IEKG3DTexture* GetTexture();
//	HRESULT AddProTexture(KG3DSFXSelectTexture** pSelTexture);//��ʱûʹ��
public:
	virtual HRESULT OnLostDevice();
	virtual HRESULT OnResetDevice();
	HRESULT GetCurPosWidthHeight(D3DXVECTOR3* pPos,DWORD* pWidth,DWORD* pHeight);
	D3DXVECTOR3 GetCurPos();
	DWORD GetTextureWidth();
	void  SetTextureWidth(DWORD dwWidth);
	DWORD GetTextureHeight();
	void  SetTextureHeight(DWORD dwHeight);
	DWORD GetWidth(); // ��ȡ��������SFX��Ч�����Ų�������
	DWORD GetHeight();//��ȡ��������SFX��Ч�����Ų�����߶�
	//DWORD GetenFoot();                   //��ʱûʹ��
	//void  SetenFoot(DWORD& dwVal );//��ʱûʹ��
	//std::string GetFootprintType();//��ʱûʹ��
	//void  SetFootprintType(std::string& scType);//��ʱûʹ��
	KG3DSFX*      m_pSFX;            // ָ�����ڵ���Ч

};
#endif //:-)