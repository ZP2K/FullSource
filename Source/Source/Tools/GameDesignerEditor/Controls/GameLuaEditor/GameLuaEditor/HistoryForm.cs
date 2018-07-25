using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;
using DataBaseIO;

namespace GameLuaEditor
{
    public partial class HistoryForm : Form
    {
        SqlConnection conn; // sql����
        private string id; // �ű���id
        private List<string> labelList = new List<string>(); // ��¼�ű�id������
        private string scriptData; // �ű�����
        private Hashtable labelTable = new Hashtable(); // �ű�label�����ݵ�hash��
        DataTable logTable; // ��־���ݱ�
        ImageList imageList; // ͼ������
        private bool recoverRecord = false; // �Ƿ�ָ���ɾ����¼
        private string path; // Ҫ�ָ�����ʷ��¼��·��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="id">�ű���id</param>
        public HistoryForm(SqlConnection conn, string id)
        {
            this.conn = conn;
            this.id = id;
            InitializeComponent();
            InitData(id);
        }

        /// <summary>
        /// �ű���������
        /// </summary>
        public string ScriptData
        {
            get
            {
                return scriptData;
            }
        }

        /// <summary>
        /// ������ͼ����������
        /// </summary>
        public ImageList TreeImageList
        {
            set
            {
                imageList = value;
            }

            get
            {
                return imageList;
            }
        }

        /// <summary>
        /// �Ƿ�ָ���ɾ������ʷ��¼
        /// </summary>
        public bool RecoverRecord
        {
            get
            {
                return recoverRecord;
            }
        }

        /// <summary>
        /// Ҫ�ָ�����ʷ��¼��·������
        /// </summary>
        public string Path
        {
            get
            {
                return path;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void InitData(string id)
        {
            listBox1.Items.Clear();
            labelList.Clear();

            try
            {
                if(conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT label, host, edittime, kind FROM {0} WHERE {1} = \'{2}\' ORDER BY {3}", 
                    "sys_script_log", "id", id, "edittime");
                logTable = GetDataTable(sqlString);

                if(logTable.Rows.Count > 0)
                {
                    string edittime;
                    string host;
                    string kind;
                    string label;

                    foreach(DataRow r in logTable.Rows)
                    {
                        if(r["edittime"] is DBNull)
                        {
                            edittime = "δ��¼ʱ��";
                        }
                        else
                        {
                            edittime = r["edittime"].ToString();
                        }
                        
                        if(r["host"] is DBNull)
                        {
                            host = "δ��¼�û�";
                        }
                        else
                        {
                            host = r["host"].ToString();
                        }

                        if(r["kind"] is DBNull)
                        {
                            kind = "δ��¼����";
                        }
                        else
                        {
                            kind = r["kind"].ToString();
                        }
                        
                        label = r["label"].ToString();
                        listBox1.Items.Add(string.Format("{0} {1} [{2}]", edittime, host, kind));
                        labelList.Add(label);
                    }
                }
                else
                {
                    MessageBox.Show("û���ҵ��ýű�����ʷ��¼��Ϣ!", "��ʷ��¼����", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
            }
            catch(SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ�ű�����ʷ��¼ʱ����sql�쳣: " + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            DataTable tb = ds.Tables[0];
            return tb;
        }

        /// <summary>
        /// �û��ı�ѡ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = listBox1.SelectedIndex;
            if(index < 0 || index >= labelList.Count) // Խ��
            {
                return;
            }
            string label = labelList[index];
            string data = labelTable[label] as string;
            if(data == null)
            {
                data = DataBaseManager.GetDataBaseManager().GetScriptHistoryData(label);
                labelTable[label] = data;
            }
                
            if(data == null)
            {
                data = "";
            }
            textBoxX1.Text = data;
        }

        /// <summary>
        /// �û�ѡ���ȡ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            scriptData = textBoxX1.Text;
            if(scriptData == "")
            {
                MessageBox.Show("�ű����ݲ���Ϊ��!", "��ʷ�ű���ȡ", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            DialogResult = DialogResult.Yes;
            this.Close();
        }

        /// <summary>
        /// �û�ѡ��ر�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }

        /// <summary>
        /// �鿴ɾ����¼
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void recoverToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RecordForm rForm = new RecordForm(conn, imageList);
            DialogResult result = rForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                InitData(rForm.ID);
                path = rForm.Path;
                this.Text = "�鿴��ʷ��¼ (��ɾ��)  " + path;
                recoverRecord = true;
                buttonX3.Enabled = true;
                buttonX1.Enabled = false;
            }
        }

        /// <summary>
        /// �ָ���ɾ����¼
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            scriptData = textBoxX1.Text;
            if (scriptData == "")
            {
                MessageBox.Show("�ű����ݲ���Ϊ��!", "��ʷ�ű��ָ�", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            DialogResult = DialogResult.Yes;
            this.Close();
        }

        /// <summary>
        /// �رմ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�</param>
        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.No;
            this.Close();
        }
    }
}