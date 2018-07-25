using System;
using System.Collections.Generic;
using System.Text;
using System.Resources;
using System.Data;
using System.Data.SqlClient;
using System.Reflection;
using System.Web;
using XSJ.Sword3.Editor.Studio;
using XSJ.Sword3.Editor.Studio.Component;

namespace XSJ.Sword3.Editor.Studio.Data
{
    /// <summary>
    /// ���ݿ�����ӿ�
    /// </summary>
    interface IDataProviderBase
    {
        #region Properties
        string ItemsTableName { get;}
        string ShopItemTableName { get;}
        string ShopListTableName { get;}
        string EquipmentTableName { get;}
        #endregion
        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="msg">������Ϣ</param>
        /// <param name="stackinfo">��ջ��Ϣ</param>
        /// <returns>����������ӳɹ�,�򷵻�true,���򷵻�false.</returns>
        bool TryConnect(out string msg,out string stackinfo);
        /// <summary>
        /// �ӿڳ�ʼ������,��ʹ�øýӿڵ�����������֮ǰ,���ȵ��ñ��ӿ�.
        /// </summary>
        /// <param name="connectionString">���ݿ������ַ���</param>
        void Init(string connectionString);
        /// <summary>
        /// ����SQL���ִ�� XXXCommand.ExecuteScalar()����.���ؼ�¼����
        /// </summary>
        /// <param name="sql">����SQL���</param>
        /// <returns>���ؼ�¼����</returns>
        int GetRecordCount(string sql);
        /// <summary>
        /// Run Transact-SQL ��� 
        /// </summary>
        /// <param name="strSqlName">Sql string</param>
        /// <returns>iret = cmd.ExecuteNonQuery();</returns>
        int RunSql(string strSqlName, bool UseTransaction);
        /// <summary>
        /// Run Transact-SQL language
        /// </summary>
        /// <param name="strSqlName">Name of Transact-SQL language.</param>
        /// <returns>iret = cmd.ExecuteNonQuery();</returns>
        int RunSql(string strSqlName, SqlParameter[] prams, bool UseTransaction);
        /// <summary>
        /// Run Transact-SQL language(Query only)
        /// </summary>
        /// <param name="strSqlName">Name of Transact-SQL language.</param>
        /// <param name="dataReader">Return result of Sql.</param>
        void RunSql(string strSqlName, out SqlDataReader dataReader);
        /// <summary>
        /// Run Transact-SQL language(Query Only)
        /// </summary>
        /// <param name="strSqlName">Name of Transact-SQL language.</param>
        /// <param name="dataReader">Return result of Sql.</param>
        void RunSql(string strSqlName, SqlParameter[] prams, out SqlDataReader dataReader);
        /// <summary>
        /// ���Է��أ۹ؼ��ֶε�ֵ��
        /// </summary>
        /// <param name="strSqlName"></param>
        /// <param name="prams"></param>
        /// <returns>���أ۹ؼ��ֶε�ֵ��</returns>
        /// <remarks>cmd.ExecuteScalar();</remarks>
        int RunSql2(string strSqlName, SqlParameter[] prams, bool UseTransaction);

        DataTable GetDataTableBySql(string sql);

        #region Item methods
        Item AddItem(Item item);
        bool UpdateItem(Item item);
        bool DeleteItem(int itemid);
        int GetItemCount();
        Item GetOneItem(int itemid);

        ItemCollection GetItemsBySql(string sql);
        ItemCollection GetItemsBySql(string sql, int PageSize, int PageIndex);
        ItemCollection GetAllItems();
        ItemCollection GetAllItems(int PageSize, int PageIndex, string SortField, bool DESC, out int retCnt);
        ItemCollection GetItemsByCategory(int genre, int subType, int detailType);
        #endregion

        #region Equipment methods
        Equipment AddEquipment(Equipment equip);
        bool UpdateEquipment(Equipment equip);
        bool DeleteEquipment(int equipid);
        int GetEquipmentCount();
        Equipment GetOneEquipment(int equipid);

        EquipmentCollection GetEquipmentsBySql(string sql);
        EquipmentCollection GetEquipmentsBySql(string sql, int PageSize, int PageIndex);
        EquipmentCollection GetAllEquipments();
        EquipmentCollection GetAllEquipments(int PageSize, int PageIndex, string SortField, bool DESC, out int retCnt);
        EquipmentCollection GetEquipmentsByParentID(int parentid);
        #endregion

        #region ShopItem methods
        ShopItem AddShopItem(ShopItem shop);
        bool UpdateShopItem(ShopItem shop);
        bool DeleteShopItem(int shopid);
        int GetShopItemCount();
        ShopItem GetOneShopItem(int shopid);
        ShopItemCollection GetShopItemsBySql(string sql);
        ShopItemCollection GetShopItemsBySql(string sql, int PageSize, int PageIndex);
        ShopItemCollection GetAllShopItems();
        ShopItemCollection GetAllShopItems(int PageSize, int PageIndex, string SortField, bool DESC, out int retCnt);

        ShopItemCollection GetShopItemsByShop(int shopid);
        bool IsItemExistsInShop(int itemIndex, int shopid);
        bool DeleteShopItemsByShopTemplateID(int shoptemplateid);
        #endregion

        #region ShopList methods
        ShopList AddShopList(ShopList shoplist);
        bool UpdateShopList(ShopList shoplist);
        bool DeleteShopList(int shoplistid);
        int GetShopListCount();
        bool IsShopListExists(string shopname);
        ShopList GetOneShopList(int shoplistid);
        ShopList GetOneShopListByShopName(string shopname);
        ShopListCollection GetShopListsBySql(string sql);
        ShopListCollection GetShopListsBySql(string sql, int PageSize, int PageIndex);
        ShopListCollection GetAllShopLists();
        ShopListCollection GetAllShopLists(int PageSize, int PageIndex, string SortField, bool DESC, out int retCnt);
        #endregion

        #region ItemCategory methods
        ItemCategory AddItemCategory(ItemCategory itemcategory);
        bool UpdateItemCategory(ItemCategory itemcategory);
        bool DeleteItemCategory(int itemcategoryid);
        int GetItemCategoryCount();
        ItemCategory GetOneItemCategory(int itemcategoryid);
        ItemCategoryCollection GetItemCategoriesBySql(string sql);
        ItemCategoryCollection GetItemCategoriesBySql(string sql, int PageSize, int PageIndex);
        ItemCategoryCollection GetAllItemCategories();
        ItemCategoryCollection GetItemCategories(int PageSize, int PageIndex, string SortField, bool DESC, out int retCnt);

        #endregion
    }
}
