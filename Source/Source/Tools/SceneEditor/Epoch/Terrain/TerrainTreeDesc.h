// File: TerrainTreeDesc.h
// Desc: Corresponding to .ttd file

#ifndef TERRAINTREEDESC_H
#define TERRAINTREEDESC_H

#include"..\Common\Common.h"

#include<vector>
using std::vector;

namespace Terrain
{

const float SPAN_DEF = 256.f;  // This value is corresponding to the original size of terrain-textures
const float VARIANT_DEF = 10.f;
const float FACTOR_DEF = 1.f;
const unsigned int DEF_TERRAIN_COLOR = 0xFF0000FF;

class DLLAPI TerrainTreeDesc
{
public:
	Common::String m_TTDFilename;  // TTD filename for opening the file which stores infos we need; ��չ���ݶ�Ϊ.ttd����ΪTerrain Tree Description

	unsigned int m_FNs;  // ÿ����������FieldNode����Ŀ��������2��n�η���n >= 0��m_FNs��ֵ���Ϊ256
	unsigned int m_QLs;  // ÿ��FieldNode�ں���(������)����������QuadLeaf����Ŀ��������2��n�η���n >= 0��m_QLs��ֵ���Ϊ64(���ǿ����ж��㷨����ѱ���)
	float m_XSpan;  // ÿ��BlockNodeҶ�����X���ϵĿ��(in World Space)
	float m_ZSpan;  // ÿ��BlockNodeҶ�����Z���ϵĿ��(in World Space)
	float m_Variant;  // �߶��ݲ�
	float m_Factor;  // �߶�����, can NOT be 0!!
	unsigned int m_MaxLOD;  // ���LOD�ȼ�������ֵ��ʾ���뾵ͷ��������εĶ�Ԫ�����������ֵԽ�󣬵��ξ�ϸ��Խ�ߣ�����Ϊ0������ʵ���ֵ���ԭò(������Debug)
	unsigned int m_TerrainBaseColor;  // ����ͼʱ����ɫ

	typedef vector<Common::String> TextureFilenameVector;
	typedef vector<Common::String> AlphaMapFilenameVector;

	TextureFilenameVector m_Textures;
	AlphaMapFilenameVector m_AlphaMaps;
	Common::String m_LightMapFilename;

public:
	TerrainTreeDesc(): m_TTDFilename(), m_FNs(0), m_QLs(0), m_XSpan(SPAN_DEF), m_ZSpan(SPAN_DEF), m_Variant(0.f), m_Factor(FACTOR_DEF), m_MaxLOD(0), m_Textures(), m_AlphaMaps(), m_LightMapFilename() {}
	TerrainTreeDesc(const Common::String& sTTDFilename): m_TTDFilename(sTTDFilename), m_FNs(0), m_QLs(0), m_XSpan(SPAN_DEF), m_ZSpan(SPAN_DEF), m_Variant(0.f), m_Factor(FACTOR_DEF), m_MaxLOD(0), m_TerrainBaseColor(DEF_TERRAIN_COLOR), m_Textures(), m_AlphaMaps(), m_LightMapFilename() {}
	TerrainTreeDesc(const TerrainTreeDesc& ttd): m_TTDFilename(ttd.m_TTDFilename), m_FNs(ttd.m_FNs), m_QLs(ttd.m_QLs), m_XSpan(ttd.m_XSpan), m_ZSpan(ttd.m_ZSpan), m_Variant(ttd.m_Variant), m_Factor(ttd.m_Factor), m_MaxLOD(ttd.m_MaxLOD), m_TerrainBaseColor(ttd.m_TerrainBaseColor), m_Textures(ttd.m_Textures), m_AlphaMaps(ttd.m_AlphaMaps), m_LightMapFilename(ttd.m_LightMapFilename) {}
	TerrainTreeDesc& operator = (const TerrainTreeDesc& ttd);

	void GetName(Common::String& sName) const { sName = m_TTDFilename; }
	void SetName(const Common::String& sName);

	void ReadFile();
};

}  // namespace

#endif