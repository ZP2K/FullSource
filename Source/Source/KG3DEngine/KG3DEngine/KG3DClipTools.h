#pragma once

class KG3DMesh;
class KG3DBip;
class KG3DAni;
class KG3DClip;


class KG3DClipTools
{
public:
	
	KG3DClipTools(void);
	~KG3DClipTools(void);
	HRESULT CheckClipByBip(KG3DClip* pClip, KG3DBip* pBip);
	HRESULT CheckClipByMesh(KG3DClip* pClip, KG3DMesh* pMesh);
	
	//���̼߳��ض���ʱ��ĳ�ʼ��飬 ������������Ŀ�Ƿ��Ŀ��ģ��һ�£� ��ϸ�ļ�飨�������֣��ļ����Ҫ�������֮����ò���Preload��׺�ĺ���
	/*HRESULT CheckClipByBipPreload(KG3DClip *pClip,
		const KG3DBip *pBip);
	HRESULT CheckClipByMeshPreload(KG3DClip* pClip, 
		KG3DMesh* pMesh);*/
private:
	HRESULT Helper_CheckClipByMeshBone(KG3DClip* pClip, KG3DMesh* pMesh);
};

extern KG3DClipTools& GetClipTool();
