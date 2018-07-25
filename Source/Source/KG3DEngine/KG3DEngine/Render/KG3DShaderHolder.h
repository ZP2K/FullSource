////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KG3DShaderHolder.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-8-12 17:04:06
/*  Comment     : �Զ��豸��ʧ��Reload�������¼��ء��Զ�Shader�ͷţ���Ϊ�¼�Listener��֪ͨ
D3D�������¼������Բ��õ����������ͷ�
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KG3DSHADERHOLDER_H_
#define _INCLUDE_KG3DSHADERHOLDER_H_
////////////////////////////////////////////////////////////////////////////////
/*
ʹ��GetEffect��
KG3DShaderHolder SH = g_GetShaderManager().Create(XX);//�ظ���Shader���Զ�����һ������
LPD3DXEFFECT pEffect = SH.GetEffect();
_ASSERTE(NULL != pEffect);
if(SUCCEEDED(pEffect->Begin(NULL, 0)))
{
	pEffect->End();
}

���¼��أ�
hr = SH.Reload();
if(FAILED(hr))
{
	����дLog��ɶҲ����������Ϊ������������ĵĿ�����ֻ�У��ļ������⣬�ڴ����ʧ�ܡ�
}

���Ҫ����һ��Shader����������ͬ���ļ�����ShaderManger�д���������ֱ�Ӹ���KG3DShaderHolder.

���ü������豸�ָ�ʲô��ȫ�����ùܣ��Ѿ�ȫ���ӹ��ˡ���������豸�ָ���ʱ����Щ���������������g_GetShaderManager().Create������һ������

��ΪEffect���SetTexture������ü���������
*/

struct  D3DXMACRO_ARRAY
{
	enum
	{
		em_max_macro = 20,	//���ֻ���������Ŀ�ĺ궨��
	};
	const D3DXMACRO* pMacroDefs;	//D3DXMACRO���飬��������NULL,NULL��D3DXMACRO������������Ǹ�Ҳ������uMacroCount����
	size_t	uMacroCount;	//����Ԫ�ظ���

	BOOL MergeDefinition(std::vector<D3DXMACRO>& buffer, const D3DXMACRO_ARRAY& other);

	static D3DXMACRO	GetEmptyMacro()
	{
		D3DXMACRO emptyOne = {NULL, NULL};
		return emptyOne;
	}
	static D3DXMACRO_ARRAY GetEmptyMacroArray()
	{
		D3DXMACRO_ARRAY emptyOne = {
			NULL,
			0,
		};
		return emptyOne;
	}
	static bool IsValid(const D3DXMACRO_ARRAY& macroArray);
};

struct KG3DShaderEventHandler 
{
	//����Ҫ����p->OnLostDevice����OnResetDevice��ShaderMgr�Ѿ��������ģ���Ҫ�����Ǵ�����������
	virtual HRESULT OnLostDevice(LPD3DXEFFECT p){return S_OK;}
	virtual	HRESULT OnResetDevice(LPD3DXEFFECT p){return S_OK;}
	virtual HRESULT OnReload(LPD3DXEFFECT p){return S_OK;}
	virtual KG3DShaderEventHandler* Clone()const = 0;
	virtual ULONG STDMETHODCALLTYPE Release() = 0;
	virtual ~KG3DShaderEventHandler() = 0{}
};

struct KG3DShaderPtrHolder;
class KG3DShaderHolder
{
public:
	LPD3DXEFFECT GetEffect();//��֤�ǿ�,����Release
    LPVOID GetEffectParamHandleBuffer(size_t *puRetBufferSize);	//���BufferҪ���ü�ȡ�����������������֤��������
	HRESULT		 Reload();
	KG3DShaderPtrHolder* GetShaderPtrHolder(){return m_pShaderHolder;}

	KG3DShaderHolder();
	explicit KG3DShaderHolder(KG3DShaderPtrHolder* pHolder);
	KG3DShaderHolder(const KG3DShaderHolder& Other);
	KG3DShaderHolder& operator=(const KG3DShaderHolder& Other);
	~KG3DShaderHolder();

	KG_TEST_DECLARE_CLASS_TEST(KG3DShaderHolder);
private:	
	KG3DShaderPtrHolder* m_pShaderHolder;
};

struct KG3DShaderPtrHolder
{
	virtual LPD3DXEFFECT GetEffect() = 0;//���ܷ���NULL
    virtual LPVOID GetEffectParamHandleBuffer(unsigned *puRetBufferSize) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
	virtual	ULONG STDMETHODCALLTYPE Release() = 0;

	virtual ~KG3DShaderPtrHolder() = 0{}
};

struct KG3DShaderPtrHolderDummy : public KG3DShaderPtrHolder
{
	virtual LPD3DXEFFECT GetEffect(){return NULL;}
    virtual LPVOID GetEffectParamHandleBuffer(size_t *puRetBufferSize) 
    {
        if (NULL != puRetBufferSize)
        {
            *puRetBufferSize = 0;
        }
        return NULL;
    }
	virtual ULONG STDMETHODCALLTYPE AddRef() {return 1;}
	virtual	ULONG STDMETHODCALLTYPE Release() {return 0;}
};

#endif //_INCLUDE_KG3DSHADERHOLDER_H_
