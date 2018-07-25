#pragma once
//#include "xmesh.h"
#include ".\meshslidebar.h"
#include ".\mesheditctrl.h"
#include ".\meshitem.h"
#include ".\meshlistbox.h"
#include "MeshCombobox.h"
#include "MeshListCtrl.h"
#include "MeshButton.h"

#define COLOR_NONE     0
#define COLOR_AMBIENT  1
#define COLOR_DIFFUSE  2
#define COLOR_SPECULAR 4
#define COLOR_EMISSIVE 8

namespace ModelTools
{
//class KModelToolMaterial :
//	public ModelUI::KModelUIBase
//{
//
//public:
//	//////////////////////////////////////////////////////////////////////////
//	//
//	enum TextOpType
//	{
//		TEXTURE_OPERATION,
//		TEXTURE_MAPMETHOD,
//
//	}m_NowShowTextOp;
//
//	ModelUI::KModelUIBase*      m_lpMtlOptionWnd; //����ѡ��Ĵ���
//	ModelUI::KModelUIComboBox*  m_lpMtlOptionComboBox;//����ѡ����������б�
//
//	
//	LPMODEL  m_CurModifyModel;
//	ModelUI::KModelUIBase*   m_lpTextOptionWnd;//��ͼѡ��Ĵ���
//	ModelUI::KModelUIItem*      m_pITextOp;//��ͼͼ�����ѡ��
//	ModelUI::KModelUIItem*      m_pITextUV;//��ͼ��������ӳ��ѡ��
//	
//	ModelUI::KModelUIComboBox*  m_lpColorOpComboBox;
//	ModelUI::KModelUIComboBox*  m_lpAlphaOpComboBox;
//	ModelUI::KModelUIComboBox*  m_lpResultComboBox;
//
//	ModelUI::KModelUIComboBox*  m_lpColorArg1ComboBox;
//	ModelUI::KModelUIComboBox*  m_lpColorArg2ComboBox;
//	ModelUI::KModelUIComboBox*  m_lpAlphaArg1ComboBox;
//	ModelUI::KModelUIComboBox*  m_lpAlphaArg2ComboBox;
//	ModelUI::KModelUIComboBox*  m_lpDropTextOp;	
//
//	ModelUI::KModelUIListBox*   m_pLTextUV;
//	//////////////////////////////////////////////////////////////////////////
//	//
//	ModelUI::KModelUIEditCtrl*  m_lpPowerEditCtrl;
//	ModelUI::KModelUIEditCtrl*  m_lpAlphaEditCtrl;
//
//	ModelUI::KModelUIItem*      m_lpTexturesItem[8];
//
//	//////////////////////////////////////////////////////////////////////////
//	//Alpha operation option
//	ModelUI::KModelUIButton*    m_lpBAlphaTest;
//	ModelUI::KModelUIButton*    m_lpBAlphaBlend;
//	ModelUI::KModelUIButton*    m_lpBSepAlphaBlend;
//	
//	ModelUI::KModelUIEditCtrl*  m_lpEAlphaTestRef;
//
//	ModelUI::KModelUIComboBox*  m_lpCAlphaFuction;
//	ModelUI::KModelUIComboBox*  m_lpCAlphaBlendOperator;
//	ModelUI::KModelUIEditCtrl*  m_lpCAlphaBlendFactor;
//	ModelUI::KModelUIComboBox*  m_lpCSrcBlendAlpha;
//	ModelUI::KModelUIComboBox*  m_lpCDestBlendAlpha;
//	
//	ModelUI::KModelUIComboBox*  m_lpABSrcComboBox;//Alpha Blend Src
//	ModelUI::KModelUIComboBox*  m_lpABDesComboBox;//Alpha Blend Dest
//	ModelUI::KModelUIComboBox*  m_lpDropAlphaOperation;
//	
//	//////////////////////////////////////////////////////////////////////////
//	//Shader System in Material
//	ModelUI::KModelUIListBox*   m_lpLVertexConst;
//	ModelUI::KModelUIComboBox*  m_lpCVertexInput;
//	ModelUI::KModelUIItem*      m_lpIVertexFile;
//	ModelUI::KModelUIButton*    m_lpBVertexEdit;
//	ModelUI::KModelUIButton*    m_lpBVertexRest;
//
//	ModelUI::KModelUIListBox*   m_lpLPixelConst;
//	ModelUI::KModelUIComboBox*  m_lpCpixelInput;
//	ModelUI::KModelUIItem*      m_lpIPixelFile;
//	ModelUI::KModelUIButton*    m_lpBPixelEdit;
//	ModelUI::KModelUIButton*    m_lpBPixelRest;
//
//	//////////////////////////////////////////////////////////////////////////
//	//Ms Effect System in Material
//	ModelUI::KModelUIListBox*   m_lpLEffectConst;
//	ModelUI::KModelUIComboBox*  m_lpCEffectInput;
//	ModelUI::KModelUIItem*      m_lpIEffectFile;
//	ModelUI::KModelUIButton*    m_lpBEffectEdit;
//	ModelUI::KModelUIButton*    m_lpBEffectRest;
//
//	//////////////////////////////////////////////////////////////////////////
//	int      m_nCurTextureOp;//��ǰ�򿪵���ͼѡ���-1��ʾû�д�
//	float    m_fTexturesTop;
//	float    m_fTexturesHeight;
//	ModelUI::KModelUIListCtrl*  m_lpMaterialListCtrl;
//	BOOL     m_bHide;
//	BOOL     m_bShowTextureOption;
//	BOOL     m_bUpdateAlphaEdit;
//	BOOL     m_bUpdatePowerEdit;
//
//	PVOID    m_lpScene;
//	DWORD    m_dwNumMaterial;
//
//	DWORD    m_dwCurColorOperation;
//
//	ModelUI::KModelUIButton*  m_pBLoad;
//	ModelUI::KModelUIButton*  m_pBSave;
//
// 	ModelUI::KModelUIButton*  m_pBAddTexture;
// 	ModelUI::KModelUIButton*  m_pBRemoveTexture;
//
//	GraphicsStruct::_Command m_CmdHide;
//	GraphicsStruct::_Command m_CmdShow;
//
//	ModelUI::KModelUIButton*  m_pBAll;
//	ModelUI::KModelUIButton*  m_pBNone;
//	ModelUI::KModelUIButton*  m_pBInvert;
//
//	ModelUI::KModelUIButton*  m_BMtlOption;
//
//	ModelUI::KModelUIButton*  m_BTextureOption[8];
//
//	ModelUI::KModelUIButton* m_pBMtlWire;
//	ModelUI::KModelUIButton* m_pBMtlZEnable;
//	ModelUI::KModelUIButton* m_pBMtlGouraud;
//	ModelUI::KModelUIButton* m_pBMtl2Faces;
//	ModelUI::KModelUIButton* m_pBMtlLighting;
//	ModelUI::KModelUIButton* m_pBMtlSpecular;
//
//	ModelUI::KModelUIItem* m_pIAmbient;
//	ModelUI::KModelUIItem* m_pIDiffuse;
//	ModelUI::KModelUIItem* m_pISpecular;
//	ModelUI::KModelUIItem* m_pIEmissive;
//	
//public:
//	HRESULT TextOpMsg(KModel::LPTEXTUREOPEX pTOp,UINT message, WPARAM wParam, LPARAM lParam);
//	HRESULT TextOpLoad();
//
//	
//	HRESULT RenderTextureOption(KModel::LPMATERIAL pMaterial,DWORD TextIndex);
//
//	HRESULT ClearTextureItemSelection();
//	HRESULT RemoveTexture(KModel::LPMATERIAL pMaterial,DWORD TextIndex);
//	HRESULT AddTexture(KModel::LPMATERIAL pMaterial);
//	HRESULT LoadTextureFile(KModel::LPMATERIAL pMaterial,DWORD TextIndex);
//
//	KModel::LPMTLALPHAOPERATION AlphaOperationGet(KModel::LPMATERIAL pMaterial);
//	KModel::LPTEXTUREOPEX   TextOpGet(KModel::LPMATERIAL pMaterial,DWORD TextIndex);
//
//	KModel::LPMTLMSEFFECT MtlEffectGet(KModel::LPMATERIAL pMaterial);
//
//	HRESULT LoadMaterial2File();
//	HRESULT SaveMaterial2File();
//	HRESULT ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
//
//
//	HRESULT SetMeshEditScene(PVOID pScene);
//	HRESULT LoadMesh(LPSTR pFileName);
//	HRESULT Render(void);
//	HRESULT RenderMaterialList(void);
//	HRESULT RefreshMaterialList(DWORD Oper);
//	HRESULT SetMaterialColor(D3DCOLORVALUE Color);
//	int GetFirstSelectedMaterial(void);
//
//	KModelToolMaterial(void);
//	virtual ~KModelToolMaterial(void);
//
//private:
//	HRESULT TextOpUnDropAll(void);
//	HRESULT TextOpRender(KModel::LPTEXTUREOPEX pTOp);
//public:
//	HRESULT MtlShaderRender();
//	HRESULT MtlShaderLoad();
//	HRESULT MtlShaderMsg(UINT message, WPARAM wParam, LPARAM lParam);
//
//	HRESULT MtlEffectOpenFile(KModel::LPMTLMSEFFECT pEffect);
//	HRESULT MtlEffectLoad();
//	HRESULT MtlEffectRender(KModel::LPMTLMSEFFECT pEffect);
//	HRESULT MtlEffectMsg(KModel::LPMTLMSEFFECT pEffect,UINT message, WPARAM wParam, LPARAM lParam);
//
//	HRESULT AlphaOperationUnDropAll();
//	HRESULT AlphaOperationRender(KModel::LPMTLALPHAOPERATION pAlpha);
//	HRESULT AlphaOperationLoad();
//	HRESULT MaterialOptionMsg(KModel::LPMATERIAL pMaterial,
//							  UINT message, WPARAM wParam, LPARAM lParam);
//	HRESULT MaterialOptionRender(KModel::LPMATERIAL pMaterial);
//	HRESULT MaterialOptionLoad();
//	HRESULT TextureOptionMsg(UINT message, WPARAM wParam, LPARAM lParam);
//	HRESULT TextUVLoad(void);
//	HRESULT TextUVRender(KModel::LPTEXTUREMAP pMap);
//	KModel::LPTEXTUREMAP TextUVGet(KModel::LPMATERIAL pMaterial,DWORD TextIndex);
//protected:
//	KModel::LPMTLSHADEROPTION MtlShaderGet(KModel::LPMATERIAL pMaterial);
//	HRESULT AlphaOperationMsg(KModel::LPMTLALPHAOPERATION pAlpha,
//							  UINT message, WPARAM wParam, LPARAM lParam);
//};
};
