Include("scripts/map/����1/api.lua")
using('API')
using('comgr')
using('Event')

create_sub = Event.Public.event_dispatch

--=================================================================
--������ 
function OnAcceptQuest_q_id_chiyao()
	--API.player.AddItem(player, 5, 1844, 1) -- ��ҩ��
	--API.player.AddItem(player, 5, 1846, 1) -- ��ҩ��
end;

--=================================================================
--��ҩ 
function onuse_hongyao(envid, player, item, item_key, questid)	
	local co = comgr.find(envid)
	local i
	g_finish = false
	print('�Ժ�ҩ')
	--print('API:'..tostring(API))
	--print('API.player:'..tostring(API.player))
	--print('API.player.GetQuestPhase='..tostring(API.player.GetQuestPhase))
 	if API.player.GetQuestPhase(player, 231) == 1 then 				
		print('��ʼ�߳�ҩ������')
		local ret = API.player.DoCustomOTAction({envid=envid, co=co}, player, 0, 1, 3 * 16, TARGET.NO_TARGET, item.dwID, THIS_FILE, "���ڳ�ҩ")    
		print('�����ҩ������')
    		if ret == '���' then
			print('�����������1Ϊ��')
			API.Quest.SetQuestValue(player, questid, 0, 1) -- �Թ���ҩ
			--ѭ���в�����������
			--��֧��ʱ
			--�Ӿ���
			create_sub(sub1, co, player, item, questid)			
			--sub2
			create_sub(sub2, co, player, item, questid)			
			--
		
			coroutine.yield() --�ȴ������̽���1
			coroutine.yield() --�ȴ������̽���2
		end 
	end
end

function onuse_main_jieyao(envid, player, item, item_key, questid) --�Խ�ҩ 
	local co = comgr.find(envid)
	local i
	g_finish = false
	print('�Խ�ҩ') 	
 	
	if API.player.GetQuestPhase(player, questid) == 1 then 	
		if API.Quest.GetQuestValue(player, questid, 0) == 1 then
			print('��ʼ�߳Խ�ҩ������')
			local ret = API.player.DoCustomOTAction({envid=envid, co=co}, player, 0, 1, 3 * 16, TARGET.NO_TARGET, item.dwID, THIS_FILE, "���ڳԽ�ҩ...")    
			print('����Խ�ҩ������')
			if ret == '���' then
				if (API.player.IsALive(player)) then
					API.Quest.SetQuestValue(player, questid, 1, 1) -- �Թ���ҩ
					--API.player.FinishQuest(player, questid) 
				end
			end
		else
			API.player.SendMessage(player, "�㻹û�гԹ���ҩ") 
		end		
	else
		API.player.SendMessage(player, "��û�нӹ�����") 
	end
end

function onusejieyao(envid)
	print('�Խ�ҩ')
	g_finish = true
end

--�Ӿ����֧ 
function sub1(envid, coparent, player, item, questid)
	local node1, node2
	local exit1 = 0;
		
	--�ж���ڽڵ�	
	local exp = 0
	node1 = function()
		API.player.Sleep({envid=envid}, player, 1*16)
		print('�� 1 ����, ����ֵ='..exp)
		API.player.AddExp(player, 1)
		exp = exp + 1
		return node2() --goto nextnode
	end
	
	node2 = function()	
	    if (API.Quest.GetQuestValue(player, questid, 1) == 1) then  --����û��� 
			return
		else
			return node1()
		end
	end

	finishnode = function()  --�Զ����ɵĽ����ڵ� 
		resume(coparent)
	end

	node1()
	
	finishnode()	
end

function sub2(envid, coparent, player, item, questid)
	API.player.Sleep({envid=envid}, player, 10*16)
    if API.Quest.GetQuestValue(player, questid, 1) == 1 then 
		print('�Ѿ��������,��������!')
 	else
		API.Quest.SetQuestValue(player, questid, 1, 1)
		print('û���ڹ涨ʱ���ڳԽ�ҩ,�������!')
  		--API.player.FinishQuest(player, questid)    		
		API.player.Die(player)					
	end
	resume(coparent)
end

--todo: questid �滻 