-- ITEM�ű�, �����ʹ�õ��ߵ�ʱ�򴥷�
Include("scripts/map/����1/event.lua")
function OnUse(player, item, dwSourceBox, dwSourceSlot, dwTargetBox, dwTargetSlot)
	local sta, err = pcall(function() AOnUse(player, item) end)
	if err then
		print("err: " .. err)
		print(debug.traceback())
	end
	return false, false
end