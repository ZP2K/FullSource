#pragma once


class KG3DHeightMap
{
	friend class KG3DSceneOutDoorSpaceMgr;
private:
	struct PNG_FILE_INFO
	{
		unsigned width;
		unsigned height;
		int bit_depth;
		int color_type;
		int interlace_type;
		int compression_type;
		int filter_type;
	};  
	PNG_FILE_INFO m_PngFileInfo;

	BITMAPINFO    m_Bmpinfo;

protected:

	int m_nWidth;
	int	m_nHeight;//�������

	float* m_lpData;
	D3DXVECTOR3* m_lpNormal;

	float m_fHeightLowest;
	float m_fHeightTotal;

	BYTE *LoadBmpFile(int&nSize,const char FileName[],BITMAPINFO *info);
	BYTE *LoadPngFile(const char file_name[]);

	HRESULT SmoothMapHeight();//ƽ����������
	HRESULT SmoothNormal();//ƽ���������ж��㷨����
	HRESULT SmoothNormal(int xStart,int xEnd,int zStart,int zEnd); //ƽ��ָ���ľ��η�Χ�ڵĵ��ζ��㷨����
	int GetPlaneBufferIndex(int nX,int nZ);
public:
	HRESULT SaveToR32File(int nWidth,int nHeight,LPCTSTR pFileName);

	int GetMapHeight();
	int GetMapWidth();
	float*  GetVertexPointer(int nX,int nZ);
	D3DXVECTOR3* GetVertexNormalPointer(int nX,int nZ);
	HRESULT SetHeight(int xStart,int xEnd,int zStart,int zEnd,float fHeight);
	HRESULT SmoothMapHeight(int xStart,int xEnd,int zStart,int zEnd,float K);//ƽ��ָ���ľ��η�Χ�ڵĵ��θ߶�
	HRESULT ComputeNormal();//����������ж��㷨����
	HRESULT ComputeNormal(int xStart,int xEnd,int zStart,int zEnd);//����ָ���ľ��η�Χ�ڵĵ��ζ��㷨����,m_lpData���Ķ��Ǿ��Ը߶�ֵʱ�õ�

	float GetHeight(float fX,float fY);
	D3DXVECTOR3 GetNormal(float fX,float fY);

	HRESULT SetHeightScale(float fLowest,float fTotal);

	HRESULT Init(int nWidth,int nHeight);
	HRESULT UnInit();

	HRESULT CreateFromPngFile(LPSTR pFileName);
	HRESULT CreateFromBmpFile(LPCTSTR FileDir);

	HRESULT SaveToHeightMapFile(const char cszFileName[]);
	HRESULT LoadFromHeightMapFile(const char cszFileName[]);

	HRESULT CreateFromR32File(int nWidth,int nHeight,LPCTSTR pFileName);
	HRESULT TranslateAbsoluteData();

	KG3DHeightMap(void);
	~KG3DHeightMap(void);
};
