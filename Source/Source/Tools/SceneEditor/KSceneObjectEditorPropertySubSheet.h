#pragma once


// KSceneObjectEditorPropertySubSheet �Ի���

class KSceneObjectEditorPropertySubSheet : public CPropertyPage
{
	DECLARE_DYNAMIC(KSceneObjectEditorPropertySubSheet)

public:
	KSceneObjectEditorPropertySubSheet();
	virtual ~KSceneObjectEditorPropertySubSheet();

// �Ի�������
	enum { IDD = IDD_OBJECTEDIT_PROPERTY_SHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
