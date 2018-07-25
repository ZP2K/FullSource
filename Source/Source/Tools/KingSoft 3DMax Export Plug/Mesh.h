// Mesh.h: interface for the CMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__80F4C5D0_8093_4479_9AF5_604A49611996__INCLUDED_)
#define AFX_MESH_H__80F4C5D0_8093_4479_9AF5_604A49611996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "windows.h"
#include "stdio.h"
#include "PSD2TGA.h"
#include "Max.h"
#include "d3dx9.h"
#include <vector>
#include <list>
using namespace std;




#define  OPTION_MATERIAL_LIGHTING    100
#define  OPTION_MATERIAL_CULLING     101
#define  OPTION_MATERIAL_FILLMODE    102
#define  OPTION_MATERIAL_MUTIPASS    103
#define  OPTION_MATERIAL_ALPHATEST   104
#define  OPTION_MATERIAL_ALPHABLEND  105
#define  OPTION_MATERIAL_ZBUFFER     106


#define  OPTION_TEXTURE_OPERATION    1000
#define  OPTION_TEXTURE_MAPMETHOD    1001
#define  OPTION_TEXTURE_TRANSFROM    1002
#define  OPTION_TEXTURE_ANIMATION    1003


#define defFILE_TAGLEN			8
#define defFILE_DESCLEN			64
#define defFILE_COMMONTAG		"KSword3D"	//�ļ�ͷ����־
#define defFILE_COPYRIGHTDESC	"Copyright 1988-2004 Kingsoft Corporation. All rights Reserved"		//��Ȩ��Ϣ��Ĭ�������ļ��У�

#define MESH_POSITION        1
#define MESH_NORMAL          1<<1
#define MESH_DIFFUSE         1<<2
#define MESH_TEXTUREUVW1     1<<3
#define MESH_TEXTUREUVW2     1<<4
#define MESH_TEXTUREUVW3     1<<5
#define MESH_FACESINDEX      1<<6
#define MESH_SUBSETINDEX     1<<7
#define MESH_SKININFO        1<<8
#define MESH_LODINFO         1<<9

#define  MATERIAL_OPTION_HIDE              1

#define  MATERIAL_OPTION_ZBUFFER_FALSE     1<<1
#define  MATERIAL_OPTION_ZBUFFER_TRUE      1<<2
#define  MATERIAL_OPTION_ZBUFFER_USEW      1<<3

#define  MATERIAL_OPTION_FILL_POINT        1<<4
#define  MATERIAL_OPTION_FILL_WIREFRAME    1<<5
#define  MATERIAL_OPTION_FILL_SOLID        1<<6

#define  MATERIAL_OPTION_SHADE_FLAT        1<<7
#define  MATERIAL_OPTION_SHADE_GOURAUD     1<<8
#define  MATERIAL_OPTION_SHADE_PHONG       1<<9

#define  MATERIAL_OPTION_CULL_NONE         1<<10
#define  MATERIAL_OPTION_CULL_CW           1<<11
#define  MATERIAL_OPTION_CULL_CCW          1<<12

#define  MATERIAL_OPTION_SPECULARENABLE    1<<13
#define  MATERIAL_OPTION_LIGHTING          1<<14


typedef class CMesh  
{
public:
	struct TFileHeader
	{
		char szCommonTag[defFILE_TAGLEN];
		char szModuleTag[defFILE_TAGLEN];
		__int32 byVersion;
		char szDescription[defFILE_DESCLEN];
		
		TFileHeader(char* pModuleTag = NULL, __int32 byVer = 0, char* pDescription = NULL)
		{
			Set(pModuleTag, byVer, pDescription);
		}
		void Set(char* pModuleTag = NULL, __int32 byVer = 0, char* pDescription = NULL)
		{
			memset(this, 0, sizeof(TFileHeader));
			strncpy(szCommonTag, defFILE_COMMONTAG, defFILE_TAGLEN);
			
			if(pModuleTag)
				strncpy(szModuleTag, pModuleTag, defFILE_TAGLEN);
			
			byVersion = byVer;
			
			if(pDescription)
				strncpy(szDescription, pDescription, defFILE_DESCLEN);
			else
				strncpy(szDescription, defFILE_COPYRIGHTDESC, defFILE_DESCLEN);
		}

	};

	struct _MeshHead
	{
		DWORD  NumVertices;
		DWORD  NumFaces;
		DWORD  NumSubset;
		DWORD  PositionBlock;
		DWORD  NormalBlock;
		DWORD  DiffuseBlock;
		DWORD  TextureUVW1Block;
		DWORD  TextureUVW2Block;
		DWORD  TextureUVW3Block;
		DWORD  FacesIndexBlock;
		DWORD  SubsetIndexBlock;
		DWORD  SkinInfoBlock;
		DWORD  LODInfoBlock;
        DWORD  FlexibleBodyBlock;
		DWORD  ExtendBlock[19];

		_MeshHead()
		{
			NumVertices      = 0;
			NumFaces         = 0;
			NumSubset        = 0;
			PositionBlock    = 0;
			NormalBlock      = 0;
			DiffuseBlock     = 0;
			TextureUVW1Block = 0;
			TextureUVW2Block = 0;
			TextureUVW3Block = 0;
			FacesIndexBlock  = 0;
			SubsetIndexBlock = 0;
			SkinInfoBlock    = 0;
			LODInfoBlock     = 0;
            FlexibleBodyBlock = 0;
			ZeroMemory(ExtendBlock,sizeof(DWORD)*19);
		}
	};

	struct _MeshFileHead
	{
		TFileHeader KSFileHeader;
		DWORD       FileMask;
		DWORD       BlockLength;
		DWORD       MaterialBlock;
		DWORD       AnimationBlock;
		DWORD       ExtendBlock[10];
		DWORD       MeshCount;
		_MeshHead   MeshHead;

		_MeshFileHead()
		{
			FileMask       = 0x4D455348;
			BlockLength    = 0;
			MaterialBlock  = 0;
			AnimationBlock = 0;
			ZeroMemory(ExtendBlock,sizeof(DWORD)*10);
			MeshCount      = 1;
		};
	};
	
	typedef struct _Vec3
	{
		float x;
		float y;
		float z;
		void Create(float X,float Y,float Z)
		{
			x = X;
			y = Y;
			z = Z;
		}
	}* LPVEC3,VEC3;

	typedef struct _Vec4
	{
		float x;
		float y;
		float z;
		float w;
		void Create(float X,float Y,float Z,float W)
		{
			x = X;
			y = Y;
			z = Z;
			w = w;
		}
	}* LPVEC4,VEC4;
		
	typedef struct _Option
	{
		DWORD Type;
		PVOID pData;
	}* LPOPTION,OPTION;
		
	typedef struct _TextureTf
	{
		VEC3 Scaling;
		VEC3 Rotation;
		VEC3 Position;
		VEC3 Speed_Scaling;
		VEC3 Speed_Rotation;
		VEC3 Speed_Position;
	}*LPTEXTURETF;

	typedef struct _Color4
	{
		float r;
		float g;
		float b;
		float a;
	}* LPCOLOR4,COLOR4;

	typedef struct _Texture
	{
		float Amount;
		LPSTR TextureType;
		LPSTR FileName;
	}*LPTEXTURE,TEXTURE;
	typedef struct _Material
	{
		COLOR4     Ambient;
		COLOR4     Diffuse;
		COLOR4     Specular;
		COLOR4     Emissive;
		float      Power;
		DWORD      NumTexture;
		LPTEXTURE  Textures;
		DWORD      Option;
		DWORD      NumTextureOption[8];
		LPOPTION   TexturesOption[8];
	}* LPMATERIAL,MATERIAL;

    typedef struct _VerticesSkinInfoItem
    {
        char  szBoneName[MAX_BONE_NAME_LENGTH];
        INode *pBoneNode;
        float fWeight;
    } VerticesSkinInfoItem;

    #define VERTICESSKININFOITEM_MAX    2

	typedef struct _VerticesSkinInfo
	{
		DWORD                   NumBoneRef;
        VerticesSkinInfoItem    Bone[VERTICESSKININFOITEM_MAX];
		LPSTR*   BoneName;
		float*   Weight;
	}*LPVERTICESSKININFO,VERTICESSKININFO;

	typedef struct _Matrix { 
		float _11; float _12; float _13; float _14;
		float _21; float _22; float _23; float _24;
		float _31; float _32; float _33; float _34;
		float _41; float _42; float _43; float _44;
	} *LPMATRIX,MATRIX;

	typedef struct _BoneInfo
	{
        INode   *pThis;
		TCHAR   szName[MAX_BONE_NAME_LENGTH];
        INode   *pParent;
		TCHAR   szParentName[MAX_BONE_NAME_LENGTH];
		DWORD   NumChild;
		LPSTR*  lpChildNames;
		DWORD   NumVerticesRef;
		DWORD*  VerticesIndexRef;
		float*  VerticesWeightRef;
		
		//offset from org in obj space
		MATRIX  MatrixOffset;
		//offset from parent bone
		MATRIX  BaseBoneMatrix;
		
		BOOL    Static;
		DWORD   Offset;//Offset in the m_lpAnimationBonesTrans.
	}* LPBONEINFO,BONEINFO;

	typedef VEC4 Quaternion;
	typedef Quaternion* LPQUATERNION;

	struct RTSBoneAnimation
	{
		D3DXVECTOR3 Translation;
		D3DXVECTOR3 Scale;
		D3DXQUATERNION Rotation;
		float Sign;
		D3DXQUATERNION SRotation;
	};

	//struct BoneAnimKeyFrame
	//{
	//	float fTime;
	//	MATRIX mat;
	//};

	//struct BoneAnimInfo
	//{
	//	int nNumKeyFrames;
	//	float TimeRange;
	//	BoneAnimKeyFrame* pKeyFrames;
	//};

	typedef struct _Socket
	{
		string  Name;
		string  ParentName;
		PVOID   Node;
		PVOID   ParentNode;
		MATRIX  MatrixOffset;

		_Socket()
		{
			Node = NULL;
			ParentNode = NULL;
		}

		bool operator == (_Socket InSocket) const
		{
			if((Node==InSocket.Node)&&(ParentNode==InSocket.ParentNode))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}*LPSOCKET,SOCKET;

	list<SOCKET>m_listSocket;

    //------ Added By Zeb 2005-11-16 -------
    list<SOCKET>   m_listCollisionSurface;
    struct FLEXIBLE_BODY_SOCKET
    {
        TCHAR ParentBoneName[30];
        TCHAR SocketName[30];
        MATRIX  Matrix;
        TCHAR BindBoneName[30];
        DWORD dwReserved[5];
    };

    struct FLEXIBLE_BODY_PARAM
    {
        BOOL            bEnable;
        D3DXVECTOR3     vGravity;
        float           fHooke;
        float           fResistance;
        BOOL            bEnableCollisionDetect;
        float            fGravityInherit;
        DWORD           dwReserved[4];
    };
    //------ Added By Zeb End --------------

	DWORD   ID;
	PVOID   m_pNode;//void pointer for inode in 3dsmax
	DWORD   m_dNumVertices;
	DWORD   m_dNumFaces;
	DWORD   m_dNumSubsets;
	BOOL    m_bSkined;            //�Ƿ��������Ȩ�ص���Ϣ
	
	LPVEC3  m_lpVerticePosition;  //λ��
	LPVEC3  m_lpVerticeNormal;    //���㷢������
	LPVEC3  m_lpVerticeTextCoord1; //��ͼ���꣨UVW��			
	LPVEC3   m_lpVerticeTextCoord2; //��ͼ���꣨UVW��			
	LPVEC3   m_lpVerticeTextCoord3; //��ͼ���꣨UVW��	
	LPCOLOR4 m_lpVerticeDiffuse;
	DWORD*  m_lpFacesIndex;       //��������
	DWORD*  m_lpSubsetID;         //��������
	DWORD*  m_lpVerticeRemap;     //����Ķ�ӳ�䣬��¼��ǰÿ�������Ӧ��ԭ�����λ��
	DWORD*  m_lpSubsetRemap;      //���ʸĶ�ӳ�䣬��¼��ǰÿ�����Ӧ��ԭ��Ĳ���

	DWORD   m_dwAnimationType;    //��������
	LPSTR   m_lpAnimationName;    //��������
	///////////////////////////////////////////////////////////////////////////////////
	//���㶯�����ؼ��壩
	DWORD   m_dNumAnimationVertices;       //�������Ķ�����Ŀ
	DWORD*  m_lpAnimationVerticesIndex;    //���������������
	LPVEC3  m_lpAnimationVerticesPosition; //ÿ��Ķ������꣬��������뾲̬�壨t=0)���������
	_Vec3*  m_lpAnimationUV;               //ÿ֡��UV����
	float   m_fFrameLength;                //����ÿ��֮���ʱ������������һ��ֻ����һ������
	DWORD   m_dNumFrames;                  //��������
	///////////////////////////////////////////////////////////////////////////////////
	//�����������ؼ��壩
	DWORD      m_dNumBone;                     //��������
	LPBONEINFO m_lpBoneInfo;                   //������Ϣ�������������ͽṹ���漰�Ķ����Ȩ��
	LPVERTICESSKININFO  m_lpVerticesSkinInfo;  //ÿ�������漰�Ĺ��������ƺͶ�Ӧ��Ȩ��
	DWORD      m_dNumAnimationBones;           //�������Ĺ�����Ŀ
	LPSTR*     m_lpAnimationBonesName;         //����������������
	LPMATRIX   m_lpAnimationBonesTrans;        //���������˶��任

	

	//////////////////////////////////////////////////////////////////////////
	//didn't use this
	LPVEC3     m_lpAniBScaling;
	LPVEC4     m_lpAniBRotation;
	LPVEC3     m_lpAniBTranslation;
	//uses the one below
	RTSBoneAnimation* m_lpRTSAniBones;
	//////////////////////////////////////////////////////////////////////////
	//vector<BoneAnimInfo> m_BoneAnimInfo;

	LPMATERIAL m_lpMaterial;         //����,����������ʱ�ٶ�����Ϊ��̬��
public:
	HRESULT MoveTextureFile(Interface* ip, LPCSTR strDirName, LPCSTR strAbsMeshPath);
	HRESULT MoveTextureFile1(Interface* ip, LPCSTR strDirName);
	HRESULT FindTexture(Interface* ip, const TCHAR* strOrg, TCHAR* strFixed);
	HRESULT CheckUpMaterial();
	HRESULT CleanUp();
	HRESULT SaveAnimation(LPSTR pFileName,LPSTR pAnimationName);
	HRESULT SaveMaterial(LPSTR pFileName);
	HRESULT SaveMesh(LPSTR pFileName,DWORD Option);
	void FixSubsetIndex(DWORD* pSubsetID, DWORD dwNumFaces, DWORD dwNumSubsets);
	CMesh();
	virtual ~CMesh();
	//void GetRelativePath(const TCHAR* strPathSrc, const TCHAR* strPathDest, TCHAR* strPathRelative);
	HRESULT GetAbsPath(const TCHAR* strFileName, TCHAR* strOutPath);
	HRESULT GetRelativePath(const TCHAR* strAbsPath1, const TCHAR* strAbsPath2, TCHAR* strRelativePath);
	HRESULT GetAbsPathFromRelativePath(const TCHAR* strAbsPath, const TCHAR* strRelativePath, TCHAR* strOutPath);
	void CompressSubset();
	
}* LPMESH;

#endif // !defined(AFX_MESH_H__80F4C5D0_8093_4479_9AF5_604A49611996__INCLUDED_)
