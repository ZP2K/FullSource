using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace CreateAICustomFunction
{
    public partial class CustomFunctionForm : Form
    {
        SqlConnection conn; // sql����
        private string functionName; // ��������
        private int functionID; // ����id

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public CustomFunctionForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            comboBoxEx1.SelectedIndex = 0;
        }

        /// <summary>
        /// ��������
        /// </summary>
        public string FunctionName
        {
            get
            {
                return functionName;
            }
        }

        /// <summary>
        /// ����ID
        /// </summary>
        public int FunctionID
        {
            get
            {
                return functionID;
            }
        }

        /// <summary>
        /// ���½ű�����
        /// </summary>
        private void UpdateScript()
        {
            string englishName = textBoxX1.Text;
            string chineseName = textBoxX2.Text;
            string arguments = textBoxX5.Text;
            string returnValue = textBoxX6.Text;

            // ȥ������ֵ�еĿհ��ַ���
            if (returnValue.Contains(" "))
            {
                string[] returnData = returnValue.Split(new char[] { ',' });
                returnValue = "";
                
                foreach (string s in returnData)
                {
                    returnValue += string.Format("{0},", returnData[0].Trim());
                }
                returnValue = returnValue.TrimEnd(new char[] { ',' });
            } 

            StringBuilder scriptText = new StringBuilder();
            scriptText.Append(string.Format("-- {0}\r\n", chineseName));
            scriptText.Append(string.Format("-- ������{0}\r\n", GetArgumentString(arguments)));
            scriptText.Append(string.Format("-- ����ֵ��{0}\r\n", returnValue));

            scriptText.Append(string.Format("function _{0}(char, p1, p2, p3, p4, p5)\r\n\r\n", englishName));
            scriptText.Append("end\r\n");

            codeEditBox.Text = scriptText.ToString();
        }

        /// <summary>
        /// ��鷽�����Ƿ��ͻ
        /// </summary>
        /// <param name="functionName">������</param>
        /// <returns>�Ƿ��ͻ</returns>
        private bool CheckFunctionConflict(string functionName)
        {
            bool conflict = false;

            string sqlString = "SELECT ename FROM AI_Action_Define";
            DataTable functionTable = GetDataTable(sqlString, conn);

            DataRow[] rows = functionTable.Select(string.Format("ename = '{0}'", functionName));
            if (rows.Length > 0)
            {
                conflict = true;
            }

            return conflict;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        public DataTable GetDataTable(string sqlString, SqlConnection conn)
        {
            DataTable tb = null;
            ConnectionState state = conn.State;

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }
                
                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                tb = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("��ȡ���ݱ�ʱ����sql�쳣��" + ex.ToString(), "��ȡ���ݱ�", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            return tb;
        }

        /// <summary>
        /// ��ȡ�����ַ���
        /// </summary>
        /// <param name="arguments">����ֵ</param>
        /// <returns>�����ַ���</returns>
        private string GetArgumentString(string arguments)
        {
            string argumentString = "";

            string[] lines = arguments.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach(string s in lines)
            {
                string line = s.TrimEnd(new char[] { '\r' });
                string[] data = line.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                argumentString += string.Format("{0}, ", data[0]);
            }
            argumentString = argumentString.TrimEnd(new char[] { ',', ' ' });

            return argumentString;
        }


        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            string englishName = textBoxX1.Text.Replace("'", "''");
            string chineseName = textBoxX2.Text.Replace("'", "''");
            string arguments = textBoxX5.Text.Replace("'", "''");
            string returnValue = textBoxX6.Text.Replace("'", "''");
            string description = textBoxX3.Text.Replace("'", "''");
            string actionType = textBoxX4.Text.Replace("'", "''");
            string scriptText = codeEditBox.Text.Replace("'", "''");
            string idString = null;
            int id = 1;
            int isSystem = comboBoxEx1.SelectedIndex;
            bool editSuccess = true;

            if (englishName == "")
            {
                MessageBox.Show("Ӣ��������Ϊ�գ�", "�����Զ��嶯��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (chineseName == "")
            {
                MessageBox.Show("����������Ϊ�գ�", "�����Զ��嶯��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (description == "")
            {
                MessageBox.Show("��������Ϊ�գ�", "�����Զ��嶯��", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            string[] returnData = returnValue.Split(new char[]{','}, StringSplitOptions.RemoveEmptyEntries);
            if(returnData.Length == 0 || returnData.Length > 3)
            {
                MessageBox.Show("��������ֵ��Ч��", "�������", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            // ȥ������ֵ�еĿհ��ַ���
            if (returnValue.Contains(" "))
            {
                returnValue = "";
                foreach (string s in returnData)
                {
                    returnValue += string.Format("{0},", returnData[0].Trim());
                }
                returnValue = returnValue.TrimEnd(new char[] { ',' });
            } 

            if (actionType == "")
            {
                actionType = "δ����";
            }

            if (isSystem == 1)
            {
                scriptText = "";
            }

            functionName = chineseName;

            // �������ݿ�            
            SqlTransaction transaction = null; // ����

            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                transaction = conn.BeginTransaction();
                SqlCommand cmd = conn.CreateCommand();
                cmd.Transaction = transaction;

                // ���䶯��ID
                string sqlString = string.Format("SELECT MAX(id) FROM AI_Action_Define");
                cmd.CommandText = sqlString;
                idString = cmd.ExecuteScalar().ToString();
                id = int.Parse(idString);
                id++;
                functionID = id + 1000;

                // �����¼
                sqlString = string.Format("INSERT INTO AI_Action_Define (id, actionid, issystem, name, ename, info, parms, rettype, script, class) VALUES ('{0}', '{1}', {2}, '{3}', '{4}', '{5}', '{6}', '{7}', '{8}', '{9}')",
                        id.ToString(), functionID.ToString(), isSystem.ToString(), chineseName, englishName, description, arguments, returnValue, scriptText, actionType);
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();    
                            
                transaction.Commit(); // �ύ���� 
                functionID = id;                               
            }
            catch (SqlException ex)
            {
                if (transaction != null) // �ع�����
                {
                    transaction.Rollback();
                }
                MessageBox.Show("�����Զ��嶯������Sql�쳣��" + ex.ToString(), "�½���ͼ", MessageBoxButtons.OK,
                    MessageBoxIcon.Information);
                editSuccess = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }

            if (editSuccess)
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
        private void buttonX2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// Ӣ�����ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void textBoxX1_TextChanged(object sender, EventArgs e)
        {
            if (textBoxX1.Text != "")
            {
                UpdateScript();
            }
        }

        /// <summary>
        /// �������ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void textBoxX2_TextChanged(object sender, EventArgs e)
        {
            if (textBoxX2.Text != "")
            {
                UpdateScript();
            }
        }

        /// <summary>
        /// �����ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void textBoxX5_TextChanged(object sender, EventArgs e)
        {
            if (textBoxX5.Text != "")
            {
                UpdateScript();
            }
        }

        /// <summary>
        /// ����ֵ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void textBoxX6_TextChanged(object sender, EventArgs e)
        {
            if (textBoxX6.Text != "")
            {
                UpdateScript();
            }
        }

        /// <summary>
        /// �������͸ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void comboBoxEx1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxEx1.SelectedIndex == 0)
            {
                codeEditBox.Enabled = true;
            }
            else
            {
                codeEditBox.Enabled = false;
            }
        }

        /// <summary>
        /// �༭��������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX3_Click(object sender, EventArgs e)
        {
            ParameterEditor pForm = new ParameterEditor(conn, textBoxX5.Text);
            if (pForm.ShowDialog() == DialogResult.OK)
            {
                textBoxX5.Text = pForm.NewValue;
            }
        }
    }
}