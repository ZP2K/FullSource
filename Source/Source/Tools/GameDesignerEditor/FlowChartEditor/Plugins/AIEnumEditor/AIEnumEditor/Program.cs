using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.Data;

namespace AIEnumEditor
{
    public class Program
    {
        private EnumValuesEditor enumValuesEditor; // ö��ֵ�б�༭��
        private AIChartSelector aiChartSelector; // AIͼѡ����
        private EnumValueSelector enumValueSelector; // ö��ֵѡ����
        private AIArgumentsEditor aiArgumentEditor; // AI�����༭��
        private ParameterEditor parameterEditor; // ���������༭��
        private CodeWriter codeWriter; // ����ת����

        public Program()
        {

        }

        /// <summary>
        /// ��������
        /// </summary>
        public string ParameterValue
        {
            get
            {
                return parameterEditor.NewValue;
            }
        }

        /// <summary>
        /// ö��ֵ�б�
        /// </summary>
        public string EnumValues
        {
            get
            {
                return enumValuesEditor.NewValue;
            }
        }

        /// <summary>
        /// AIͼ��ID
        /// </summary>
        public string AIChartID
        {
            get
            {
                return aiChartSelector.AIChartID;
            }
        }

        /// <summary>
        /// AIͼ��·��
        /// </summary>
        public string AIChartPath
        {
            get
            {
                return aiChartSelector.AIChartPath;
            }
        }

        /// <summary>
        /// ö��ֵ�����
        /// </summary>
        public int EnumValueIndex
        {
            get
            {
                return enumValueSelector.EnumValueIndex;
            }
        }

        /// <summary>
        /// ö��ֵ���ַ���
        /// </summary>
        public string EnumValueString
        {
            get
            {
                return enumValueSelector.EnumValueString;
            }
        }

        /// <summary>
        /// AI����
        /// </summary>
        public string AIArguments
        {
            get
            {
                return aiArgumentEditor.AIArguments;
            }
        }

        /// <summary>
        /// �༭ö��ֵ�б�
        /// </summary>
        /// <param name="oldValue">��ֵ</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool EditEnumValues(string oldValue)
        {
            bool editSuccess = false;
            
            enumValuesEditor = new EnumValuesEditor(oldValue);
            DialogResult result = enumValuesEditor.ShowDialog();

            if(result == DialogResult.OK)
            {
                editSuccess = true;
            }

            return editSuccess;
        }

        /// <summary>
        /// ѡ��AIͼ
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="id">��ǰAIͼID</param>
        /// <returns>�Ƿ�ѡ��ɹ�</returns>
        public bool SelectAIChart(SqlConnection conn, string id)
        {
            bool selected = false;

            aiChartSelector = new AIChartSelector(conn, id);
            DialogResult result = aiChartSelector.ShowDialog();

            if(result == DialogResult.OK)
            {
                selected = true;
            }

            return selected;
        }

        /// <summary>
        /// �༭AI����
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="diagramPath">����ͼ·��</param>        
        /// <param name="oldValue">��ֵ</param>
        /// <param name="filtField">���˵��ֶ�</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool EditAIParam(SqlConnection conn, string diagramPath, string oldValue, string filtField)
        {
            bool success = false;

            aiArgumentEditor = new AIArgumentsEditor(conn, diagramPath, oldValue, filtField);
            DialogResult result = aiArgumentEditor.ShowDialog();
            
            if(result == DialogResult.OK)
            {
                success = true;
            }

            return success;
        }        

        /// <summary>
        /// �༭��������
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="oldValue">��ֵ</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool EditParameter(SqlConnection conn, string oldValue)
        {
            bool success = false;

            parameterEditor = new ParameterEditor(conn, oldValue);
            DialogResult result = parameterEditor.ShowDialog();

            if(result == DialogResult.OK)
            {
                success = true;
            }

            return success;
        }

        /// <summary>
        /// ��ȡ�����ַ���
        /// </summary>
        /// <param name="diagramPath">����ͼ·��</param>
        /// <param name="value">ԭ����ֵ</param>
        /// <returns>�����ַ���</returns>
        public string GetArgumentsValue(string diagramPath, string value)
        {
            if(codeWriter == null)
            {
                codeWriter = new CodeWriter();
            }

            string newValue = codeWriter.GetArgumentsValue(diagramPath, value);
            return newValue;
        }

        /// <summary>
        /// ����AI�ļ�
        /// </summary>
        /// <param name="rootDir">�ͻ��˸�Ŀ¼</param>
        /// <param name="conn">sql����</param>
        /// <returns>�Ƿ񵼳��ɹ�</returns>
        public bool ExportAIFile(string rootDir, SqlConnection conn)
        {
            bool success = false;

            AIFileWriter aiFileWriter = new AIFileWriter(rootDir, conn);
            aiFileWriter.ExportLSFile();
            aiFileWriter.ExportCustomFunctionsFile();

            return success;
        }
    }
}
