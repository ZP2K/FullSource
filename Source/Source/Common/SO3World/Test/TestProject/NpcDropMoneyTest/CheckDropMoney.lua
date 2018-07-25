---------------------------------------------------------------------->
-- �ű�����:	CheckDropMoney.lua
-- ����ʱ��:	09/05/20 9:20:30
-- �����û�:	�����
-- �ű�˵��:	�������õĻ���·���Զ������ɲ������� 
--						��������RTX caiyutao���� MailTo: caiyutao@kingsoft.com
----------------------------------------------------------------------<

local tDropMoneyTab = {}
local tNpcTemplete = {}
local szReport = "NPC����\tNPCID\t��������ı���\t��ǰֵ\t�淶���ֵ\n"

-- �������ܺ���������ַ���
string.Split = function(szString, szSpliter)
	local tSplited = {}
	local nCount = 0								-- ��¼�ҵ����зַ�����
	local nStartLoc = 0								-- ��¼�������з��ַ����Ŀ�ʼ��
	local nEndLoc = 0								-- ��¼�������з��ַ����Ľ�����
	local bFoundSpliter = false						-- ��¼�Ƿ��ҵ��зַ�
	
	if not szSpliter then							-- ���û�д���ָ�����зַ�, ��Ĭ��Ϊ�Ʊ��tab
		szSpliter = "\t"
	end
	local szFormated = szString .. szSpliter		-- ��ʽ��ԭ�ַ���
	
	repeat
		nCount = nCount + 1
		bFoundSpliter, nEndLoc = szFormated:find(szSpliter, nStartLoc)
		if bFoundSpliter then
			tSplited[nCount] = szFormated:sub(nStartLoc, nEndLoc - #szSpliter)
			if not tSplited[nCount] then
				tSplited[nCount] = ""
			end
			nStartLoc = nEndLoc + 1
		end
	until not bFoundSpliter
	return tSplited
end

string.SplitHead = function(szString, szSpliter)
	local tSplited = {}
	local nCount = 0								-- ��¼�ҵ����зַ�����
	local nStartLoc = 0								-- ��¼�������з��ַ����Ŀ�ʼ��
	local nEndLoc = 0								-- ��¼�������з��ַ����Ľ�����
	local bFoundSpliter = false						-- ��¼�Ƿ��ҵ��зַ�
	
	if not szSpliter then							-- ���û�д���ָ�����зַ�, ��Ĭ��Ϊ�Ʊ��tab
		szSpliter = "\t"
	end
	local szFormated = szString .. szSpliter		-- ��ʽ��ԭ�ַ���
	
	repeat
		nCount = nCount + 1
		bFoundSpliter, nEndLoc = szFormated:find(szSpliter, nStartLoc)
		if bFoundSpliter then
			tSplited[szFormated:sub(nStartLoc, nEndLoc - #szSpliter)] = nCount
			if not tSplited[szFormated:sub(nStartLoc, nEndLoc - #szSpliter)] then
				tSplited[szFormated:sub(nStartLoc, nEndLoc - #szSpliter)] = ""
			end
			nStartLoc = nEndLoc + 1
		end
	until not bFoundSpliter
	return tSplited
end

ReadMoneyDropTab = function(TabFile)
	local fTabfile = io.open(TabFile, "r")
	local szHead = fTabfile:read("*line")
	repeat
		local szData = fTabfile:read("*line")
		if szData then
			local tDataTemp = szData:Split("\t")
			tDropMoneyTab[#tDropMoneyTab + 1] = {Level = tonumber(tDataTemp[1]), MinMoney = tonumber(tDataTemp[2]), MaxMoney = tonumber(tDataTemp[3]), DropRate = tonumber(tDataTemp[4]), Intensity = tonumber(tDataTemp[5])}
		end		
	until not szData
	fTabfile:close()
	
end

ReadNpcTempleteTab = function(TabFile)
	local fTabfile = io.open(TabFile, "r")
	local szTabHeadLine = fTabfile:read("*line")
	local tHeadLine = szTabHeadLine:SplitHead("\t")
	repeat
		local szData = fTabfile:read("*line")
		if szData then
			local tDataTemp = szData:Split("\t")
			local Level = 0
			if tDataTemp[tHeadLine["Level"]] == "" then
				Level = 1
			else
				Level = tonumber(tDataTemp[tHeadLine["Level"]])
			end
			
			if tDataTemp[tHeadLine["Level"]] == "" then
				Level = 1
			else
				Level = tonumber(tDataTemp[tHeadLine["Level"]])
			end
			
			local EquipLevel = 0
			if tDataTemp[tHeadLine["_EquipLevel"]] == "" then
				EquipLevel = 1
			else
				EquipLevel = tonumber(tDataTemp[tHeadLine["_EquipLevel"]])
			end
			
			if tDataTemp[tHeadLine["_EquipLevel"]] == "" then
				EquipLevel = 1
			else
				EquipLevel = tonumber(tDataTemp[tHeadLine["_EquipLevel"]])
			end
			
			if tDataTemp[tHeadLine["Species"]] == "" then
				nSpecies = 1
			else
				nSpecies = tonumber(tDataTemp[tHeadLine["Species"]])
			end
			
			tNpcTemplete[tonumber(tDataTemp[tHeadLine["ID"]])] = {ID = tonumber(tDataTemp[tHeadLine["ID"]]), 
			Name = tDataTemp[tHeadLine["Name"]], 
			_Intensity = tonumber(tDataTemp[tHeadLine["_Intensity"]]), 
			Level = math.min(Level, EquipLevel), 
			MinMoney = tonumber(tDataTemp[tHeadLine["MoneyMin"]]) or 0, 
			MaxMoney = tonumber(tDataTemp[tHeadLine["MoneyMax"]]) or 0, 
			MoneyDropRate = tonumber(tDataTemp[tHeadLine["MoneyDropRate"]]) or 0,
			Species = nSpecies}
		end		
	until not szData
	fTabfile:close()
	
end


DoTest = function(fMoneyDroptab, fNpcTemplatetab, fResultTab)
	ReadMoneyDropTab(fMoneyDroptab)
	ReadNpcTempleteTab(fNpcTemplatetab)
	
	for nAddCount in pairs(tNpcTemplete) do
	
		if tNpcTemplete[nAddCount].Species ~= 1 and tNpcTemplete[nAddCount].Species ~= 4 then
			if tNpcTemplete[nAddCount].MaxMoney ~= 0 then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "����Ǯ" .. "\t" .. tNpcTemplete[nAddCount].MaxMoney .. "\t" .. "0" .. "\n"
			end
			
			if tNpcTemplete[nAddCount].MinMoney ~= 0 then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "��С��Ǯ" .. "\t" .. tNpcTemplete[nAddCount].MinMoney .. "\t" .. "0" .. "\n"
			end
			
			if tNpcTemplete[nAddCount].MoneyDropRate ~= 0 then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "��Ǯ������" .. "\t" .. tNpcTemplete[nAddCount].MoneyDropRate .. "\t" .. "0" .. "\n"
			end
		else
			local nMaxMoneyLine = 0
			local nMinMoneyLine = 0
			local nDropMoneyRateLine = 0
			for nCount in pairs(tDropMoneyTab) do
				if tNpcTemplete[nAddCount].Level == tDropMoneyTab[nCount].Level and tNpcTemplete[nAddCount]._Intensity == tDropMoneyTab[nCount].Intensity then
					nMaxMoneyLine = tDropMoneyTab[nCount].MaxMoney
					nMinMoneyLine = tDropMoneyTab[nCount].MinMoney
					nDropMoneyRateLine = tDropMoneyTab[nCount].DropRate
				end
			end
			
			if tNpcTemplete[nAddCount].MinMoney > nMinMoneyLine then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "��С��Ǯ" .. "\t" .. tNpcTemplete[nAddCount].MinMoney .. "\t" .. nMinMoneyLine .. "\n"
			end
			
			if tNpcTemplete[nAddCount].MaxMoney > nMaxMoneyLine then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "����Ǯ" .. "\t" .. tNpcTemplete[nAddCount].MaxMoney .. "\t" .. nMaxMoneyLine .. "\n"
			end
			
			if tNpcTemplete[nAddCount].MoneyDropRate > nDropMoneyRateLine then
				szReport = szReport .. tNpcTemplete[nAddCount].Name .. "\t" .. tNpcTemplete[nAddCount].ID .. "\t" .. "��С��Ǯ" .. "\t" .. tNpcTemplete[nAddCount].MoneyDropRate .. "\t" .. nDropMoneyRateLine .. "\n"
			end
		end
	
	end
	
	local ObjectFile = io.open(fResultTab, "a+")
	ObjectFile:write(szReport)
	ObjectFile:close()
	
end

	local fOutputTab = "D:\\FTP\\kingsoft\\game\\sword3-products\\MoneyCheck\\MoneyCheck\\Result" .. (os.date("%m%d",os.time())) .. ".tab"
	DoTest("D:\\FTP\\kingsoft\\game\\sword3-products\\MoneyCheck\\MoneyCheck\\npcdropmoney.tab", "D:\\FTP\\kingsoft\\game\\sword3-products\\client\\settings\\NpcTemplate.tab" , fOutputTab)