#pragma once

//#include ".\sceneswordtest.h"
#include "KEditorDocumentBase.h"

// KSwordTestDoc �ĵ�

class KSwordTestDoc : public CDocument , public KEditorDocumentBase
{
	DECLARE_DYNCREATE(KSwordTestDoc)

public:
	//HRESULT SetScene(KSceneSwordTest *pScene);

	//KSceneSwordTest* m_lpSwordTest;

	KSwordTestDoc();
	virtual ~KSwordTestDoc();
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
