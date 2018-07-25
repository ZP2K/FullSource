using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Windows.Forms;

using DataElementLibrary;
using FlowChartEditor.BaseGraphElement;

namespace FlowChartEditor
{
    [Serializable]
    public class InterfaceNodeEditor : DataElement
    {
        private string interfaceName = ""; // �ӿ�����
        private bool isInput = true; // �Ƿ������

        /// <summary>
        /// �ӿ�����
        /// </summary>
        public string InterfaceName
        {
            get
            {
                return interfaceName;
            }
        }

        /// <summary>
        /// �Ƿ������
        /// </summary>
        public bool IsInput
        {
            get
            {
                return isInput;
            }
        }

        /// <summary>
        /// ��ʼ��������Ϣ
        /// </summary>
        private void InitDisplayInfo()
        {
            string mark = "��";
            description = "����";

            if (isInput)
            {
                mark = "��";
                description = "���";
            }

            // text = string.Format("{0} {1}", mark, interfaceName);
            text = interfaceName;
            tooltipText = string.Format("<bold>����\r\n{0}", description);
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="o">����Ԫ������</param>
        /// <return>�Ƿ�༭�ɹ�</return>
        public override bool EditData(Hashtable table)
        {
            bool editSuccess = false;

            Hashtable infoTable = new Hashtable();        

            InterfaceNodeForm interfaceNodeForm = new InterfaceNodeForm(interfaceName, isInput);

            if (interfaceNodeForm.ShowDialog() == DialogResult.OK)
            {
                interfaceName = interfaceNodeForm.InterfaceName;
                isInput = interfaceNodeForm.IsInput;

                infoTable["interfaceName"] = interfaceName;
                infoTable["isInput"] = isInput;

                // ��ʼ��������Ϣ
                InitDisplayInfo();

                this.data = infoTable;
                
                DocumentManager documentManager = DocumentManager.GetDocumentManager();
                InterfaceGraphElement interfaceGraphElement = documentManager.CurrentFlowChartManager.CurrentDataManager.FindGraphElementByID(id) as InterfaceGraphElement; 
                
                if (isInput)
                {
                    interfaceGraphElement.InSlotCount = 0;
                    interfaceGraphElement.OutSlotCount = 1;
                }
                else
                {
                    interfaceGraphElement.InSlotCount = 1;
                    interfaceGraphElement.OutSlotCount = 0;
                }

                // �������ڲ��
                interfaceGraphElement.LockOutSlot();

                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// �ض�����
        /// </summary>
        /// <param name="table">��ǰ����</param>
        /// <return>�Ƿ��ض��ɹ�</return>
        public override bool ReloadData(Hashtable table)
        {
            Hashtable infoTable = data as Hashtable;
            
            if (infoTable != null)
            {
                interfaceName = infoTable["interfaceName"] as string;
                isInput = (bool)infoTable["isInput"];

                // ��ʼ��������Ϣ
                InitDisplayInfo();
            }             

            return true;
        }

        /// <summary>
        /// ��ȡ����Ĳ���
        /// </summary>
        /// <returns>����Ĳ���</returns>
        public override Hashtable GetOutputArgs()
        {
            Hashtable infoTable = data as Hashtable;

            return infoTable;
        }
    }
}
