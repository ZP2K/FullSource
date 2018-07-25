using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartEditor
{
    /// <summary>
    /// ����ö��
    /// </summary>
    public enum Direction
    {
        Up, // ��
        Down, // ��
        Left, // ��
        Right, // ��
        Up_Left, // ����
        Up_Right, // ����
        Down_Left, // ����
        Down_Right, // ����
        None // ��
    }

    /// <summary>
    /// �༭����ö��
    /// </summary>
    public enum EditMode
    {
        Cut,
        Copy,
        Normal
    }

    /// <summary>
    /// ͼԪ����ö��
    /// </summary>
    public enum GraphType
    {
        ConditionNode, // ������� 
        ActionNode, // �������
        EventNode, // �¼����        
        Line, // ������
        AIActionNode, // AI�������
        AIActionsNode, // AI��������
        AIStateNode, // AI״̬���
        AILine, // AI������
        InnerChart, // �ӻ�ͼ
        InterfaceNode // �ӿڽ��
    }

    /// <summary>
    /// �߼���������ö��
    /// </summary>
    public enum LogicType
    {
        CreateAtPoint, // ��ָ��λ�ô���ͼԪ(�Ҽ��˵���)
        InitializeDataElement, // ��ʼ������Ԫ
        Connect, // ����ͼԪ
        Disconnect, // �������ͼԪ
        BeforeDelete, // ɾ��ͼԪǰ�Ĵ���
        AfterDelete, // ɾ��ͼԪ��Ĵ���
        Edit, // �༭ͼԪ
        CreateLine, // ����������
        CreateAndConnectLine, // ���������߲��Զ�����ͼԪ
        CreateChart, // ������ͼ
        AfterLoad, // ��ͼ���غ�
        PopUpContextMenu, // �����Ҽ��˵�
        ViewCode, // �鿴����
        ViewFolder, // �鿴�ļ���
        CompileCode, // ��������ͼ�Ĵ���
        CheckLogic, // �������ͼ���߼�
        ManageArguments, // ��������ͼ�Ĳ���
        Save, // ��������ͼ
        Paste, // ճ��ͼԪ
    }

    /// <summary>
    /// �α�����ö��
    /// </summary>
    public enum RodmanType 
    { 
        None, 
        Horizonal, 
        Vertical 
    }
}
