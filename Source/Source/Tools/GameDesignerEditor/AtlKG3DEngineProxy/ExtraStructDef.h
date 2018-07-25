
#ifndef DESIGN_EXTRA_STRUCT_DEF
#define DESIGN_EXTRA_STRUCT_DEF

typedef struct _AtlVector3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} AtlVector3;


#define DE_MAX_FIELDS 32
typedef struct _AtlObjInfo
{
	BSTR strType;					// "NPC", "Doodad" ��
	LONG iIndex;					// ���
	LONG iLogicObjPtr;				// �߼�����ṹ��ָ�룬����NpcInfo*
	LONG iRepresentObjPtr;			// 3D����obj��ָ�룬��LogicObj����ΪpPointer
	LONG bChanged;					// editor�ã���ʾ�Ƿ���Ҫ���µ��߼�����
	LONG iFieldsCount;				// ֵ����
	BSTR strFields[DE_MAX_FIELDS];	// ��
	BSTR strValues[DE_MAX_FIELDS];	// ֵ
} AtlObjInfo;

#endif
