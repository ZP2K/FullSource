using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace DBManager
{
    /// <summary>
    /// ʹ�ñ������������������ַ���
    /// </summary>
    //DFAͼ
    //{{DIAGRAM_START
    //
    //            .----.           .----.           .----.          .----.         .----.         .----.
    //            |һ  |           |һ  |           |[   |          |[   |         |]   |         |]   |
    //            |    |           |    |           |    |          |    |         |    |         |    |
    //    .----.  '----'           '----'           '----'          '----'         '----'         '----'
    //    |    >----------->----.          .----.           .----.          .----.         .----.         .----.
    //    |1   <-----------<2   >---------->3   >----------->4   >---------->5   >--------->6   >--------->1'  |
    //    |    |  .----.   |    |          |    |           |    |          |    <---------<    |         |    |
    //    |    |  |��һ|   '----'          'v--v'           'v---'          'v---'         '----'         ^----'
    //    |    |  |    |                    |  |             |               |      .----.                |
    //    |    |  '----'       .----.       |  |             |               |      |����|                |
    //    '----^               |�س�|       |  |             |               |      |    |                |
    //         |               |    |       |  |             |               |      '----'                |
    //         |               '----'       |  |             |               |                            |
    //         ------------------------------  |             |.----.         |                            |
    //                                   .----.|             ||����|         | .----.                     |
    //                                   |����||             ||    |         | |����|                     |
    //                                   |    ||             |'----'         | |    |                     |
    //                                   '----'|             |               | '----'                     |
    //                                         |             |               |                            |
    //                                         |             |              .v---.                        |
    //              .----.             .----<---             |              |5   |                        |
    //              |7'  <-------------<7   <-----------------              |    |                        |
    //              |    |             |    |                               '----'                        |
    //              '----'   .----.    '---v'                                                             |
    //                       |�ǻ�|        ----------------------------------------------------------------
    //                       |��  |                                     .----.
    //                       '----'                                     |�س�|
    //                                                                  |    |
    //                                                                  '----'
    //
    //            .----.           .----.                    .----.           .----.
    //            |"   |           |\   |                    |'   |           |\   |
    //            |    |           |    |                    |    |           |    |
    //    .----.  '----'   .----.  '----'  .----.     .----. '----' .----.    '----'  .----.
    //    |1   >----------->8   >---------->9   |     |1   >-------->10  >------------>11  |
    //    |    <-----------<    <----------<    |     |    <--------<    <------------<    |
    //    '----'  .----.   '----'  .----.  '----'     '----' .----. '----'    .----.  '----'
    //            |"   |           |����|                    |'   |           |����|
    //            |    |           |    |                    |    |           |    |
    //            '----'           '----'                    '----'           '----'
    //
    //            .----.           .----.              .----.         .----.
    //            |[   |           |[   |              |]   |         |]   |
    //            |    |           |    |              |    |         |    |
    //            '----'           '----'              '----'         '----'
    //    .----.           .----.               .----.        .----.          .----.
    //    |1   >----------->12  >--------------->13  >-------->14  >---------->1'  |
    //    |    <-----------<    |               |    <--------<    |          |    |
    //    '----'           'v--v'               'v---' .----. '----'          '----'
    //            .----.    |  |   .----.        |     |����|
    //            |����|    |  |   |����|        |     |    |
    //            |    |    |  |   |    |        |     '----'
    //            '----'    |  |   '----'        |
    //                      |  |                 |  .----.
    //                      |  |                 |  |����|
    //                      |  |                 |  |    |
    //            .----.    |  |   .----.        |  '----'
    //            |'   |    |  |   |''  |        |
    //            |    |    |  |   |    |       .v---.
    //            '----'    |  |   '----'       |13' |
    //                      |  |                |    |
    //              .----<---  -->----.         '----'
    //              |10  |       |8   |
    //              |    |       |    |
    //              '----'       '----'
    //}}DIAGRAM_END
    public class DFATable
    {
        // �ַ���
        Hashtable ht_Char = new Hashtable();

        //=============״̬��=======================
        //״̬��  ʵ��        ˵��
        // 1.     xxx         ����
        // 2.     -           ����ȫ����ע��
        // 3.     --          ����ȫ����ע��
        // 4.     --[         ����ȫ����ע��
        // 5.     --[[        ��׼����ע��
        // 6.     --[[]       ����ȫ����ע�ͽ���
        // 7.     --xxxxx     ��׼����ע��
        // 8.     "           ��׼�����ַ���
        // 9.     "xx\        ��׼�����ַ���(ת��)
        // 10.    '           ��׼�����ַ���
        // 11.    'xx\        ��׼�����ַ���(ת��)
        // 12.    [           ����ȫ�����ַ���
        // 13.    [[          ��׼�����ַ���
        // 14.    [[]         ����ȫ�����ַ�������
        //==========================================

        //״̬Ǩ�Ʊ�
        //�У���״̬     �ַ�      ��״̬     �ص�����
        ArrayList al_Table = new ArrayList();

        public DFATable()
        {
            //��ʼ���ַ���
            ht_Char[0] = "";        // 0��ʾƥ�������ַ�
            ht_Char[1] = "'";
            ht_Char[2] = "\"";
            ht_Char[3] = "-";
            ht_Char[4] = "[";
            ht_Char[5] = "]";
            ht_Char[6] = "\\";
            ht_Char[7] = "\n";

            //��ʼ��״̬Ǩ�Ʊ�
            al_Table.Add(new ZTableItem(1, 1, 10, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(1, 2, 8, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(1, 3, 2, null));
            al_Table.Add(new ZTableItem(1, 4, 12, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(1, 0, 1, null));    // 0��ʾƥ�������ַ�����������0�����ھ�״̬�����һ��

            al_Table.Add(new ZTableItem(2, 1, 10, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(2, 2, 8, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(2, 3, 3, null));
            al_Table.Add(new ZTableItem(2, 0, 1, null));

            al_Table.Add(new ZTableItem(3, 4, 4, null));
            al_Table.Add(new ZTableItem(3, 7, 1, null));
            al_Table.Add(new ZTableItem(3, 0, 7, null));

            al_Table.Add(new ZTableItem(4, 4, 5, null));
            al_Table.Add(new ZTableItem(4, 7, 1, null));
            al_Table.Add(new ZTableItem(4, 0, 7, null));

            al_Table.Add(new ZTableItem(5, 5, 6, null));
            al_Table.Add(new ZTableItem(5, 0, 5, null));

            al_Table.Add(new ZTableItem(6, 5, 1, null));
            al_Table.Add(new ZTableItem(6, 0, 5, null));

            al_Table.Add(new ZTableItem(7, 7, 1, null));
            al_Table.Add(new ZTableItem(7, 0, 7, null));

            al_Table.Add(new ZTableItem(8, 2, 1, new TEndRecord()));
            al_Table.Add(new ZTableItem(8, 6, 9, new TReocrd()));
            al_Table.Add(new ZTableItem(8, 0, 8, new TReocrd()));

            al_Table.Add(new ZTableItem(9, 0, 8, new TReocrd()));

            al_Table.Add(new ZTableItem(10, 1, 1, new TEndRecord()));
            al_Table.Add(new ZTableItem(10, 6, 11, new TReocrd()));
            al_Table.Add(new ZTableItem(10, 0, 10, new TReocrd()));

            al_Table.Add(new ZTableItem(11, 0, 10, new TReocrd()));

            al_Table.Add(new ZTableItem(12, 1, 10, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(12, 2, 8, new TClearAndRecord()));
            al_Table.Add(new ZTableItem(12, 4, 13, new TReocrd()));
            al_Table.Add(new ZTableItem(12, 0, 1, null));

            al_Table.Add(new ZTableItem(13, 5, 14, new TReocrd()));
            al_Table.Add(new ZTableItem(13, 0, 13, new TReocrd()));

            al_Table.Add(new ZTableItem(14, 5, 1, new TEndRecord()));
            al_Table.Add(new ZTableItem(14, 0, 13, new TReocrd()));
        }

        public Hashtable getStringList(string strCode)
        {

            CData.al_ret = new Hashtable();
            int zt = 1;
            for (int i = 0; i < strCode.Length; i++)
            {
                string strCh = strCode.Substring(i, 1);
                for (int j = 0; j < al_Table.Count; j++)
                {
                    ZTableItem zti = al_Table[j] as ZTableItem;
                    if (zti.zfrom == zt)
                    {
                        string strTableChar = ht_Char[zti.charTableIndex].ToString();
                        if (strTableChar == "" || strTableChar == strCh)
                        {
                            zt = zti.zto;
                            if (zti.funcdo != null)
                                zti.funcdo.Do(zt, strCh, zti.zto, i);
                            break;
                        }
                    }
                }
            }
            return CData.al_ret;
        }
    }

    /// <summary>
    /// ȫ������
    /// </summary>
    public static class CData
    {
        // �����ַ�������
        public static string strString = "";

        // ����ȫ������
        public static string strIndex = "0";

        // ����ֵ
        public static Hashtable al_ret = new Hashtable();
    }

    /// <summary>
    /// ���崦��ӿ�
    /// </summary>
    public interface IToDo
    {
        void Do(int zfrom, string charInput, int zto, int index);
    }
    
    /// <summary>
    /// ׷���ַ�������
    /// </summary>
    public class TReocrd : IToDo
    {
        public void Do(int zfrom, string charInput, int zto, int index)
        {
            CData.strString += charInput;
        }
    };

    /// <summary>
    /// ���û����ַ�
    /// </summary>
    public class TClearAndRecord : IToDo
    {
        public void Do(int zfrom, string charInput, int zto, int index)
        {
            CData.strIndex = index.ToString();
            CData.strString = charInput;
        }
    };

    /// <summary>
    /// ��¼�ַ�����¼��������������
    /// </summary>
    public class TEndRecord : IToDo
    {
        public void Do(int zfrom, string charInput, int zto, int index)
        {
            CData.strString += charInput;

            // ������Լ��뿪ͷΪ���ĵ��жϣ������Ҫ�Ļ���
            if (CheckStringAvail(CData.strString))
            {
                CData.al_ret.Add(CData.strIndex, CData.strString);
            }

            CData.strString = "";
        }

        public bool CheckStringAvail(string s)
        {
            bool avail = false;
            string content = s.Trim(new char[] { '"', '\'', ' ', ']', '[', '\r', '\n', '\t' });

            if (content.EndsWith(".lua") || content.EndsWith(".ls") || content.EndsWith(".lh")) // ����ǰ����ļ������ַ����Ͳ�����ȡ��
            {
                avail = false;
            }
            else
            {
                char[] charArray = content.ToCharArray();

                // ASCII��֮����ַ�����Ҫ��ȡ����
                foreach (char c in charArray)
                {
                    if ((int)c > 127)
                    {
                        avail = true;
                        break;
                    }
                }
            }

            return avail;
        }
    }

    /// <summary>
    /// ״̬Ǩ�Ʊ����
    /// </summary>
    public class ZTableItem
    {
        public ZTableItem(int zfrom, int charTableIndex, int zto, IToDo funcdo)
        {
            this.zfrom = zfrom;
            this.zto = zto;
            this.charTableIndex = charTableIndex;
            this.funcdo = funcdo;
        }

        public int zfrom = 0;
        public int zto = 0;
        public int charTableIndex = 0;
        public IToDo funcdo = null;
    }
}
