using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace luaEditor
{
    public class leTableItem
    {
        public leTableItem(string name, string type, object value, string info)
        {
            this.name = name;
            this.type = type;
            this.value = value;
            this.info = info;
        }
        public string name;         //��ʾֵ
        public string type;         //����
        public object value;        //ʵ�����ֵ
        public string info;         //˵����Ϣ
    };
}
