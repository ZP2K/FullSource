////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : wndminimap.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2006-9-15 10:15:00
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_WNDMINIMAP_H_
#define _INCLUDE_WNDMINIMAP_H_

////////////////////////////////////////////////////////////////////////////////

#include "KG3DEngine/KG3DInterface.h"
#include "./wndwindow.h"

namespace UI
{
	class KWndMinimap : public KWndWindow
	{
		enum KMINIMAPTYPE
		{
			MINIMAP_RECTANGLE = 0,
			MINIMAP_CIRCLE,
		};

		struct KMINIMAPBLOCKINFO
		{
			int nLayer;
			int nX;
			int nY;
			bool operator<(const KMINIMAPBLOCKINFO &rh) const
			{
				if (nLayer < rh.nLayer)
					return true;
				if (nLayer == rh.nLayer)
				{
					if (nX < rh.nX)
						return true;
					if (nX == rh.nX)
						return nY < rh.nY;
				}
				return false;
			};
		};
		typedef std::map<KMINIMAPBLOCKINFO, KG3DUIImage2> KMINIMAPBLOCKTOIMAGE;

		enum KOBJECTTYPE
		{
			OBJECT_POINT,
			OBJECT_ANIMATE,
			OBJECT_ARROW,
		};

		struct KOBJECTID
		{
			DWORD dwType;
			DWORD dwID;

			KOBJECTID(DWORD dwType, DWORD dwID)
			{
				this->dwType = dwType;
				this->dwID = dwID;
			};
			KOBJECTID()
			{
			};
			bool operator<(const KOBJECTID& crh) const
			{
				if (dwType < crh.dwType)
					return true;
				if (dwType == crh.dwType)
					return dwID < crh.dwID;
				return false;
			}
		};

		struct KOBJECTINFO
		{
			D3DXVECTOR2 vPos;
			D3DXVECTOR2 vMin;
			D3DXVECTOR2 vMax;

			int nType;
			int nLeftCount;
			union
			{
				int nFrame;
				int nAnimateIndex;
			};
			int nArrowFrame;
		};
		typedef std::map<KOBJECTID, KOBJECTINFO> KOBJECTINFOMAP;

		struct KSELFINFO : public KOBJECTINFO
		{
			float fDirection;
			int nLayer;
		};

		typedef std::vector<KG3DUIImageRotate2> KMINIMAPARROWARRAY;
		typedef std::vector<KG3DUIImage2> KMINIMAPIMAGEARRAY;

	public:
		KWndMinimap();
		virtual ~KWndMinimap();

		void* operator new(size_t nSize, std::nothrow_t const&) throw();
		void operator delete(void* pData, std::nothrow_t const&) throw();

		void* operator new(size_t nSize);
		void operator delete(void* pData) throw();

	protected:
		void* operator new[](std::size_t nSize, std::nothrow_t const&) throw();
		void operator delete[](void* pData, std::nothrow_t const&) throw();

		void* operator new[](std::size_t nSize) throw(std::bad_alloc);
		void operator delete[](void* pData) throw();

		void* operator new(size_t, void*) throw();
		void operator delete(void*, void*) throw();

		void* operator new[](std::size_t, void*) throw();
		void operator delete[](void*, void*) throw();

#ifdef KGUI_LUA_LOADER
    public:
        static int  Create(IKUIObject **ppObj, Lua_State *pLua, int nTableIndex);
        virtual int Init(Lua_State *pLua, int nTableIndex);
#endif

	public:        
		virtual int			PtInWindow(int x, int y);
		virtual LRESULT		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual int			CanCastTo(LPCSTR szType) const;
        virtual LPCSTR		GetWndType() const;

		int SetMapPath(LPCSTR pcszPath);
		int SetScale(float fScale);
		float GetScale();

		void UpdataStaticPoint(DWORD dwType, DWORD dwID, int nFrame, float fX, float fY, int nLeftCount = -1);
		void UpdataAnimatePoint(DWORD dwType, DWORD dwID, int nGroup, float fX, float fY, int nLeftCount = -1 );
		void UpdataArrowPoint(DWORD dwType, DWORD dwID, int nFrame, int nArrawFrame, float fX, float fY, int nLeftCount = -1);

		void UpdataSelfPos(int nLayer, float fX, float fY, float fDirection);
		
		void RemovePoint(DWORD dwType, DWORD dwID);
		void RemovePoint(DWORD dwType);
		void Clear();

		void GetOverObj(DWORD &dwType, DWORD &dwID);
		void GetSendPos(float &fX, float &fY);

		void SetMinimapType(int nType);
		void LoadPointImageFile(LPCSTR szFile);
		void LoadDefaultImageFile(LPCSTR szFile);
		void LoadSharpImageFile(LPCSTR szFile);
		void SetSelfImageFrame(int nFrame);

	protected:
		virtual void OnPaint();
		virtual void OnBreathe();

	private:
		void FormatDrawData();
		void FormatMapDrawData();
		void FormatSelfDrawData();

		void FormatStaticPoint(KOBJECTINFO &Obj);
		void FormatAnimatePoint(KOBJECTINFO &Obj);
		void FormatArrowPoint(KOBJECTINFO &Obj);
		
		inline void MinimapPosToWndPos(D3DXVECTOR2 &vPos);
		inline void WndPosToMinimapPos(D3DXVECTOR2 &vPos);

	private:
		DWORD m_dwMinimapType;	//��ͼ����

		char m_szMinimapPath[MAX_PATH];	//С��ͼ�ļ���·��
		char m_szPointFile[MAX_PATH];
		float m_fSceneToMinimapScale;	//��Ϸ������С��ͼ�����ű�
		float m_fEachBlockWidth;	//ÿС��Ŀ�

		DWORD m_dwDefaultImageID; //Ĭ�ϵı�����ͼ
		DWORD m_dwSharpImageID;	//С��ͼ��״�ı�����ͼ

		float m_fWndToMinimapScale;	//���ڵ�С��ͼ�����űȡ�

		int m_nImageInfoIndex;	//С��ͼ�϶�����ͼ

		KSELFINFO m_Self;	//�Լ�����Ϣ
		KOBJECTINFOMAP m_Obj;

		KMINIMAPBLOCKTOIMAGE m_Minimap; //��ͼ�Ļ�������
		KMINIMAPARROWARRAY m_aArrow;	//����ͷ�Ļ�������
		KMINIMAPIMAGEARRAY m_aImage;	//��ͨ�Ļ�������

		float m_fSendX;
		float m_fSendY;

		BOOL m_bDataUnFormated;
		BOOL m_bMouseInMinimap;

		BOOL m_bLastOverObj;
		BOOL m_bLastOverSelf;
		KOBJECTID m_LastMouseOverObj;

		char m_szMapFile[MAX_PATH];

		KG3DUIImage2 m_SharpImage;
	};

}

#endif //_INCLUDE_WNDMINIMAP_H_
