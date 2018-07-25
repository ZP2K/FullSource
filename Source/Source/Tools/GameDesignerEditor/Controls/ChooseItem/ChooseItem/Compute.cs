using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using System.Data.SqlClient;

namespace ChooseItem
{
    class Compute
    {
        /// <summary>
        /// ��ʾ���㴰��
        /// </summary>
        /// <param name="dropValue">����ֵ</param>
        /// <param name="luckyRate">����Ȩֵ</param>
        public void Show(string dropValue, string luckyRate)
        {
            int dropV = 0;
            int luckyR = 0;

            try
            {
                dropV = int.Parse(dropValue);
                luckyR = int.Parse(luckyRate);
            }
            catch(Exception ex)
            {
                MessageBox.Show("�ڼ������յ�����ʱ�����쳣: " + ex.ToString());
                return;
            }

            ComputeDropRate cForm = new ComputeDropRate();
            cForm.DropValue = dropV;
            cForm.LuckyRate = luckyR;
            DialogResult result = cForm.ShowDialog();
        }
    }
}
