using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data;

namespace FlowChartEditor
{
    class TreeCommand:Command
    {
        protected DocumentManager documentManager; // ���������

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="data">��ǰ����</param>
        /// <param name="description">���������</param>
        public TreeCommand(object data, string description):base(data, description)
        {
            dataBeforeExecute = new SerialMemento();
            dataAfterExecute = new SerialMemento();
        }

        /// <summary>
        /// ��ǰ�������������
        /// </summary>
        public DocumentManager CurrentDocumentManager
        {
            set
            {
                documentManager = value;
            }
        }

        /// <summary>
        /// ��鵱ǰ·���Ƿ���Ч
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <param name="text">��ǰ�ı�</param>
        /// <returns>��ǰ·���Ƿ���Ч</returns>
        protected bool CheckPathValid(TreeNode node, string text)
        {
            bool valid = true;

            if (node != null)
            {
                foreach (TreeNode childNode in node.Nodes)
                {
                    if (childNode.Text == text)
                    {
                        valid = false;
                        break;
                    }
                }
            }
            else // �ڸ�����ϼ��
            {
                TreeView tree = data as TreeView;
                foreach(TreeNode childNode in tree.Nodes)
                {
                    if (childNode.Text == text)
                    {
                        valid = false;
                        break;
                    }
                }
            }

            return valid;
        }

        /// <summary>
        /// ������ִ��ǰ�����������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void  SaveBeforeExecute(object o)
        {
            dataBeforeExecute.SaveData(o);
        }

        /// <summary>
        /// ������ִ�к󱣴��������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        protected override void  SaveAfterExecute(object o)
        {
            dataAfterExecute.SaveData(o);
        }

        /// <summary>
        /// ��ȡ������·��
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <returns>������·��</returns>
        protected string GetNodePath(TreeNode node)
        {
            string path = node.Text;
            TreeNode parentNode = node.Parent;
            while (parentNode != null)
            {
                path = parentNode.Text + "\\" + path;
                parentNode = parentNode.Parent;
            }

            return path;
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="tree">��ǰ������</param>
        /// <param name="table">��¼��������Ϣ�����ݱ�</param>
        protected void ConstructTree(TreeView tree, DataTable table)
        {
            string fullPath;
            string id;
            bool visible;
            bool focused;
            bool copy;
            bool cut;
            bool classifyNode;
            string[] data;
            TreeNode selectedNode = null;
            List<TreeNode> nodeList = new List<TreeNode>();
            string classification;
            TreeNodeCollection currentNodes;
            TreeNode newNode = null;

            tree.Nodes.Clear(); // ��������
            documentManager.CurrentTreeMode = EditMode.Normal; // ��������״̬

            foreach (DataRow row in table.Rows)
            {
                fullPath = row["FullPath"].ToString();
                id = row["ID"].ToString();
                visible = (bool)row["Visible"];
                focused = (bool)row["Focused"];
                copy = (bool)row["CopyNode"];
                cut = (bool)row["CutNode"];
                classifyNode = (bool)row["ClassifyNode"];
                data = fullPath.Split(new char[] { '\\' });
                currentNodes = tree.Nodes;

                for (int i = 0; i < data.Length; i++)
                {
                    classification = data[i];

                    if ((i == data.Length - 1) && !classifyNode) // ����ͼ���
                    {
                        newNode = currentNodes.Add(classification);
                        newNode.Tag = id;
                        newNode.ImageIndex = 1;
                        newNode.SelectedImageIndex = 1;
                    }
                    else // ����ͼ����
                    {
                        bool find = false;

                        foreach (TreeNode node in currentNodes)
                        {
                            if (node.Text == classification)
                            {
                                find = true;
                                currentNodes = node.Nodes;
                                break;
                            }
                        }

                        if (!find) // ��ǰ���಻����
                        {
                            newNode = currentNodes.Add(classification);
                            newNode.Tag = id;
                            newNode.ImageIndex = 0;
                            newNode.SelectedImageIndex = 0;
                            currentNodes = newNode.Nodes;
                        }
                    }
                }

                if (visible) // ��ǰ���ɼ�
                {
                    nodeList.Add(newNode);
                }

                if (focused) // ��ǰ��㱻ѡ��
                {
                    selectedNode = newNode;
                }

                if (copy) // ��ǰ��㱻����
                {
                    documentManager.CopyTreeNode = newNode;
                    documentManager.CurrentTreeMode = EditMode.Copy;
                }

                if (cut) // ��ǰ��㱻����
                {
                    documentManager.CutTreeNode = newNode;
                    documentManager.CurrentTreeMode = EditMode.Cut;
                }
            }

            // �ָ�������չ״̬
            foreach (TreeNode node in nodeList)
            {
                node.EnsureVisible();
            }

            // �ָ����Ľ���
            tree.SelectedNode = selectedNode;
        }

        /// <summary>
        /// ���������
        /// </summary>
        /// <param name="tree">��ǰ������</param>
        /// <returns>��¼��������Ϣ�����ݱ�</returns>
        protected DataTable SaveTree(TreeView tree)
        {
            DataTable table = new DataTable("");
            Type stringType = Type.GetType("System.String");
            Type boolType = Type.GetType("System.Boolean");

            table.Columns.Add("ID", stringType);
            table.Columns.Add("FullPath", stringType);
            table.Columns.Add("Visible", boolType);
            table.Columns.Add("Focused", boolType);
            table.Columns.Add("CopyNode", boolType);
            table.Columns.Add("CutNode", boolType);
            table.Columns.Add("ClassifyNode", boolType);
            List<TreeNode> flowChartNodeList = new List<TreeNode>();

            // �����ҳ����е�����ͼ���
            foreach (TreeNode node in tree.Nodes)
            {
                SeekLeafNode(node, flowChartNodeList);
            }

            // �������ݱ�
            string id;
            string fullPath;
            TreeNode parentNode;
            List<string> pathList = new List<string>();
            DataRow newRow;

            foreach (TreeNode node in flowChartNodeList)
            {
                id = node.Tag.ToString();
                parentNode = node.Parent;
                pathList.Clear();
                pathList.Add(node.Text);

                while (parentNode != null)
                {
                    pathList.Add(parentNode.Text);
                    parentNode = parentNode.Parent;
                }

                fullPath = "";
                for (int i = pathList.Count - 1; i >= 0; i--)
                {
                    fullPath += pathList[i] + "\\";
                }
                fullPath = fullPath.TrimEnd(new char[] { '\\' });

                newRow = table.NewRow();
                newRow["FullPath"] = fullPath;
                newRow["ID"] = id;
                newRow["Visible"] = node.IsVisible;
                newRow["Focused"] = node.IsSelected;
                newRow["CopyNode"] = ((documentManager.CurrentTreeMode == EditMode.Copy) && 
                    (node == documentManager.CopyTreeNode));
                newRow["CutNode"] = ((documentManager.CurrentTreeMode == EditMode.Cut) && 
                    (node == documentManager.CutTreeNode));
                newRow["ClassifyNode"] = (node.ImageIndex == 0);
                table.Rows.Add(newRow);
            }

            return table;
        }

        /// <summary>
        /// ����Ҷ�ӽ��
        /// </summary>
        /// <param name="node">��ǰ�����</param>
        /// <param name="list">����Ҷ�ӽ�������</param>
        private void SeekLeafNode(TreeNode node, List<TreeNode> list)
        {
            if (node.Nodes.Count == 0) // ��ǰ�������Ҷ�ӽ��
            {
                list.Add(node);
            }
            else // ��ǰ������Ƿ�����
            {
                foreach (TreeNode classifyNode in node.Nodes)
                {
                    SeekLeafNode(classifyNode, list);
                }
            }
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        /// <return>�Ƿ�ִ�гɹ�</return>
        public override bool Execute(object o)
        {
            return false;
        }

        /// <summary>
        /// ִ������
        /// </summary>
        /// <param name="o">��ǰ����</param>
        public override void Execute()
        {
            object o = dataAfterExecute.LoadData();
            TreeView tree = data as TreeView;
            ConstructTree(tree, o as DataTable);
        }

        /// <summary>
        /// ȡ��ִ������
        /// </summary>
        /// <returns>��ǰ����</returns>
        public override void Unexecute()
        {
            object o = dataBeforeExecute.LoadData();
            TreeView tree = data as TreeView;
            ConstructTree(tree, o as DataTable);
        }
    }
}
