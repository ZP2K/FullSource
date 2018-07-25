
#include <vector>
#include <map>

struct KTRACK_POINT
{
    int nDirection;
    int nSpeed;
    int nX;
    int nY;
    int nZ;
};

struct KROAD_NODE 
{
    DWORD dwMapID;      // ��������
    DWORD dwForce;      // ��������
    int   nCity;
    int   nLink[4];     // �ڽӱ�
};

class KTrackList
{
public:
    KTrackList();

    BOOL Load();

    BOOL Save();

    // ���չ켣�ƶ�ʱ,������һ����,��������յ�,���߳���,����false
    BOOL GetTrackPoint(KTRACK_POINT* pPoint, DWORD dwTrackID, int nFrame);

private:

    struct KCRITICAL_POINT
    {
        int nFrame;
        int nX;
        int nY;
        int nZ;
    };

    typedef std::vector<KCRITICAL_POINT> KTRACK;

    struct  KTRACK_INFO
    {
        DWORD   dwSrcNode;
        DWORD   dwDstNode;
        KTRACK  Track;
    };

    typedef std::map<DWORD, KTRACK_INFO> KTRACK_TABLE;

    KTRACK_TABLE m_TrackTable;

private:
    BOOL SaveTrack(DWORD dwID, const KTRACK& crTrack);
    BOOL LoadTrack(DWORD dwID, KTRACK* pTrack);

    BOOL CalculateCriticalFrame(KTRACK* pTrack);
};

