#pragma once

//////////////////////////////////////////////////////////////////////////
//����

class KPhysicsForceField
{
private:
	char   m_szProperty[512];//������������ڴ�ռ�
	AABBOX m_Box;//��Χ��
	DWORD  m_dwType;

public:
	HRESULT GetForce(D3DXVECTOR3* pForce,D3DXVECTOR3* pPosition);

	KPhysicsForceField(void);
	virtual ~KPhysicsForceField(void);
};

class KPhysicsForceFieldManager
{
public:

public:
	virtual HRESULT GetForce(D3DXVECTOR3* pForce,D3DXVECTOR3* pPosition);

	KPhysicsForceFieldManager(void);
	virtual ~KPhysicsForceFieldManager(void);
};
