using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Data.SqlClient;

namespace StandardNpcCreate
{
    public partial class WeaponSelectForm : Form
    {
        public enum WeaponType { LeftHand, RightHand };
        private WeaponType weaponType;
        private string sourceID;
        private string weaponFilePath;
        private string currentBoneNumber;
        private string folderPath;
        private SqlConnection conn;

        private DataTable boneToActionTable;
        private DataTable itemForInsertTable;
        private DataTable modelTable;
        private Hashtable boneNumberTable = new Hashtable();

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">���ݿ�����</param>
        /// <param name="boneToActionTable">���ݱ�</param>
        /// <param name="itemForInsertTable">���ݱ�</param>
        /// <param name="weaponType">��������</param>
        /// <param name="currentBoneNumber">BoneNumber</param>
        /// <param name="itemForce">��������</param>
        public WeaponSelectForm(SqlConnection conn, DataTable boneToActionTable, DataTable itemForInsertTable, DataTable modelTable, WeaponType weaponType, string currentBoneNumber, string sourceID)
        {
            this.conn = conn;
            this.boneToActionTable = boneToActionTable;
            this.itemForInsertTable = itemForInsertTable;
            this.modelTable = modelTable;
            this.weaponType = weaponType;
            this.sourceID = sourceID;
            this.currentBoneNumber = currentBoneNumber;
            InitializeComponent();
            Init();
        }

        /// <summary>
        /// �����ļ�·��
        /// </summary>
        public string WeaponFilePath
        {
            get
            {
                return weaponFilePath;
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ��ʼ��������
            DataGridViewTextBoxColumn itemNumberColumn = new DataGridViewTextBoxColumn();
            itemNumberColumn.Name = "ItemNumber";
            itemNumberColumn.HeaderText = "ItemNumber";

            DataGridViewTextBoxColumn itemTypeColumn = new DataGridViewTextBoxColumn();
            itemTypeColumn.Name = "ItemType";
            itemTypeColumn.HeaderText = "��������";

            dataGridViewX1.Columns.Add(itemNumberColumn);
            dataGridViewX1.Columns.Add(itemTypeColumn);

            // ��ʼ������
            foreach (DataRow dataRow in boneToActionTable.Rows)
            {
                string boneNumber = dataRow["BoneNumber"] as string;
                string detailType = dataRow["DetailType"] as string;
                string weaponType = dataRow["WeaponType"] as string;

                if (!string.IsNullOrEmpty(boneNumber) && !string.IsNullOrEmpty(detailType))
                {
                    if (boneNumber == currentBoneNumber || string.IsNullOrEmpty(currentBoneNumber))
                    {
                        string displayValue = string.Format("{0} [{1}]    {2}", boneNumber, weaponType, detailType);
                        listBox1.Items.Add(displayValue);
                    }                    
                }
            }

            // ��ʼ��ͼƬ�ļ���·��
            folderPath = Path.Combine(Application.StartupPath, @"Design\WeaponImage");
        }

        /// <summary>
        /// ��ȡ��ʾ��Ϣ
        /// </summary>
        /// <param name="displayValue">��ʾ�ַ���</param>
        /// <returns>��ʾ��Ϣ����</returns>
        private string[] GetDisplayInfo(string displayValue)
        {
            string[] displayInfo = new string[3];

            int index = displayValue.IndexOf("[");
            displayInfo[0] = displayValue.Substring(0, index - 1);
            string tempString = displayValue.Substring(index + 1, displayValue.Length - index - 1);

            index = tempString.IndexOf("]");
            displayInfo[1] = tempString.Substring(0, index);
            tempString = tempString.Substring(index + 1, tempString.Length - index - 1);

            displayInfo[2] = tempString.Trim();

            return displayInfo;
        }

        /// <summary>
        /// ��ȡ�����ļ�·��
        /// </summary>
        /// <param name="itemNumber">ItemNumber</param>
        /// <returns>�����ļ�·��</returns>
        private string GetWeaponFilePath(string itemNumber)
        {
            string filePath = null;
            string relrevatePath; // ���·��

            if (itemNumber.StartsWith("RH_"))
            {
                string tempString = itemNumber.Replace("RH_", "");
                int tempIndex = tempString.IndexOf("_");
                tempString = tempString.Substring(0, tempIndex);
                relrevatePath = string.Format(@"data\source\item\weapon\{0}", tempString);
            }
            else if (itemNumber.StartsWith("LH_"))
            {
                string tempString = itemNumber.Replace("LH_", "");
                int tempIndex = tempString.IndexOf("_");
                tempString = tempString.Substring(0, tempIndex);
                relrevatePath = string.Format(@"data\source\item\weapon\{0}", tempString);
            }
            else
            {
                relrevatePath = @"data\source\item\prop";
            }

            filePath = string.Format(@"{0}\{1}.Mesh", relrevatePath, itemNumber);

            return filePath;
        }

        /// <summary>
        /// ��ȡ����ID
        /// </summary>
        /// <returns>����ID</returns>
        private string GetForceID()
        {
            string forceID = "";

            DataRow[] rows = modelTable.Select(string.Format("SourceID = '{0}'", sourceID));
            if (rows.Length > 0)
            {
                string forceName = rows[0]["Force"] as string;

                switch(forceName)
                {
                    case "����":
                        {
                            forceID = "1";
                            break;
                        }
                    case "����":
                        {
                            forceID = "2";
                            break;
                        }
                    case "����":
                        {
                            forceID = "3";
                            break;
                        }
                    case "����":
                        {
                            forceID = "4";
                            break;
                        }
                    case "���":
                        {
                            forceID = "5";
                            break;
                        }
                    case "�嶾":
                        {
                            forceID = "6";
                            break;
                        }
                    case "����":
                        {
                            forceID = "7";
                            break;
                        }
                    case "��":
                        {
                            forceID = "8";
                            break;
                        }
                    case "�ؽ�":
                        {
                            forceID = "9";
                            break;
                        }
                    case "����":
                        {
                            forceID = "10";
                            break;
                        }
                    case "�Ե�":
                        {
                            forceID = "11";
                            break;
                        }
                    case "�ھ�":
                        {
                            forceID = "12";
                            break;
                        }
                    case "���½�":
                        {
                            forceID = "13";
                            break;
                        }
                    case "��ʬ��":
                        {
                            forceID = "14";
                            break;
                        }
                    case "��߾�":
                        {
                            forceID = "15";
                            break;
                        }
                    case "ʮ��������":
                        {
                            forceID = "16";
                            break;
                        }
                    case "��һ��":
                        {
                            forceID = "17";
                            break;
                        }
                    case "����":
                        {
                            forceID = "18";
                            break;
                        }
                    case "����":
                        {
                            forceID = "19";
                            break;
                        }
                    case "�Ѿ�":
                        {
                            forceID = "20";
                            break;
                        }
                    case "���˹�":
                        {
                            forceID = "21";
                            break;
                        }
                    case "������":
                        {
                            forceID = "22";
                            break;
                        }
                    case "һ����":
                        {
                            forceID = "23";
                            break;
                        }
                    case "������":
                        {
                            forceID = "24";
                            break;
                        }
                    case "��Ԫ��":
                        {
                            forceID = "25";
                            break;
                        }
                    case "ؤ��":
                        {
                            forceID = "26";
                            break;
                        }
                    case "������":
                        {
                            forceID = "28";
                            break;
                        }
                    case "����":
                        {
                            forceID = "29";
                            break;
                        }
                    case "���͵�":
                        {
                            forceID = "30";
                            break;
                        }
                    case "�̻�":
                        {
                            forceID = "31";
                            break;
                        }
                    case "ͭǮ��":
                        {
                            forceID = "32";
                            break;
                        }
                    case "����":
                        {
                            forceID = "33";
                            break;
                        }
                    case "����":
                        {
                            forceID = "34";
                            break;
                        }
                    case "����":
                        {
                            forceID = "35";
                            break;
                        }
                    case "���":
                        {
                            forceID = "36";
                            break;
                        }
                    case "������":
                        {
                            forceID = "37";
                            break;
                        }
                    case "����":
                        {
                            forceID = "38";
                            break;
                        }
                    case "����":
                        {
                            forceID = "39";
                            break;
                        }
                }
            }

            return forceID;
        }

        /// <summary>
        /// �Ƚ�����
        /// </summary>
        /// <param name="forceValue">�����ֶ�ֵ</param>
        /// <param name="forceID">����ID</param>
        /// <returns>�����Ƿ��������</returns>
        private bool CompareForce(string forceValue, string forceID)
        {
            bool forceEqual = false;

            if (forceValue == "")
            {
                forceEqual = true;
            }
            else
            {
                string[] tempArray = forceValue.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                foreach(string s in tempArray)
                {
                    if (s == forceID)
                    {
                        forceEqual = true;
                        break;
                    }
                }
            }

            return forceEqual;
        }

        /// <summary>
        /// ��ȡItemType
        /// </summary>
        /// <param name="displayWeaponType">���������ַ���</param>
        /// <returns>ItemType</returns>
        private string GetItemType(string displayWeaponType)
        {
            string itemType = null;

            switch (displayWeaponType)
            {
                case "����":
                    {
                        itemType = "1";
                        break;
                    }
                case "����":
                    {
                        itemType = "2";
                        break;
                    }
                case "����":
                    {
                        itemType = "3";
                        break;
                    }
                case "ȭ��":
                    {
                        itemType = "4";
                        break;
                    }
                case "��":
                    {
                        itemType = "5";
                        break;
                    }
                case "����":
                    {
                        itemType = "6";
                        break;
                    }
                case "����":
                    {
                        itemType = "7";
                        break;
                    }
                default:
                    {
                        itemType = displayWeaponType;
                        break;
                    }
            }

            return itemType;
        }

        /// <summary>
        /// ��ȡ��������������
        /// </summary>
        /// <param name="itemNumber">ItemNumber</param>
        /// <returns>��������������</returns>
        private string GetWeaponDescription(string itemNumber)
        {
            string description = "";

            DataRow[] rows = itemForInsertTable.Select(string.Format("ItemNumber = '{0}'", itemNumber));
            if (rows.Length > 0)
            {
                description = rows[0]["requirement"].ToString();
            }

            return description;
        }

        /// <summary>
        /// ��ȡ������Ӧ��ͼƬ
        /// </summary>
        /// <param name="itemNumber">ItemNumber</param>
        /// <returns>������Ӧ��ͼƬ</returns>
        private Image GetWeaponImage(string itemNumber)
        {
            Image image = null;

            try
            {
                if (conn.State == ConnectionState.Closed)
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT Image FROM WeaponImage WHERE ItemNumber = '{0}'", itemNumber);
                SqlCommand cmd = conn.CreateCommand();
                cmd.CommandText = sqlString;
                object executeResult = cmd.ExecuteScalar();

                if (executeResult != null)
                {
                    byte[] imageData = (byte[])executeResult;
                    MemoryStream ms = new MemoryStream();
                    ms.Write(imageData, 0, imageData.Length);
                    image = Image.FromStream(ms);
                    ms.Close();
                }                
            }
            catch (Exception ex)
            {
                // MessageBox.Show("�ڶ�ȡ����ͼƬʱ����sql�쳣��" + ex.Message, "ѡ������ͼƬ", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            finally
            {
                if (conn.State == ConnectionState.Open)
                {
                    conn.Close();
                }
            }

            // ͼƬ����
            if (image != null)
            {
                int desWidth = image.Width;
                int desHeight = image.Height;

                if (desWidth > previewLabel.Width)
                {
                    desWidth = previewLabel.Width;
                }

                if (desHeight > previewLabel.Height)
                {
                    desHeight = previewLabel.Height;
                }

                if (desWidth != image.Width || desHeight != image.Height)
                {
                    Bitmap bmp = new Bitmap(desWidth, desHeight);
                    Graphics g = Graphics.FromImage(bmp);
                    g.DrawImage(image, 0, 0, desWidth, desHeight);
                    image = bmp;
                }
            }

            return image;
        }

        /// <summary>
        /// ��ȡ��������������
        /// </summary>
        /// <param name="itemType">��������</param>
        /// <returns>��������������</returns>
        private string GetWeaponDisplayType(string itemType)
        {
            string weaponDisplayType = null;

            switch (itemType)
            {
                case "1":
                    {
                        weaponDisplayType = "����";
                        break;
                    }
                case "2":
                    {
                        weaponDisplayType = "����";
                        break;
                    }
                case "3":
                    {
                        weaponDisplayType = "����";
                        break;
                    }
                case "4":
                    {
                        weaponDisplayType = "ȭ��";
                        break;
                    }
                case "5":
                    {
                        weaponDisplayType = "��";
                        break;
                    }
                case "6":
                    {
                        weaponDisplayType = "����";
                        break;
                    }
                case "7":
                    {
                        weaponDisplayType = "����";
                        break;
                    }
                default:
                    {
                        weaponDisplayType = itemType;
                        break;
                    } 
            }

            return weaponDisplayType;
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            if (dataGridViewX1.SelectedCells.Count > 0)
            {
                DataGridViewCell currentCell = dataGridViewX1.SelectedCells[0];

                if (currentCell.RowIndex >= 0)
                {
                    string itemNumber = currentCell.OwningRow.Cells["ItemNumber"].Value.ToString();
                    weaponFilePath = GetWeaponFilePath(itemNumber);
                }
                else
                {
                    weaponFilePath = "";
                }
            }
            else
            {
                weaponFilePath = "";
            }

            DialogResult = DialogResult.OK;
            this.Close();
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
        /// ѡ���ItemNumber�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = listBox1.SelectedIndex;

            if (index >= 0)
            {
                dataGridViewX1.Rows.Clear();
                string displayValue = listBox1.Items[index].ToString();
                string[] displayInfo = GetDisplayInfo(displayValue);
                string boneNumber = displayInfo[0];
                string displayWeaponType = displayInfo[1];
                string detailType = displayInfo[2];
                string itemType = GetItemType(displayWeaponType);
                string priorityField = null;

                switch (weaponType)
                {
                    case WeaponType.LeftHand:
                        {
                            priorityField = "S_LH_MeshFile";
                            break;
                        }
                    case WeaponType.RightHand:
                        {
                            priorityField = "S_RH_MeshFile";
                            break;
                        }
                }

                string sqlString = null;
                string itemForce = GetForceID();

                switch (displayWeaponType)
                {
                    case "����": // ֻ����ȭ������
                        {
                            sqlString = sqlString = string.Format("ItemType = '4' AND ([{0}] = '1' OR [{0}] = '2')", priorityField);
                            break;
                        }
                    case "˫��": // ����Ϊ�ǵ������κ�����
                        {
                            sqlString = string.Format("ItemType <> '7' AND ([{0}] = '1' OR [{0}] = '2')", priorityField);
                            break;
                        }                    
                    default:
                        {
                            sqlString = string.Format("ItemType = '{0}' AND ([{1}] = '1' OR [{1}] = '2')", itemType, priorityField);
                            break;
                        }
                }
                
                DataRow[] rows = itemForInsertTable.Select(sqlString);

                int rowIndex = 0;
                for (int i = 0; i < rows.Length; i++)
                {
                    if (CompareForce(rows[i]["ItemForce"].ToString(), itemForce))
                    {
                        dataGridViewX1.Rows.Add(1);
                        DataGridViewRow newRow = dataGridViewX1.Rows[rowIndex];
                        newRow.Cells["ItemNumber"].Value = rows[i]["ItemNumber"].ToString();
                        newRow.Cells["ItemType"].Value = GetWeaponDisplayType(rows[i]["ItemType"].ToString());
                        rowIndex++;
                    }                                       
                }

                // �������ǰ��ѡ����
                dataGridViewX1.ClearSelection();
            }
        }

        /// <summary>
        /// ѡ������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX1_CellMouseDown(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex >=0)
            {
                DataGridViewRow currentRow = dataGridViewX1.Rows[e.RowIndex];
                string itemNumber = currentRow.Cells["ItemNumber"].Value.ToString();
                previewLabel.Image = GetWeaponImage(itemNumber);
                textBoxX1.Text = GetWeaponDescription(itemNumber);
            }
        }
    }
}