using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace GameDesingerTools
{
    //add by cuiwei 07.8.23

    //��Ҫͬ������Ĳ������棬���ڴ���DBCustomClass.ModifyValue()
    class syncItem
    {
        public syncItem(string str1, string str2, object o1, string str3)
        {
            this.m_strFieldName = str2;
            this.m_strMainTableName = str1;
            this.m_strFindKeys = str3;
            this.m_oFieldValue = o1;
        }
        public string MainTableName
        {
            get { return m_strMainTableName; }
        }
        public string FieldName
        {
            get { return m_strFieldName; }
        }
        public object FieldValue
        {
            get { return m_oFieldValue; }
        }
        public string FindKeys
        {
            get { return m_strFindKeys; }
        }

        private string m_strMainTableName;  //������
        private string m_strFieldName;      //�ֶ���
        private object m_oFieldValue;       //�ֶ�ֵ
        private string m_strFindKeys;       //FindKeys
    };

    //���ڱ����֪ͨȡ�޺���
    public class unBoldItem : queryCommand
    {

    };

    //ȥ���ݿ������ݲ�ת�������ݽṹ������������棬���ڴ���SelectObject()
    public class queryCommand
    {
        public queryCommand()
        {

        }
        public queryCommand(DBCustomClass dc, object[] ao)
        {
            this.ao = ao;
            this.dc = dc;
        }
        public DBCustomClass cls = null;

        private DBCustomClass dc;
        private object[] ao;
        public ArrayList m_syncList = new ArrayList();      //��Ҫ֪ͨͬ������Ŀ�б�

        public ArrayList syncList
        {
            get { return m_syncList; }
        }
        public DBCustomClass dbClass
        {
            get { return dc; }
        }
        public object[] Args
        {
            get { return ao; }
        }
    };
    //add end
}
