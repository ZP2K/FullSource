using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;

using AtlKG3DEngineProxyLib;

namespace DesignerSceneEditor
{
	public partial class ObjectsEditForm: Form
	{       
        private  _AtlObjInfo[] objectInfoArray; // ������������
        private bool beginEdit = false; // �Ƿ��Ѿ���ʼ�༭
        private Font normalTextFont = new Font("SimSun", 9, FontStyle.Regular); // ��ͨ�ı�����
        private Font boldTextFont = new Font("SimSun", 9, FontStyle.Bold); // �����ı�����
        private string currentObjectType; // ��ǰ�ı༭��������
        private BaseDoc baseDoc;

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="baseDoc">BaseDoc����</param>
        /// <param name="objectInfoArray">������������</param>
        public ObjectsEditForm(BaseDoc baseDoc, _AtlObjInfo[] objectInfoArray)
		{
            this.baseDoc = baseDoc;
            this.objectInfoArray = objectInfoArray;
			InitializeComponent();
            Init();
		}        

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            // ����������
            if(objectInfoArray.Length > 0)
            {
                _AtlObjInfo objectInfo = objectInfoArray[0];
                currentObjectType = objectInfo.strType;
               
                DataGridViewTextBoxColumn typeColumn = new DataGridViewTextBoxColumn();
                typeColumn.Name = "Type";
                typeColumn.Tag = "Type";
                typeColumn.HeaderText = "���";
                typeColumn.ReadOnly = true;

                dataGridViewX1.Columns.Add(typeColumn);                

                for(int i = 0; i < objectInfo.iFieldsCount; i++)
                {                    
                    DataGridViewTextBoxColumn newColumn = new DataGridViewTextBoxColumn();
                    newColumn.Name = objectInfo.strFields[i];
                    newColumn.Tag = objectInfo.strFields[i];
                    newColumn.HeaderText = GetPropertyDisplayName(objectInfo.strFields[i]);
                    newColumn.ReadOnly = IsPropertyReadOnly(objectInfo.strFields[i]);                    

                    dataGridViewX1.Columns.Add(newColumn);
                }                              

                // ����������
                ShowObjectData(objectInfo.strType);                          
            }                        
        }

        /// <summary>
        /// ��������Ϣת��Ϊ���ݱ�
        /// </summary>
        /// <param name="objectInfo">������Ϣ</param>
        /// <returns>���ݱ�</returns>
        private Hashtable GetInfoTable(_AtlObjInfo objectInfo)
        {
            Hashtable infoTable = new Hashtable();

            for (int i = 0; i < objectInfo.iFieldsCount; i++)
            {
                infoTable[objectInfo.strFields[i]] = objectInfo.strValues[i];
            }

            return infoTable;
        }

        /// <summary>
        /// ��������
        /// </summary>
        private void SaveData()
        {
            foreach(DataGridViewRow row in dataGridViewX1.Rows)
            {
                _AtlObjInfo objectInfo = (_AtlObjInfo)row.Tag;
                Hashtable infoTable = GetObjectValues(row);

                for(int i = 0; i < objectInfo.iFieldsCount; i++)
                {
                    string strField = objectInfo.strFields[i];
                    string strValue = objectInfo.strValues[i];
                    string newValue = infoTable[strField] as string;

                    if(newValue != strValue)
                    {
                        objectInfo.strValues[i] = newValue;
                        objectInfo.bChanged = 1;
                    }
                }
            }
        }

        /// <summary>
        /// ��ȡ������ֵ��ϣ��
        /// </summary>
        /// <param name="row">��ǰ������</param>
        /// <returns>ֵ��ϣ��</returns>
        private Hashtable GetObjectValues(DataGridViewRow row)
        {
            Hashtable infoTable = new Hashtable();
            foreach(DataGridViewCell c in row.Cells)
            {
                if(c.ColumnIndex > 0)
                {
                    infoTable[c.OwningColumn.Tag] = c.Value != null ? c.Value.ToString() : string.Empty;
                }
            }

            return infoTable;
        }

        /// <summary>
        /// ��ʾ��������
        /// </summary>
        /// <param name="objectType">��������</param>
        private void ShowObjectData(string objectType)
        {
            beginEdit = false;
            string filtValue = objectType;
            dataGridViewX1.Rows.Clear();

            int index = 0;
            for (int i = 0; i < objectInfoArray.Length; i++)
            {
                _AtlObjInfo objectInfo = objectInfoArray[i];
                Hashtable infoTable = GetInfoTable(objectInfo);

                if(objectInfo.strType == filtValue)
                {
                    dataGridViewX1.Rows.Add(1);
                    DataGridViewRow newRow = dataGridViewX1.Rows[index];                    
                    newRow.Cells["Type"].Value = objectInfo.strType;
                    newRow.Cells["Type"].Tag = objectInfo.strType;                    

                    for (int j = 0; j < objectInfo.iFieldsCount; j++)
                    {
                        string field = objectInfo.strFields[j];
                        newRow.Cells[field].Value = infoTable[field];
                        newRow.Cells[field].Tag = infoTable[field];
                    }                        
                    newRow.Tag = objectInfo;

                    index++;
                }                
            }

            beginEdit = true;
        }

        /// <summary>
        /// �ҽӽű��ļ�
        /// </summary>
        /// <param name="currentRow">��ǰ������</param>
        private void AttachScriptFile(DataGridViewRow currentRow)
        {
            string filePath = Path.Combine(Helper.RootDir, "scripts");
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = filePath;
            openFileDialog.Filter = "lua�ű��ļ�|*.lua";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string fileName = openFileDialog.FileName.Replace(Helper.RootDir, "").TrimStart(new char[] { '\\' });
                currentRow.Cells["szScriptName"].Value = fileName;
            }
        }

        /// <summary>
        /// ����NPC��AI����
        /// </summary>
        /// <param name="npcIndex">NPC�ı��</param>
        /// <param name="fieldName">������</param>
        /// <param name="fieldValue">����ֵ</param>
        private void UpdateNpcAIParameter(int npcIndex, string fieldName, int fieldValue)
        {
            baseDoc.DocLogical.UpdateNpcAIParameter(npcIndex, fieldName, fieldValue);
        }

        /// <summary>
        /// �����Ƿ�ֻ��
        /// </summary>
        /// <param name="fieldName">��������</param>
        /// <returns>�����Ƿ�ֻ��</returns>
        private bool IsPropertyReadOnly(string fieldName)
        {
            bool propertyReadOnly = false;

            switch (currentObjectType)
            {
                case "NPC":
                    {
                        switch (fieldName)
                        {
                            case "szScriptName":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                            case "nIndex":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                            case "dwScriptID":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                            case "AISetID":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                        }

                        break;
                    }
                case "Doodad":
                    {
                        switch (fieldName)
                        {
                            case "szScriptName":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                            case "nIndex":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                            case "dwScriptID":
                                {
                                    propertyReadOnly = true;
                                    break;
                                }
                        }

                        break;
                    }
            }

            return propertyReadOnly;
        }

        /// <summary>
        /// ��ȡ���Եı�������
        /// </summary>
        /// <param name="fieldName">��������</param>
        /// <returns>��������</returns>
        private string GetPropertyDisplayName(string fieldName)
        {
            string displayName = fieldName;

            switch (currentObjectType)
            {
                case "NPC":
                    {
                        switch (fieldName)
                        {
                            case "szName":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "szNickName":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "szScriptName":
                                {
                                    displayName = "�ű�";
                                    break;
                                }
                            case "nIndex":
                                {
                                    displayName = "���";
                                    break;
                                }
                            case "nTempleteID":
                                {
                                    displayName = "ģ��ID";
                                    break;
                                }
                            case "nLevel":
                                {
                                    displayName = "�ȼ�";
                                    break;
                                }
                            case "nX":
                                {
                                    displayName = "x����";
                                    break;
                                }
                            case "nY":
                                {
                                    displayName = "y����";
                                    break;
                                }
                            case "nZ":
                                {
                                    displayName = "z����";
                                    break;
                                }
                            case "nKind":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "dwScriptID":
                                {
                                    displayName = "�ű�ID";
                                    break;
                                }
                            case "nFaceDirection":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "dwRepresentID":
                                {
                                    displayName = "����ID";
                                    break;
                                }
                            case "ReliveID":
                                {
                                    displayName = "������ID";
                                    break;
                                }
                            case "RandomID":
                                {
                                    displayName = "�����ID";
                                    break;
                                }
                            case "TeamID":
                                {
                                    displayName = "�����ID";
                                    break;
                                }
                            case "AISetID":
                                {
                                    displayName = "AI��ID";
                                    break;
                                }
                            case "nProgressID":
                                {
                                    displayName = "��������ID";
                                    break;
                                }
                        }

                        break;
                    }
                case "Doodad":
                    {
                        switch (fieldName)
                        {
                            case "szName":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "szNickName":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "szScriptName":
                                {
                                    displayName = "�ű�";
                                    break;
                                }
                            case "nIndex":
                                {
                                    displayName = "���";
                                    break;
                                }
                            case "nTempleteID":
                                {
                                    displayName = "ģ��ID";
                                    break;
                                }
                            case "nLevel":
                                {
                                    displayName = "�ȼ�";
                                    break;
                                }
                            case "nX":
                                {
                                    displayName = "x����";
                                    break;
                                }
                            case "nY":
                                {
                                    displayName = "y����";
                                    break;
                                }
                            case "nZ":
                                {
                                    displayName = "z����";
                                    break;
                                }
                            case "nKind":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "dwScriptID":
                                {
                                    displayName = "�ű�ID";
                                    break;
                                }
                            case "nFaceDirection":
                                {
                                    displayName = "����";
                                    break;
                                }
                            case "dwRepresentID":
                                {
                                    displayName = "����ID";
                                    break;
                                }
                            case "ReliveID":
                                {
                                    displayName = "������ID";
                                    break;
                                }
                        }

                        break;
                    }
            }

            return displayName;
        }

        /// <summary>
        /// �༭AI����
        /// </summary>
        /// <param name="currentRow">��ǰ������</param>
        private void EditAIParameter(DataGridViewRow currentRow)
        {
            int npcIndex = int.Parse(currentRow.Cells["nIndex"].Value.ToString());
            string infoString = "";
            baseDoc.DocLogical.GetNpcAIParameterInfo(npcIndex, ref infoString);

            if (infoString != "")
            {
                Hashtable infoTable = new Hashtable();
                string[] tempArray = infoString.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

                foreach (string s in tempArray)
                {
                    string[] dataArray = s.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    infoTable[dataArray[0]] = dataArray[1];                    
                }
                
                AIParametersEditForm aForm = new AIParametersEditForm(npcIndex, infoTable, new AIParametersEditForm.UpdateAIParameter(UpdateNpcAIParameter));
                aForm.ShowDialog();
            }            
        }

        /// <summary>
        /// ѡ��ȷ��
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void buttonX1_Click(object sender, EventArgs e)
        {
            SaveData();
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
        /// ��Ԫֵ��ֵ�ı�
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (beginEdit)
            {
                DataGridViewCell currentCell = dataGridViewX1[e.ColumnIndex, e.RowIndex] as DataGridViewCell;                                

                if (e.ColumnIndex > 0)
                {
                    if (currentCell.EditedFormattedValue.ToString() == currentCell.Tag.ToString())
                    {
                        currentCell.Style.Font = normalTextFont;                        
                    }
                    else
                    {
                        currentCell.Style.Font = boldTextFont;
                    }
                }                
            }
        }

        /// <summary>
        /// �������������
        /// </summary>
        /// <param name="sender">�¼�������</param>
        /// <param name="e">�¼�����</param>
        private void dataGridViewX1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex >= 0) // �ȼ���ǲ�����Ч��������
            {
                DataGridViewRow currentRow = dataGridViewX1.Rows[e.RowIndex];

                switch (currentObjectType)
                {
                    case "NPC":
                        {
                            switch (dataGridViewX1.Columns[e.ColumnIndex].Tag as string)
                            {
                                case "szScriptName":
                                    {
                                        AttachScriptFile(currentRow);
                                        break;
                                    }
                                case "AISetID":
                                    {
                                        EditAIParameter(currentRow);
                                        break;
                                    }
                                default:
                                    {
                                        break;
                                    }
                            }

                            break;
                        }
                    case "Doodad":
                        {
                            switch (dataGridViewX1.Columns[e.ColumnIndex].Tag as string)
                            {
                                case "szScriptName":
                                    {
                                        AttachScriptFile(currentRow);
                                        break;
                                    }
                                default:
                                    {
                                        break;
                                    }
                            }

                            break;
                        }
                }
            } 
        }
	}
}