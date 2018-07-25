using System;
using System.Collections.Generic;
using System.Text;

using System.Xml;
using System.Collections;

namespace FlowChartVarialbe
{
    public class FCVarialbes
    {
        string m_strTypeId;//��������

        
        XmlDataDocument m_XmlResult;//����������ϵĲ��������Ҳ��Ҫ���صĽ��
        XmlDataDocument m_XmlTemp;//Ϊ���m_XmlResult�������ĸ���


        
        public FCVarialbes(string strtypeid, string strEventTag)
        {
            m_XmlResult = new XmlDataDocument();
            m_XmlTemp = new XmlDataDocument();

            SetInnerXml(m_XmlResult, strEventTag);
            m_strTypeId = strtypeid;
        }

        /// <summary>
        /// ����һ���¼��ڵ��tag�ӽ���
        /// </summary>
        /// <param name="strEventTag"></param>
        public void AddNextEventTag(string strEventTag)
        {
            if(strEventTag == "")
            {
                return;
            }
            SetInnerXml(m_XmlTemp, strEventTag);
            Union2Xml();
        }

        /// <summary>
        /// ����Xmldoc������ΪstrInnerXml
        /// </summary>
        /// <param name="Xmldoc"></param>
        /// <param name="strInnerXml">Ϊ��ʱ��ʼ��Xmldoc</param>
        protected void SetInnerXml(XmlDataDocument Xmldoc, string strInnerXml)
        {
            if (strInnerXml == "")
            {
                strInnerXml = @"<variables></variables>";
            }
            if (Xmldoc.HasChildNodes)
            {
                Xmldoc.RemoveAll();
            }
            Xmldoc.InnerXml = strInnerXml;
        }

        /// <summary>
        /// ȡm_XmlResult, m_XmlTemp����.����ŵ�m_XmlResult
        /// </summary>
        protected void Union2Xml()
        {
            XmlNodeList nodelist = m_XmlTemp.DocumentElement.SelectNodes("variable");
            foreach (XmlNode node in nodelist)
            {
                if (!HasNode(node))
                {
                    AddNode(node);
                }
            }

        }
        /// <summary>
        /// �ж�m_XmlResult�Ƿ��нڵ�node1
        /// </summary>
        /// <param name="node1"></param>
        /// <returns></returns>
        protected bool HasNode(XmlNode node1)
        {
            if (null == node1)
            {
                return true;
            }
            string strname = node1.SelectSingleNode("name").FirstChild.Value;
            string strtypeid = node1.SelectSingleNode("typeid").FirstChild.Value;
            string strpath = string.Format("//variable[name='{0}'][typeid='{1}']", strname, strtypeid);
            XmlNode root = m_XmlResult.DocumentElement.SelectSingleNode(strpath);
            return (root != null);
        }
        /// <summary>
        /// ��m_XmlResult��ӽڵ�node1
        /// </summary>
        /// <param name="node1"></param>
        protected void AddNode(XmlNode node1)
        {
            XmlElement root = m_XmlResult.DocumentElement;
            XmlElement newVariable = m_XmlResult.CreateElement("variable");
            newVariable.InnerXml = node1.InnerXml;
            root.AppendChild(newVariable);
        }


        public string Show(ArrayList UsedVariablesList, string strOldValue, string strTypeName)
        {
            string strValue = "";
            VariableForm myform = new VariableForm(m_strTypeId, m_XmlResult, UsedVariablesList,strOldValue,strTypeName);
            if (myform.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                m_XmlResult = myform.RtnResultXml();
                strValue = myform.RtnChoosedName();
            }
            return strValue;
            
        }

        /// <summary>
        /// ����m_XmlResult��InnerXml
        /// </summary>
        /// <returns></returns>
        public string RtnResultXml()
        {
            return m_XmlResult.InnerXml;
        }
    }
}
