using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;
using System.Threading;
using System.Diagnostics;
using LuaInterface;
using DevComponents.AdvTree;
using DevComponents.DotNetBar;
using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
    unsafe public partial class BaseForm
	{
        private Node lastSearchDoodadNode = null; // ��һ����������doodad

        /// <summary>
        /// ��ʼ��Doodad��� ahpho suntao
        /// </summary>
        private void InitDoodad()
        {
            // ��ʼ�����ݱ�
            string sqlString = "SELECT ID, Name, RepresentID, MapName FROM tbl_doodad";
            doodadTable = Helper.GetDataTable(sqlString, Conn);
            sqlString = "SELECT RepresentID, IdleModelFile, ReviveModelFile, DieModelFile, BeHitModelFile, AfterHitModelFile FROM doodad_represent";
            doodadRepresentTable = Helper.GetDataTable(sqlString, Conn);

            // ����������            
            doodadTree.Nodes.Clear();
            foreach (DataRow row in doodadTable.Rows)
            {
                string doodadID = row["ID"].ToString();
                string doodadName = row["Name"].ToString();
                string mapName = row["MapName"].ToString();
                string representID = row["RepresentID"].ToString();

                if (mapName == "")
                {
                    mapName = "δ����";
                }

                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", doodadName, doodadID);
                doodadInfoTable[newNode] = row;

                if (representID != "")
                {
                    DataRow[] rows = doodadRepresentTable.Select(string.Format("RepresentID = '{0}'", representID));
                    if (rows.Length > 0)
                    {
                        List<string> fileNameList = new List<string>();
                        string fileName = rows[0]["IdleModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["ReviveModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["DieModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["BeHitModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["AfterHitModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        newNode.Tag = fileNameList;
                    }
                }

                Node mapNode = null;

                foreach (Node node in doodadTree.Nodes)
                {
                    if (node.Text == mapName)
                    {
                        mapNode = node;
                        break;
                    }
                }

                if (mapNode == null)
                {
                    mapNode = new Node();
                    mapNode.Text = mapName;
                    doodadTree.Nodes.Add(mapNode);
                }

                mapNode.Nodes.Add(newNode);
            }

            // ���س���
            previewCanvas2.PreviewType = "Doodad";
            EngineLayer.ATLBase.LoadDoodadScene((int)previewCanvas2.Handle);
        }


        /// <summary>
        /// ѡ��Doodad
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void doodadTree_AfterNodeSelect(object sender, AdvTreeNodeEventArgs e)
        {
            Node currentNode = doodadTree.SelectedNode;

            if (currentNode != null)
            {
                if (currentNode.Level == 0) // ѡ�����
                {
                    previewCanvas2.BeginInit = false;
                    buttonX25.Enabled = false;
                    buttonX26.Enabled = false;
                }
                else // ѡ��Doodad
                {
                    List<string> fileNameList = currentNode.Tag as List<string>;

                    if (fileNameList.Count > 0)
                    {
                        // ��ȡģ���ļ�·��                
                        EngineLayer.ATLBase.LoadDoodadModel(fileNameList[0]);
                        string[] data = fileNameList[0].Split(new char[] { '\\' });
                        labelX2.Text = data[data.Length - 1];
                        previewCanvas2.BeginInit = true;
                        buttonX25.Enabled = true;
                        buttonX26.Enabled = true;

                        // �л���ˢ
                        DataRow row = doodadInfoTable[currentNode] as DataRow;
                        int doodadID = int.Parse(row["ID"].ToString());
                        string doodadName = row["Name"].ToString();
                        EngineLayer.ATLBase.SwitchDoodadBrush(doodadID, doodadName, 0);
                    }
                }

                labelX2.Text = "��ģ��";
            }
        }

        /// <summary>
        /// ����Doodad
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX23_Click(object sender, EventArgs e)
        {
            string searchName = textBoxX2.Text;
            bool findLastNode = false;
            bool find = false;

            foreach (Node mapNode in doodadTree.Nodes)
            {
                foreach (Node doodadNode in mapNode.Nodes)
                {
                    string doodadName = doodadNode.Text;

                    if (lastSearchDoodadNode == null)
                    {
                        findLastNode = true;
                    }

                    if (doodadNode == lastSearchDoodadNode)
                    {
                        findLastNode = true;
                        continue;
                    }

                    if (doodadName.Contains(searchName) && findLastNode)
                    {
                        doodadTree.SelectedNode = doodadNode;
                        lastSearchDoodadNode = doodadNode;
                        find = true;

                        break;
                    }
                }

                if (find)
                {
                    break;
                }
            }

            if (!find)
            {
                lastSearchDoodadNode = null;
            }
        }

        /// <summary>
        /// ˢ��Doodad
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX24_Click(object sender, EventArgs e)
        {
            // ����������            
            doodadTree.Nodes.Clear();
            doodadInfoTable.Clear();

            foreach (DataRow row in doodadTable.Rows)
            {
                string doodadID = row["ID"].ToString();
                string doodadName = row["Name"].ToString();
                string mapName = row["MapName"].ToString();
                string representID = row["RepresentID"].ToString();

                if (mapName == "")
                {
                    mapName = "δ����";
                }

                Node newNode = new Node();
                newNode.Text = string.Format("{0} ({1})", doodadName, doodadID);
                doodadInfoTable[newNode] = row;

                if (representID != "")
                {
                    DataRow[] rows = doodadRepresentTable.Select(string.Format("RepresentID = '{0}'", representID));
                    if (rows.Length > 0)
                    {
                        List<string> fileNameList = new List<string>();
                        string fileName = rows[0]["IdleModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["ReviveModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["DieModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["BeHitModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        fileName = rows[0]["AfterHitModelFile"].ToString();
                        fileName = Path.Combine(rootDir, fileName);
                        fileNameList.Add(fileName);
                        newNode.Tag = fileNameList;
                    }
                }

                Node mapNode = null;

                foreach (Node node in doodadTree.Nodes)
                {
                    if (node.Text == mapName)
                    {
                        mapNode = node;
                        break;
                    }
                }

                if (mapNode == null)
                {
                    mapNode = new Node();
                    mapNode.Text = mapName;
                    doodadTree.Nodes.Add(mapNode);
                }

                mapNode.Nodes.Add(newNode);
            }
        }


        /// <summary>
        /// Ԥ����һ��ģ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX25_Click(object sender, EventArgs e)
        {
            Node currentNode = doodadTree.SelectedNode;
            List<string> doodadModelList = currentNode.Tag as List<string>;
            doodadModelIndex--;
            if (doodadModelIndex < 0)
            {
                doodadModelIndex = 0;
            }

            if (doodadModelList.Count > 0)
            {
                string fileName = doodadModelList[doodadModelIndex];
                string[] data = fileName.Split(new char[] { '\\' });
                string modelName = data[data.Length - 1];

                labelX2.Text = modelName;
                fileName = fileName.Replace(rootDir, "");
                fileName = fileName.TrimStart(new char[] { '\\' });
                EngineLayer.ATLBase.LoadDoodadModel(fileName);
            }
        }

        /// <summary>
        /// Ԥ����һ��ģ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX26_Click(object sender, EventArgs e)
        {
            Node currentNode = doodadTree.SelectedNode;
            List<string> doodadModelList = currentNode.Tag as List<string>;
            doodadModelIndex++;

            if (doodadModelIndex > doodadModelList.Count - 1)
            {
                doodadModelIndex = doodadModelList.Count - 1;
            }

            if (doodadModelList.Count > 0)
            {
                string fileName = doodadModelList[doodadModelIndex];
                string[] data = fileName.Split(new char[] { '\\' });
                string modelName = data[data.Length - 1];

                labelX2.Text = modelName;
                fileName = fileName.Replace(rootDir, "");
                fileName = fileName.TrimStart(new char[] { '\\' });
                EngineLayer.ATLBase.LoadDoodadModel(fileName);
            }
        }

        /// <summary>
        /// �滻ģ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bReplaceDoodadTemplate_Click(object sender, EventArgs e)
        {
            Node currentNode = doodadTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                DataRow dataRow = doodadInfoTable[currentNode] as DataRow;

                if (dataRow != null)
                {
                    int newTemplateID = int.Parse(dataRow["ID"].ToString());

                    // ��ȡ���㣬�л���������
                    PANEL.Focus();

                    m_doc.DocLogical.ReplaceDoodadTemplate(newTemplateID, 0);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�滻��ģ������", "�滻ģ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ȫ�����滻ģ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bReplaceAllDoodadTemplate_Click(object sender, EventArgs e)
        {
            Node currentNode = doodadTree.SelectedNode;

            if (currentNode != null && currentNode.Level == 1)
            {
                DataRow dataRow = doodadInfoTable[currentNode] as DataRow;

                if (dataRow != null)
                {
                    int newTemplateID = int.Parse(dataRow["ID"].ToString());

                    // ��ȡ���㣬�л���������
                    PANEL.Focus();

                    m_doc.DocLogical.ReplaceDoodadTemplate(newTemplateID, 1);
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�滻��ģ������", "�滻ģ��", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }  
	}
}
