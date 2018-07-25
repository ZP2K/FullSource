using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices; // �� DllImport ���ô� �����ռ� 
using System.Reflection; // ʹ�� Assembly �����ô� �����ռ� 
using System.Reflection.Emit; // ʹ�� ILGenerator ���ô� �����ռ� 
using System.Collections;
 

namespace IconBrowser
{
    /// <summary> 
    /// �������ݷ�ʽö�� ,ByValue ��ʾֵ���� ,ByRef ��ʾַ���� 
    /// </summary> 
    public enum ModePass
    {
        ByValue = 0x0001,
        ByRef = 0x0002
    } 
    /*class CacheInfo
    {
        AssemblyBuilder MyAssemblyBuilder;
        ModuleBuilder MyModuleBuilder;
        MethodBuilder MyMethodBuilder;
        ILGenerator IL;
        MethodInfo MyMethodInfo;
    };*/
 
    class DllHelper
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr LoadLibrary(string lpFileName);
        [DllImport("kernel32.dll")]
        static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);
        [DllImport("kernel32", EntryPoint = "FreeLibrary", SetLastError = true)]
        static extern bool FreeLibrary(IntPtr hModule);
        private IntPtr hModule = IntPtr.Zero;
        Hashtable m_htFuns = new Hashtable();
        object[] m_ObjArray_Parameter;

        /// <summary> 
        /// GetProcAddress ���صĺ���ָ�� 
        /// </summary> 
        private IntPtr farProc = IntPtr.Zero;
        private string curFunName;

        public void LoadDll(string lpFileName)
        {
            hModule = LoadLibrary(lpFileName);
            if (hModule == IntPtr.Zero)
                throw (new Exception(" û���ҵ� :" + lpFileName + "."));
        }
        public void LoadDll(IntPtr HMODULE)        
        {
            if (HMODULE == IntPtr.Zero)
                throw (new Exception(" ������ĺ�����ģ��ľ�� HMODULE Ϊ�� ."));
            hModule = HMODULE;
        }

        /// <summary> 
        /// ��ú���ָ�� 
        /// </summary> 
        /// <param name="lpProcName"> ���ú��������� </param> 
        public void LoadFun(string lpProcName)
        { // ��������ģ��ľ��Ϊ�գ����׳��쳣 
            if (hModule == IntPtr.Zero)
                throw (new Exception(" ������ģ��ľ��Ϊ�� , ��ȷ���ѽ��� LoadDll ���� !"));
            // ȡ�ú���ָ�� 
            farProc = GetProcAddress(hModule, lpProcName);
            //m_htFuns[lpProcName] = farProc;
            // ������ָ�룬���׳��쳣 
            if (farProc == IntPtr.Zero)
                throw (new Exception(" û���ҵ� :" + lpProcName + " �����������ڵ� "));
        }
        /// <summary> 
        /// ��ú���ָ�� 
        /// </summary> 
        /// <param name="lpFileName"> ��������ú����� DLL �ļ��� </param> 
        /// <param name="lpProcName"> ���ú��������� </param> 
        public void LoadFun(string lpFileName, string lpProcName)        
        { // ȡ�ú�����ģ��ľ�� 
            hModule = LoadLibrary(lpFileName);
            // ��������ģ��ľ��Ϊ�գ����׳��쳣 
            if (hModule == IntPtr.Zero)
                throw (new Exception(" û���ҵ� :" + lpFileName + "."));
            // ȡ�ú���ָ�� 
            farProc = GetProcAddress(hModule, lpProcName);
            // ������ָ�룬���׳��쳣 
            if (farProc == IntPtr.Zero)
                throw (new Exception(" û���ҵ� :" + lpProcName + " �����������ڵ� "));
        }
        public void UnLoadDll()
        {
            m_htFuns.Clear();
            FreeLibrary(hModule);
            hModule = IntPtr.Zero;
            farProc = IntPtr.Zero;
        }
        /// <summary> 
        /// �������趨�ĺ��� 
        /// </summary> 
        /// <param name="ObjArray_Parameter"> ʵ�� </param> 
        /// <param name="TypeArray_ParameterType"> ʵ������ </param> 
        /// <param name="ModePassArray_Parameter"> ʵ�δ��ͷ�ʽ </param> 
        /// <param name="Type_Return"> �������� </param> 
        /// <returns> ���������ú����� object</returns> 
        public object Invoke(object[] ObjArray_Parameter, Type[] TypeArray_ParameterType, ModePass[] ModePassArray_Parameter, Type Type_Return)
        {
            // ���� 3 �� if �ǽ��а�ȫ��� , ������ͨ�� , ���׳��쳣 
            if (hModule == IntPtr.Zero)
                throw (new Exception(" ������ģ��ľ��Ϊ�� , ��ȷ���ѽ��� LoadDll ���� !"));
            if (farProc == IntPtr.Zero)
                throw (new Exception(" ����ָ��Ϊ�� , ��ȷ���ѽ��� LoadFun ���� !"));
            if (ObjArray_Parameter.Length != ModePassArray_Parameter.Length)
                throw (new Exception(" �����������䴫�ݷ�ʽ�ĸ�����ƥ�� ."));
            // �����Ǵ��� MyAssemblyName ���������� Name ���� 
            AssemblyName MyAssemblyName = new AssemblyName();
            MyAssemblyName.Name = "InvokeFun";
            // ���ɵ�ģ����� 
            AssemblyBuilder MyAssemblyBuilder = AppDomain.CurrentDomain.DefineDynamicAssembly(MyAssemblyName, AssemblyBuilderAccess.Run);
            ModuleBuilder MyModuleBuilder = MyAssemblyBuilder.DefineDynamicModule("InvokeDll");
            // ����Ҫ���õķ��� , ������Ϊ�� MyFun �������������ǡ� Type_Return �����������ǡ� TypeArray_ParameterType �� 
            MethodBuilder MyMethodBuilder = MyModuleBuilder.DefineGlobalMethod("MyFun", MethodAttributes.Public | MethodAttributes.Static, Type_Return, TypeArray_ParameterType);
            // ��ȡһ�� ILGenerator �����ڷ�������� IL 
            ILGenerator IL = MyMethodBuilder.GetILGenerator();
            int i;
            for (i = 0; i < ObjArray_Parameter.Length; i++)
            {// ��ѭ������������ѹ���ջ 
                switch (ModePassArray_Parameter[i])
                {
                    case ModePass.ByValue:
                        IL.Emit(OpCodes.Ldarg, i);
                        break;
                    case ModePass.ByRef:
                        IL.Emit(OpCodes.Ldarga, i);
                        break;
                    default:
                        throw (new Exception(" �� " + (i + 1).ToString() + " ������û�и�����ȷ�Ĵ��ݷ�ʽ ."));
                }
            }
            if (IntPtr.Size == 4)
            {// �жϴ���������
                IL.Emit(OpCodes.Ldc_I4, farProc.ToInt32());
            }
            else if (IntPtr.Size == 8)
            {
                IL.Emit(OpCodes.Ldc_I8, farProc.ToInt64());
            }
            else            
            {
                throw new PlatformNotSupportedException();
            }
            IL.EmitCalli(OpCodes.Calli, CallingConvention.StdCall, Type_Return, TypeArray_ParameterType);
            IL.Emit(OpCodes.Ret); // ����ֵ 
            MyModuleBuilder.CreateGlobalFunctions();
            // ȡ�÷�����Ϣ 
            MethodInfo MyMethodInfo = MyModuleBuilder.GetMethod("MyFun");
            m_htFuns[curFunName] = MyMethodInfo;
            return MyMethodInfo.Invoke(null, ObjArray_Parameter);// ���÷�������������ֵ 
        }        
        /// <summary> 
        /// �������趨�ĺ��� 
        /// </summary> 
        /// <param name="IntPtr_Function"> ����ָ�� </param> 
        /// <param name="ObjArray_Parameter"> ʵ�� </param> 
        /// <param name="TypeArray_ParameterType"> ʵ������ </param> 
        /// <param name="ModePassArray_Parameter"> ʵ�δ��ͷ�ʽ </param> 
        /// <param name="Type_Return"> �������� </param> 
        /// <returns> ���������ú����� object</returns> 
        public object Invoke(IntPtr IntPtr_Function, object[] ObjArray_Parameter, Type[] TypeArray_ParameterType, ModePass[] ModePassArray_Parameter, Type Type_Return)
        {
            // ���� 2 �� if �ǽ��а�ȫ��� , ������ͨ�� , ���׳��쳣 
            if (hModule == IntPtr.Zero)
                throw (new Exception(" ������ģ��ľ��Ϊ�� , ��ȷ���ѽ��� LoadDll ���� !"));
            if (IntPtr_Function == IntPtr.Zero)
                throw (new Exception(" ����ָ�� IntPtr_Function Ϊ�� !"));
            farProc = IntPtr_Function;
            return Invoke(ObjArray_Parameter, TypeArray_ParameterType, ModePassArray_Parameter, Type_Return);
        }

        public object Invoke(string strFunctionName, object[] ObjArray_Parameter, Type[] TypeArray_ParameterType, ModePass[] ModePassArray_Parameter, Type Type_Return)
        {
            curFunName = strFunctionName;
            m_ObjArray_Parameter = ObjArray_Parameter;
            if (m_htFuns[strFunctionName] == null)
            {
                LoadFun(strFunctionName);
            }
            else
            {
                LoadFun(strFunctionName);
                MethodInfo minfo = (MethodInfo)m_htFuns[strFunctionName];
                return minfo.Invoke(null, ObjArray_Parameter);                
            }

            if (farProc == IntPtr.Zero)
                return null;
            else
            {
                return Invoke(farProc, ObjArray_Parameter, TypeArray_ParameterType, ModePassArray_Parameter, Type_Return);
            }
        }
        public object InvokeLastMethod()
        {
            string strFunctionName = curFunName;
            
            MethodInfo MyMethodInfo = (MethodInfo)m_htFuns[strFunctionName];
            return MyMethodInfo.Invoke(null, m_ObjArray_Parameter);
        }
    }
}
