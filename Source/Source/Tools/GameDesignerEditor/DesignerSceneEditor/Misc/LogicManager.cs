using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;
using LuaInterface;

namespace DesignerSceneEditor
{
	class LogicManager
	{
        private static LogicManager logicManager; // �߼�����������
        private string newValue; // ��ֵ
        private string rootPath; // �ͻ��˸�Ŀ¼
        Lua lua; // lua�����

        /// <summary>
        /// ˽�й��캯��
        /// </summary>
        private LogicManager()
        {            
        }

        /// <summary>
        /// ��ֵ
        /// </summary>
        public string NewValue
        {
            get
            {
                return newValue;
            }
        }

        /// <summary>
        /// �ͻ��˸�Ŀ¼
        /// </summary>
        public string RootPath
        {
            set
            {
                rootPath = value;
                if(lua != null)
                {
                    lua["rootPath"] = value;
                }                
            }
        }

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            string luaFile = Path.Combine(Application.StartupPath, "ConfigEdit.lua");
            FileInfo fi = new FileInfo(luaFile);
            if(fi.Exists)
            {
                lua = new Lua();
                lua.DoFile(luaFile);
            }
        }

        /// <summary>
        /// ��ȡ�߼�������ʵ��
        /// </summary>
        /// <returns>�߼�������ʵ��</returns>
        public static LogicManager GetLogicManager()
        {
            if(logicManager == null)
            {
                logicManager = new LogicManager();
                logicManager.Init();
            }

            return logicManager;
        }

        /// <summary>
        /// �Զ���༭
        /// </summary>
        /// <param name="modelName">ģ����</param>
        /// <param name="propertyName">������</param>
        /// <param name="oldValue">��ֵ</param>
        /// <returns>�Ƿ�༭�ɹ�</returns>
        public bool ConfigEdit(string modelName, string propertyName, string oldValue)
        {
            bool success = false;

            if(lua != null)
            {
                LuaFunction function = lua.GetFunction("ConfigEdit");
                object[] results = function.Call(modelName, propertyName, oldValue);
                if(results != null && results.Length > 1)
                {
                    success = (bool)results[0];
                    if (success)
                    {
                        newValue = results[1].ToString();
                    }                    
                }
            }
            
            return success;
        }

        /// <summary>
        /// �Զ������
        /// </summary>
        /// <param name="modelName">ģ����</param>
        /// <returns>�༭�ֶ�����</returns>
        public List<string> ConfigLoad(string modelName)
        {
            List<string> fieldList = new List<string>();

            if(lua != null)
            {
                LuaFunction function = lua.GetFunction("ConfigLoad");
                object[] results = function.Call(modelName);
                if(results != null && results.Length > 0 && results[0] != null)
                {
                    string[] fields = (results[0] as string).Split(new char[] { ',' });
                    fieldList.AddRange(fields);
                }
            }

            return fieldList;
        }
	}
}
