#pragma once

//#include "engine/KG_SimpleVector.h"

enum enuStateBlockType
{
	STATE_BLOCK_RENDERSTATE,
	STATE_BLOCK_TEXTURESTATE,
	STATE_BLOCK_SAMPLERSTATE,
	STATE_BLOCK_FORCE_DWORD = 0xffffffff,
};

struct StateBlock
{
	DWORD m_dwType;
	union 
	{
		D3DRENDERSTATETYPE m_dwRenderState;
		struct
		{
			DWORD m_dwTextureStageIndex;
			D3DTEXTURESTAGESTATETYPE m_dwTextureState;
		};
		struct  
		{
			DWORD dwSampler;
			D3DSAMPLERSTATETYPE dwType;
		};
	};
	DWORD m_dwValue;
};


class KG3DRenderState
{
public:
	KG3DRenderState(void);
	~KG3DRenderState(void);
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	HRESULT SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	HRESULT SetSamplerState(
		DWORD Sampler,
		D3DSAMPLERSTATETYPE Type,
		DWORD Value
		);
	void Restore();
private:
	std::vector<StateBlock> m_vecStates;	//��SimpleVector��Ŀ���Ǽ����ڴ���䣬����vector��reserve��ͬ�������ã���vector�Ͳ��ð�SimpleVector��ģ�����ͷ�ļ�������ÿ��ʹ��RenderState�Ķ����ı���ʱ����
};

KG3DRenderState& g_GetRenderStateMgr();
