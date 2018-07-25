#pragma once
#define CAPTION_INVALIDATE_INDEX 0xffffffff

class KG3DTexture;
class KG3DCaptionTextureManager;

class KG3DCaption2D
{
public:
	KG3DCaption2D();
	~KG3DCaption2D(){};
	//����ȫ�ֵĿɼ���
	void SetVisible(DWORD dwID,
		DWORD dwIndex, 
		BOOL bVisible);

	HRESULT Init();
	void UnInit();

    HRESULT OnLostDevice();
    HRESULT OnResetDevice();

	DWORD Add(DWORD dwID,
		DWORD dwIndex, 
		LPCSTR strContent);
	void Remove(unsigned int uID);
	void SetRenderData(DWORD dwID, 
		const D3DXVECTOR3 *pPos, 
		const D3DCOLORVALUE *pColor, 
		const float *pPercentage,
		const D3DCOLORVALUE *pPercentageColor);
	HRESULT Render();
	HRESULT Render2D();
	const static DWORD MAX_NUM_CAPTION = 4;
private:
	struct LineContent
	{
		DWORD dwMgrIndex;
		DWORD dwBlockIndex;
		D3DCOLORVALUE color;
		LineContent()
		{
			UnInit();
		}
		void UnInit()
		{
			dwBlockIndex = CAPTION_INVALIDATE_INDEX;
			dwMgrIndex = 0;
		}
	};

	struct RenderData
	{
		BOOL bUsed;
		D3DXVECTOR3 vec3Pos;
		float fPercentage;
		D3DCOLORVALUE PercentageColor;
		BOOL m_bVisible[MAX_NUM_CAPTION + 1];//�ɼ��ԣ� MAX_NUM_CAPTION�������Ѫ����
		LineContent Content[MAX_NUM_CAPTION];
		RenderData();
		void UnInit();
	};
	std::vector<RenderData> m_RenderData;
	std::set<DWORD> m_Free;
	KG3DCaptionTextureManager *m_pTextureMgr;

	struct Vertex 
	{
		D3DXVECTOR3 vec3Pos;
		D3DXVECTOR2 vec2Tex0;
		const static DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};
	Vertex m_Vertex[6];
	Vertex m_Percentage[6];
	float m_fCaptionHeight;
	//���������������ļ���Ѫ������
	KG3DTexture *m_pFrontTexture;//Ѫ����ǰ����ͼ
	KG3DTexture *m_pBackTexture;//Ѫ���ı�����ͼ
	float m_fPercentageWidth;//Ѫ���Ŀ��
	float m_fPercentageHeight;//Ѫ���ĸ߶�
	float m_fBorderX;//Ѫ���ߵ�X���
	float m_fBorderY;//Ѫ���ߵ�Y���
	DWORD m_dwBackBorderColor;//Ѫ���߿����ɫ
	float m_fBackBorderColorScale;//Ѫ������ɫ˥������ǰ��ɫ
};

class KG3DCaptionTextureManager
{
	friend void KG3DCaption2D::UnInit();
public:
	enum ItemState
	{
		IS_PENDING,//�ռ���� ��Ҫ����
		IS_READY,//��� ����ʹ��
	};

	struct ItemInfo
	{
		DWORD dwWidth;
		DWORD dwHeight;
		ItemState State;
		std::string strContent;
		int nRef;
		ItemInfo()
		{
			Reset();
		}
		void Reset()
		{
			dwWidth = 0;
			dwHeight = 0;
			State = IS_PENDING;
			strContent = "";
			nRef = 0;
		}
	};

	KG3DCaptionTextureManager();
	LPDIRECT3DTEXTURE9 m_pTexture;
	const static float s_fOffset;

	HRESULT Init(LPCSTR strFontFile, 
		int nSize, 
		BOOL bBold, 
		BOOL bItalic, 
		DWORD dwMgrIndex);
	void UnInit();

    HRESULT OnLostDevice();
    HRESULT OnResetDevice();

	DWORD Add(LPCSTR strContent, 
		DWORD &dwMgrIndex);
	void Remove(DWORD dwMgrIndex, 
		DWORD dwBlockIndex);

	HRESULT GetUVCoordinate(DWORD dwMgrIndex,
		DWORD dwBlockIndex, 
		D3DXVECTOR2 &vA, 
		D3DXVECTOR2 &vC);

	float GetAspect();

	HRESULT OnRender(bool bNeedRestore);
	KG3DCaptionTextureManager *m_pNextMgr;

	HRESULT SetTexture(DWORD dwStage,
		DWORD dwIndex);

private:
	HRESULT CreateFont();
	typedef struct _tagTT_OFFSET_TABLE{
		USHORT	uMajorVersion;
		USHORT	uMinorVersion;
		USHORT	uNumOfTables;
		USHORT	uSearchRange;
		USHORT	uEntrySelector;
		USHORT	uRangeShift;
	}TT_OFFSET_TABLE;

	typedef struct _tagTT_TABLE_DIRECTORY{
		char	szTag[4];			//table name
		ULONG	uCheckSum;			//Check sum
		ULONG	uOffset;			//Offset from beginning of file
		ULONG	uLength;			//length of the table in bytes
	}TT_TABLE_DIRECTORY;

	typedef struct _tagTT_NAME_TABLE_HEADER{
		USHORT	uFSelector;			//format selector. Always 0
		USHORT	uNRCount;			//Name Records count
		USHORT	uStorageOffset;		//Offset for strings storage, from start of the table
	}TT_NAME_TABLE_HEADER;

	typedef struct _tagTT_NAME_RECORD{
		USHORT	uPlatformID;
		USHORT	uEncodingID;
		USHORT	uLanguageID;
		USHORT	uNameID;
		USHORT	uStringLength;
		USHORT	uStringOffset;	//from start of storage area
	}TT_NAME_RECORD;
	
	HRESULT GetFontNameFromFile(LPCSTR lpszFilePath, 
		LPSTR strFaceName,
		size_t uSize);

	DWORD FindContentNext(LPCSTR strContent, DWORD& dwIndex);
	static std::string m_strFontName;
	static std::string m_strFontFaceName;
	static BOOL m_bBold;
	static BOOL m_bItalic;
	static int m_nFontSize;
	static DWORD m_dwBlockWidth;
	static DWORD m_dwBlockHeight;
	static DWORD m_dwNumCloumn;
	static DWORD m_dwNumRow;
	static LPD3DXFONT m_pFont;
	std::map<std::string, DWORD> m_StringToData;
	std::set<DWORD> m_FreeBlock;
	std::deque<DWORD> m_Released;
	std::set<DWORD> m_DataPending;
	std::vector<ItemInfo> m_Data;
	BOOL m_bNeedClear;
	DWORD m_dwMgrIndex;//Mgr��Index
	static HANDLE m_hFontHandle;
};


extern KG3DCaption2D g_cCaption2D;