// RecordPlayer.cpp : Implementation of CRecordPlayer

#include "stdafx.h"
#include "RecordPlayer.h"

extern IKG3DEngineManager* g_p3DEngine;

// CRecordPlayer


STDMETHODIMP CRecordPlayer::RePlay(int* pRetResult)
{
    // TODO: Add your implementation code here
    *pRetResult = false;
    HRESULT hRetCode = m_piRecord->RePlay();
    if (hRetCode == S_OK)
        *pRetResult = true;

    return S_OK;
}

STDMETHODIMP CRecordPlayer::PauseRePlay(int* pRetResult)
{
    // TODO: Add your implementation code here
    *pRetResult = false;
    HRESULT hRetCode = m_piRecord->PauseRePlay();
    if (hRetCode == S_OK)
        *pRetResult = true;

    return S_OK;
}

STDMETHODIMP CRecordPlayer::StopRePlay(void)
{
    // TODO: Add your implementation code here
    m_piRecord->StopRePlay();

    return S_OK;
}

STDMETHODIMP CRecordPlayer::LoadRecordFile(BSTR pstrFileName)
{
    // TODO: Add your implementation code here
    HRESULT hResult       = E_FAIL;
    _bstr_t bTemp = pstrFileName;
    CHAR* pszFileName = bTemp;

    IERecPlayer* piRecord = static_cast<IERecPlayer*>(g_p3DEngine->GetNewRecPlayer());
    SetRecord(piRecord);

    if (piRecord)
    {
        hResult = piRecord->LoadRecordFile(pszFileName);
        if (FAILED(hResult))
        {
            MessageBoxA(NULL, "¼���ļ���ȡʧ�ܣ������˳�","����",MB_OK | MB_ICONWARNING);
            goto Exit0;
        }
    }

    InitMap();
    hResult = S_OK;
Exit0:

    return hResult;
}

STDMETHODIMP CRecordPlayer::SetReplayWindow(ULONG uhWnd)
{
    // TODO: Add your implementation code here
    m_piRecord->SetReplayWindow((HWND)uhWnd);

    return S_OK;
}

STDMETHODIMP CRecordPlayer::FrameMoveRePlay(void)
{
    // TODO: Add your implementation code here
    m_piRecord->FrameMoveRePlay();
    g_p3DEngine->FrameMove();

    return S_OK;
}

STDMETHODIMP CRecordPlayer::IsPlayFinished(int* pbFinished)
{
    // TODO: Add your implementation code here

    *pbFinished = m_piRecord->IsPlayFinished();

    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetLoadingProgress(FLOAT* pnLoadFilePer, FLOAT* pnStepToEventPer, INT* pnLoadScenePer)
{
    // TODO: Add your implementation code here

    return S_OK;
}

STDMETHODIMP CRecordPlayer::SaveRecordFile(BSTR pstrFileName)
{
    // TODO: Add your implementation code here

    return S_OK;
}

STDMETHODIMP CRecordPlayer::SetOption(int nWidth, int nHeight)
{
    // TODO: Add your implementation code here'
    RecordOption recordOption;

    recordOption.Size.cx = nWidth;
    recordOption.Size.cy = nHeight;
    recordOption.nAutoScale = FALSE;
    recordOption.FiterType = D3DTEXF_LINEAR;
    recordOption.fInterval = 41;
    recordOption.ImageType = D3DXIFF_JPG;
    recordOption.bKeep = FALSE;

    g_p3DEngine->SetRecordOption(&recordOption);

    return S_OK;
}

STDMETHODIMP CRecordPlayer::FreeCamera(void)
{
    // TODO: Add your implementation code here

    m_piRecord->FreeCamera();
    return S_OK;
}

STDMETHODIMP CRecordPlayer::IsFreeCamera(int* pnRetResult)
{
    // TODO: Add your implementation code here
    HRESULT hRetCode = E_FAIL;
    *pnRetResult = 0;
    hRetCode = m_piRecord->IsFreeCamera();
    if (hRetCode == S_OK)
        *pnRetResult = 1;
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetCameraType(void)
{
    // TODO: Add your implementation code here

    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetObjSize(int* pnSize)
{
    // TODO: Add your implementation code here
    *pnSize = m_piRecord->GetObjNum();
    return S_OK;
}

HRESULT CRecordPlayer::GetDesByID(string& strDes,FunctionType Ftype)
{
    switch (Ftype)
    {
    case FT_NONE:
        strDes = "FT_NONE";
        break;
    case FT_KG3DMODEL_LOADFROMFILE:
        strDes = "FT_KG3DMODEL_LOADFROMFILE";
        break;
    case FT_KG3DSCENEEDITOR_LOADFROMFILE:
        strDes = "FT_KG3DSCENEEDITOR_LOADFROMFILE";
        break;
    case FT_KG3DMODEL_SETTRANSLATION:		
        strDes = "FT_KG3DMODEL_SETTRANSLATION";
        break;
    case FT_KG3DMODEL_SETROTATION:		
        strDes = "FT_KG3DMODEL_SETROTATION";
        break;
    case FT_KG3DMODEL_SETROTATIONCENTER:		
        strDes = "FT_KG3DMODEL_SETROTATIONCENTER";
        break;
    case FT_KG3DMODEL_SETSCALING:	
        strDes = "FT_KG3DMODEL_SETSCALING";
        break;
    case FT_KG3DMODEL_SETSCALINGCENTER:		
        strDes = "FT_KG3DMODEL_SETSCALINGCENTER";
        break;
    case FT_KG3DMODEL_SETSCALINGROTATION:		
        strDes = "FT_KG3DMODEL_SETSCALINGROTATION";
        break;
    case FT_KG3DMODEL_RESETTRANSFORM:
        strDes = "FT_KG3DMODEL_RESETTRANSFORM";
        break;
    case FT_KG3DMODEL_BINDTOSOCKET:	
        strDes = "FT_KG3DMODEL_BINDTOSOCKET";
        break;
    case FT_KG3DMODEL_BINDTOBONE:		
        strDes = "FT_KG3DMODEL_BINDTOBONE";
        break;
    case FT_KG3DMODEL_ATTACHMODEL:		
        strDes = "FT_KG3DMODEL_ATTACHMODEL";
        break;
    case FT_KG3DMODEL_DETCHMODEL:		
        strDes = "FT_KG3DMODEL_DETCHMODEL";
        break;
    case FT_KG3DMODEL_PLAYANIMATION:		
        strDes = "FT_KG3DMODEL_PLAYANIMATION";
        break;
    case FT_KG3DMODEL_PALYSPLITANIMATION:	
        strDes = "FT_KG3DMODEL_PALYSPLITANIMATION";
        break;
    case FT_KG3DMODEL_LOADMATERIALFROMFILE:		
        strDes = "FT_KG3DMODEL_LOADMATERIALFROMFILE";
        break;
    case FT_KG3DMODEL_SETALPHA:		
        strDes = "FT_KG3DMODEL_SETALPHA";
        break;
    case FT_KG3DMODEL_SETCALLBACKRENDER:
        strDes = "FT_KG3DMODEL_SETCALLBACKRENDER";
        break;
    case FT_KG3DMODEL_DESTRUCT:
        strDes = "FT_KG3DMODEL_DESTRUCT";
        break;
    case FT_KG3DSCENEEDITOR_DESTRUCT:
        strDes = "FT_KG3DSCENEEDITOR_DESTRUCT";
        break;
    case FT_KG3DSCENEEDITOR_ADDRENDERENTITY:	
        strDes = "FT_KG3DSCENEEDITOR_ADDRENDERENTITY";
        break;
    case FT_KG3DSCENEEDITOR_REMOVERENDERENTITY:
        strDes = "FT_KG3DSCENEEDITOR_REMOVERENDERENTITY";
        break;
    case FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW:
        strDes = "FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW";
        break;
    case FT_KG3DCAMERA_UPDATECAMERA:	
        strDes = "FT_KG3DCAMERA_UPDATECAMERA";
        break;
    case FT_KG3DCAMERA_SETPOSITION:		
        strDes = "FT_KG3DCAMERA_SETPOSITION";
        break;
    case FT_KG3DCAMERA_SETLOOKATPOSITION:		
        strDes = "FT_KG3DCAMERA_SETLOOKATPOSITION";
        break;
    case FT_KG3DCAMERA_SETUPDIRECTION:		
        strDes = "FT_KG3DCAMERA_SETUPDIRECTION";
        break;
    case FT_KG3DCAMERA_SETGLOBALPERSPECTIVE:		
        strDes = "FT_KG3DCAMERA_SETGLOBALPERSPECTIVE";
        break;
    case FT_KG3DCAMERA_SETGLOBALORTHOGONAL:
        strDes = "FT_KG3DCAMERA_SETGLOBALORTHOGONAL";
        break;
    case FT_KG3DCAMERA_SETTRACKINFO:
        strDes = "FT_KG3DCAMERA_SETTRACKINFO";
        break;	
    default:
        strDes = "DEFAULT";
        break;
    }
    return S_OK;
}

HRESULT CRecordPlayer::GetFuncByDes(string strDes,FunctionType &Ftype)
{
    if (strDes == "FT_NONE")
        Ftype = FT_NONE;
    else if (strDes == "FT_KG3DMODEL_LOADFROMFILE")
        Ftype = FT_KG3DMODEL_LOADFROMFILE;
    else if (strDes == "FT_KG3DSCENEEDITOR_LOADFROMFILE")
        Ftype = FT_KG3DSCENEEDITOR_LOADFROMFILE;
    else if (strDes == "FT_KG3DMODEL_SETTRANSLATION")
        Ftype = FT_KG3DMODEL_SETTRANSLATION;
    else if (strDes == "FT_KG3DMODEL_SETROTATION")
        Ftype = FT_KG3DMODEL_SETROTATION;
    else if (strDes == "FT_KG3DMODEL_SETROTATIONCENTER")		
        Ftype = FT_KG3DMODEL_SETROTATIONCENTER;
    else if (strDes == "FT_KG3DMODEL_SETSCALING")
        Ftype = FT_KG3DMODEL_SETSCALING;
    else if (strDes == "FT_KG3DMODEL_SETSCALINGCENTER")
        Ftype = FT_KG3DMODEL_SETSCALINGCENTER;
    else if (strDes == "FT_KG3DMODEL_SETSCALINGROTATION")
        Ftype = FT_KG3DMODEL_SETSCALINGROTATION;
    else if (strDes == "FT_KG3DMODEL_RESETTRANSFORM")
        Ftype = FT_KG3DMODEL_RESETTRANSFORM;
    else if (strDes == "FT_KG3DMODEL_BINDTOSOCKET")
        Ftype = FT_KG3DMODEL_BINDTOSOCKET;
    else if (strDes == "FT_KG3DMODEL_BINDTOBONE")
        Ftype = FT_KG3DMODEL_BINDTOBONE;
    else if (strDes == "FT_KG3DMODEL_ATTACHMODEL")
        Ftype = FT_KG3DMODEL_ATTACHMODEL;
    else if (strDes == "FT_KG3DMODEL_DETCHMODEL")
        Ftype = FT_KG3DMODEL_DETCHMODEL;
    else if (strDes == "FT_KG3DMODEL_PLAYANIMATION")
        Ftype = FT_KG3DMODEL_PLAYANIMATION;
    else if (strDes == "FT_KG3DMODEL_PALYSPLITANIMATION")
        Ftype = FT_KG3DMODEL_PALYSPLITANIMATION;
    else if (strDes == "FT_KG3DMODEL_LOADMATERIALFROMFILE")	
        Ftype = FT_KG3DMODEL_LOADMATERIALFROMFILE;
    else if (strDes == "FT_KG3DMODEL_SETALPHA")
        Ftype = FT_KG3DMODEL_SETALPHA;
    else if (strDes == "FT_KG3DMODEL_SETCALLBACKRENDER")
        Ftype = FT_KG3DMODEL_SETCALLBACKRENDER;
    else if (strDes == "FT_KG3DMODEL_DESTRUCT")
        Ftype = FT_KG3DMODEL_DESTRUCT;
    else if (strDes == "FT_KG3DSCENEEDITOR_DESTRUCT")
        Ftype = FT_KG3DSCENEEDITOR_DESTRUCT;
    else if (strDes == "FT_KG3DSCENEEDITOR_ADDRENDERENTITY")
        Ftype = FT_KG3DSCENEEDITOR_ADDRENDERENTITY;
    else if (strDes == "FT_KG3DSCENEEDITOR_REMOVERENDERENTITY")
        Ftype = FT_KG3DSCENEEDITOR_REMOVERENDERENTITY;
    else if (strDes == "FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW")
        Ftype = FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW;
    else if (strDes == "FT_KG3DCAMERA_UPDATECAMERA")
        Ftype = FT_KG3DCAMERA_UPDATECAMERA;
    else if (strDes == "FT_KG3DCAMERA_SETPOSITION")
        Ftype = FT_KG3DCAMERA_SETPOSITION;
    else if (strDes == "FT_KG3DCAMERA_SETLOOKATPOSITION")
        Ftype = FT_KG3DCAMERA_SETLOOKATPOSITION;
    else if (strDes == "FT_KG3DCAMERA_SETUPDIRECTION")
        Ftype = FT_KG3DCAMERA_SETUPDIRECTION;
    else if (strDes == "FT_KG3DCAMERA_SETGLOBALPERSPECTIVE")
        Ftype = FT_KG3DCAMERA_SETGLOBALPERSPECTIVE;
    else if (strDes == "FT_KG3DCAMERA_SETGLOBALORTHOGONAL")
        Ftype = FT_KG3DCAMERA_SETGLOBALORTHOGONAL;
    else if (strDes == "FT_KG3DCAMERA_SETTRACKINFO")
        Ftype = FT_KG3DCAMERA_SETTRACKINFO;
    else
        Ftype = FT_NONE;
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetFirstObj(int* pnID, BSTR* pstrRetName, int* pnType)
{
    // TODO: Add your implementation code here
    RecObj* pObj = NULL;
    DWORD dwID = 0;
    HRESULT hRetCode = E_FAIL;

    hRetCode = m_piRecord->GetFirstObj(&dwID, &pObj);
    if (hRetCode == E_FAIL)
    {
        *pnID = -1;
        *pstrRetName = NULL;
        *pnType = 0;
        m_pRecObj = NULL;
    }
    else
    {
        *pnID = dwID;
        // A2BSTR(pObj->ObjName.c_str())�������Ҳ����
        *pstrRetName = _com_util::ConvertStringToBSTR(pObj->ObjName.c_str());
        *pnType = pObj->ObjType;
        m_pRecObj = pObj;
    }
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetNextObj(int* pnID, BSTR* pstrRetName, int* pnType)
{
    // TODO: Add your implementation code here
    RecObj* pObj = NULL;
    DWORD dwID = 0;
    HRESULT hRetCode = E_FAIL;
    
    hRetCode = m_piRecord->GetNextObj(&dwID, &pObj);
    if (hRetCode == E_FAIL)
    {
        *pnID = -1;
        *pstrRetName = NULL;
        *pnType = 0;
        m_pRecObj = NULL;
    }
    else
    {
        *pnID = dwID;
        *pstrRetName = _com_util::ConvertStringToBSTR(pObj->ObjName.c_str());
        *pnType = pObj->ObjType;
        m_pRecObj = pObj;
    }
    return S_OK;
}

STDMETHODIMP CRecordPlayer::InitTree(void)
{
    // TODO: Add your implementation code here
    m_piRecord->Init();

    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetFirstEvent(BSTR* pnFunction, int* pnInfo)
{
    // TODO: Add your implementation code here
    EventRecord* pEvent = m_pRecObj->pEventHead;
    string strFunction;
    if (pEvent == NULL)
    {
        goto Exit0;
    }

    FunctionType funtionType = pEvent->EventFunction;
    GetDesByID(strFunction, funtionType);
    *pnFunction = _com_util::ConvertStringToBSTR(strFunction.c_str());
    
    m_pCurrentEvent = pEvent;
    *pnInfo = (int)pEvent;

Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetNextEvent(BSTR* pnFunction, int* pnInfo)
{
    // TODO: Add your implementation code here
    string strFunction;

    EventRecord* pEvent = m_pCurrentEvent->pNextEvent;

    if (pEvent == NULL)
    {
        *pnFunction = NULL;
        goto Exit0;
    }

    FunctionType funtionType = pEvent->EventFunction;
    GetDesByID(strFunction, funtionType);
    *pnFunction = _com_util::ConvertStringToBSTR(strFunction.c_str());
    
    m_pCurrentEvent = pEvent;
    *pnInfo = (int)pEvent;

Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetInformation(int pnInfo, 
                                           BSTR* pstrLabel, 
                                           BSTR* pstrTextBox, 
                                           int* pnNum)
{
    // TODO: Add your implementation code here
    HRESULT hResult = E_FAIL;
    EventRecord* pEvent = (EventRecord*)pnInfo;
    string strLabel = "";
    string strTextBox = "";
    char szTemp[MAX_PATH]; // ������
    int  nTime = -2;
    *pnNum = 0;

    KG_PROCESS_ERROR(pEvent);

    strLabel += "����ʱ�� ";
    strLabel += "����֡�� ";
    
    m_piRecord->GetEventTime(pEvent, nTime);
    KG_PROCESS_ERROR(nTime >= 0);
    sprintf_s(szTemp, "%d", nTime);
    strTextBox += szTemp;
    strTextBox += " ";
    sprintf_s(szTemp, "%lu", pEvent->dwFrame);
    strTextBox += szTemp;
    strTextBox += " ";
    
    switch(pEvent->EventFunction)
    {
    case FT_NONE:
        break;
    case FT_KG3DMODEL_LOADFROMFILE:
        {
            strLabel += "�����ļ�·�� ";
            strLabel += "�����ļ���ϣ ";
            strLabel += "�����ļ�ѡ��";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            char* pszFileName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(pszFileName);
            unsigned* puFileNameHash = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]]));
            KG_PROCESS_ERROR(puFileNameHash);
            unsigned* puOption = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[3]]));
            KG_PROCESS_ERROR(puOption);

            strTextBox += pszFileName;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puFileNameHash);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puOption);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DSCENEEDITOR_LOADFROMFILE:
        {
            strLabel += "�����ļ�·�� ";
            strLabel += "�����ļ���ϣ ";
            strLabel += "�����ļ�ѡ��";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            char* pszFileName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(pszFileName);
            unsigned* puFileNameHash = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]]));
            KG_PROCESS_ERROR(puFileNameHash);
            unsigned* puOption = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[3]]));
            KG_PROCESS_ERROR(puOption);

            strTextBox += pszFileName;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puFileNameHash);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puOption);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETTRANSLATION:
        {
            strLabel += "λ��x���� ";
            strLabel += "λ��y���� ";
            strLabel += "λ��z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecValue = (D3DXVECTOR3*)(&pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]);
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETROTATION:
        {
            strLabel += "��תx���� ";
            strLabel += "��תy���� ";
            strLabel += "��תz���� ";
            strLabel += "��תw����";
            *pnNum = 4;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXQUATERNION* pvecValue = (D3DXQUATERNION*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->w);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETROTATIONCENTER:
        {
            strLabel += "��ת����x���� ";
            strLabel += "��ת����y���� ";
            strLabel += "��ת����z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecValue = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETSCALING:
        {
            strLabel += "����x���� ";
            strLabel += "����y���� ";
            strLabel += "����z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecValue = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETSCALINGCENTER:
        {
            strLabel += "��������x���� ";
            strLabel += "��������y���� ";
            strLabel += "��������z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecValue = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETSCALINGROTATION:
        {
            strLabel += "������תx���� ";
            strLabel += "������תy���� ";
            strLabel += "������תz���� ";
            strLabel += "��ת��תw����";
            *pnNum = 4; 
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXQUATERNION* pvecValue = (D3DXQUATERNION*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecValue);

            sprintf_s(szTemp, "%f", pvecValue->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->z);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecValue->w);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_RESETTRANSFORM:
        break;
    case FT_KG3DMODEL_BINDTOSOCKET:
        {
            strLabel += "�󶨵�����ID ";
            strLabel += "�󶨵��ؽ�����";
            *pnNum = 2;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwModelID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwModelID);
            char* cszScoketName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(cszScoketName);
            
            sprintf_s(szTemp, "%lu", *pdwModelID);
            strTextBox += szTemp;
            strTextBox += " ";
            strTextBox += cszScoketName;
        }
        break;
    case FT_KG3DMODEL_BINDTOBONE:
        {
            strLabel += "�󶨵�����ID ";
            strLabel += "�󶨵���������";
            *pnNum = 2;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwModelID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwModelID);
            char* cszBoneName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(cszBoneName);

            sprintf_s(szTemp, "%lu", *pdwModelID);
            strTextBox += szTemp;
            strTextBox += " ";
            strTextBox += cszBoneName;
        }
        break;
    case FT_KG3DMODEL_ATTACHMODEL:
        {
            strLabel += "�󶨵�����ID";
            *pnNum = 1;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwModelID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwModelID);

            sprintf_s(szTemp, "%lu", *pdwModelID);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_DETCHMODEL:
        {
            strLabel += "�������ID";
            *pnNum = 1;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwModelID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwModelID);
            
            sprintf_s(szTemp, "%lu", *pdwModelID);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_PLAYANIMATION:
        {
            strLabel += "�������� ";
            strLabel += "����·�� ";
            strLabel += "�����ٶ� ";
            strLabel += "��ʼʱ��";
            *pnNum = 4;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwPlayType = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwPlayType);
            char* cszFileName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            // KG_PROCESS_ERROR(cszFileName);
            FLOAT* pfSpeed = (FLOAT*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]])); 
            KG_PROCESS_ERROR(pfSpeed);
            int* pnOffsetTime = (int*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[3]]));
            KG_PROCESS_ERROR(pnOffsetTime);

            sprintf_s(szTemp, "%lu", *pdwPlayType);
            strTextBox += szTemp;
            strTextBox += " ";
            strTextBox += cszFileName;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", *pfSpeed);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%d", *pnOffsetTime);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_PALYSPLITANIMATION:
        {
            strLabel += "����·�� ";
            strLabel += "SPLIT���� ";
            strLabel += "������Ϣ ";
            strLabel += "�������� ";
            strLabel += "�����ٶ� ";
            strLabel += "��ʼʱ��";
            *pnNum = 6;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            char* strAnimationName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(strAnimationName);
            DWORD* pdwSplitType = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(pdwSplitType);
            DWORD* pdwExtraInfo = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]]));
            KG_PROCESS_ERROR(pdwExtraInfo);
            DWORD* pdwPlayType = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[3]]));
            KG_PROCESS_ERROR(pdwPlayType);
            FLOAT* pfSpeed = (FLOAT*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[4]]));
            KG_PROCESS_ERROR(pfSpeed);
            int* pnOffsetTime = (int*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[5]]));
            KG_PROCESS_ERROR(pnOffsetTime);

            strTextBox += strAnimationName;
            strTextBox += " ";
            sprintf_s(szTemp, "%lu", *pdwSplitType);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%lu", *pdwExtraInfo);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%lu", *pdwPlayType);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", *pfSpeed);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%d", *pnOffsetTime);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_LOADMATERIALFROMFILE:
        {
            strLabel += "����·�� ";
            strLabel += "����ѡ��";
            *pnNum = 2;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            char* cszFileName = (char*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(cszFileName);
            DWORD* pOption = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]])); 
            KG_PROCESS_ERROR(pOption);

            strTextBox += cszFileName;
            strTextBox += " ";
            sprintf_s(szTemp, "%lu", *pOption);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETALPHA:
        {
            strLabel += "����͸���� ";
            strLabel += "�Ƿ�ʹ������ģʽ";
            *pnNum = 2;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            float* pfAlpha = (float*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pfAlpha);
            BOOL* pbUseSpecail = (BOOL*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]])); 
            KG_PROCESS_ERROR(pbUseSpecail);

            sprintf_s(szTemp, "%f", *pfAlpha);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%d", *pbUseSpecail);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DMODEL_SETCALLBACKRENDER:
        break;
    case FT_KG3DMODEL_DESTRUCT:
        break;
    case FT_KG3DSCENEEDITOR_DESTRUCT:
        break;
    case FT_KG3DSCENEEDITOR_ADDRENDERENTITY:
        {
            strLabel += "����ID ";
            strLabel += "��� ";
            strLabel += "�Ƿ��ѡ��";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwEntityID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwEntityID);
            unsigned* puClass = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(puClass);
            BOOL* pbSelectable = (BOOL*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]]));
            KG_PROCESS_ERROR(pbSelectable);

            sprintf_s(szTemp, "%lu", *pdwEntityID);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puClass);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%d", *pbSelectable);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DSCENEEDITOR_REMOVERENDERENTITY:
        {
            strLabel += "����ID ";
            strLabel += "��� ";
            strLabel += "�Ƿ��ѡ��";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            DWORD* pdwEntityID = (DWORD*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pdwEntityID);
            unsigned* puClass = (unsigned*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[1]]));
            KG_PROCESS_ERROR(puClass);
            BOOL* pbSelectable = (BOOL*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[2]]));
            KG_PROCESS_ERROR(pbSelectable);
            
            sprintf_s(szTemp, "%lu", *pdwEntityID);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%u", *puClass);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%d", *pbSelectable);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW:
        break;
    case FT_KG3DCAMERA_UPDATECAMERA:
        break;
    case FT_KG3DCAMERA_SETPOSITION:
        {
            strLabel += "��ͷλ��x���� ";
            strLabel += "��ͷλ��y���� ";
            strLabel += "��ͷλ��z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecPos = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecPos);

            sprintf_s(szTemp, "%f", pvecPos->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecPos->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecPos->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DCAMERA_SETLOOKATPOSITION:
        {
            strLabel += "��ͷĿ��λ��x���� ";
            strLabel += "��ͷĿ��λ��y���� ";
            strLabel += "��ͷĿ��λ��z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecPos = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecPos);

            sprintf_s(szTemp, "%f", pvecPos->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecPos->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecPos->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DCAMERA_SETUPDIRECTION:
        {
            strLabel += "��ͷ�Ϸ���x���� ";
            strLabel += "��ͷ�Ϸ���y���� ";
            strLabel += "��ͷ�Ϸ���z����";
            *pnNum = 3;
            KGLOG_PROCESS_ERROR(pEvent->AParameters.pData);
            D3DXVECTOR3* pvecUp = (D3DXVECTOR3*)(&(pEvent->AParameters.pData[pEvent->AParameters.nFrom[0]]));
            KG_PROCESS_ERROR(pvecUp);

            sprintf_s(szTemp, "%f", pvecUp->x);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecUp->y);
            strTextBox += szTemp;
            strTextBox += " ";
            sprintf_s(szTemp, "%f", pvecUp->z);
            strTextBox += szTemp;
        }
        break;
    case FT_KG3DCAMERA_SETGLOBALPERSPECTIVE:
        break;
    case FT_KG3DCAMERA_SETGLOBALORTHOGONAL:
        break;
    case FT_KG3DCAMERA_SETTRACKINFO:
        break;
    default:
        break;
    }

    *pnNum += 2;
    *pstrLabel = _com_util::ConvertStringToBSTR(strLabel.c_str());
    *pstrTextBox = _com_util::ConvertStringToBSTR(strTextBox.c_str());

    hResult = S_OK;
Exit0:
    return S_OK;
}

HRESULT CRecordPlayer::InitMap()
{
    HRESULT hResult = E_FAIL;

    m_mapStringType["�����������"] = FT_KG3DMODEL_LOADFROMFILE;
    m_mapStringType["������������"] = FT_KG3DSCENEEDITOR_LOADFROMFILE;
    m_mapStringType["����λ������"] = FT_KG3DMODEL_SETTRANSLATION;
    m_mapStringType["������ת����"] = FT_KG3DMODEL_SETROTATION;
    m_mapStringType["������ת��������"] = FT_KG3DMODEL_SETROTATIONCENTER;
    m_mapStringType["������������"] = FT_KG3DMODEL_SETSCALING;
    m_mapStringType["����������������"] = FT_KG3DMODEL_SETSCALINGCENTER;
    m_mapStringType["����������ת����"] = FT_KG3DMODEL_SETSCALINGROTATION;
    m_mapStringType["����λ��������ת"] = FT_KG3DMODEL_RESETTRANSFORM;
    m_mapStringType["�󶨹ؽ�����"] = FT_KG3DMODEL_BINDTOSOCKET;
    m_mapStringType["�󶨹�������"] = FT_KG3DMODEL_BINDTOBONE;
    m_mapStringType["�󶨶�������"] = FT_KG3DMODEL_ATTACHMODEL;
    m_mapStringType["�����������"] = FT_KG3DMODEL_DETCHMODEL;
    m_mapStringType["���Ŷ�������"] = FT_KG3DMODEL_PLAYANIMATION;
    m_mapStringType["����SPLIT��������"] = FT_KG3DMODEL_PALYSPLITANIMATION;
    m_mapStringType["���ز���ѡ��"] = FT_KG3DMODEL_LOADMATERIALFROMFILE;
    m_mapStringType["���ö���͸������"] = FT_KG3DMODEL_SETALPHA;
    m_mapStringType["���ö���ص�����"] = FT_KG3DMODEL_SETCALLBACKRENDER;
    m_mapStringType["��������"] = FT_KG3DMODEL_DESTRUCT;
    m_mapStringType["��������"] = FT_KG3DSCENEEDITOR_DESTRUCT;
    m_mapStringType["������Ⱦ����"] = FT_KG3DSCENEEDITOR_ADDRENDERENTITY;
    m_mapStringType["ɾ����Ⱦ����"] = FT_KG3DSCENEEDITOR_REMOVERENDERENTITY;
    m_mapStringType["���þ�ͷλ��"] = FT_KG3DCAMERA_SETPOSITION;
    m_mapStringType["���þ�ͷĿ��λ��"] = FT_KG3DCAMERA_SETLOOKATPOSITION;
    m_mapStringType["���þ�ͷ�Ϸ���"] = FT_KG3DCAMERA_SETUPDIRECTION;

    hResult = S_OK;
// Exit0:
    return hResult;
}
STDMETHODIMP CRecordPlayer::AddEvent(int nNum, BSTR strName, BSTR strValue)
{
    // TODO: Add your implementation code here

    HRESULT hResult = E_FAIL;
    _bstr_t bsTemp;
    string strNameChange = "";
    string strValueChange = ""; 
    FunctionType FucType;

    bsTemp = strName;
    strNameChange = bsTemp;
    if (m_mapStringType.find(strNameChange) == m_mapStringType.end())
    {
        KG_PROCESS_ERROR(true);
    }
    FucType = m_mapStringType[strNameChange];
    switch (FucType)
    {
    case FT_NONE:
        break;
    default:
        break;
    }
    
    hResult = S_OK;
Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::RemoveEvent(int nInfo)
{
    // TODO: Add your implementation code here
    EventRecord* pEvent = (EventRecord*)nInfo;
    if (pEvent->EventFunction == FT_KG3DMODEL_LOADFROMFILE
        || pEvent->EventFunction == FT_KG3DSCENEEDITOR_LOADFROMFILE
    )
    {
        if (pEvent->pNextEvent == NULL)
        {
            // m_piRecord->DeleteEvent(pEvent);
        }
    }
    else if (pEvent->EventFunction == FT_KG3DSCENEEDITOR_ADDOUTPUTWINDOW)
    {
        // �о�ͷ��Ϣ������ɾ��
    }
    else
    {
        //  m_piRecord->DeleteEvent(pEvent);
    }
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetNowTime(int* pnNowTime)
{
    // TODO: Add your implementation code here
    DWORD dwTime = m_piRecord->GetNowTime();
    *pnNowTime = (int)dwTime;
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetTotalTime(int* pnTotalTime)
{
    // TODO: Add your implementation code here
    DWORD dwTime = m_piRecord->GetTotalTime();
    *pnTotalTime = (int)dwTime;
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetPlayState(int* pPlayState)
{
    // TODO: Add your implementation code here
    IERecPlayer::PlayState playState = m_piRecord->GetPlayarState();
    *pPlayState = (int)playState;
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetObjInfo(int nID, int* pnSuccess)
{
    // TODO: Add your implementation code here
    DWORD dwID = (DWORD)nID;
    HRESULT hResult = E_FAIL;
    RecObj* pRecObj = NULL;
    *pnSuccess = 0;

    hResult = m_piRecord->GetIDObj(dwID, &pRecObj);
    KG_COM_PROCESS_ERROR(hResult);

    m_pRecObj = pRecObj;
    *pnSuccess = 1;
   
Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetFirstEventTime(int* pnTime, int* pnInfo)
{
    // TODO: Add your implementation code here
    HRESULT hResult = E_FAIL;
    EventRecord* pEvent = NULL;
    *pnTime = -2;
    ASSERT(m_pRecObj);

    pEvent = m_pRecObj->pEventHead;
    m_pCurrentEvent = pEvent;
    if (pEvent != NULL)
    {
        int nTime;
        hResult = m_piRecord->GetEventTime(pEvent, nTime);
        KG_COM_PROCESS_ERROR(hResult);
        if (nTime < 0)
            *pnTime = -1;
        else
            *pnTime = nTime;
        *pnInfo = (int)pEvent;
    }
    
Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetNextEventTime(int* pnTime, int* pnInfo)
{
    // TODO: Add your implementation code here
    HRESULT hResult = E_FAIL;
    EventRecord* pEvent = NULL;
    *pnTime = -2;
    ASSERT(m_pCurrentEvent);

    pEvent = m_pCurrentEvent->pNextEvent;
    m_pCurrentEvent = pEvent;
    if (pEvent != NULL)
    {
        int nTime;
        hResult = m_piRecord->GetEventTime(pEvent, nTime);
        KG_COM_PROCESS_ERROR(hResult);
        if (nTime < 0)
            *pnTime = -1;
        else
            *pnTime = nTime;
        *pnInfo = (int)pEvent;
    }

Exit0:
    return S_OK;
}

STDMETHODIMP CRecordPlayer::GetEventFunction(int pnInfo, BSTR* pstrFunction)
{
    // TODO: Add your implementation code here
    EventRecord* pEvent = (EventRecord*) pnInfo;
    string strFunction;
    
    KG_PROCESS_ERROR(pEvent);

    FunctionType funtionType = pEvent->EventFunction;
    GetDesByID(strFunction, funtionType);
    *pstrFunction = _com_util::ConvertStringToBSTR(strFunction.c_str());

Exit0:
    return S_OK;
}
