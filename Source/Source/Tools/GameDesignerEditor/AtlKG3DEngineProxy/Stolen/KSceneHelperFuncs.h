////////////////////////////////////////////////////////////////////////////////
//
//  FileName    : KSceneHelperFuncs.h
//  Version     : 1.0
//  Creator     : Chen Tianhong
//  Create Date : 2008-4-21 9:41:19
/*  Comment     : ��SSE��¶�Ľӿ��У�����֧��ֱ���ò�ͬ�ĺ�������Obj��
���۴���ʲô���͵�RepresentObject������һ��BuildKG3DRepresentObject��������
�����ڽṹ�����棬���������������ܷ��㣬�ܹ�ʵ�������Ĳ�ͬ�����RepresentObject
�Ķ�ȡ���ӿ�ά����������ף���Ϊ�������Ϊ���������Լ�����ģ���
Ϊ�˷���ʹ�ã����ⲿ�����°�װ�ɶ�����ĺ����������Ӻ�����һ�٣�������ʵ����
����������ͱ༭��֮�������Եģ�KG3DSceneSceneEditor�п��Բ�ά����Щ����OBJ��
��Ϊ�����ⲿʹ�ú�ά����Ȼ�ܷ��㡣
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_KSCENEHELPERFUNCS_H_
#define _INCLUDE_KSCENEHELPERFUNCS_H_

interface IEKG3DEngineManager;
interface IEKG3DRepresentObject;
interface IEKG3DSceneEditorBase;
interface IEKG3DSceneSFXEditor;
interface IEKG3DSceneSelectionToolAccessProxy;

////////////////////////////////////////////////////////////////////////////////
//���ַ������ϵ��ӿ���ֻ���ýӿڸ��Ӹ��ӣ������һ����������+�ڲ�һ��Factory�����ýӿڼ򻯺ܶ�
IEKG3DRepresentObject* BuildTrafficPoint(IEKG3DEngineManager* pManager, DWORD dwSetID, DWORD dwType, LPCTSTR lpName);
IEKG3DRepresentObject* BuildWayPoint(IEKG3DEngineManager* pManager, DWORD dwSetID, LPCTSTR lpName);
IEKG3DRepresentObject* BuildRefreshPoint(IEKG3DEngineManager* pManager, DWORD dwType, DWORD dwSetID, LPCTSTR lpName);
IEKG3DRepresentObject* BuildRepresentDoodad(IEKG3DEngineManager* pManager, DWORD dwDoodadTempleteID,
								   LPCTSTR pDoodadName,
								   LPCTSTR pMeshName,
								   LPCTSTR pMtlName,
								   LPCTSTR pAniName,
								   DWORD   dwReliveID,
								   BOOL    bUseDefaultObstacle,
								   BOOL    bGenerateObstacle
								   );
IEKG3DRepresentObject* BuildRepresentNPC(IEKG3DEngineManager* pManager
								, DWORD dwNPCTempleteID
								, LPCTSTR pNPCName
								, LPCTSTR pMdlName
								, LPCTSTR pAniName
								, LPCTSTR pFaceMesh
								, LPCTSTR pFaceMtl
								, int nLevel
								, int nType
								//, int nAIType
								, float fScale
								, DWORD dwRepresentID
								, float nAlertRange
								, float nAttackRange
								, DWORD dwReliveID
								, DWORD dwRandomID
								, DWORD dwAISet);

namespace KG3D_SCENE_FUNCTIONS
{
	IEKG3DSceneEntity* GetUniqueSelectedEntity(IEKG3DSceneEditorBase* pScene);
	IEKG3DSceneEntity* GetUniqueSelectedEntity(IEKG3DSceneSFXEditor* pScene);
	IEKG3DSceneEntity* GetFirstSelectedEntity(IEKG3DSceneSFXEditor* pScene);
	IEKG3DSceneEntity* GetFirstSelectedEntity(IEKG3DSceneSceneEditor* pEditor);
	IEKG3DSceneEntity* GetFirstSelectedEntity(IEKG3DSceneModelEditor* pScene);
	IEKG3DSceneSelectionToolAccessProxy* GetSelectionProxy(IEKG3DSceneSFXEditor* pScene);
	IEKG3DSceneSelectionToolAccessProxy* GetSelectionProxy(IEKG3DSceneEditorBase* pScene);
	ULONG	GetSelectionCount(IEKG3DSceneSceneEditor* pScene);

	struct SceneFileSaveConfirmer 
	{
		struct	TypeFileWrite
		{
			std::tstring strFullPath;
			FILETIME ftLastWriteTime;
			BOOL	bWriteSucceeded;
		};
		typedef std::vector<TypeFileWrite>	VecFileWriteTimes;
		VecFileWriteTimes m_vecFileWriteTimes;

		VOID RecordSaveFiles(DWORD dwSaveOp, LPCTSTR strFileFullPath);
		VOID ConfirmFileWrite();
	};
};
namespace KSF = KG3D_SCENE_FUNCTIONS;




#endif //_INCLUDE_KSCENEHELPERFUNCS_H_
