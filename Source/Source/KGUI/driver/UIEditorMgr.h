////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : UIEditorMgr.h
//  Version     : 1.0
//  Creator     : Hu Changyin
//  Create Date : 2007-12-5 14:57:45
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_UIEDITORMGR_H_
#define _INCLUDE_UIEDITORMGR_H_

////////////////////////////////////////////////////////////////////////////////

#include "SO3UI/ipluginshell.h"

namespace UI
{

class KUIEditorMgr
{
public:
	int OpenUIEditor(int nForce);
	int CLoseUIEditor();
	int IsUIEditorOpend();

	//����༭�����ÿͻ���
	static LRESULT UIEditorCallback(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//�ͻ��˵��ý���༭��
	LRESULT UIEditorOperation(UINT uMsg, WPARAM wParam, LPARAM lParam);

	KUIEditorMgr();
	virtual ~KUIEditorMgr();
protected:
private:
	HMODULE m_hUIEditor;
	IPluginShell *m_pUiEditor;
};

}

#endif //_INCLUDE_UIEDITORMGR_H_
