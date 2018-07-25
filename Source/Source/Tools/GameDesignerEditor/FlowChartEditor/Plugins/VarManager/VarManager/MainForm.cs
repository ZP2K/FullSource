using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Collections;

using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;

namespace VarManager
{
    public partial class MainForm : Office2007Form
    {
        SqlConnection conn; // sql����
        private DataTable table; // ���ݱ�

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        public MainForm(SqlConnection conn)
        {
            this.conn = conn;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ȡ���ݱ�����
            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = "SELECT * FROM VarData";
                table = GetDataTable(sqlString);
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ��������ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            // �����������
            DataGridViewCheckBoxColumn checkColumn = new DataGridViewCheckBoxColumn();
            checkColumn.Name = "checkColumn";
            checkColumn.HeaderText = "�༭";
            DataGridViewTextBoxColumn idColumn = new DataGridViewTextBoxColumn();
            idColumn.Name = "idColumn";
            idColumn.HeaderText = "����ID";
            idColumn.ReadOnly = true;
            DataGridViewTextBoxColumn nameColumn = new DataGridViewTextBoxColumn();
            nameColumn.Name = "nameColumn";
            nameColumn.HeaderText = "��������";
            nameColumn.ReadOnly = true;
            DataGridViewTextBoxColumn bindTypeColumn = new DataGridViewTextBoxColumn();
            bindTypeColumn.Name = "bindTypeColumn";
            bindTypeColumn.HeaderText = "������";
            bindTypeColumn.ReadOnly = true;
            DataGridViewTextBoxColumn bindObjectColumn = new DataGridViewTextBoxColumn();
            bindObjectColumn.Name = "bindObjectColumn";
            bindObjectColumn.HeaderText = "�󶨶���";
            bindObjectColumn.ReadOnly = true;
            DataGridViewTextBoxColumn typeColumn = new DataGridViewTextBoxColumn();
            typeColumn.Name = "typeColumn";
            typeColumn.HeaderText = "��������";
            typeColumn.ReadOnly = true;
            DataGridViewTextBoxColumn userColumn = new DataGridViewTextBoxColumn();
            userColumn.Name = "userColumn";
            userColumn.HeaderText = "������";
            userColumn.ReadOnly = true;
            DataGridViewTextBoxColumn descriptionColumn = new DataGridViewTextBoxColumn();
            descriptionColumn.Name = "descriptionColumn";
            descriptionColumn.HeaderText = "��������";
            descriptionColumn.ReadOnly = true;

            dataGridViewX1.Columns.Add(checkColumn);
            dataGridViewX1.Columns.Add(idColumn);
            dataGridViewX1.Columns.Add(nameColumn);
            dataGridViewX1.Columns.Add(bindTypeColumn);
            dataGridViewX1.Columns.Add(bindObjectColumn);
            dataGridViewX1.Columns.Add(typeColumn);
            dataGridViewX1.Columns.Add(userColumn);
            dataGridViewX1.Columns.Add(descriptionColumn);

            // ���������
            int index = 0;
            foreach(DataRow row in table.Rows)
            {
                dataGridViewX1.Rows.Add(1);
                DataGridViewRow newRow = dataGridViewX1.Rows[index];
                newRow.Cells["idColumn"].Value = row["ID"];
                newRow.Cells["nameColumn"].Value = row["Name"];
                newRow.Cells["bindTypeColumn"].Value = row["BindType"];
                newRow.Cells["bindObjectColumn"].Value = row["BindObject"];
                newRow.Cells["typeColumn"].Value = row["Type"];
                newRow.Cells["userColumn"].Value = row["UserName"];
                newRow.Cells["descriptionColumn"].Value = row["Description"];
                index++;
            }

            // ��յ�ǰ�û��ϵ���
            ClearLock();
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        /// <summary>
        /// ��ȡ���ñ�����ƫ��
        /// </summary>
        /// <param name="varLength">�����ĳ���</param>
        /// <param name="bindType">������</param>
        /// <param name="bindID">�󶨶����ID</param>
        /// <returns>���ñ�����ƫ��</returns>
        private int GetVarOffset(int varLength, string bindType, string bindID)
        {
            int varOffset = -1;
            int head = 0;
            int tail = head + varLength - 1;
            int maxLength = -1;

            switch(bindType)
            {
                case "����":
                    {
                        maxLength = 1023;
                        break;
                    }
                case "���":
                    {
                        maxLength = 127;
                        break;
                    }
            }

            List<int[]> list = new List<int[]>();
            foreach(DataRow row in table.Rows)
            {
                if(row["BindType"].ToString() == bindType && row["BindID"].ToString() == bindID)
                {
                    int startIndex = (int)row["StartIndex"];
                    int length = (int)row["Length"];
                    int[] border = new int[] { startIndex, startIndex + length - 1 };
                    list.Add(border);
                }                
            }

            while(tail <= maxLength)
            {
                bool find = true;
                int newIndex = tail;

                foreach(int[] border in list)
                {
                    int startIndex = border[0];
                    int endIndex = border[1];

                    if((startIndex >= head && startIndex <= tail) ||
                        (endIndex >= head && endIndex <= tail)) // ������������������Ƿ��н���
                    {
                        find = false;
                        newIndex = endIndex + 1;
                        break;
                    }
                }

                if(find)
                {
                    break;
                }
                else
                {
                    head = newIndex;
                    tail = head + varLength - 1;
                }
            }

            if(tail <= maxLength) // �ҵ��ɷ���Ŀռ�
            {
                varOffset = head;
            }

            return varOffset;
        }

        /// <summary>
        /// ����Ҫ�󶨱����Ķ���
        /// </summary>
        /// <param name="bindType">������</param>
        /// <param name="bindID">�󶨶����ID</param>
        /// <param name="userName">��ǰ�û���</param>
        /// <returns>�Ƿ������ɹ�</returns>
        private bool LockVarData(string bindType, string bindID, string userName)
        {
            bool success = false;

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT ID FROM VarLock WHERE BindType = '{0}' AND BindID = '{1}'",
                    bindType, bindID);
                DataTable lockTable = GetDataTable(sqlString);
                if(lockTable.Rows.Count == 0) // û����������ǰ���󣬿�ʼ����
                {
                    sqlString = string.Format("INSERT INTO VarLock (BindType, BindID, LockUser) VALUES ('{0}', '{1}', '{2}')",
                        bindType, bindID, userName);
                    SqlCommand cmd = conn.CreateCommand();
                    cmd.CommandText = sqlString;
                    cmd.ExecuteNonQuery();
                    success = true;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("����������ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return success;
        }

        /// <summary>
        /// �����󶨵Ķ���
        /// </summary>
        /// <param name="bindType">�󶨵�����</param>
        /// <param name="bindID">�󶨶����ID</param>
        /// <param name="userName">��ǰ�û���</param>
        /// <returns>�Ƿ�����ɹ�</returns>
        private bool UnlockVarData(string bindType, string bindID, string userName)
        {
            bool success = false;

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("DELETE FROM VarLock WHERE BindType = '{0}' AND BindID = {1} AND LockUser = '{2}'",
                    bindType, bindID, userName);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
                success = true;
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڽ�������ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return success;
        }

        /// <summary>
        /// ��յ�ǰ�û��ϵ�������
        /// </summary>
        private void ClearLock()
        {
            string userName = System.Net.Dns.GetHostName();

            try
            {
                if(conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("DELETE FROM VarLock WHERE LockUser = '{0}'", userName);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                cmd.ExecuteNonQuery();
            }
            catch (SqlException ex)
            {
            	MessageBox.Show("�ڽ��������ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if(conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }
        }

        /// <summary>
        /// ˢ�����ݱ��
        /// </summary>
        /// <param name="userName">��ǰ�û���</param>
        /// <param name="varType">��������</param>
        /// <param name="bindType">������</param>
        /// <param name="bindObject">�󶨶���</param>
        private void RefreshDataGridView(string userName, string varType, string bindType, string bindObject)
        {
            List<string> conditionList = new List<string>();

            if(userName != "�����û�")
            {
                conditionList.Add(string.Format("UserName = '{0}'", userName));                
            }

            if(varType != "���б�������")
            {
                conditionList.Add(string.Format("Type = '{0}'", varType));
            }

            if(bindType != "���а�����")
            {
                conditionList.Add(string.Format("BindType = '{0}'", bindType));
            }

            if(bindObject != "���а󶨶���")
            {
                conditionList.Add(string.Format("BindObject = '{0}'", bindObject));
            }

            dataGridViewX1.Rows.Clear();
            int index = 0;
            if(conditionList.Count > 0)
            {
                string conditionString = "";
                foreach(string s in conditionList)
                {
                    conditionString += s + " AND ";
                }
                conditionString = conditionString.Substring(0, conditionString.Length - 5);   
                DataRow[] rows = table.Select(conditionString);
                foreach(DataRow row in rows)
                {
                    dataGridViewX1.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX1.Rows[index];
                    newRow.Cells["idColumn"].Value = row["ID"];
                    newRow.Cells["nameColumn"].Value = row["Name"];
                    newRow.Cells["bindTypeColumn"].Value = row["BindType"];
                    newRow.Cells["bindObjectColumn"].Value = row["BindObject"];
                    newRow.Cells["typeColumn"].Value = row["Type"];
                    newRow.Cells["userColumn"].Value = row["UserName"];
                    newRow.Cells["descriptionColumn"].Value = row["Description"];
                    index++;
                }
            }
            else
            {
                foreach(DataRow row in table.Rows)
                {
                    dataGridViewX1.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX1.Rows[index];
                    newRow.Cells["idColumn"].Value = row["ID"];
                    newRow.Cells["nameColumn"].Value = row["Name"];
                    newRow.Cells["bindTypeColumn"].Value = row["BindType"];
                    newRow.Cells["bindObjectColumn"].Value = row["BindObject"];
                    newRow.Cells["typeColumn"].Value = row["Type"];
                    newRow.Cells["userColumn"].Value = row["UserName"];
                    newRow.Cells["descriptionColumn"].Value = row["Description"];
                    index++;
                }
            }            
        }

        /// <summary>
        /// �鿴����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bView_Click(object sender, EventArgs e)
        {
            ViewForm vForm = new ViewForm(table);
            DialogResult result = vForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                Hashtable viewData = vForm.ViewData;
                string userName = viewData["userName"] as string;
                string varType = viewData["type"] as string;
                string bindType = viewData["bindType"] as string;
                string bindObject = viewData["bindObject"] as string;

                RefreshDataGridView(userName, varType, bindType, bindObject);
            }
        }

        /// <summary>
        /// ��ӱ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAdd_Click(object sender, EventArgs e)
        {
            AddForm addForm = new AddForm(conn);
            DialogResult result = addForm.ShowDialog();
            if(result == DialogResult.OK)
            {
                Hashtable newVarData = addForm.NewVarData;
                string name = newVarData["name"] as string;
                string userName = newVarData["userName"] as string;
                string varType = newVarData["type"] as string;
                string desctiption = newVarData["description"] as string;
                string bindType = newVarData["bindType"] as string;
                string bindObject = newVarData["bindObject"] as string;
                string bindID = newVarData["bindID"] as string;
                string stringLength = newVarData["stringLength"] as string;

                
                // �����󶨵Ķ���
                bool lockSuccess = LockVarData(bindType, bindID, userName);
                if(lockSuccess) // �����ɹ�
                {
                    string sqlString = "SELECT * FROM VarData ORDER BY StartIndex";
                    try
                    {
                        if (conn.State == ConnectionState.Closed)
                        {
                            conn.Open();
                        }

                        // ˢ�����ݱ��������
                        table = GetDataTable(sqlString);
                    }
                    catch (SqlException ex)
                    {
                        MessageBox.Show("�ڶ�ȡ������Ϣʱ����sql�쳣��" + ex.ToString());
                    }
                    finally
                    {
                        if (conn.State == ConnectionState.Open)
                        {
                            conn.Close();
                        }
                    }

                    // ������¼ӱ�����ƫ�ƺͳ���
                    int varLength = 0;
                    switch (varType)
                    {
                        case "����":
                            {
                                varLength = 1;
                                break;
                            }
                        case "8λ�޷�������":
                            {
                                varLength = 1;
                                break;
                            }
                        case "16λ�޷�������":
                            {
                                varLength = 2;
                                break;
                            }
                        case "32λ�޷�������":
                            {
                                varLength = 4;
                                break;
                            }
                        case "8λ�з�������":
                            {
                                varLength = 1;
                                break;
                            }
                        case "16λ�з�������":
                            {
                                varLength = 2;
                                break;
                            }
                        case "32λ�з�������":
                            {
                                varLength = 4;
                                break;
                            }
                        case "�ַ���":
                            {
                                varLength = int.Parse(stringLength) * 2;
                                break;
                            }
                    }

                    int varOffset = GetVarOffset(varLength, bindType, bindID);
                    if (varOffset >= 0) // �ҵ��ɷ���Ŀռ�
                    {
                        // �������ݿ�
                        try
                        {
                            if (conn.State == ConnectionState.Closed)
                            {
                                conn.Open();
                            }

                            sqlString = string.Format("INSERT INTO VarData (Name, UserName, Type, Description, StartIndex, Length, Version, BindID, BindType, BindObject) VALUES ('{0}', '{1}', '{2}', '{3}', '{4}', '{5}', '{6}', '{7}', '{8}', '{9}')",
                                name, userName, varType, desctiption, varOffset, varLength, 1, bindID, bindType, bindObject);
                            SqlCommand cmd = conn.CreateCommand();
                            cmd.CommandText = sqlString;
                            cmd.ExecuteNonQuery();
                            sqlString = "SELECT * FROM VarData";
                            table = GetDataTable(sqlString);
                        }
                        catch (SqlException ex)
                        {
                            MessageBox.Show("�ڸ��±�����Ϣʱ����sql�쳣��" + ex.ToString());
                        }
                        finally
                        {
                            if (conn.State == ConnectionState.Open)
                            {
                                conn.Close();
                            }
                        }

                        // �����
                        UnlockVarData(bindType, bindID, userName);

                        // ˢ������
                        RefreshDataGridView("�����û�", "���б�������", "���а�����", "���а󶨶���");
                    }
                    else
                    {
                        MessageBox.Show("�Զ������ݿ��������޷�����ռ����ǰ������", "��������",
                            MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                else
                {
                    MessageBox.Show("��ǰ�󶨶����Ѿ�����������ȴ��������������½�������", "��������",
                        MessageBoxButtons.OK, MessageBoxIcon.Information);
                }                
            }
        }

        /// <summary>
        /// ɾ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDelete_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("ȷ��Ҫɾ��ѡ�еı�����", "��������",
                MessageBoxButtons.YesNo, MessageBoxIcon.Information);
            if(result == DialogResult.Yes)
            {
                List<string> idList = new List<string>();
                foreach(DataGridViewRow row in dataGridViewX1.Rows)
                {
                    if((bool)row.Cells["checkColumn"].EditedFormattedValue)
                    {
                        idList.Add(row.Cells["idColumn"].EditedFormattedValue as string);
                    }
                }

                try
                {
                    if(conn.State == ConnectionState.Closed)
                    {
                        conn.Open();
                    }

                    string sqlString;
                    SqlCommand cmd = conn.CreateCommand();

                    foreach (string s in idList)
                    {
                        sqlString = string.Format("DELETE FROM VarData WHERE ID = '{0}'", s);
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                    }

                    
                    sqlString = "SELECT * FROM VarData";
                    table = GetDataTable(sqlString);                    
                }
                catch (SqlException ex)
                {
                    MessageBox.Show("��ɾ������ʱ����sql�쳣��" + ex.ToString());
                }
                finally
                {
                    if(conn.State == ConnectionState.Open)
                    {
                        conn.Close();
                    }
                }

                // ˢ������
                RefreshDataGridView("�����û�", "���б�������", "���а�����", "���а󶨶���");
            }
        }

        /// <summary>
        /// �޸ı���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bChange_Click(object sender, EventArgs e)
        {
            DataGridViewRow editRow = null;

            foreach(DataGridViewRow row in dataGridViewX1.Rows)
            {
                if((bool)row.Cells["checkColumn"].EditedFormattedValue)
                {
                    editRow = row;
                    break;
                }
            }

            if(editRow != null)
            {
                DataRow[] rows = table.Select(string.Format("ID = '{0}'", 
                    editRow.Cells["idColumn"].EditedFormattedValue));
                EditForm eForm = new EditForm(rows[0]);
                DialogResult result = eForm.ShowDialog();
                if(result == DialogResult.OK)
                {
                    Hashtable editData = eForm.EditData;
                    string id = editData["id"] as string;
                    string name = editData["name"] as string;
                    string description = editData["description"] as string;
                    string startIndex = editData["startIndex"] as string;
                    string length = editData["length"] as string;

                    try
                    {
                        if(conn.State == ConnectionState.Closed)
                        {
                            conn.Open();
                        }

                        string sqlString = string.Format("UPDATE VarData SET Name = '{0}', Description = '{1}', StartIndex = '{2}', Length = '{3}' WHERE ID = '{4}'",
                            name, description, startIndex, length, id);
                        SqlCommand cmd = conn.CreateCommand();
                        cmd.CommandText = sqlString;
                        cmd.ExecuteNonQuery();
                        sqlString = "SELECT * FROM VarData";
                        table = GetDataTable(sqlString);
                    }
                    catch (SqlException ex)
                    {
                        MessageBox.Show("�ڸ��±�����Ϣʱ����sql�쳣��" + ex.ToString());
                    }
                    finally
                    {
                        if(conn.State == ConnectionState.Open)
                        {
                            conn.Close();
                        }
                    }

                    // ˢ������
                    RefreshDataGridView("�����û�", "���б�������", "���а�����", "���а󶨶���");
                }
            }
            else
            {
                MessageBox.Show("����ѡ��Ҫ�޸ĵ������У�", "��������",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
            }            
        }

        /// <summary>
        /// �鿴�����������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bViewSpace_Click(object sender, EventArgs e)
        {
            SpaceForm sForm = new SpaceForm(table);
            sForm.ShowDialog();
        }
    }
}