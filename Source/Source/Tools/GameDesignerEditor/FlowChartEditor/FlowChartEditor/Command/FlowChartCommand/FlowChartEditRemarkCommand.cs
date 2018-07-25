using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Collections;
using System.Windows.Forms;
using System.Drawing;
using FlowChartEditor.BaseGraphElement;
using GraphElementLibrary;
using DataElementLibrary;

namespace FlowChartEditor.Command.FlowChartCommand
{
    class FlowChartEditRemarkCommand:FlowChartBaseCommand
    {
        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public FlowChartEditRemarkCommand(object data, string description):
            base(data, description)
        {
            
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">����Ĳ���</param>
        /// <returns>�Ƿ�ִ�гɹ�</returns>
        public override bool Execute(object o)
        {
            bool success = false;           
            SlotContainer slotContainer = o as SlotContainer;
            string remark = "";

            try
            {
                Assembly assembly = Assembly.LoadFrom("Plugins\\FlowChart\\TextEditor.dll");
                DataElement dataElement = assembly.CreateInstance("TextEditor.TextEditor") as DataElement;
                Hashtable table = new Hashtable();
                table["data"] = slotContainer.Remark;                
                if(dataElement.EditData(table))
                {
                    success = true;
                    remark = dataElement.Data as string;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("�ڱ༭ͼԪע��ʱ�����쳣: " + ex.ToString(), "ע�ͱ༭", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }

            // ����ע�ͽ��
            if(success)
            {
                // ����ִ��ǰ������
                description = "�༭ͼԪע�� " + slotContainer.Name;
                if (firstCommand) // ֻ�е�һ�������ִ��ǰ������
                {
                    SaveBeforeExecute(flowChartManager.GetArguments());                    
                }   
                
                slotContainer.Remark = remark;
                RemarkGraphElement remarkGraphElement;
                if (slotContainer.RemarkNode == null) // ��Ҫ����ע�ͽ��
                {
                    remarkGraphElement = new RemarkGraphElement(slotContainer, slotContainer.Location + new Size(slotContainer.ElementSize.Width + 50, 0),
                                                                new Size(1, 1));
                    remarkGraphElement.Text = remark;
                    remarkGraphElement.Name = "ע�ͽ��";
                    remarkGraphElement.Enable = true;                    
                    remarkGraphElement.Refresh();
                    remarkGraphElement.AdjustElementSize();
                    remarkGraphElement.AdjustLine();
                    slotContainer.RemarkNode = remarkGraphElement;
                    slotContainer.ShowRemark = true;
                }
                else
                {                    
                    remarkGraphElement = slotContainer.RemarkNode;
                    remarkGraphElement.Text = remark;
                    remarkGraphElement.AdjustElementSize();
                }                                                                          

                // ����ִ�к������
                flowChartManager.ContentChanged = true;
                SaveAfterExecute(flowChartManager.GetArguments());            
            }

            return success;
        }
    }
}
