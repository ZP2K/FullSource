using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace GameDesingerTools
{
    public interface ICommand
    {
        void Execute();
        void UnExecute();
    };

    class Command : ICommand
    {
        public virtual void Execute()
        {
            string s = string.Format("δʵ�ִ˹��ܡ�");
            //System.Windows.Forms.MessageBox.Show(s);
            throw new Exception(s);
        }
        public virtual void UnExecute()
        {
            string s = string.Format("δʵ�ִ˹��ܡ�");
            //System.Windows.Forms.MessageBox.Show(s);
            throw new Exception(s);
        }
        
    };

    class SaveCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ���湦�ܡ�");
            }
        }
    }
    
    class SetValueCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ�޸�ֵ�Ĺ��ܡ�");
            }
        }
    }
    class NewSubTalbeRecordCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                string s = "�Ѿ�����ֹ�½��ӱ��¼�Ĺ��ܡ�";
                MessageBox.Show(s, "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                throw new Exception(s);
            }
        }
    }
    class DelSubTalbeRecordCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹɾ���ӱ��¼�Ĺ��ܡ�");
            }
        }
    }
    class ModifyTreeCommand : Command
    {
        public ModifyTreeCommand(TreeView tree)
        {
            if (!RightsSettings.AllowSave)
            {
                tree.AllowDrop = false;
                tree.BeforeLabelEdit -= new NodeLabelEditEventHandler(this.baseTree_BeforeLabelEdit);
                tree.BeforeLabelEdit -= new NodeLabelEditEventHandler(this.baseTree_BeforeLabelEdit);
                tree.BeforeLabelEdit += new NodeLabelEditEventHandler(this.baseTree_BeforeLabelEdit);
                tree.NodeMouseClick -= new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView_NodeMouseClick);
                tree.NodeMouseClick -= new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView_NodeMouseClick);
                tree.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeView_NodeMouseClick);
            }
        }
        public override void Execute()
        {
            /*if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ�޸����ṹ�Ĺ��ܡ�");
            }*/
        }
        private void baseTree_BeforeLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            //MessageBox.Show("�Ѿ�����ֹ�޸����ṹ�Ĺ��ܡ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            throw new Exception("�Ѿ�����ֹ�޸����ṹ�Ĺ��ܡ�");
        }
        private void treeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.Node != null)
            {
                //MessageBox.Show("�Ѿ�����ֹ�޸����ṹ�Ĺ��ܡ�", "����", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                throw new Exception("�Ѿ�����ֹ�޸����ṹ�Ĺ��ܡ�");
            }
        }
    }

    class OpenDBToolsCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ�����ݱ�༭���Ĺ��ܡ�");
            }
        }
    }
    
    class OpenMetaInfoEditorCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ�򿪱�Ԫ�༭���Ĺ��ܡ�");
            }
        }
    }
    class ModifyScriptCommand : Command
    {
        public override void Execute()
        {
            if (!RightsSettings.AllowSave)
            {
                throw new Exception("�Ѿ�����ֹ�޸Ľű��Ĺ��ܡ�");
            }
        }
    }
}
