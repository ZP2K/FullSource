using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using DevComponents.DotNetBar.Controls;
using DevComponents.DotNetBar;

namespace ArgumentsEditor
{
    public partial class OperationEditForm : Form
    {
        private enum DataType { None, Function, Expression, ExpressionEnd }; // ��������ö�� 
        private enum ExpressionState { None, Not, Function, FunctionEnd, Operator }; // ���ʽ״̬ö��
        private SqlConnection conn; // sql����
        private string oldValue; // ��ֵ
        private string value; // ���ʽֵ
        private char[] operators = new char[] { '&', '!', '|' };
        private List<char> operatorList = new List<char>(); // ����������
        private DataTable functionTable; // ��������
        private Bitmap bmp = new Bitmap(1, 1); // ��ȡGraphic�����õ���ʱλͼ
        private int lastSelectionStart = -1; // ��һ��ѡ����ַ�������ʼλ��
        private int lastSelectionLength = -1; // ��һ��ѡ����ַ����ĳ���

        /// <summary>
        /// ���캯��
        /// </summary>       
        /// <param name="conn">sql����</param>
        /// <param name="oldValue">��ֵ</param>
        public OperationEditForm(SqlConnection conn, string oldValue)
        {
            this.conn = conn;
            this.oldValue = oldValue;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// ���ʽֵ
        /// </summary>
        public string Value
        {
            get
            {
                return value;
            }
        }

        /// <summary>
        /// ��ȡ��ǰ����������
        /// </summary>
        /// <returns>��ǰ����������</returns>
        private DataType GetCurrentDataType()
        {
            DataType currentType = DataType.Function;

            int startIndex = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;
            char[] contentArray = content.ToCharArray();

            if(startIndex == 0) // ���ַ���ǰ��
            {
                return DataType.None;
            }

            if (startIndex == content.Length)
            {
                if (operatorList.Contains(contentArray[startIndex - 1]))
                {
                    lastSelectionStart = startIndex - 1;
                    lastSelectionLength = 1;
                    richTextBox1.Select(startIndex - 1, 1);
                    return DataType.ExpressionEnd;
                }
                else
                {
                    return DataType.None;
                }
            }

            if(operatorList.Contains(contentArray[startIndex - 1]))
            {
                lastSelectionStart = startIndex - 1;
                lastSelectionLength = 1;
                richTextBox1.Select(startIndex - 1, 1);
                return DataType.Expression;
            }

            if(contentArray[startIndex - 1] == ')')
            {
                return DataType.None;
            }

            if(operatorList.Contains(contentArray[startIndex]))
            {
                return DataType.None;
            }

            // ѡ�е�ǰ�ĺ���
            int leftIndex = startIndex - 1;
            char c = contentArray[leftIndex];
            while(!operatorList.Contains(c))
            {
                if(c == ')')
                {
                    break;
                }
                else
                {
                    leftIndex--;
                }
                
                if(leftIndex >= 0)
                {
                    c = contentArray[leftIndex];
                }
                else
                {
                    break;
                }
            }
            leftIndex++;

            int rightIndex = startIndex;
            c = contentArray[rightIndex];
            while(!operatorList.Contains(c))
            {
                if(c == ')')
                {
                    rightIndex++;
                    break;
                }
                else
                {
                    rightIndex++;
                }
                
                if(rightIndex < content.Length)
                {
                    c = contentArray[rightIndex];
                }
                else
                {
                    break;
                }                
            }

            rightIndex--;
            lastSelectionStart = leftIndex;
            lastSelectionLength = rightIndex - leftIndex + 1;
            richTextBox1.Select(leftIndex, rightIndex - leftIndex + 1);

            return currentType;
        }

        /// <summary>
        /// �����ʽ�Ƿ���Ч
        /// </summary>
        /// <param name="content">���ʽ</param>
        /// <returns>���ʽʱ����Ч</returns>
        private bool CheckExpressionValid(string content)
        {
            bool valid = true;
            char[] contentArray = content.ToCharArray();
            ExpressionState currentState = ExpressionState.None;

            for (int i = 0; i < content.Length; i++)
            {
                char c = contentArray[i];

                switch(c)
                {
                    case ')':
                        {
                            switch(currentState)
                            {
                                case ExpressionState.None:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Not:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Function:
                                    {
                                        currentState = ExpressionState.FunctionEnd;
                                        break;
                                    }
                                case ExpressionState.Operator:
                                    {
                                        return false;
                                    }
                                case ExpressionState.FunctionEnd:
                                    {
                                        return false;
                                    }
                            }

                            break;
                        }
                    case '!':
                        {
                            switch(currentState)
                            {
                                case ExpressionState.None:
                                    {
                                        currentState = ExpressionState.Not;
                                        break;
                                    }
                                case ExpressionState.Not:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Function:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Operator:
                                    {
                                        currentState = ExpressionState.Not;
                                        break;
                                    }
                                case ExpressionState.FunctionEnd:
                                    {
                                        return false;
                                    }
                            }

                            break;
                        }
                    case '&':
                        {
                            switch(currentState)
                            {
                                case ExpressionState.None:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Not:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Function:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Operator:
                                    {
                                        return false;
                                    }
                                case ExpressionState.FunctionEnd:
                                    {
                                        currentState = ExpressionState.Operator;
                                        break;
                                    }
                            }

                            break;
                        }
                    case '|':
                        {
                            switch (currentState)
                            {
                                case ExpressionState.None:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Not:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Function:
                                    {
                                        return false;
                                    }
                                case ExpressionState.Operator:
                                    {
                                        return false;
                                    }
                                case ExpressionState.FunctionEnd:
                                    {
                                        currentState = ExpressionState.Operator;
                                        break;
                                    }
                            }

                            break;
                        }
                    default:
                        {
                            switch(currentState)
                            {
                                case ExpressionState.None:
                                    {
                                        currentState = ExpressionState.Function;
                                        break;
                                    }
                                case ExpressionState.Not:
                                    {
                                        currentState = ExpressionState.Function;
                                        break;
                                    }
                                case ExpressionState.Function:
                                    {
                                        break;
                                    }
                                case ExpressionState.Operator:
                                    {
                                        currentState = ExpressionState.Function;
                                        break;
                                    }
                                case ExpressionState.FunctionEnd:
                                    {
                                        return false;
                                    }
                            }

                            break;
                        }
                }
            }

            if(currentState == ExpressionState.Operator || currentState == ExpressionState.Not
                || currentState == ExpressionState.Function)
            {
                valid = false;
            }

            return valid;
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            string sqlString = string.Format("SELECT * FROM expression_functions");
            functionTable = GetDataTable(sqlString);
            richTextBox1.Text = oldValue;
            operatorList.AddRange(operators);
        }

        /// <summary>
        /// ����ı������Ƿ񱻷�ѡ��
        /// </summary>
        /// <param name="point">��ǰ��</param>
        /// <returns>�ı������Ƿ񱻷�ѡ��</returns>
        private bool CheckContentUnselected(Point point)
        {
            bool unselected = false;

            if(lastSelectionLength > 0)
            {
                Rectangle rectangle = Rectangle.Empty;
                Graphics g = Graphics.FromImage(bmp);

                if(lastSelectionStart > 0)
                {
                    if(lastSelectionStart + lastSelectionLength <= richTextBox1.Text.Length) // ������Ч
                    {
                        string previousSelection = richTextBox1.Text.Substring(0, lastSelectionStart);
                        SizeF previousSelectionSize = g.MeasureString(previousSelection, richTextBox1.Font);
                        rectangle.Location = new Point((int)previousSelectionSize.Width, 0);
                        string selection = richTextBox1.Text.Substring(lastSelectionStart, lastSelectionLength);
                        SizeF selectionSize = g.MeasureString(selection, richTextBox1.Font);
                        rectangle.Size = new Size((int)selectionSize.Width, (int)selectionSize.Height);
                        if (point.X >= rectangle.Left && point.X <= rectangle.Right)
                        {
                            unselected = true;
                        }
                    }                    
                }
                else
                {
                    string selection = richTextBox1.Text.Substring(0, lastSelectionLength);
                    SizeF selectionSize = g.MeasureString(selection, richTextBox1.Font);
                    rectangle.Location = Point.Empty;
                    rectangle.Size = new Size((int)selectionSize.Width, (int)selectionSize.Height);
                    if (point.X >= rectangle.Left && point.X <= rectangle.Right)
                    {
                        unselected = true;
                    }
                }
            }

            return unselected;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sqlString">��ѯ�ַ���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sqlString)
        {
            DataTable table = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                SqlDataAdapter adp = new SqlDataAdapter(sqlString, conn);
                adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
                DataSet ds = new DataSet();
                adp.Fill(ds);
                table = ds.Tables[0];
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ���ݱ�ʱ����sql�쳣��" + ex.ToString());
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            return table;
        }

        /// <summary>
        /// �༭����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void EditFunction(object sender, EventArgs e)
        {
            FunctionEditForm eForm = new FunctionEditForm(conn);
            if(eForm.ShowDialog() == DialogResult.OK)
            {
                int index = richTextBox1.SelectionStart;
                string content = richTextBox1.Text;
                string functionString = eForm.Value;

                if(richTextBox1.SelectedText == "") // ��Ӻ���
                {
                    content = content.Insert(index, functionString);                    
                }
                else // �༭����
                {
                    content = content.Remove(index, richTextBox1.SelectedText.Length);
                    content = content.Insert(index, functionString);                    
                }

                if (CheckExpressionValid(content))
                {
                    richTextBox1.ForeColor = Color.Black;
                }
                else
                {
                    richTextBox1.ForeColor = Color.Red;
                }

                richTextBox1.Text = content;
                lastSelectionStart = index;
                lastSelectionLength = functionString.Length;
                richTextBox1.SelectionStart = index;
                richTextBox1.SelectionLength = functionString.Length;
            }
        }

        /// <summary>
        /// �ڱ��ʽĩβ��Ӻ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void AddFunctionAtEnd(object sender, EventArgs e)
        {
            FunctionEditForm eForm = new FunctionEditForm(conn);
            if (eForm.ShowDialog() == DialogResult.OK)
            {
                int index = richTextBox1.SelectionStart;
                string content = richTextBox1.Text;
                string functionString = eForm.Value;

                content = content.Insert(index + 1, functionString);
                if (CheckExpressionValid(content))
                {
                    richTextBox1.ForeColor = Color.Black;
                }
                else
                {
                    richTextBox1.ForeColor = Color.Red;
                }

                richTextBox1.Text = content;
                lastSelectionStart = index + 1;
                lastSelectionLength = functionString.Length;
                richTextBox1.SelectionStart = index + 1;
                richTextBox1.SelectionLength = functionString.Length;                
            }
        }

        /// <summary>
        /// ��ȡ�˵�λ��
        /// </summary>
        /// <returns>�˵���ʾλ��</returns>
        private Point GetMenuLocation()
        {
            Point location = Point.Empty;

            int index = richTextBox1.SelectionStart;
            if(index > 0)
            {
                Graphics g = Graphics.FromImage(bmp);
                SizeF size = g.MeasureString(richTextBox1.Text.Substring(0, index), richTextBox1.Font);
                location = PointToScreen(richTextBox1.Location + new Size((int)size.Width, (int)size.Height + 20));
            }
            else
            {
                location = PointToScreen(richTextBox1.Location + new Size(0, 30));
            }

            return location;
        }

        /// <summary>
        /// ��갴���¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void richTextBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if(CheckContentUnselected(e.Location)) // ��귴ѡ��ѡ�е����ֵ�ʱ�򲻵����Ҽ��˵�
            {
                lastSelectionStart = richTextBox1.SelectionStart;
                lastSelectionLength = 0;
            }
            else
            {
                Point location = GetMenuLocation();
                DataType currentType = GetCurrentDataType();

                switch (currentType)
                {
                    case DataType.None:
                        {
                            bAll.Popup(location);
                            break;
                        }
                    case DataType.Expression:
                        {
                            bExpression.Popup(location);
                            break;
                        }
                    case DataType.ExpressionEnd:
                        {
                            bExpressionEnd.Popup(location);
                            break;
                        }
                    case DataType.Function:
                        {
                            bFunction.Popup(location);
                            break;
                        }
                }
            }                                   
        }

        /// <summary>
        /// ���̰����¼���Ӧ
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void richTextBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;                        
        }

        /// <summary>
        /// ɾ��ѡ�еĺ���
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteFunction_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;
            content = content.Remove(richTextBox1.SelectionStart, richTextBox1.SelectionLength);
            if (CheckExpressionValid(content))
            {
                richTextBox1.ForeColor = Color.Black;
            }
            else
            {
                richTextBox1.ForeColor = Color.Red;
            }

            richTextBox1.Text = content;
            lastSelectionStart = index;
            lastSelectionLength = 0;
            richTextBox1.SelectionStart = index;
            richTextBox1.SelectionLength = 0;
        }

        /// <summary>
        /// ɾ��ѡ�еĲ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bDeleteExpression_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;
            content = content.Remove(richTextBox1.SelectionStart, richTextBox1.SelectionLength);
            if (CheckExpressionValid(content))
            {
                richTextBox1.ForeColor = Color.Black;
            }
            else
            {
                richTextBox1.ForeColor = Color.Red;
            }

            richTextBox1.Text = content;
            lastSelectionStart = index;
            lastSelectionLength = 0;
            richTextBox1.SelectionStart = index;
            richTextBox1.SelectionLength = 0;
        }

        /// <summary>
        /// ���/�༭�ǲ�����
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddNot_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;

            if (richTextBox1.SelectedText == "")
            {
                content = content.Insert(index, "!");
            }
            else
            {
                content = content.Remove(index, 1);
                content = content.Insert(index, "!");
            }

            if (CheckExpressionValid(content))
            {
                richTextBox1.ForeColor = Color.Black;
            }
            else
            {
                richTextBox1.ForeColor = Color.Red;
            }

            richTextBox1.Text = content;
            lastSelectionStart = index;
            lastSelectionLength = 1;
            richTextBox1.SelectionStart = index;
            richTextBox1.SelectionLength = 1;
        }

        /// <summary>
        /// ���/�༭�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddAnd_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;

            if (richTextBox1.SelectedText == "")
            {                
                content = content.Insert(index, "&");
            }
            else
            {
                content = content.Remove(index, 1);
                content = content.Insert(index, "&");                
            }

            if (CheckExpressionValid(content))
            {
                richTextBox1.ForeColor = Color.Black;
            }
            else
            {
                richTextBox1.ForeColor = Color.Red;
            }

            richTextBox1.Text = content;
            lastSelectionStart = index;
            lastSelectionLength = 1;
            richTextBox1.SelectionStart = index;
            richTextBox1.SelectionLength = 1;
        }

        /// <summary>
        /// ���/�༭�������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void bAddOr_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;

            if (richTextBox1.SelectedText == "")
            {                
                content = content.Insert(index, "|");
            }
            else
            {
                content = content.Remove(index, 1);
                content = content.Insert(index, "|");                
            }

            if(CheckExpressionValid(content))
            {
                richTextBox1.ForeColor = Color.Black;
            }
            else
            {
                richTextBox1.ForeColor = Color.Red;
            }

            richTextBox1.Text = content;
            lastSelectionStart = index;
            lastSelectionLength = 1;
            richTextBox1.SelectionStart = index;
            richTextBox1.SelectionLength = 1;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if(CheckExpressionValid(richTextBox1.Text))
            {
                value = richTextBox1.Text;
                DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                MessageBox.Show("���ʽ������Ч�������Ƿ����﷨����", "������",
                    MessageBoxButtons.OK, MessageBoxIcon.Information);
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
        /// ����ȡ��
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void bNotFunction_Click(object sender, EventArgs e)
        {
            int index = richTextBox1.SelectionStart;
            string content = richTextBox1.Text;
            char[] contentArray = content.ToCharArray();

            if(index > 0)
            {
                if(contentArray[index - 1] == '!')
                {
                    content = content.Remove(index - 1, 1);
                    richTextBox1.Text = content;
                    lastSelectionStart = index - 1;
                    lastSelectionLength = 0;
                    richTextBox1.SelectionStart = index - 1;
                    richTextBox1.SelectionLength = 0;
                }
                else
                {
                    content = content.Insert(index, "!");
                    richTextBox1.Text = content;
                    lastSelectionStart = index;
                    lastSelectionLength = 1;
                    richTextBox1.SelectionStart = index;
                    richTextBox1.SelectionLength = 1;
                }
            }
            else
            {
                content = content.Insert(index, "!");
                richTextBox1.Text = content;
                lastSelectionStart = index;
                lastSelectionLength = 1;
                richTextBox1.SelectionStart = index;
                richTextBox1.SelectionLength = 1;
            }
        }
    }
}