Include("scripts/flowlib/custom/api.lua")
using('API')
using('comgr')
using('Event')


--=================================================================
--������ 
function OnAcceptQuest_q_id_chiyao()
	--API.player.AddItem(player, 5, 1844, 1) -- ��ҩ��
	--API.player.AddItem(player, 5, 1846, 1) -- ��ҩ��
end;

--=================================================================
--��ҩ 
function onuse_hongyao(context, player, item, item_key, questid)	
	local co = context.co
	local node1, node2, node3, node4, node5, node6
	node1 = function(context, parentcontext)
		print('�Ժ�ҩ')		
		print('��ʼ�߳�ҩ������')
		print(THIS_FILE)
		local ret = API.player.DoCustomOTAction(context, player, 0, 1, 3 * 16, TARGET.NO_TARGET, item.dwID, THIS_FILE, "���ڳ�ҩ")    
		print('�����ҩ������')
		if ret == '���' then
			print('to node2' .. tostring(node2))
			return node2(context, parentcontext)
		else
			return finishnode(parentcontext)
		end 
	end

	node2 = function(context, parentcontext)
		print('�����������1Ϊ��')
		API.Quest.SetQuestValue(player, questid, 0, 1) -- �Թ���ҩ
		--ѭ���в�����������
		--��֧��ʱ
		--�Ӿ���
		create_subflow(node3, context)

		--sub2		
		create_subflow(node5, context)
	
		coroutine.yield() --�ȴ������̽���1
		coroutine.yield() --�ȴ������̽���2

	end

	node3 = function(context, parentcontext)
		API.player.Sleep({envid=envid}, player, 1*16)
		print('�� 1 ����')
		API.player.AddExp(player, 1)
		return node4(context, parentcontext) --goto nextnode
	end

	node4 = function(context, parentcontext)
		if (API.Quest.GetQuestValue(player, questid, 1) == 1) then  --����û��� 
			return finishnode(parentcontext)
		else
			return node3(context, parentcontext)
		end
	end

	node5 = function(context, parentcontext)
		API.player.Sleep({envid=envid}, player, 10*16)
		if API.Quest.GetQuestValue(player, questid, 1) == 1 then 
			print('�Ѿ��������,��������!')
			return finishnode(parentcontext)
		else
			return node6(context, parentcontext)
		end
	end

	node6 = function(context, parentcontext)
		API.Quest.SetQuestValue(player, questid, 1, 1)
		print('û���ڹ涨ʱ���ڳԽ�ҩ,�������!')
		--API.player.FinishQuest(player, questid)    		
		API.player.Die(player)		
		return finishnode(parentcontext)
	end

	node1(context, nil)
end

function onuse_main_jieyao(context, player, item, item_key, questid) --�Խ�ҩ 
	local co = context.co
	local node1, node2, node4, node5, node6, node7
	node1 = function(envid)
		print('�Խ�ҩ') 	
		return node2(envid)
	end

	node2 = function(envid)
		if API.Quest.GetQuestValue(player, questid, 0) == 1 then
			return node4(envid)
		else
			return node5(envid)
		end		
	end

	node4 = function(envid)
		print('��ʼ�߳Խ�ҩ������')
		local ret = API.player.DoCustomOTAction({envid=envid, co=co}, player, 0, 1, 3 * 16, TARGET.NO_TARGET, item.dwID, THIS_FILE, "���ڳԽ�ҩ...")    
		print('����Խ�ҩ������')
		if ret == '���' then
			return node7(envid)
		end
	end

	node5 = function(envid)
		API.player.SendMessage(player, "�㻹û�гԹ���ҩ") 
	end

	node6 = function(envid)
		API.Quest.SetQuestValue(player, questid, 1, 1) -- �Թ���ҩ
		--API.player.FinishQuest(player, questid) 
	end

	node7 = function(envid)
		if (API.player.IsALive(player)) then
			return node6(envid)
		end
	end

	node1(envid)
end

--ע���¼�
if Event.OnUse.event1.FunList["5,1844"] == nil then
	Event.OnUse.event1.FunList["5,1844"] = {}
end
Event.OnUse.event1.FunList["5,1844"][onuse_hongyao]		= {nil, nil, "5,1844",231}

if Event.OnUse.event1.FunList["5,1846"] == nil then 
	Event.OnUse.event1.FunList["5,1846"] = {} 
end
Event.OnUse.event1.FunList["5,1846"][onuse_main_jieyao]		= {nil, nil, "5,1846",231}
--todo: questid �滻 