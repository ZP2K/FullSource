Include("scripts/flowlib/custom/api.lua")		--��Ϊϵͳ�ӿ�bug������DoCustomOTAction��ص�������ļ������Ա��ļ������Ҫ���ҵ�api.lua��OnCustomEvent���С�
Include("scripts/flowlib/custom/eventfilter.lua")
using('Event')

function AOnAllEvent(eventid, key, ...)
	env = Event[eventid]
	if env.FunList[key] ~= nil then
		for k, v in pairs(env.FunList[key]) do
			local ta = {}
			for i = 1, #arg do
			   ta[i] = arg[i]
			end
			for i = 1, #v do
			   ta[i +#arg] = v[i]
			end
			if env.Condition(unpack(ta)) then
				event_dispatch(k, env.Args(unpack(ta)))
			end
		end
	else
		print(string.format('����δע����¼��ص�(eventid = %d, key = %s ->type=(%s))����ȷ�� eventfilter.lua ��д��ȷ', eventid, tonumber(key), type(key)))
	end
end

function AOnAcceptQuest(key, ...)
	AOnAllEvent(8, key, ...)
end

function AOnCancelQuest(key, ...)
	--AOnAllEvent(?, key, ...)
end

function AOnUse(key, ...)
	AOnAllEvent(2, key, ...)
end

function AOnEnterTrap(key, ...)
	--AOnAllEvent(?, key, ...)	
end

function AOnDialogue(key, ...)
	print('AOnDialogue')
	AOnAllEvent(7, key, ...)	
end



--[[
	if Event.OnUse.event1.Condition(player,item,nil,nil,"5,1844",231) then
		event_dispatch(onuse_hongyao, Event.OnUse.event1.Args(player,item,nil,nil,"5,1844",231))
	end
	if Event.OnUse.event1.Condition(player,item,nil,nil,"5,1846",231) then		
		event_dispatch(onuse_main_jieyao, Event.OnUse.event1.Args(player,item,nil,nil,"5,1846",231))
	end
--]]