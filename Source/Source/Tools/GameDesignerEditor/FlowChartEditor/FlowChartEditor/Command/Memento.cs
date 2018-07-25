using System;
using System.Collections.Generic;
using System.Text;

namespace FlowChartEditor.Command
{
    public abstract class Memento
    {
        /// <summary>
        /// �����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        public abstract void SaveData(object o);

        /// <summary>
        /// ���ض�������
        /// </summary>
        /// <returns>������</returns>
        public abstract object LoadData();
    }
}
