////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : IEEditorTypes.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-12-18 16:39:33
//  Comment     : 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_IEEDITORTYPES_H_
#define _INCLUDE_IEEDITORTYPES_H_

////////////////////////////////////////////////////////////////////////////////
enum
{
     SCENESTATE_SELECT	= 0,
     SCENESTATE_MOVE    = 1,
     SCENESTATE_ROTATE  = 2,
     SCENESTATE_SCALE   = 3,
     SCENESTATE_COPY    = 4,
     SCENESTATE_SELECTREGION   = 5,  //ѡ��REGION
     SCENESTATE_SELECTSECTION  = 6,  //ѡ��SECTION
     SCENESTATE_SELECTZONE     = 7,  //ѡ��ZONE
	
     SCENESTATE_ADDPOLYVERTEX  = 9,  //��Ӷ���ζ���

     SCENESTATE_SFX_GEO  = 10,

     SCENESTATE_TERRAIN  = 100,   //�޸ĵ���

     SCENESTATE_PLACEOBJ = 200,  //�������

     SCENESTATE_PLACEUNIT = 500,   //����NPC DOODA
     SCENESTATE_PLACENPCEDITOR = 501,   //���ñ༭���ڽ�NPC

     SCENESTATE_CELLLOGICAL		= 400,   //�������
     SCENESTATE_CAMERAANIMATION  = 700,   //�������
     SCENESTATE_BENCHMARK = 713,

     SCENESTATE_SFX_ADD_LAUNCHER = 300,
     SCENESTATE_SFX_ADD_BILLBOARD = 301,


     SCENESTATE_TERRAINSELECTION   = 401,//����ѡ��
	//     SCENESTATE_TERRAINPASTE  = 402,//��������
     SCENESTATE_TERRAINSELECTIONMODIFY = 403, //����ѡ��Χ�༭״̬
     SCENESTATE_COPY_REPRESENTOBJECT = 404, // �������ƶ��� add by suntao

	 SCENESTATE_NONE = 1000,
};

//ExecuteAction�õ�ID
enum
{
	//�������
	EXEACTION_ROTATE_CAMERA	= 1,	//��ת�����
	EXEACTION_ZOOM_VIEW		= 2,	//������ͼ
	EXEACTION_PAN_VIEW		= 3,	//Pan��ͼ
	EXEACTION_ROTATE_VIEW	= 4,	//��ת��ͼ
	EXEACTION_INVERT_ACTION = 5,	//�������   

	//NPC 
	EXEACTION_GO_FORWARD	= 6,	//��ǰ��
	EXEACTION_GO_BACKWARD	= 7,	//���
	EXEACTION_TURN_LEFT		= 8,	//ת��
	EXEACTION_TURN_RIGHT	= 9,	//��
	EXEACTION_MOV_LEFT		= 10,	//����
	EXEACTION_MOV_RIGHT		= 11,
	EXEACTION_CAM_CIRCLE	= 12,	//�����תȦ
	EXEACTION_JUMP			= 13,	//��
	EXEACTION_TURN_LEFT_VIEW_FOLLOWED = 14,		//��ת�����������
	EXEACTION_TURN_RIGHT_VIEW_FOLLOWED = 15,	//��ת�����������

	//����
	EXEACTION_PRESURE		= 16,	//ѹ��
	EXEACTION_INNER_CIRCLE	= 17,	//��Ȧ��С
	EXEACTION_OUTTER_CIRCLE = 18,	//��Ȧ

	//����
	EXEACTION_LEFE_KEY_DOWN = 19,	//�������
	EXEACTION_SELECTION_ADD = 20,	//��ѡ
	EXEACTION_SELECTION_SUTRACT = 21,//��ѡ

	EXEACTION_SMOOTH_TERRAIN = 23,	//ƽ������
	
	EXEACTION_SPEED_UP		= 24,	//����
	EXEACTION_SPEED_DOWN	= 25,	//����

	EXEACTION_MOV_UP			= 26,	//����
	EXEACTION_MOV_DOWN			= 27,	//�½�

	EXEACTION_MOUSE_MOVE		= 30,	//����ƶ������ƶ���ת������״̬����Ҫ������ꡣ������������ϡ�state������Ƿ��¡�WPARAM��LPARAM����ӦWM_MOUSEMOVE��wParam���Եõ����״̬��lParam��low��x��high��y
	EXEACTION_MOUSE_WHEEL		= 31,	//�м���WPARAM��LPARAM����ӦWindowsMessage

	EXEACTION_ZOOM_TO_OBJECT = 1001,	//���ž�ͷ�������С
	EXEACTION_PLAY_CAM_ANI = 1002,	//���ž�ͷ����

	// �߻������༭��
	//EXEACTION_DE_FORWARD		= 2000,
	//EXEACTION_DE_BACKWARD		= 2001,
	//EXEACTION_DE_LEFT			= 2002,
	//EXEACTION_DE_RIGHT			= 2003,
	EXEACTION_DE_MOVE_ANEAR_GROUND = 2004, // �������

	//1234(QWER)
	EXEACTION_SELECT            = 2100,
	EXEACTION_MOVE              = 2101,
	EXEACTION_ROTATE            = 2102,
	EXEACTION_SCALE             = 2103,
};

enum
{
	EM_EDITOR_TOOL_STATE_NOTATIVATED,
	EM_EDITOR_TOOL_STATE_ATIVATED,
	EM_EDITOR_TOOL_STATE_TRANSFORMING,
};

enum	//С����Щ�����ĸ��ģ���ɴ��̶�ʧ����ʹ���
{
	EM_SAVE_OP_TERRAIN_AND_ENTITY = 1,
	EM_SAVE_OP_SCENE_SETTINGS = 1 << 1,
	EM_SAVE_OP_CAMERA_ANIMATION = 1 << 2,
	EM_SAVE_OP_REGION_INFO = 1 << 3,
	EM_SAVE_OP_ENVIRONMENT = 1 << 4,
    EM_SAVE_OP_EFFECTPARAMS = 1 << 5,
	//EM_SAVE_OP_ROAD = 1 << 6,
	//EM_SAVE_OP_RIVER = 1 << 7,	
	EM_SAVE_OP_COUNT = 6,	//�¼ӵ����ͼ���������棬����Ҫ����EM_SAVE_OP_COUNT��������g_GetSaveOptionDescд���������������ڽ������Զ����ɶ�ѡ
	EM_SAVE_OP_ALL = (1 << EM_SAVE_OP_COUNT) - 1,	//ֻҪEM_SAVE_OP_COUNT�ǶԵģ�����������Ϳ��Ը����λȫ��1��ֵ�����Ҹպõ�������Op�Ļ����㡣����ѡ�����Ҫ�õ���EM_SAVE_OP_ALL��������Op�Ļ����㡱����ٶ������Բ�Ҫ�Ҹ����ֵ�����㷽ʽ
};

struct KEditorOptionDesc 
{
	DWORD dwOp;
	LPCTSTR	strDesc;
};

typedef KEditorOptionDesc (&TypeSaveOptionDescArrayRef)[EM_SAVE_OP_COUNT];	//����һ������������õ������﷨

static TypeSaveOptionDescArrayRef g_GetSaveOptionDesc()	//��Щֵ�������
{
	static KEditorOptionDesc descSaveOption[EM_SAVE_OP_COUNT] = 
	{
		{EM_SAVE_OP_TERRAIN_AND_ENTITY, _T("���κ����")},
		{EM_SAVE_OP_SCENE_SETTINGS, _T("SceneSettings")},
		{EM_SAVE_OP_CAMERA_ANIMATION, _T("���������")},
		{EM_SAVE_OP_REGION_INFO, _T("����ֿ�")},
		{EM_SAVE_OP_ENVIRONMENT, _T("��������,����")},
        {EM_SAVE_OP_EFFECTPARAMS, _T("PostRender����")},
		//{EM_SAVE_OP_ROAD, _T("��·")},
		//{EM_SAVE_OP_RIVER, _T("����")},		
	};
	return descSaveOption;
}

#endif //_INCLUDE_IEEDITORTYPES_H_
