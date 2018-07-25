#include "stdafx.h"
#include "./kdebugconfig.h"

namespace UI
{
	int KDebugConfig::ms_nResizeWindowHandleNodeHalfSize = 4;

	bool KDebugConfig::ms_aBoolFlag[BOOL_END] =
	{
		false,	// BOOL_SHOW_MOUSE_OVER_WND,
		false,	// BOOL_SHOW_FRAME,
		false,	// BOOL_SHOW_NAME,
		false,	// BOOL_SHOW_FRAME_DRAG_AREA_BODER,
		false,	// BOOL_SHOW_TEXT_ATOM_BOUNDARY,
		false,	// BOOL_RESIZE_WND_AND_ITEM_MODE,
		false,	// BOOL_DRAG_WND_AND_ITEM_MODE,
		false,	// BOOL_SELECT_WINDOW_IN_CLIENT,

		false,	// BOOL_AUTO_SCREEN_SHOT,

		false,	// BOOL_ENABLE_LOG_3D_DEBUG_INFO,

		false,	// BOOL_EDIT_MODE,

		true,	// BOOL_ENABLE_RENDER_UI,
		false,	// BOOL_ENABLE_TIMER_MGR,
		false,	// BOOL_ENABLE_GLOBAL_BACK_BUFFER
	};

}


