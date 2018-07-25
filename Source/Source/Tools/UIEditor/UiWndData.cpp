#include "Stdafx.h"
#include "UiWndData.h"

static	const char* s_pTypeString[UI_WND_TYPE_COUNT][3] = 
{
    { "WndWindow",		            "�鴰��",		        "Wnd"		            },
    { "WndImage",		            "ͼ��",		            "Img"		            },
    { "WndButton",		            "��ť",			        "Btn"		            },
    { "WndNewScrollBar",	        "������",		        "Scroll"		        },
    { "WndEdit",	                "���ֱ༭��",	        "Edit"		            },
    { "WndShowAnimate",             "������ʾ",             "ShowAni"		        },
    { "WndPage",                    "ҳ��",                 "Page"		            },
    { "WndPageSet",                 "ҳ������",             "PageSet"		        },
    { "WndFrame",                   "����",                 "Frame"                 },
    { "WndCheckBox",                "��ѡ��",               "CheckBox"              },
    { "WndModel",					"3Dģ����Ⱦ����",       "3DModel"				},
    { "WndTexture",					"3D��ͼ��Ⱦ����",       "3DTexture"             },
    { "WndMinimap",					"С��ͼ",				"Minimap"               },
    { "WndScene",					"��Ϸ����",				"Scene"                 },
    { "WndWebPage",					"��Ƕ��ҳ",				"WebPage"               },
                                                 
    { "Null",                       "�����",              	"Null"                  },
    { "Text",                       "�������",            	"Text"                  },
    { "Image",                      "ͼƬ���",            	"Image"                 },
    { "Shadow",                     "��Ӱ���",            	"Shadow"                },
    { "Animate",                    "�������",            	"Animate"               },
	{ "Box",						"��ק��",            	"Box"                   },
    { "Sence",                      "�������",            	"Sence"                 },
    { "Handle",                     "�������",            	"Handle"                },
    { "TreeLeaf",                   "���νṹһ��",       	"TreeLeaf"              },
};


int	g_GetWndType(const char* pszType, UI_WND_TYPE& Type)
{
	if (pszType && pszType[0])
	{
		for (int i = 0; i < UI_WND_TYPE_COUNT; i++)
		{
			if (!stricmp(s_pTypeString[i][0], pszType))
			{
				Type = (UI_WND_TYPE)i;
				return true;
			}
		}
	}
	Type = UI_WND_WINDOW;
	return false;
}

int g_GetWndType(const char* pszType)
{
	if (pszType && pszType[0])
	{
		for (int i = 0; i < UI_WND_TYPE_COUNT; i++)
		{
			if (stricmp(s_pTypeString[i][0], pszType) == 0) 
				return i;
		}
	}

	return UI_WND_TYPE_UNKNOWN;
}

const char* g_GetWndTypeKey(UI_WND_TYPE Type)
{
	if (Type >= 0 && Type < UI_WND_TYPE_COUNT)
		return s_pTypeString[Type][0];
	return "";
}

const char*	g_GetWndTypeString(UI_WND_TYPE Type)
{
	if (Type >= 0 && Type < UI_WND_TYPE_COUNT)
		return s_pTypeString[Type][1];
	return "";
}

const char *g_GetWndTypePrefix(UI_WND_TYPE Type)
{
	if (Type >= 0 && Type < UI_WND_TYPE_COUNT)
		return s_pTypeString[Type][2];
	return "";

}
