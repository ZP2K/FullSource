using System;
using System.Collections.Generic;
using System.Text;
using AIDiagramPlugin;
using DataInteractor;

namespace Compiler_Sword3_AI
{
    public static class CodeProviderClass
    {                        
        /// <summary>
        /// ��ȡAction����Ϣ
        /// </summary>
        /// <param name="data">Action������</param>
        /// <returns>Action����Ϣ</returns>
        public static string[] GetActionInformation(object data)
        {
            if (data == null || !(data is AI_SingleAction))
            {
                return null;
            }

            AI_SingleAction saction = data as AI_SingleAction;
            AI_Action action = saction.Action;

            //��ȡ����
            string strRet = "";
            foreach (AI_Parm parm in action.Args)
            {
                strRet += parm.Value.GetCode() + ", ";
            }
            strRet = strRet.TrimEnd((new char[] { ',', ' ' }));

            // ����ֵ����3���ַ�����
            // 0: Action��
            // 1: �����ַ������Ѿ���,�ָ�����
            // 2: ActionID
            // 3: ActionInfo 
            return new string[] { action.Ename.Trim(), strRet, action.ActionID, action.Info };
        }

        /// <summary>
        /// �ֽ⶯����
        /// </summary>
        /// <param name="data">�����������</param>
        /// <returns>������������</returns>
        public static List<object> SplitMultiActionIntoSingle(object data)
        {
            if (data == null || !(data is AI_MultiAction))
            {
                return null;
            }

            AI_MultiAction maction = data as AI_MultiAction;
            List<object> list = new List<object>();
            foreach (AI_SingleAction saction in maction.ActionList)
            {
                list.Add(saction);
            }

            return list;
        }

        /// <summary>
        /// ��ȡ״̬�������
        /// </summary>
        /// <param name="data">״̬�������</param>
        /// <returns>״̬�������</returns>
        public static string GetStateString(object data)
        {
            string stateString = null;

            if (data != null)
            {
                AI_State state = data as AI_State;

                if (state != null)
                {
                    stateString = state.Ename.Trim();
                }                
            }
            
            return stateString;
        }

        /// <summary>
        /// ��ȡ�¼�����
        /// </summary>
        /// <param name="data">�¼�����������</param>
        /// <returns>�¼�����</returns>
        public static string GetEventString(object data)
        {
            string eventString = null;

            if (data != null)
            {
                AI_Event evt = data as AI_Event;

                if (evt != null)
                {
                    if (int.Parse(evt.eventid) > 1000) // �Զ����¼�
                    {
                        eventString = evt.eventid;
                    }
                    else // ϵͳ�¼�
                    {                        
                        eventString = "AI_EVENT." + evt.Ename.Trim();
                    }
                }                
            }            

            return eventString;
        }

        /// <summary>
        /// ��ȡ������㷵��ֵ����
        /// </summary>
        /// <param name="data">�����������</param>
        /// <returns>����ֵ����</returns>
        public static int GetActionRetCount(object data)
        {
            int count = 0;

            if (data != null)
            {
                AI_SingleAction saction = data as AI_SingleAction;
                
                if (saction != null)
                {
                    AI_Action action = saction.Action;
                    count = action.RetList.Count;
                }                
            }
           
            return count;
        }
        
        /// <summary>
        /// ��ȡ״̬�����¼�����
        /// </summary>
        /// <param name="data">״̬�������</param>
        /// <returns>�¼�����</returns>
        public static int GetStateEvtCount(object data)
        {
            int count = 0;

            if (data != null)
            {
                AI_State state = data as AI_State;

                if (state != null)
                {
                    count = state.EventList.Count;
                }
            }            

            return count;
        }
    }
}
