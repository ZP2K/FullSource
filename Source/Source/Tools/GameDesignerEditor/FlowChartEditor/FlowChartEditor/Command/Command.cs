using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace FlowChartEditor.Command
{
    abstract public class Command
    {
        protected string description; // ���������
        protected object data; // ��ǰ����
        protected Memento dataBeforeExecute; // ����ִ��ǰ������
        protected Memento dataAfterExecute; // ����ִ�к������
        protected bool firstCommand = false; // �Ƿ��ǵ�һ������         

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public Command(object data, string description)
        {
            this.data = data;
            this.description = description;
        }

        /// <summary>
        /// �������������
        /// </summary>
        public virtual string Description
        {
            get
            {
                return description;
            }
        }

        /// <summary>
        /// �Ƿ��ǵ�һ����������
        /// </summary>
        public virtual bool FirstCommand
        {
            set
            {
                firstCommand = value;
            }

            get
            {
                return firstCommand;
            }
        }

        /// <summary>
        /// ������ִ��ǰ���浱ǰ���������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected abstract void SaveBeforeExecute(object o);

        /// <summary>
        /// ������ִ�к󱣴浱ǰ���������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected abstract void SaveAfterExecute(object o);

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <return>�Ƿ�ִ�гɹ�</return>
        public abstract bool Execute(object o);

        /// <summary>
        /// ִ������
        /// </summary>
        public abstract void Execute();

        /// <summary>
        /// ȡ��ִ������
        /// </summary>
        public abstract void Unexecute();
    }
}
