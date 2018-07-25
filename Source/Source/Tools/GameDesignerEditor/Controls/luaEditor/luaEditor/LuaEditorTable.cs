using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;

namespace luaEditor
{
    public class LuaEditorTableItem
    {
        private readonly string name; // ��ʾֵ
        private readonly string itemType; // ����
        private readonly object itemValue; // ʵ�����ֵ
        private readonly string info; // ˵����Ϣ

        public LuaEditorTableItem(string name, string type, object value, string info)
        {
            this.name = name;
            this.itemType = type;
            this.itemValue = value;
            this.info = info;
        }

        public string Name
        {
            get
            {
                return name;
            }
        }

        public string ItemType
        {
            get
            {
                return itemType;
            }
        }

        public object ItemValue
        {
            get
            {
                return itemValue;
            }
        }

        public string Info
        {
            get
            {
                return info;
            }
        }
    };
}
