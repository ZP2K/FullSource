using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Xml;

using DevComponents.DotNetBar;

namespace GuideConfig
{
    public partial class ListConfigForm : Office2007Form
    {
        private SqlConnection conn;
        private string configInfo;
        private string listType; // ����������
        private bool initFinished = false;
        private Dictionary<string, DataTable> tableInfoDictionary = new Dictionary<string, DataTable>();

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="listType">����������</param> 
        /// <param name="conn">sql����</param>
        /// <param name="configInfo">������Ϣ</param>
        public ListConfigForm(SqlConnection conn, string listType, string configInfo)
        {
            this.conn = conn;
            this.listType = listType;
            this.configInfo = configInfo;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ������Ϣ
        /// </summary>
        public string ConfigInfo
        {
            get
            {
                return configInfo;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {            
            // ��ʼ�����ݱ������Ϣ
            string sqlString = "SELECT name FROM sysobjects WHERE xtype = 'u' AND name NOT LIKE 'sys_' ORDER BY name";
            DataTable dataTable = Helper.GetDataTable(sqlString, conn);

            foreach (DataRow dataRow in dataTable.Rows)
            {
                string tableName = dataRow["name"].ToString();
                sqlString = string.Format("SELECT TOP 1 * FROM {0}", tableName);
                DataTable rowInfo = Helper.GetDataTable(sqlString, conn);
                tableInfoDictionary[tableName] = rowInfo;

                tableNameBox.Items.Add(tableName);
            }

            // ��ʾ��ҳ��Ϣ
            switch (listType)
            {
                case "text":
                    {
                        listConfigControl.SelectedTabIndex = 0;
                        break;
                    }
                case "database":
                    {
                        listConfigControl.SelectedTabIndex = 1;
                        break;
                    }
            }

            // ��ȡ������Ϣ
            if (!string.IsNullOrEmpty(configInfo))
            {
                XmlDocument xmlDocument = new XmlDocument();
                xmlDocument.LoadXml(configInfo);

                XmlNode listNode = xmlDocument.ChildNodes[0];
                string typeString = listNode.Attributes["type"].Value;

                if (typeString == listType)
                {
                    switch (typeString)
                    {
                        case "text":
                            {
                                foreach (XmlNode itemNode in listNode.ChildNodes)
                                {
                                    string itemName = itemNode.Attributes["name"].Value;
                                    string itemValue = itemNode.Attributes["value"].Value;

                                    string itemInfo = string.Format("{0} ({1})", itemName, itemValue);
                                    listTextBox.Items.Add(itemInfo);
                                }

                                break;
                            }
                        case "database":
                            {
                                string tableName = listNode.Attributes["tableName"].Value;
                                string displayField = listNode.Attributes["displayField"].Value;
                                string valueField = listNode.Attributes["valueField"].Value;

                                tableNameBox.Text = tableName;
                                displayFieldBox.Text = displayField;
                                valueFieldBox.Text = valueField;

                                break;
                            }
                    }
                }                
            }

            initFinished = true;
        }

        /// <summary>
        /// ��ȡѡ����Ϣ
        /// </summary>
        /// <param name="infoString">ѡ���ַ�������</param>
        /// <returns>ѡ����Ϣ</returns>
        private string[] GetItemInfo(string infoString)
        {
            int index = infoString.LastIndexOf("(");
            string itemName = infoString.Substring(0, index - 1);
            string itemValue = infoString.Substring(index + 1, infoString.Length - index - 2);

            return new string[] { itemName, itemValue };
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bOK_Click(object sender, EventArgs e)
        {
            bool valid = true;

            switch (listConfigControl.SelectedTab.Text)
            {
                case "�ı�":
                    {
                        if (listTextBox.Items.Count > 0)
                        {
                            StringBuilder infoString = new StringBuilder();
                            infoString.AppendLine("<listInfo type = \"text\">");                            
                            
                            foreach (string listItem in listTextBox.Items)
                            {
                                string[] itemInfo = GetItemInfo(listItem);
                                infoString.AppendLine(string.Format("<listItem name = \"{0}\" value = \"{1}\">",
                                                                    itemInfo[0], itemInfo[1]));
                                infoString.AppendLine("</listItem>");
                            }

                            infoString.AppendLine("</listInfo>");

                            configInfo = infoString.ToString();
                        }
                        else
                        {
                            valid = false;
                            MessageBox.Show("��������ı�ѡ�", "����������",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }

                        break;
                    }
                case "���ݿ�����":
                    {
                        string tableName = tableNameBox.Text;
                        string displayField = displayFieldBox.Text;
                        string valueField = valueFieldBox.Text;

                        if (!string.IsNullOrEmpty(tableName) && !string.IsNullOrEmpty(displayField)
                            && !string.IsNullOrEmpty(valueField))
                        {
                            StringBuilder infoString = new StringBuilder();
                            infoString.AppendLine(string.Format("<listInfo type = \"database\" tableName = \"{0}\" displayField = \"{1}\" valueField = \"{2}\">",
                                                                tableName, displayField, valueField));                                                    
                            infoString.AppendLine("</listInfo>");

                            configInfo = infoString.ToString();
                        }
                        else
                        {
                            valid = false;
                            MessageBox.Show("�������������ݿ������Ĳ�����", "����������",
                                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                        }

                        break;
                    }
            }

            if (valid)
            {
                DialogResult = DialogResult.OK;
                this.Close();
            }
        }

        /// <summary>
        /// ѡ��ȡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// ���ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddListItem_Click(object sender, EventArgs e)
        {
            string itemName = listItemNameBox.Text;
            string itemValue = listItemValueBox.Text;

            if (!string.IsNullOrEmpty(itemName) && !string.IsNullOrEmpty(itemValue))
            {
                string itemInfo = string.Format("{0} ({1})", itemName, itemValue);

                bool itemExist = false;

                foreach (string listItem in listTextBox.Items)
                {
                    if (listItem == itemInfo)
                    {
                        itemExist = true;
                        break;
                    }
                }

                if (!itemExist)
                {
                    listItemNameBox.Text = "";
                    listItemValueBox.Text = "";
                    listTextBox.Items.Add(itemInfo);
                    listTextBox.SelectedIndex = listTextBox.Items.Count - 1;
                }
                else
                {
                    MessageBox.Show("�������ظ���ѡ�", "���ѡ��",
                                    MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            else
            {
                MessageBox.Show("�����������ѡ������ƺ�ֵ��", "���ѡ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ɾ��ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bRemove_Click(object sender, EventArgs e)
        {
            int index = listTextBox.SelectedIndex;

            if (index != -1)
            {
                listTextBox.Items.RemoveAt(index);
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫɾ����ѡ�", "ɾ��ѡ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ����ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMoveUp_Click(object sender, EventArgs e)
        {
            int index = listTextBox.SelectedIndex;

            if (index != -1)
            {
                if (index > 0)
                {
                    string itemInfo = listTextBox.Items[index] as string;
                    listTextBox.Items.RemoveAt(index);
                    listTextBox.Items.Insert(index - 1, itemInfo);
                    listTextBox.SelectedIndex = index - 1;
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���Ƶ�ѡ�", "����ѡ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ����ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bMoveDown_Click(object sender, EventArgs e)
        {
            int index = listTextBox.SelectedIndex;

            if (index != -1)
            {
                if (index < listTextBox.Items.Count - 1)
                {
                    string itemInfo = listTextBox.Items[index] as string;
                    listTextBox.Items.RemoveAt(index);
                    listTextBox.Items.Insert(index + 1, itemInfo);
                    listTextBox.SelectedIndex = index + 1;
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ���Ƶ�ѡ�", "����ѡ��",
                                MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        /// <summary>
        /// ѡ�����ݱ�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void tableNameBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (initFinished)
            {
                string tableName = tableNameBox.Text;
                DataTable tableInfo = tableInfoDictionary[tableName];

                foreach (DataColumn dataColumn in tableInfo.Columns)
                {
                    displayFieldBox.Items.Add(dataColumn.ColumnName);
                    valueFieldBox.Items.Add(dataColumn.ColumnName);
                }
            }
        }
    }
}