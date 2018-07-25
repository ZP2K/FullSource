#pragma once

#include "IEEditor.h"

DWORD GetVariableLengthByType(KG3DDATAFLOWVARIABLETYPE eType);

class KG3DDataFlowDataDefine
{
protected:

	struct VariableDefine 
	{
		KG3DDATAFLOWVARIABLETYPE  eType;//���ݳ�Ա����
		string szName;//���ݳ�Ա����
		DWORD  dwPostionStart;//���ݳ�Ա��ʼλ��
		DWORD  dwLength;//���ݳ�Ա����

		VariableDefine()
		{
			eType = VARIABLETYPE_INT;
			dwPostionStart = 0;
			dwLength =4;
			szName = "NonameVariable";
		}
	};

	HRESULT LoadFromFile(LPCTSTR pFileName);
	HRESULT SaveToFile(LPCTSTR pFileName);

	HRESULT ComputeDataLength();
	DWORD m_dwDataLength;

	HRESULT FindVariableByName(int& nIndex,LPCTSTR pName);

public:
	DWORD GetDataLength()
	{
		return m_dwDataLength;
	}

	DWORD  m_dwType;//�����ݽṹ�����ͱ��
	string m_szName;//�����ݽṹ������
	vector<VariableDefine>m_vecVariableDefine;//�����ݽṹ��ÿ����Ա�Ķ���

	HRESULT SetName(LPCTSTR pName);
	HRESULT GetName(LPCTSTR pName);
	HRESULT SetNumVariable(DWORD dwNum);
	DWORD   GetNumVariable();
	HRESULT SetVariable(DWORD dwIndex,KG3DDATAFLOWVARIABLETYPE eType,LPCTSTR pName);
	HRESULT GetVariable(DWORD dwIndex,KG3DDATAFLOWVARIABLETYPE* peType,LPCTSTR pName);

public:
	KG3DDataFlowDataDefine(void);
	virtual ~KG3DDataFlowDataDefine(void);
};
