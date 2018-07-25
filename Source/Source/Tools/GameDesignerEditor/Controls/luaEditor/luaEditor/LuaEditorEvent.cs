using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.IO;
using System.Collections;

namespace luaEditor
{
    /// <summary>
    /// �¼��Ĳ������Ų������Ͳ�������
    /// ps:Ҳ����������� :D  ���ڴ������Ѿ��������������ٹ��ˣ�����ʵ���᲻���ع�������
    /// ps:�����������������;�����ˣ���������ʲô�����ˡ�ֱ�Ӿ��ǹ����ϵĸ��ֶ��󡣰����߼����󣬼ٶ���
    /// </summary>
    public class LuaEditorParameter
    {
        public LuaEditorParameter(string name, Type type, MethodInfo mi, string strNS)
        {
            this.parm_name = name;
            this.parm_type = type;
            this.mi = mi;
            this.parm_ns = strNS;
        }

        public LuaEditorParameter(string name, string parm_classname, string strNS)
        {
            this.parm_name = name;
            this.parm_type = null;
            this.mi = null;
            this.parm_classname = parm_classname;
            this.parm_ns = strNS;
        }

        public LuaEditorParameter(string name, Type type, string strNS)
        {
            this.parm_name = name;
            this.parm_type = type;
            this.mi = null;
            this.parm_ns = strNS;
        }

        public override string ToString()
        {
            return (string.Format("{0}({1})", parm_name,parm_ns));
        }

        public string parm_name;        //����
        public Type parm_type;          //����
        public MethodInfo mi;           //������Ϣ
        public string parm_ns;          //���ƿռ�
        public string parm_classname;   //רΪluanet.import_typeʹ�ã���š�VisualEditor.Doit��֮���
    };

    /// <summary>
    /// �¼������¼������Լ������б�����һЩ˵����Ϣ
    /// </summary>
    public class LuaEditorEvent
    {
        public string eventName;
        public List<LuaEditorParameter> parms;
        public string info;

        public LuaEditorEvent(string eventName, List<LuaEditorParameter> parms, string info)
        {
            this.eventName = eventName;
            this.parms = parms;
            this.info = info;
        }

        public LuaEditorEvent(string eventName, List<LuaEditorParameter> parms)
        {
            this.eventName = eventName;
            this.parms = parms;
        }

        public override string ToString()
        {
            string sret = eventName;
            string sparm = "";
            foreach(LuaEditorParameter lp in parms)
            {
                sparm += lp.parm_name + ",";
            }
            if(sparm != "" )
            {
                sparm = "(" + sparm.Substring(0, sparm.Length - 1) + ")";
            }
            sret += sparm;
            return (sret);
        }
    }

    public class leEventConnection
    {
        public List<LuaEditorEvent> m_list = new List<LuaEditorEvent>();
    };

    public static class ParameterCache
    {
        //���в����Ļ��棬����Ҫ
        public static List<LuaEditorParameter> leParm_Cache = new List<LuaEditorParameter>();

        //����load������asm�Ļ��漯��key: string ����  value: asm����
        public static Hashtable asmCache = new Hashtable();

        //���캯������ʱ��û�뵽Ҫ��ʲô
        static ParameterCache()
        {

        }

        /// <summary>
        /// ������ĺ���
        /// </summary>
        /// <param name="e"></param>
        /// <param name="funcName"></param>
        /// <param name="strInfo"></param>
        /// <param name="leps"></param>
        /// 
        public static void regEvent(QueryEventsArgs e, string funcName, string strInfo)
        {
            List<LuaEditorParameter> tlist = new List<LuaEditorParameter>();
            e.leec.m_list.Add(new LuaEditorEvent(funcName, tlist, strInfo));
        }
        public static void regEvent(QueryEventsArgs e, string funcName, string strInfo, LuaEditorParameter lep1)
        {
            List<LuaEditorParameter> tlist = new List<LuaEditorParameter>();
            tlist.Add(lep1);
            e.leec.m_list.Add(new LuaEditorEvent(funcName, tlist, strInfo));
        }
        public static void regEvent(QueryEventsArgs e, string funcName, string strInfo, LuaEditorParameter lep1, LuaEditorParameter lep2)
        {
            List<LuaEditorParameter> tlist = new List<LuaEditorParameter>();
            tlist.Add(lep1);
            tlist.Add(lep2);
            e.leec.m_list.Add(new LuaEditorEvent(funcName, tlist, strInfo));
        }
        public static void regEvent(QueryEventsArgs e, string funcName, string strInfo, LuaEditorParameter lep1, LuaEditorParameter lep2, LuaEditorParameter lep3)
        {
            List<LuaEditorParameter> tlist = new List<LuaEditorParameter>();
            tlist.Add(lep1);
            tlist.Add(lep2);
            tlist.Add(lep3);
            e.leec.m_list.Add(new LuaEditorEvent(funcName, tlist, strInfo));
        }
        public static void regEvent(QueryEventsArgs e, string funcName, string strInfo, LuaEditorParameter lep1, LuaEditorParameter lep2, LuaEditorParameter lep3, LuaEditorParameter lep4)
        {
            List<LuaEditorParameter> tlist = new List<LuaEditorParameter>();
            tlist.Add(lep1);
            tlist.Add(lep2);
            tlist.Add(lep3);
            tlist.Add(lep4);
            e.leec.m_list.Add(new LuaEditorEvent(funcName, tlist, strInfo));
        }

        /// <summary>
        /// ��ȡ���з�������ȡ��ͬʱ�����ȡ���ķ�����
        /// </summary>
        /// <param name="strFullName">�������֣�����prop.ID</param>
        /// <returns>�������ַ����б���methodname+method   �� funcname+var</returns>
        public static List<string> findMethodInfoByFullName(string strFullName, string strNs)
        {
            List<string> ret = new List<string>();

            //�ж����������
            //if(strFullName.Length>2 && strFullName.Substring(strFullName.Length-2) == "[]")
            //{
            //    strFullName = strFullName.Substring(0, strFullName.Length);
            //    haskou = true;
            //}

            for (int i = 0; i < leParm_Cache.Count; i++)
            {
                LuaEditorParameter lp = leParm_Cache[i];
                if (lp.parm_name == strFullName && (lp.parm_ns == strNs || lp.parm_ns == ""))
                {
                    if (lp.parm_type == null)
                    //���ֿ��ܣ�1���������� ���������ˣ���ȻʲôҲû�С� 
                    //          2��luanet.import_type()�� �����������ֵ������δ֪
                    {
                        if (lp.parm_classname == "" || lp.parm_classname == null)        //��һ�ֿ���Ӧ����
                            return (ret);
                        if (!asmCache.ContainsKey(lp.parm_classname))                   //���ǣ��Ҳ�������
                            return (ret);
                        Assembly asm = (Assembly)asmCache[lp.parm_classname];
                        lp.parm_type = asm.GetType(lp.parm_classname);
                    }

                    //��ȡ�����б�
                    MethodInfo[] ami = lp.parm_type.GetMethods();
                    foreach (MethodInfo mi in ami)
                    {
                        if (mi.Name.Length > 4)
                        {
                            string head = mi.Name.Substring(0, 4);
                            if (head == "get_" || head == "set_")
                                continue;
                        }

                        //�����ŵ����
                        MethodInfo tmi = mi.ReturnType.GetMethod("get_Item", new Type[1] { typeof(int) });
                        if (tmi == null) 
                            tmi = mi.ReturnType.GetMethod("Get", new Type[1] { typeof(int) });
                        if (tmi != null)
                        {
                            if (!hasItem(strFullName + ":" + mi.Name + "()[]", lp.parm_ns))
                                leParm_Cache.Add(new LuaEditorParameter(strFullName + ":" + mi.Name + "()[]", tmi.ReturnType, lp.parm_ns));
                        }

                        ret.Add(mi.Name + "+method");
                        if (!hasItem(strFullName + ":" + mi.Name + "()", lp.parm_ns, mi.ToString()))
                            leParm_Cache.Add(new LuaEditorParameter(strFullName + ":" + mi.Name + "()", mi.ReturnType, mi, lp.parm_ns));
                        if (!hasItem(strFullName + ":" + mi.Name, lp.parm_ns, mi.ToString()))
                            leParm_Cache.Add(new LuaEditorParameter(strFullName + ":" + mi.Name, null, mi, lp.parm_ns));
                    }

                    //��ȡ�����б�
                    PropertyInfo[] api = lp.parm_type.GetProperties();
                    foreach (PropertyInfo pi in api)
                    {

                        //�����ŵ����
                        MethodInfo tmi = pi.PropertyType.GetMethod("get_Item", new Type[1] { typeof(int) });
                        if (tmi == null) 
                            tmi = pi.PropertyType.GetMethod("Get", new Type[1] { typeof(int) });
                        if (tmi != null)
                        {
                            if (!hasItem(strFullName + ":" + pi.Name + "[]", lp.parm_ns))
                                leParm_Cache.Add(new LuaEditorParameter(strFullName + "." + pi.Name + "[]", tmi.ReturnType, lp.parm_ns));
                        }

                        ret.Add(pi.Name + "+table");
                        if (!hasItem(strFullName + ":" + pi.Name, lp.parm_ns))
                            leParm_Cache.Add(new LuaEditorParameter(strFullName + "." + pi.Name, pi.PropertyType, lp.parm_ns));
                    }

                    //��ȡ�����б�
                    FieldInfo[] afi = lp.parm_type.GetFields();
                    foreach (FieldInfo fi in afi)
                    {
                        //�����ŵ����
                        MethodInfo tmi = fi.FieldType.GetMethod("get_Item", new Type[1] { typeof(int) });
                        if (tmi == null) 
                            tmi = fi.FieldType.GetMethod("Get", new Type[1] { typeof(int) });
                        if (tmi != null)
                        {
                            if (!hasItem(strFullName + ":" + fi.Name + "[]", lp.parm_ns))
                                leParm_Cache.Add(new LuaEditorParameter(strFullName + "." + fi.Name + "[]", tmi.ReturnType, lp.parm_ns));
                        }

                        ret.Add(fi.Name + "+var");
                        if (!hasItem(strFullName + ":" + fi.Name, lp.parm_ns))
                            leParm_Cache.Add(new LuaEditorParameter(strFullName + "." + fi.Name, fi.FieldType, lp.parm_ns));
                    }
                    break; 
                }
            }
            return (ret);
        }

        /// <summary>
        /// ����������
        /// ���磺 c.value = b.width.tostring()
        /// b.width.tostring()����֪���ͣ�Ҳ����objold
        /// c.value ��δ֪���ͣ�����ͳ�����֪���� :)
        /// </summary>
        /// <param name="objnew"></param>
        /// <param name="nsnew"></param>
        /// <param name="objold"></param>
        /// <param name="nsold"></param>
        public static void bindTwoObjects(string objnew,string nsnew, string objold, string nsold)
        {
            //�ɵ�û�У�������
            if(!hasItem(objold, nsold, true))
            {
                return;
            }

            //�µ����ˣ�Ҳ���ˣ���ʵ��Ӧ�ø��ǵģ���ʱ����һ�£�
            if (hasItem(objnew, nsnew, true))
            {
                return;
            }

            //���Ҳ��bug��  a = func,   a. ��ʱ�򣬳�����func�����أ���Ϊ���ҵ�һ����break�ˡ����������Ǿ仰������һ�� 
            for (int i = 0; i < leParm_Cache.Count; i++)
            {
                LuaEditorParameter lp = leParm_Cache[i];
                if (lp.parm_name == objold && (lp.parm_ns == nsold || lp.parm_ns == ""))
                {
                    LuaEditorParameter tlp = new LuaEditorParameter(objnew, lp.parm_type, lp.mi, nsnew);
                    tlp.parm_classname = lp.parm_classname;
                    leParm_Cache.Add(tlp);
                    break;
                }
            }
        }


        /// <summary>
        /// ͨ�������ú����Ĳ����������صģ���%newline%��Ϊ�ָ���
        /// </summary>
        /// <param name="strFullName">��txt1.Value.Split</param>
        /// <returns></returns>
        public static string FindMethodParameterByFullName(string strFullName, string strNs)
        {
            string ret = "";
            for (int i = 0; i < leParm_Cache.Count; i++)
            {
                LuaEditorParameter lp = leParm_Cache[i];
                if (lp.parm_name == strFullName && (lp.parm_ns == strNs || lp.parm_ns == "") &&  lp.parm_classname != "")
                {
                    //����cls()֮��Ķ����Ĺ��캯���Ĳ���
                    //Ԥ������������
                }

                if (lp.parm_name == strFullName && lp.mi != null && (lp.parm_ns == strNs || lp.parm_ns == "") && lp.parm_type == null)
                {
                    //��ȡ�����б�
                    ParameterInfo[] api = lp.mi.GetParameters();
                    string p = "";
                    foreach (ParameterInfo pi in api)
                    {
                        p +=  pi.ToString() + ",";
                    }
                    if (p != "") p = p.Substring(0, p.Length - 1);
                    string com = lp.mi.ReturnType.FullName + " " + lp.parm_name + "("  + p + ")";
                    ret += com + "\r\n<%newline%>";
                }
            }
            return (ret);
        }

        /// <summary>
        /// ��ȡ���ܸ�֪�б�
        /// </summary>
        /// <param name="key">ǰ������ĸ</param>
        /// <returns></returns>
        public static List<String> GetIntelliSenceWordList(String key, string strNs)
        {
            List<String> ret = new List<string>();
            foreach (LuaEditorParameter lp in leParm_Cache)
            {
                if (lp.parm_name == "") 
                    continue;
                if(lp.parm_ns == strNs || lp.parm_ns == "")
                {
                    char[] asp = { ':', '(', '.' };
                    string strf = lp.parm_name.Split(asp)[0];
                    string bstrf = "|" + strf.ToLower();
                    if (bstrf.IndexOf("|" + key.ToLower()) != -1)
                    {
                        if (!ret.Contains(strf))
                            ret.Add(strf);
                    }
                }
            }
            List<string> stdlst = StandardLib.getIntelliSenceWordList();
            foreach (string lp in stdlst)
            {
                if (lp == "") continue;
                string bstrf = "|" + lp.ToLower();
                if (bstrf.IndexOf("|" + key.ToLower()) != -1)
                {
                    if (!ret.Contains(lp))
                        ret.Add(lp);
                }
            }
            return (ret);
        }

        /// <summary>
        /// �ж��Ƿ񻺴��а���ĳ��
        /// </summary>
        /// <param name="leParm_name">����</param>
        /// <param name="strns">�ռ�</param>
        /// <param name="igNs">�������������ʾ���Կ������ռ�</param>
        /// <returns></returns>
        public static bool hasItem(string leParm_name, string strns, bool igNs)
        {
            foreach (LuaEditorParameter lp in leParm_Cache)
            {
                if (lp.parm_name == leParm_name && (lp.parm_ns == strns || lp.parm_ns == ""))
                    return (true);
            }
            return (false);
        }

        /// <summary>
        /// �ж��Ƿ񻺴��а���ĳ��
        /// </summary>
        /// <param name="leParm_name">����</param>
        /// <param name="strns">�ռ�</param>
        /// <returns></returns>
        public static bool hasItem(string leParm_name, string strns)
        {
            foreach (LuaEditorParameter lp in leParm_Cache)
            {
                if (lp.parm_name == leParm_name && lp.parm_ns == strns)
                    return (true);
            }
            return (false);
        }

        /// <summary>
        /// �ж��Ƿ񻺴��а���ĳ��
        /// </summary>
        /// <param name="leParm_name">����</param>
        /// <param name="strns">�ռ�</param>
        /// <param name="strmi">�����ִ�</param>
        /// <returns></returns>
        public static bool hasItem(string leParm_name, string strns, string strmi)
        {
            foreach(LuaEditorParameter lp in leParm_Cache )
            {
                if (lp.parm_name == leParm_name && lp.mi.ToString() == strmi && lp.parm_ns == strns)
                    return (true);
            }
            return (false);
        }

        /// <summary>
        /// ר����luanet.load_assembly()�����غ󱣴���asmCache��
        /// </summary>
        /// <param name="dllname">dll����</param>
        public static void loadDll(string dllname)
        {
            foreach( string fullname in Directory.GetFiles(Directory.GetCurrentDirectory(),dllname) )
            {
                Assembly asm =  Assembly.LoadFrom(fullname);
                foreach(Type t in asm.GetTypes())
                {
                    if(!asmCache.ContainsKey(t.FullName))
                        asmCache.Add(t.FullName, asm);
                }
                break;
            }
        }
    };
}