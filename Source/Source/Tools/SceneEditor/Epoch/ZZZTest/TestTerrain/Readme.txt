�ļ���չ��˵����
===========================================================================

.inf�ļ�
	�ı��ļ�
	һ���ؿ�ͨ�������ֻ��һ��������ɣ���ĳЩ�������(���ɽ֮��)������
	�����ɸ��˴˷���Ľ�С������ɣ�inf�ļ��г���������Щ���ε��ļ�����
	��λ�á��������Ϣ

.lev�ļ�
	�ı��ļ�
	����ؿ��������ļ��������˾��顢��Ʒ���ϰ����ľ�����ݡ��¼�����
	���򡢵�����������ȶ������Ϣ(����ģ���ļ�����λ�á������)��������
	StartPoint����ʽδ��

.ttd�ļ�
	�ı��ļ�
	�ǵ������������ļ���������������Ϣ���ڴ�������������ʽ�μ�Demo.ttd

.hmv�ļ�
	�������ļ�
	�Ǹ߶�ͼ�ļ������εĸ߶����ݼ����ڴ��ļ��У���ʽ���£�

	Type		Size (bytes)			Identifier		Desc
	unsigned int	4				TypeOfHP		Specify the type of height-point, 0 means char, 1 short, 2 int, 3 float.
	unsigned int	4				NumOfHPs		The number of height-points
	unsigned int	4				NumOfHPsPerRow		The number of height-points per row (ie., width of the heightmap)
	unsigned int	4				NumOfHPsPerColumn	The number of height-points per column (ie., height of the heightmap)
	TypeOfHP	NumOfHPs * sizeof(TypeOfHP)	HPBuffer		The buffer of height-points