-- NPC�ű�, �Ի��¼�����
function OnDialogue(npc, player)	    
	player.OpenWindow(TARGET.NPC, npc.dwID,	    
	    npc.GetQuestString()
        )
end