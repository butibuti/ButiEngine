#pragma once
#include"../Common/ButiMath.h"
#include<string>

namespace ButiEngine {


	

	namespace GUI
	{
		using GuiCol=int;
		using GuiCond=int;
		using GuiDataType=int;
		using GuiDir=int;
		using GuiKey=int;
		using DrawCornerFlags=int;
		using DrawListFlags=int;
		using FontAtlasFlags=int;
		using GuiBackendFlags=int;
		using GuiButtonFlags=int;
		using GuiColorEditFlags=int;
		using GuiConfigFlags=int;
		using GuiComboFlags=int;
		using GuiDragDropFlags=int;
		using GuiFocusedFlags=int;
		using GuiHoveredFlags=int;
		using GuiInputTextFlags=int;
		using GuiKeyModFlags=int;
		using GuiPopupFlags=int;
		using GuiSelectableFlags=int;
		using GuiSliderFlags=int;
		using GuiTabBarFlags=int;
		using GuiTabItemFlags=int;
		using GuiTreeNodeFlags=int;
		using GuiWindowFlags=int;



		enum DrawCornerFlags_
		{
			DrawCornerFlags_None = 0,
			DrawCornerFlags_TopLeft = 1 << 0, // 0x1
			DrawCornerFlags_TopRight = 1 << 1, // 0x2
			DrawCornerFlags_BotLeft = 1 << 2, // 0x4
			DrawCornerFlags_BotRight = 1 << 3, // 0x8
			DrawCornerFlags_Top = DrawCornerFlags_TopLeft | DrawCornerFlags_TopRight,   // 0x3
			DrawCornerFlags_Bot = DrawCornerFlags_BotLeft | DrawCornerFlags_BotRight,   // 0xC
			DrawCornerFlags_Left = DrawCornerFlags_TopLeft | DrawCornerFlags_BotLeft,    // 0x5
			DrawCornerFlags_Right = DrawCornerFlags_TopRight | DrawCornerFlags_BotRight,  // 0xA
			DrawCornerFlags_All = 0xF     // In your function calls you may use ~0 (= all bits sets) instead of DrawCornerFlags_All, as a convenience
		};

		enum DrawListFlags_
		{
			DrawListFlags_None = 0,
			DrawListFlags_AntiAliasedLines = 1 << 0,  // Enable anti-aliased lines/borders (*2 the number of triangles for 1.0f wide line or lines thin enough to be drawn using textures, otherwise *3 the number of triangles)
			DrawListFlags_AntiAliasedLinesUseTex = 1 << 1,  // Enable anti-aliased lines/borders using textures when possible. Require backend to render with bilinear filtering.
			DrawListFlags_AntiAliasedFill = 1 << 2,  // Enable anti-aliased edge around filled shapes (rounded rectangles, circles).
			DrawListFlags_AllowVtxOffset = 1 << 3   // Can emit 'VtxOffset > 0' to allow large meshes. Set when 'GuiBackendFlags_RendererHasVtxOffset' is enabled.
		};

		enum FontAtlasFlags_
		{
			FontAtlasFlags_None = 0,
			FontAtlasFlags_NoPowerOfTwoHeight = 1 << 0,   // Don't round the height to next power of two
			FontAtlasFlags_NoMouseCursors = 1 << 1,   // Don't build software mouse cursors into the atlas (save a little texture memory)
			FontAtlasFlags_NoBakedLines = 1 << 2    // Don't build thick line textures into the atlas (save a little texture memory). The AntiAliasedLinesUseTex features uses them, otherwise they will be rendered using polygons (more expensive for CPU/GPU).
		};

		enum GuiCond_
		{
			GuiCond_None = 0,        // No condition (always set the variable), same as _Always
			GuiCond_Always = 1 << 0,   // No condition (always set the variable)
			GuiCond_Once = 1 << 1,   // Set the variable once per runte session (only the first call will succeed)
			GuiCond_FirstUseEver = 1 << 2,   // Set the variable if the object/window has no persistently saved data (no entry in .ini file)
			GuiCond_Appearing = 1 << 3    // Set the variable if the object/window is appearing after being hidden/inactive (or the first te)
		};

		enum GuouseCursor_
		{
			GuouseCursor_None = -1,
			GuouseCursor_Arrow = 0,
			GuouseCursor_TextInput,         // When hovering over InputText, etc.
			GuouseCursor_ResizeAll,         // (Unused by Dear Gui functions)
			GuouseCursor_ResizeNS,          // When hovering over an horizontal border
			GuouseCursor_ResizeEW,          // When hovering over a vertical border or a column
			GuouseCursor_ResizeNESW,        // When hovering over the bottom-left corner of a window
			GuouseCursor_ResizeNWSE,        // When hovering over the bottom-right corner of a window
			GuouseCursor_Hand,              // (Unused by Dear Gui functions. Use for e.g. hyperlinks)
			GuouseCursor_NotAllowed,        // When hovering something with disallowed interaction. Usually a crossed circle.
			GuouseCursor_COUNT
		};

		enum GuouseButton_
		{
			GuouseButton_Left = 0,
			GuouseButton_Right = 1,
			GuouseButton_Middle = 2,
			GuouseButton_COUNT = 5
		};

		enum GuiSliderFlags_
		{
			GuiSliderFlags_None = 0,
			GuiSliderFlags_AlwaysClamp = 1 << 4,       // Clamp value to min/max bounds when input manually with CTRL+Click. By default CTRL+Click allows going out of bounds.
			GuiSliderFlags_Logarithmic = 1 << 5,       // Make the widget logarithmic (linear otherwise). Consider using GuiSliderFlags_NoRoundToFormat with this if using a format-string with small amount of digits.
			GuiSliderFlags_NoRoundToFormat = 1 << 6,       // Disable rounding underlying value to match precision of the display format string (e.g. %.3f values are rounded to those 3 digits)
			GuiSliderFlags_NoInput = 1 << 7,       // Disable CTRL+Click or Enter key allowing to input text directly into the widget
			GuiSliderFlags_InvalidMask_ = 0x7000000F    // [Internal] We treat using those bits as being potentially a 'float power' argument from the previous API that has got miscast to this enum, and will trigger an assert if needed.

		};

		enum GuiColorEditFlags_
		{
			GuiColorEditFlags_None = 0,
			GuiColorEditFlags_NoAlpha = 1 << 1,   //              // ColorEdit, ColorPicker, ColorButton: ignore Alpha component (will only read 3 components from the input pointer).
			GuiColorEditFlags_NoPicker = 1 << 2,   //              // ColorEdit: disable picker when clicking on color square.
			GuiColorEditFlags_NoOptions = 1 << 3,   //              // ColorEdit: disable toggling options menu when right-clicking on inputs/small preview.
			GuiColorEditFlags_NoSmallPreview = 1 << 4,   //              // ColorEdit, ColorPicker: disable color square preview next to the inputs. (e.g. to show only the inputs)
			GuiColorEditFlags_NoInputs = 1 << 5,   //              // ColorEdit, ColorPicker: disable inputs sliders/text widgets (e.g. to show only the small preview color square).
			GuiColorEditFlags_NoTooltip = 1 << 6,   //              // ColorEdit, ColorPicker, ColorButton: disable tooltip when hovering the preview.
			GuiColorEditFlags_NoLabel = 1 << 7,   //              // ColorEdit, ColorPicker: disable display of inline text label (the label is still forwarded to the tooltip and picker).
			GuiColorEditFlags_NoSidePreview = 1 << 8,   //              // ColorPicker: disable bigger color preview on right side of the picker, use small color square preview instead.
			GuiColorEditFlags_NoDragDrop = 1 << 9,   //              // ColorEdit: disable drag and drop target. ColorButton: disable drag and drop source.
			GuiColorEditFlags_NoBorder = 1 << 10,  //              // ColorButton: disable border (which is enforced by default)

			// User Options (right-click on widget to change some of them).
			GuiColorEditFlags_AlphaBar = 1 << 16,  //              // ColorEdit, ColorPicker: show vertical alpha bar/gradient in picker.
			GuiColorEditFlags_AlphaPreview = 1 << 17,  //              // ColorEdit, ColorPicker, ColorButton: display preview as a transparent color over a checkerboard, instead of opaque.
			GuiColorEditFlags_AlphaPreviewHalf = 1 << 18,  //              // ColorEdit, ColorPicker, ColorButton: display half opaque / half checkerboard, instead of opaque.
			GuiColorEditFlags_HDR = 1 << 19,  //              // (WIP) ColorEdit: Currently only disable 0.0f..1.0f lits in RGBA edition (note: you probably want to use GuiColorEditFlags_Float flag as well).
			GuiColorEditFlags_DisplayRGB = 1 << 20,  // [Display]    // ColorEdit: override _display_ type among RGB/HSV/Hex. ColorPicker: select any combination using one or more of RGB/HSV/Hex.
			GuiColorEditFlags_DisplayHSV = 1 << 21,  // [Display]    // "
			GuiColorEditFlags_DisplayHex = 1 << 22,  // [Display]    // "
			GuiColorEditFlags_Uint8 = 1 << 23,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0..255.
			GuiColorEditFlags_Float = 1 << 24,  // [DataType]   // ColorEdit, ColorPicker, ColorButton: _display_ values formatted as 0.0f..1.0f floats instead of 0..255 integers. No round-trip of value via integers.
			GuiColorEditFlags_PickerHueBar = 1 << 25,  // [Picker]     // ColorPicker: bar for Hue, rectangle for Sat/Value.
			GuiColorEditFlags_PickerHueWheel = 1 << 26,  // [Picker]     // ColorPicker: wheel for Hue, triangle for Sat/Value.
			GuiColorEditFlags_InputRGB = 1 << 27,  // [Input]      // ColorEdit, ColorPicker: input and output data in RGB format.
			GuiColorEditFlags_InputHSV = 1 << 28,  // [Input]      // ColorEdit, ColorPicker: input and output data in HSV format.

			// Defaults Options. You can set application defaults using SetColorEditOptions(). The intent is that you probably don't want to
			// override them in most of your calls. Let the user choose via the option menu and/or call SetColorEditOptions() once during startup.
			GuiColorEditFlags__OptionsDefault = GuiColorEditFlags_Uint8 | GuiColorEditFlags_DisplayRGB | GuiColorEditFlags_InputRGB | GuiColorEditFlags_PickerHueBar,

			// [Internal] Masks
			GuiColorEditFlags__DisplayMask = GuiColorEditFlags_DisplayRGB | GuiColorEditFlags_DisplayHSV | GuiColorEditFlags_DisplayHex,
			GuiColorEditFlags__DataTypeMask = GuiColorEditFlags_Uint8 | GuiColorEditFlags_Float,
			GuiColorEditFlags__PickerMask = GuiColorEditFlags_PickerHueWheel | GuiColorEditFlags_PickerHueBar,
			GuiColorEditFlags__InputMask = GuiColorEditFlags_InputRGB | GuiColorEditFlags_InputHSV

		};

		enum GuiButtonFlags_
		{
			GuiButtonFlags_None = 0,
			GuiButtonFlags_MouseButtonLeft = 1 << 0,   // React on left mouse button (default)
			GuiButtonFlags_MouseButtonRight = 1 << 1,   // React on right mouse button
			GuiButtonFlags_MouseButtonMiddle = 1 << 2,   // React on center mouse button

			// [Internal]
			GuiButtonFlags_MouseButtonMask_ = GuiButtonFlags_MouseButtonLeft | GuiButtonFlags_MouseButtonRight | GuiButtonFlags_MouseButtonMiddle,
			GuiButtonFlags_MouseButtonDefault_ = GuiButtonFlags_MouseButtonLeft
		};

		enum GuiStyleVar_
		{
			// Enum name --------------------- // Member in GuiStyle structure (see GuiStyle for descriptions)
			GuiStyleVar_Alpha,               // float     Alpha
			GuiStyleVar_WindowPadding,       // Vec2    WindowPadding
			GuiStyleVar_WindowRounding,      // float     WindowRounding
			GuiStyleVar_WindowBorderSize,    // float     WindowBorderSize
			GuiStyleVar_WindowMinSize,       // Vec2    WindowMinSize
			GuiStyleVar_WindowTitleAlign,    // Vec2    WindowTitleAlign
			GuiStyleVar_ChildRounding,       // float     ChildRounding
			GuiStyleVar_ChildBorderSize,     // float     ChildBorderSize
			GuiStyleVar_PopupRounding,       // float     PopupRounding
			GuiStyleVar_PopupBorderSize,     // float     PopupBorderSize
			GuiStyleVar_FramePadding,        // Vec2    FramePadding
			GuiStyleVar_FrameRounding,       // float     FrameRounding
			GuiStyleVar_FrameBorderSize,     // float     FrameBorderSize
			GuiStyleVar_ItemSpacing,         // Vec2    ItemSpacing
			GuiStyleVar_ItemInnerSpacing,    // Vec2    ItemInnerSpacing
			GuiStyleVar_IndentSpacing,       // float     IndentSpacing
			GuiStyleVar_ScrollbarSize,       // float     ScrollbarSize
			GuiStyleVar_ScrollbarRounding,   // float     ScrollbarRounding
			GuiStyleVar_GrabMinSize,         // float     GrabMinSize
			GuiStyleVar_GrabRounding,        // float     GrabRounding
			GuiStyleVar_TabRounding,         // float     TabRounding
			GuiStyleVar_ButtonTextAlign,     // Vec2    ButtonTextAlign
			GuiStyleVar_SelectableTextAlign, // Vec2    SelectableTextAlign
			GuiStyleVar_COUNT
		};

		enum GuiWindowFlags_
		{
			GuiWindowFlags_None = 0,
			GuiWindowFlags_NoTitleBar = 1 << 0,   // Disable title-bar
			GuiWindowFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
			GuiWindowFlags_NoMove = 1 << 2,   // Disable user moving the window
			GuiWindowFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
			GuiWindowFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
			GuiWindowFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it
			GuiWindowFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
			GuiWindowFlags_NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Silar as using SetNextWindowBgAlpha(0.0f).
			GuiWindowFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
			GuiWindowFlags_NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
			GuiWindowFlags_MenuBar = 1 << 10,  // Has a menu-bar
			GuiWindowFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(Vec2(width,0.0f)); prior to calling Begin() to specify width. Read code in gui_demo in the "Horizontal Scrolling" section.
			GuiWindowFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
			GuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
			GuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
			GuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
			GuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
			GuiWindowFlags_NoNavInputs = 1 << 18,  // No gamepad/keyboard navigation within the window
			GuiWindowFlags_NoNavFocus = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
			GuiWindowFlags_UnsavedDocument = 1 << 20,  // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. When used in a tab/docking context, tab is selected on closure and closure is deferred by one frame to allow code to cancel the closure (with a confirmation popup, etc.) without flicker.
			GuiWindowFlags_NoNav = GuiWindowFlags_NoNavInputs | GuiWindowFlags_NoNavFocus,
			GuiWindowFlags_NoDecoration = GuiWindowFlags_NoTitleBar | GuiWindowFlags_NoResize | GuiWindowFlags_NoScrollbar | GuiWindowFlags_NoCollapse,
			GuiWindowFlags_NoInputs = GuiWindowFlags_NoMouseInputs | GuiWindowFlags_NoNavInputs | GuiWindowFlags_NoNavFocus,

			// [Internal]
			GuiWindowFlags_NavFlattened = 1 << 23,  // [BETA] Allow gamepad/keyboard navigation to cross over parent border to this child (only use on child that have no scrolling!)
			GuiWindowFlags_ChildWindow = 1 << 24,  // Don't use! For internal use by BeginChild()
			GuiWindowFlags_Tooltip = 1 << 25,  // Don't use! For internal use by BeginTooltip()
			GuiWindowFlags_Popup = 1 << 26,  // Don't use! For internal use by BeginPopup()
			GuiWindowFlags_Modal = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
			GuiWindowFlags_ChildMenu = 1 << 28   // Don't use! For internal use by BeginMenu()

			// [Obsolete]
			//GuiWindowFlags_ResizeFromAnySide    = 1 << 17,  // --> Set io.ConfigWindowsResizeFromEdges=true and make sure mouse cursors are supported by backend (io.BackendFlags & GuiBackendFlags_HasMouseCursors)
		};


		enum GuiInputTextFlags_
		{
			GuiInputTextFlags_None = 0,
			GuiInputTextFlags_CharsDecal = 1 << 0,   // Allow 0123456789.+-*/
			GuiInputTextFlags_CharsHexadecal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
			GuiInputTextFlags_CharsUppercase = 1 << 2,   // Turn a..z into A..Z
			GuiInputTextFlags_CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
			GuiInputTextFlags_AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
			GuiInputTextFlags_EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every te the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
			GuiInputTextFlags_CallbackCompletion = 1 << 6,   // Callback on pressing TAB (for completion handling)
			GuiInputTextFlags_CallbackHistory = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
			GuiInputTextFlags_CallbackAlways = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
			GuiInputTextFlags_CallbackCharFilter = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
			GuiInputTextFlags_AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
			GuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
			GuiInputTextFlags_NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
			GuiInputTextFlags_AlwaysInsertMode = 1 << 13,  // Insert mode
			GuiInputTextFlags_ReadOnly = 1 << 14,  // Read-only mode
			GuiInputTextFlags_Password = 1 << 15,  // Password mode, display all characters as '*'
			GuiInputTextFlags_NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
			GuiInputTextFlags_CharsScientific = 1 << 17,  // Allow 0123456789.+-*/eE (Scientific notation input)
			GuiInputTextFlags_CallbackResize = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/gui_stdlib.h for an example of using this)
			GuiInputTextFlags_CallbackEdit = 1 << 19,  // Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
			// [Internal]
			GuiInputTextFlags_Multiline = 1 << 20,  // For internal use by InputTextMultiline()
			GuiInputTextFlags_NoMarkEdited = 1 << 21   // For internal use by functions using InputText() before reformatting data
		};


		enum GuiTreeNodeFlags_
		{
			GuiTreeNodeFlags_None = 0,
			GuiTreeNodeFlags_Selected = 1 << 0,   // Draw as selected
			GuiTreeNodeFlags_Framed = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
			GuiTreeNodeFlags_AllowItemOverlap = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
			GuiTreeNodeFlags_NoTreePushOnOpen = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
			GuiTreeNodeFlags_NoAutoOpenOnLog = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
			GuiTreeNodeFlags_DefaultOpen = 1 << 5,   // Default node to be open
			GuiTreeNodeFlags_OpenOnDoubleClick = 1 << 6,   // Need double-click to open node
			GuiTreeNodeFlags_OpenOnArrow = 1 << 7,   // Only open when clicking on the arrow part. If GuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
			GuiTreeNodeFlags_Leaf = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
			GuiTreeNodeFlags_Bullet = 1 << 9,   // Display a bullet instead of arrow
			GuiTreeNodeFlags_FramePadding = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
			GuiTreeNodeFlags_SpanAvailWidth = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
			GuiTreeNodeFlags_SpanFullWidth = 1 << 12,  // Extend hit box to the left-most and right-most edges (bypass the indented area).
			GuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
			//GuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
			GuiTreeNodeFlags_CollapsingHeader = GuiTreeNodeFlags_Framed | GuiTreeNodeFlags_NoTreePushOnOpen | GuiTreeNodeFlags_NoAutoOpenOnLog
		};

		enum GuiPopupFlags_
		{
			GuiPopupFlags_None = 0,
			GuiPopupFlags_MouseButtonLeft = 0,        // For BeginPopupContext*(): open on Left Mouse release. Guaranteed to always be == 0 (same as GuouseButton_Left)
			GuiPopupFlags_MouseButtonRight = 1,        // For BeginPopupContext*(): open on Right Mouse release. Guaranteed to always be == 1 (same as GuouseButton_Right)
			GuiPopupFlags_MouseButtonMiddle = 2,        // For BeginPopupContext*(): open on Middle Mouse release. Guaranteed to always be == 2 (same as GuouseButton_Middle)
			GuiPopupFlags_MouseButtonMask_ = 0x1F,
			GuiPopupFlags_MouseButtonDefault_ = 1,
			GuiPopupFlags_NoOpenOverExistingPopup = 1 << 5,   // For OpenPopup*(), BeginPopupContext*(): don't open if there's already a popup at the same level of the popup stack
			GuiPopupFlags_NoOpenOverItems = 1 << 6,   // For BeginPopupContextWindow(): don't return true when hovering items, only when hovering empty space
			GuiPopupFlags_AnyPopupId = 1 << 7,   // For IsPopupOpen(): ignore the unsigned int parameter and test for any popup.
			GuiPopupFlags_AnyPopupLevel = 1 << 8,   // For IsPopupOpen(): search/test at any level of the popup stack (default test in the current level)
			GuiPopupFlags_AnyPopup = GuiPopupFlags_AnyPopupId | GuiPopupFlags_AnyPopupLevel
		};


		enum GuiSelectableFlags_
		{
			GuiSelectableFlags_None = 0,
			GuiSelectableFlags_DontClosePopups = 1 << 0,   // Clicking this don't close parent popup window
			GuiSelectableFlags_SpanAllColumns = 1 << 1,   // Selectable frame can span all columns (text will still fit in current column)
			GuiSelectableFlags_AllowDoubleClick = 1 << 2,   // Generate press events on double clicks too
			GuiSelectableFlags_Disabled = 1 << 3,   // Cannot be selected, display grayed out text
			GuiSelectableFlags_AllowItemOverlap = 1 << 4    // (WIP) Hit testing to allow subsequent widgets to overlap this one
		};


		enum GuiComboFlags_
		{
			GuiComboFlags_None = 0,
			GuiComboFlags_PopupAlignLeft = 1 << 0,   // Align the popup toward the left by default
			GuiComboFlags_HeightSmall = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
			GuiComboFlags_HeightRegular = 1 << 2,   // Max ~8 items visible (default)
			GuiComboFlags_HeightLarge = 1 << 3,   // Max ~20 items visible
			GuiComboFlags_HeightLargest = 1 << 4,   // As many fitting items as possible
			GuiComboFlags_NoArrowButton = 1 << 5,   // Display on the preview box without the square arrow button
			GuiComboFlags_NoPreview = 1 << 6,   // Display only a square arrow button
			GuiComboFlags_HeightMask_ = GuiComboFlags_HeightSmall | GuiComboFlags_HeightRegular | GuiComboFlags_HeightLarge | GuiComboFlags_HeightLargest
		};


		enum GuiTabBarFlags_
		{
			GuiTabBarFlags_None = 0,
			GuiTabBarFlags_Reorderable = 1 << 0,   // Allow manually dragging tabs to re-order them + New tabs are appended at the end of list
			GuiTabBarFlags_AutoSelectNewTabs = 1 << 1,   // Automatically select new tabs when they appear
			GuiTabBarFlags_TabListPopupButton = 1 << 2,   // Disable buttons to open the tab list popup
			GuiTabBarFlags_NoCloseWithMiddleMouseButton = 1 << 3,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
			GuiTabBarFlags_NoTabListScrollingButtons = 1 << 4,   // Disable scrolling buttons (apply when fitting policy is GuiTabBarFlags_FittingPolicyScroll)
			GuiTabBarFlags_NoTooltip = 1 << 5,   // Disable tooltips when hovering a tab
			GuiTabBarFlags_FittingPolicyResizeDown = 1 << 6,   // Resize tabs when they don't fit
			GuiTabBarFlags_FittingPolicyScroll = 1 << 7,   // Add scroll buttons when tabs don't fit
			GuiTabBarFlags_FittingPolicyMask_ = GuiTabBarFlags_FittingPolicyResizeDown | GuiTabBarFlags_FittingPolicyScroll,
			GuiTabBarFlags_FittingPolicyDefault_ = GuiTabBarFlags_FittingPolicyResizeDown
		};


		enum GuiTabItemFlags_
		{
			GuiTabItemFlags_None = 0,
			GuiTabItemFlags_UnsavedDocument = 1 << 0,   // Append '*' to title without affecting the ID, as a convenience to avoid using the ### operator. Also: tab is selected on closure and closure is deferred by one frame to allow code to undo it without flicker.
			GuiTabItemFlags_SetSelected = 1 << 1,   // Trigger flag to programmatically make the tab selected when calling BeginTabItem()
			GuiTabItemFlags_NoCloseWithMiddleMouseButton = 1 << 2,   // Disable behavior of closing tabs (that are submitted with p_open != NULL) with middle mouse button. You can still repro this behavior on user's side with if (IsItemHovered() && IsMouseClicked(2)) *p_open = false.
			GuiTabItemFlags_NoPushId = 1 << 3,   // Don't call PushID(tab->ID)/PopID() on BeginTabItem()/EndTabItem()
			GuiTabItemFlags_NoTooltip = 1 << 4,   // Disable tooltip for the given tab
			GuiTabItemFlags_NoReorder = 1 << 5,   // Disable reordering this tab or having another tab cross over this tab
			GuiTabItemFlags_Leading = 1 << 6,   // Enforce the tab position to the left of the tab bar (after the tab list popup button)
			GuiTabItemFlags_Trailing = 1 << 7    // Enforce the tab position to the right of the tab bar (before the scrolling buttons)
		};


		enum GuiFocusedFlags_
		{
			GuiFocusedFlags_None = 0,
			GuiFocusedFlags_ChildWindows = 1 << 0,   // IsWindowFocused(): Return true if any children of the window is focused
			GuiFocusedFlags_RootWindow = 1 << 1,   // IsWindowFocused(): Test from root window (top most parent of the current hierarchy)
			GuiFocusedFlags_AnyWindow = 1 << 2,   // IsWindowFocused(): Return true if any window is focused. portant: If you are trying to tell how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMouse' instead! Please read the FAQ!
			GuiFocusedFlags_RootAndChildWindows = GuiFocusedFlags_RootWindow | GuiFocusedFlags_ChildWindows
		};

		enum GuiHoveredFlags_
		{
			GuiHoveredFlags_None = 0,        // Return true if directly over the item/window, not obstructed by another window, not obstructed by an active popup or modal blocking inputs under them.
			GuiHoveredFlags_ChildWindows = 1 << 0,   // IsWindowHovered() only: Return true if any children of the window is hovered
			GuiHoveredFlags_RootWindow = 1 << 1,   // IsWindowHovered() only: Test from root window (top most parent of the current hierarchy)
			GuiHoveredFlags_AnyWindow = 1 << 2,   // IsWindowHovered() only: Return true if any window is hovered
			GuiHoveredFlags_AllowWhenBlockedByPopup = 1 << 3,   // Return true even if a popup window is normally blocking access to this item/window
			//GuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 4,   // Return true even if a modal popup window is normally blocking access to this item/window. FIXME-TODO: Unavailable yet.
			GuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 5,   // Return true even if an active item is blocking access to this item/window. Useful for Drag and Drop patterns.
			GuiHoveredFlags_AllowWhenOverlapped = 1 << 6,   // Return true even if the position is obstructed or overlapped by another window
			GuiHoveredFlags_AllowWhenDisabled = 1 << 7,   // Return true even if the item is disabled
			GuiHoveredFlags_RectOnly = GuiHoveredFlags_AllowWhenBlockedByPopup | GuiHoveredFlags_AllowWhenBlockedByActiveItem | GuiHoveredFlags_AllowWhenOverlapped,
			GuiHoveredFlags_RootAndChildWindows = GuiHoveredFlags_RootWindow | GuiHoveredFlags_ChildWindows
		};

		enum GuiCol_
		{
			GuiCol_Text,
			GuiCol_TextDisabled,
			GuiCol_WindowBg,              
			GuiCol_ChildBg,               
			GuiCol_PopupBg,               
			GuiCol_Border,
			GuiCol_BorderShadow,
			GuiCol_FrameBg,               
			GuiCol_FrameBgHovered,
			GuiCol_FrameBgActive,
			GuiCol_TitleBg,
			GuiCol_TitleBgActive,
			GuiCol_TitleBgCollapsed,
			GuiCol_MenuBarBg,
			GuiCol_ScrollbarBg,
			GuiCol_ScrollbarGrab,
			GuiCol_ScrollbarGrabHovered,
			GuiCol_ScrollbarGrabActive,
			GuiCol_CheckMark,
			GuiCol_SliderGrab,
			GuiCol_SliderGrabActive,
			GuiCol_Button,
			GuiCol_ButtonHovered,
			GuiCol_ButtonActive,
			GuiCol_Header,                
			GuiCol_HeaderHovered,
			GuiCol_HeaderActive,
			GuiCol_Separator,
			GuiCol_SeparatorHovered,
			GuiCol_SeparatorActive,
			GuiCol_ResizeGrip,
			GuiCol_ResizeGripHovered,
			GuiCol_ResizeGripActive,
			GuiCol_Tab,
			GuiCol_TabHovered,
			GuiCol_TabActive,
			GuiCol_TabUnfocused,
			GuiCol_TabUnfocusedActive,
			GuiCol_PlotLines,
			GuiCol_PlotLinesHovered,
			GuiCol_PlotHistogram,
			GuiCol_PlotHistogramHovered,
			GuiCol_TextSelectedBg,
			GuiCol_DragDropTarget,
			GuiCol_NavHighlight,         
			GuiCol_NavWindowingHighlight,
			GuiCol_NavWindowingDBg,    
			GuiCol_ModalWindowDBg,     
			GuiCol_COUNT

		};

		enum GuiDragDropFlags_
		{
			GuiDragDropFlags_None = 0,
			// BeginDragDropSource() flags
			GuiDragDropFlags_SourceNoPreviewTooltip = 1 << 0,   
			GuiDragDropFlags_SourceNoDisableHover = 1 << 1,   
			GuiDragDropFlags_SourceNoHoldToOpenOthers = 1 << 2,  
			GuiDragDropFlags_SourceAllowNullID = 1 << 3,   // Allow items such as Text(), age() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear gui ecosystem and so we made it explicit.
			GuiDragDropFlags_SourceExtern = 1 << 4,   // External source (from outside of dear gui), won't attempt to read current item/window info. Will always return true. Only one Extern source can be active sultaneously.
			GuiDragDropFlags_SourceAutoExpirePayload = 1 << 5,   // Automatically expire the payload if the source cease to be submitted (otherwise payloads are persisting while being dragged)
			// AcceptDragDropPayload() flags
			GuiDragDropFlags_AcceptBeforeDelivery = 1 << 10,  // AcceptDragDropPayload() will returns true even before the mouse button is released. You can then call IsDelivery() to test if the payload needs to be delivered.
			GuiDragDropFlags_AcceptNoDrawDefaultRect = 1 << 11,  // Do not draw the default highlight rectangle when hovering over target.
			GuiDragDropFlags_AcceptNoPreviewTooltip = 1 << 12,  // Request hiding the BeginDragDropSource tooltip from the BeginDragDropTarget site.
			GuiDragDropFlags_AcceptPeekOnly = GuiDragDropFlags_AcceptBeforeDelivery | GuiDragDropFlags_AcceptNoDrawDefaultRect  // For peeking ahead and inspecting the payload before delivery.
		};


		enum GuiDataType_
		{
			GuiDataType_S8,       // signed char / char (with sensible compilers)
			GuiDataType_U8,       // unsigned char
			GuiDataType_S16,      // short
			GuiDataType_U16,      // unsigned short
			GuiDataType_S32,      // int
			GuiDataType_U32,      // unsigned int
			GuiDataType_S64,      // long long / __int64
			GuiDataType_U64,      // unsigned long long / unsigned __int64
			GuiDataType_Float,    // float
			GuiDataType_Double,   // double
			GuiDataType_COUNT
		};


		enum GuiDir_
		{
			GuiDir_None = -1,
			GuiDir_Left = 0,
			GuiDir_Right = 1,
			GuiDir_Up = 2,
			GuiDir_Down = 3,
			GuiDir_COUNT
		};


		enum GuiKey_
		{
			GuiKey_Tab,
			GuiKey_LeftArrow,
			GuiKey_RightArrow,
			GuiKey_UpArrow,
			GuiKey_DownArrow,
			GuiKey_PageUp,
			GuiKey_PageDown,
			GuiKey_Home,
			GuiKey_End,
			GuiKey_Insert,
			GuiKey_Delete,
			GuiKey_Backspace,
			GuiKey_Space,
			GuiKey_Enter,
			GuiKey_Escape,
			GuiKey_KeyPadEnter,
			GuiKey_A,                 // for text edit CTRL+A: select all
			GuiKey_C,                 // for text edit CTRL+C: copy
			GuiKey_V,                 // for text edit CTRL+V: paste
			GuiKey_X,                 // for text edit CTRL+X: cut
			GuiKey_Y,                 // for text edit CTRL+Y: redo
			GuiKey_Z,                 // for text edit CTRL+Z: undo
			GuiKey_COUNT
		};

		// To test io.KeyMods (which is a combination of individual fields io.KeyCtrl, io.KeyShift, io.KeyAlt set by user/backend)
		enum GuiKeyModFlags_
		{
			GuiKeyModFlags_None = 0,
			GuiKeyModFlags_Ctrl = 1 << 0,
			GuiKeyModFlags_Shift = 1 << 1,
			GuiKeyModFlags_Alt = 1 << 2,
			GuiKeyModFlags_Super = 1 << 3
		};

		
		enum GuiNavInput_
		{
			// Gamepad Mapping
			GuiNavInput_Activate,      // activate / open / toggle / tweak value       // e.g. Cross  (PS4), A (Xbox), A (Switch), Space (Keyboard)
			GuiNavInput_Cancel,        // cancel / close / exit                        // e.g. Circle (PS4), B (Xbox), B (Switch), Escape (Keyboard)
			GuiNavInput_Input,         // text input / on-screen keyboard              // e.g. Triang.(PS4), Y (Xbox), X (Switch), Return (Keyboard)
			GuiNavInput_Menu,          // tap: toggle menu / hold: focus, move, resize // e.g. Square (PS4), X (Xbox), Y (Switch), Alt (Keyboard)
			GuiNavInput_DpadLeft,      // move / tweak / resize window (w/ PadMenu)    // e.g. D-pad Left/Right/Up/Down (Gamepads), Arrow keys (Keyboard)
			GuiNavInput_DpadRight,     //
			GuiNavInput_DpadUp,        //
			GuiNavInput_DpadDown,      //
			GuiNavInput_LStickLeft,    // scroll / move window (w/ PadMenu)            // e.g. Left Analog Stick Left/Right/Up/Down
			GuiNavInput_LStickRight,   //
			GuiNavInput_LStickUp,      //
			GuiNavInput_LStickDown,    //
			GuiNavInput_FocusPrev,     // next window (w/ PadMenu)                     // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
			GuiNavInput_FocusNext,     // prev window (w/ PadMenu)                     // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)
			GuiNavInput_TweakSlow,     // slower tweaks                                // e.g. L1 or L2 (PS4), LB or LT (Xbox), L or ZL (Switch)
			GuiNavInput_TweakFast,     // faster tweaks                                // e.g. R1 or R2 (PS4), RB or RT (Xbox), R or ZL (Switch)

			// [Internal] Don't use directly! This is used internally to differentiate keyboard from gamepad inputs for behaviors that require to differentiate them.
			// Keyboard behavior that have no corresponding gamepad mapping (e.g. CTRL+TAB) will be directly reading from io.KeysDown[] instead of io.NavInputs[].
			GuiNavInput_KeyMenu_,      // toggle menu                                  // = io.KeyAlt
			GuiNavInput_KeyLeft_,      // move left                                    // = Arrow keys
			GuiNavInput_KeyRight_,     // move right
			GuiNavInput_KeyUp_,        // move up
			GuiNavInput_KeyDown_,      // move down
			GuiNavInput_COUNT,
			GuiNavInput_InternalStart_ = GuiNavInput_KeyMenu_
		};


		enum GuiConfigFlags_
		{
			GuiConfigFlags_None = 0,
			GuiConfigFlags_NavEnableKeyboard = 1 << 0,   // Master keyboard navigation enable flag. NewFrame() will automatically fill io.NavInputs[] based on io.KeysDown[].
			GuiConfigFlags_NavEnableGamepad = 1 << 1,   // Master gamepad navigation enable flag. This is mostly to instruct your gui backend to fill io.NavInputs[]. Backend also needs to set GuiBackendFlags_HasGamepad.
			GuiConfigFlags_NavEnableSetMousePos = 1 << 2,   // Instruct navigation to move the mouse cursor. May be useful on TV/console systems where moving a virtual mouse is awkward. Will update io.MousePos and set io.WantSetMousePos=true. If enabled you MUST honor io.WantSetMousePos requests in your backend, otherwise Gui will react as if the mouse is jumping around back and forth.
			GuiConfigFlags_NavNoCaptureKeyboard = 1 << 3,   // Instruct navigation to not set the io.WantCaptureKeyboard flag when io.NavActive is set.
			GuiConfigFlags_NoMouse = 1 << 4,   // Instruct gui to clear mouse position/buttons in NewFrame(). This allows ignoring the mouse information set by the backend.
			GuiConfigFlags_NoMouseCursorChange = 1 << 5,   // Instruct backend to not alter mouse cursor shape and visibility. Use if the backend cursor changes are interfering with yours and you don't want to use SetMouseCursor() to change mouse cursor. You may want to honor requests from gui by reading GetMouseCursor() yourself instead.

			// User storage (to allow your backend/engine to communicate to code that may be shared between multiple projects. Those flags are not used by core Dear Gui)
			GuiConfigFlags_IsSRGB = 1 << 20,  // Application is SRGB-aware.
			GuiConfigFlags_IsTouchScreen = 1 << 21   // Application is using a touch screen instead of a mouse.
		};

		// Backend capabilities flags stored in io.BackendFlags. Set by gui_pl_xxx or custom backend.
		enum GuiBackendFlags_
		{
			GuiBackendFlags_None = 0,
			GuiBackendFlags_HasGamepad = 1 << 0,   // Backend Platform supports gamepad and currently has one connected.
			GuiBackendFlags_HasMouseCursors = 1 << 1,   // Backend Platform supports honoring GetMouseCursor() value to change the OS cursor shape.
			GuiBackendFlags_HasSetMousePos = 1 << 2,   // Backend Platform supports io.WantSetMousePos requests to reposition the OS mouse position (only used if GuiConfigFlags_NavEnableSetMousePos is set).
			GuiBackendFlags_RendererHasVtxOffset = 1 << 3    // Backend Renderer supports DrawCmd::VtxOffset. This enables output of large meshes (64K+ vertices) while still using 16-bit indices.
		};

		// Enumeration for PushStyleColor() / PopStyleColor()
		


		static char objectName[128];
		static char newObjectName[128];
		static char cbufferName[128];
		static char tagName[128];
		static void ObjectNameReset() {
			memset(objectName, 0, sizeof(objectName));
		}
		static void NewObjectNameReset() {
			memset(newObjectName, 0, sizeof(newObjectName));
		}
		static void CBufferNameReset() {
			memset(cbufferName, 0, sizeof(cbufferName));
		}
		static void TagNameReset() {
			memset(tagName, 0, sizeof(tagName));
		}


		struct GuiIO
		{

			int    ConfigFlags;            
			int   BackendFlags;            
			Vector2      DisplaySize;      
			float       DeltaTime;         
			float       IniSavingRate;     
			const char* IniFilename;                  
			const char* LogFilename;                  
			float       MouseDoubleClickTime;         
			float       MouseDoubleClickMaxDist;      
			float       MouseDragThreshold;           
			int         KeyMap[22];         
			float       KeyRepeatDelay;               
			float       KeyRepeatRate;                
			void* UserData;                       
			float       FontGlobalScale;              
			bool        FontAllowUserScaling;         
			
			Vector2      DisplayFramebufferScale;     

			// Miscellaneous options
			bool        MouseDrawCursor;                
			bool        ConfigMacOSXBehaviors;          
			bool        ConfigInputTextCursorBlink;     
			bool        ConfigWindowsResizeFromEdges;   
			bool        ConfigWindowsMoveFromTitleBarOnly;
			float       ConfigWindowsMemoryCompactTimer;

			// Optional: Platform/Renderer backend name (informational only! will be displayed in About Window) + User data for backend/wrappers to store their own stuff.
			const char* BackendPlatformName;   
			const char* BackendRendererName;   
			void* BackendPlatformUserData;     
			void* BackendRendererUserData;     
			void* BackendLanguageUserData;     

			
			
			const char* (*GetClipboardTextFn)(void* user_data);
			void        (*SetClipboardTextFn)(void* user_data, const char* text);
			void* ClipboardUserData;

			
			
			void        (*eSetInputScreenPosFn)(int x, int y);
			void* eWindowHandle;               


			Vector2      MousePos;                    
			bool        MouseDown[5];                 
			float       MouseWheel;                   
			float       MouseWheelH;                  
			bool        KeyCtrl;                      
			bool        KeyShift;                     
			bool        KeyAlt;                       
			bool        KeySuper;                     
			bool        KeysDown[512];                
			float       NavInputs[21]; 

			// Functions              
			bool        WantCaptureMouse;             
			bool        WantCaptureKeyboard;          
			bool        WantTextInput;                
			bool        WantSetMousePos;              
			bool        WantSaveIniSettings;          
			bool        NavActive;                    
			bool        NavVisible;                   
			float       Framerate;                    
			int         MetricsRenderVertices;        
			int         MetricsRenderIndices;         
			int         MetricsRenderWindows;         
			int         MetricsActiveWindows;         
			int         MetricsActiveAllocations;     
			Vector2     MouseDelta;                   


			int KeyMods;                   
			Vector2      MousePosPrev;                 
			Vector2      MouseClickedPos[5];           
			double      MouseClickedTime[5];           
			bool        MouseClicked[5];               
			bool        MouseDoubleClicked[5];         
			bool        MouseReleased[5];              
			bool        MouseDownOwned[5];             
			bool        MouseDownWasDoubleClick[5];    
			float       MouseDownDuration[5];          
			float       MouseDownDurationPrev[5];      
			Vector2      MouseDragMaxDistanceAbs[5];   
			float       MouseDragMaxDistanceSqr[5];    
			float       KeysDownDuration[512];         
			float       KeysDownDurationPrev[512];     
			float       NavInputsDownDuration[21];
			float       NavInputsDownDurationPrev[21];
			float       PenPressure;                   
			unsigned short   InputQueueSurrogate;      
		};


		bool          Begin(const std::string& name, bool* p_open = nullptr, GuiWindowFlags flags = 0);
		void          End();

		bool          BeginChild(const std::string& str_id, const Vector2& size = Vector2(0, 0), bool border = false, GuiWindowFlags flags = 0);
		bool          BeginChild(unsigned int id, const Vector2& size = Vector2(0, 0), bool border = false, GuiWindowFlags flags = 0);
		void          EndChild();

		// Windows Utilities
		// - 'current window' = the window we are appending into while inside a Begin()/End() block. 'next window' = next window we will Begin() into.
		bool          IsWindowAppearing();
		bool          IsWindowCollapsed();
		bool          IsWindowFocused(GuiFocusedFlags flags = 0); // is current window focused? or its root/child, depending on flags. see flags for options.
		bool          IsWindowHovered(GuiHoveredFlags flags = 0); // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to gui or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
		
		Vector2        GetWindowPos();                             // get current window position in screen space (useful if you want to do your own drawing via the DrawList API)
		Vector2        GetWindowSize();                            // get current window size
		float         GetWindowWidth();                           // get current window width (shortcut for GetWindowSize().x)
		float         GetWindowHeight();                          // get current window height (shortcut for GetWindowSize().y)

		// Content region
		// - Those functions are bound to be redesigned soon (they are confusing, incomplete and return values in local window coordinates which increases confusion)
		Vector2        GetContentRegionMax();                                          // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
		Vector2        GetContentRegionAvail();                                        // == GetContentRegionMax() - GetCursorPos()
		Vector2        GetWindowContentRegionMin();                                    // content boundaries min (roughly (0,0)-Scroll), in window coordinates
		Vector2        GetWindowContentRegionMax();                                    // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
		float         GetWindowContentRegionWidth();                                  //


		// Parameters stacks (current window)
		void          PushItemWidth(float item_width);                                // push width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side). 0.0f = default to ~2/3 of windows width,
		void          PopItemWidth();
		void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
		float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position. NOT necessarily the width of last item unlike most 'Item' functions.
		void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // push word-wrapping position for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
		void          PopTextWrapPos();
		void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
		void          PopAllowKeyboardFocus();
		void          PushButtonRepeat(bool repeat);                                  // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
		void          PopButtonRepeat();

		// Cursor / Layout
		// - By "cursor" we mean the current output position.
		// - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
		// - You can call SameLine() between widgets to undo the last carriage return and output at the right of the preceding widget.
		// - Attention! We currently have inconsistencies between window-local and absolute positions we will a to fix with future API:
		//    Window-local coordinates:   SameLine(), GetCursorPos(), SetCursorPos(), GetCursorStartPos(), GetContentRegionMax(), GetWindowContentRegion*(), PushTextWrapPos()
		//    Absolute coordinate:        GetCursorScreenPos(), SetCursorScreenPos(), all DrawList:: functions.
		void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
		void          SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
		void          NewLine();                                                      // undo a SameLine() or force a new line when in an horizontal-layout context.
		void          Spacing();                                                      // add vertical spacing.
		void          Dummy(const Vector2& size);                                      // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
		void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
		void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by style.IndentSpacing or indent_w if != 0
		void          BeginGroup();                                                   // lock horizontal starting position
		void          EndGroup();
		void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
		float         GetTextLineHeight();                                            // ~ FontSize
		float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
		float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
		float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

		// ID stack/scopes
		// - Read the FAQ for more details about how ID are handled in dear gui. If you are creating widgets in a loop you most
		//   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
		// - The resulting ID are hashes of the entire stack.
		// - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
		// - In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
		//   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
		void          PushID(const std::string& str_id);                                     // push string into the ID stack (will hash string).
		void          PushID(const std::string& str_id_begin, const std::string& str_id_end);       // push string into the ID stack (will hash string).
		void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
		void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
		void          PopID();                                                        // pop from the ID stack.
		unsigned int       GetID(const std::string& str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into GuiStorage yourself
		unsigned int       GetID(const std::string& str_id_begin, const std::string& str_id_end);
		unsigned int       GetID(const void* ptr_id);



		bool InputText(const std::string& label, char* buf, size_t buf_size, GuiInputTextFlags flags = 0,  void* user_data = NULL);
		bool InputTextMultiline(const std::string& label, char* buf, size_t buf_size, const Vector2& size = Vector2(0, 0), GuiInputTextFlags flags = 0,  void* user_data = NULL);
		bool InputTextWithHint(const std::string& label, const std::string& hint, char* buf, size_t buf_size, GuiInputTextFlags flags = 0, void* user_data = NULL);
		bool InputFloat(const std::string& label, float& v, float step = 0.0f, float step_fast = 0.0f, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat2(const std::string& label, Vector2& v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat3(const std::string& label, Vector3& v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat4(const std::string& label, Vector4& v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat(const std::string& label, float* v, float step = 0.0f, float step_fast = 0.0f, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat2(const std::string& label, float* v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat3(const std::string& label, float* v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputFloat4(const std::string& label, float* v, const std::string& format = "%.3f", GuiInputTextFlags flags = 0);
		bool InputInt(const std::string& label, int& v, int step = 1, int step_fast = 100, GuiInputTextFlags flags = 0);
		bool InputInt2(const std::string& label, int v[2], GuiInputTextFlags flags = 0);
		bool InputInt3(const std::string& label, int v[3], GuiInputTextFlags flags = 0);
		bool InputInt4(const std::string& label, int v[4], GuiInputTextFlags flags = 0);
		bool InputDouble(const std::string& label, double& v, double step = 0.0, double step_fast = 0.0, const std::string& format = "%.6f", GuiInputTextFlags flags = 0);

		bool DragFloat(const std::string& label, float& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat2(const std::string& label, Vector2& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat3(const std::string& label, Vector3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat4(const std::string& label, Vector4& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat(const std::string& label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat2(const std::string& label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat3(const std::string& label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloat4(const std::string& label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool DragFloatRange2(const std::string& label, float& v_current_min, float& v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const std::string& format = "%.3f", const std::string& format_max = NULL, GuiSliderFlags flags = 0);
		bool DragInt(const std::string& label, int& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool DragInt(const std::string& label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool DragInt2(const std::string& label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool DragInt3(const std::string& label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool DragInt4(const std::string& label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool DragIntRange2(const std::string& label, int& v_current_min, int& v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const std::string& format = "%d", const std::string& format_max = NULL, GuiSliderFlags flags = 0);


		bool SliderFloat(const std::string& label, float& v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat2(const std::string& label, Vector2& v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat3(const std::string& label, Vector3& v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat4(const std::string& label, Vector4& v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat(const std::string& label, float* v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat2(const std::string& label, float* v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat3(const std::string& label, float* v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderFloat4(const std::string& label, float* v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool SliderAngle(const std::string& label, float& v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const std::string& format = "%.0f deg", GuiSliderFlags flags = 0);
		bool SliderInt(const std::string& label, int& v, int v_min, int v_max, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool SliderInt2(const std::string& label, int v[2], int v_min, int v_max, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool SliderInt3(const std::string& label, int v[3], int v_min, int v_max, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool SliderInt4(const std::string& label, int v[4], int v_min, int v_max, const std::string& format = "%d", GuiSliderFlags flags = 0);
		bool SliderScalar(const std::string& label, int data_type, void* p_data, const void* p_min, const void* p_max, const std::string& format = NULL, GuiSliderFlags flags = 0);
		bool SliderScalarN(const std::string& label, int data_type, void* p_data, int components, const void* p_min, const void* p_max, const std::string& format = NULL, GuiSliderFlags flags = 0);
		bool VSliderFloat(const std::string& label, const Vector2& size, float& v, float v_min, float v_max, const std::string& format = "%.3f", GuiSliderFlags flags = 0);
		bool VSliderInt(const std::string& label, const Vector2& size, int& v, int v_min, int v_max, const std::string& format = "%d", GuiSliderFlags flags = 0);


		void TextUnformatted(const std::string& text, const std::string& text_end = "");
		void Text(const std::string& fmt, ...);
		void TextV(const std::string& fmt, va_list args);
		void TextColored(const Vector4& col, const std::string& fmt, ...);
		void TextColoredV(const Vector4& col, const std::string& fmt, va_list args);
		void TextDisabled(const std::string& fmt, ...);
		void TextDisabledV(const std::string& fmt, va_list args);
		void TextWrapped(const std::string& fmt, ...);
		void TextWrappedV(const std::string& fmt, va_list args);
		void LabelText(const std::string& label, const std::string& fmt, ...);
		void LabelTextV(const std::string& label, const std::string& fmt, va_list args);
		void BulletText(const std::string& fmt, ...);
		void BulletTextV(const std::string& fmt, va_list args);

		bool Button(const std::string& label, const Vector2& size = Vector2(0, 0));
		bool SmallButton(const std::string& label);
		bool InvisibleButton(const std::string& str_id, const Vector2& size, GuiButtonFlags flags = 0);
		bool ArrowButton(const std::string& str_id, GuiDir dir);   

		bool Checkbox(const std::string& label, bool* v);
		bool CheckboxFlags(const std::string& label, unsigned int& flags, unsigned int flags_value);
		bool RadioButton(const std::string& label, bool active);                   
		bool RadioButton(const std::string& label, int& v, int v_button);           
		void ProgressBar(float fraction, const Vector2& size_arg = Vector2(-1, 0), const std::string& overlay = NULL);
		void Bullet();                                                      


		bool BeginCombo(const std::string& label, const std::string& preview_value, GuiComboFlags flags = 0);
		void EndCombo();
		bool Combo(const std::string& label, int& current_item, const std::vector< std::string>& items, int items_count, int popup_max_height_in_items = -1);
		bool Combo(const std::string& label, int& current_item, const std::string& items_separated_by_zeros, int popup_max_height_in_items = -1);     
		bool Combo(const std::string& label, int& current_item, bool(*items_getter)(void* data, int idx, const char**  out_text), void* data, int items_count, int popup_max_height_in_items = -1);


		bool TreeNode(const std::string& label);
		bool TreeNode(const std::string& str_id, const std::string& fmt, ...);
		bool TreeNode(const void* ptr_id, const std::string& fmt, ...);
		bool TreeNodeV(const std::string& str_id, const std::string& fmt, va_list args);
		bool TreeNodeV(const void* ptr_id, const std::string& fmt, va_list args);
		bool TreeNodeEx(const std::string& label, GuiTreeNodeFlags flags = 0);
		bool TreeNodeEx(const std::string& str_id, GuiTreeNodeFlags flags, const std::string& fmt, ...);
		bool TreeNodeEx(const void* ptr_id, GuiTreeNodeFlags flags, const std::string& fmt, ...);
		bool TreeNodeExV(const std::string& str_id, GuiTreeNodeFlags flags, const std::string& fmt, va_list args);
		bool TreeNodeExV(const void* ptr_id, GuiTreeNodeFlags flags, const std::string& fmt, va_list args);
		void TreePush(const std::string& str_id);
		void TreePush(const void* ptr_id = nullptr);
		void TreePop();
		float GetTreeNodeToLabelSpacing();
		bool CollapsingHeader(const std::string& label, GuiTreeNodeFlags flags = 0);
		bool CollapsingHeader(const std::string& label, bool* p_open, GuiTreeNodeFlags flags = 0);
		void SetNextItemOpen(bool is_open, int cond = 0);

		bool  Selectable(const std::string& label, bool selected = false, GuiSelectableFlags flags = 0, const Vector2& size = Vector2(0, 0));
		bool  Selectable(const std::string& label, bool* p_selected, GuiSelectableFlags flags = 0, const Vector2& size = Vector2(0, 0));


		bool ListBox(const std::string& label, int& current_item, const std::vector< std::string>& arg_item,  int height_in_items = -1);
		bool ListBox(const std::string& label, int& current_item, char** arg_item, int items_count, int height_in_items = -1);
		bool ListBox(const std::string& label, int& current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
		bool ListBoxHeader(const std::string& label, const Vector2& size = Vector2(0, 0)); 
		bool ListBoxHeader(const std::string& label, int items_count, int height_in_items = -1); 
		void ListBoxFooter(); 

		void PlotLines(const std::string& label, const float& values, int values_count, int values_offset = 0, const std::string& overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2(0, 0), int stride = sizeof(float));
		void PlotLines(const std::string& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const std::string& overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2(0, 0));
		void PlotHistogram(const std::string& label, const float& values, int values_count, int values_offset = 0, const std::string& overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2(0, 0), int stride = sizeof(float));
		void PlotHistogram(const std::string& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const std::string& overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2 graph_size = Vector2(0, 0));
		

		void Value(const std::string& prefix, bool b);
		void Value(const std::string& prefix, int v);
		void Value(const std::string& prefix, unsigned int v);
		void Value(const std::string& prefix, float v, const std::string& float_format = NULL);

		void EndMenuBar();
		bool BeginMainMenuBar();
		void EndMainMenuBar();
		bool BeginMenu(const std::string& label, bool enabled = true);
		void EndMenu();
		bool MenuItem(const std::string& label, const std::string& shortcut = "", bool selected = false, bool enabled = true);
		bool MenuItem(const std::string& label, const std::string& shortcut, bool* p_selected, bool enabled = true);

		void BeginTooltip();
		void EndTooltip();
		void SetTooltip(const std::string& fmt, ...);
		void SetTooltipV(const std::string& fmt, va_list args);

		bool BeginPopup(const std::string& str_id, GuiPopupFlags flags = 0);
		bool BeginPopupModal(const std::string& name, bool* p_open = NULL, GuiPopupFlags flags = 0);
		void EndPopup();

		void OpenPopup(const std::string& str_id, GuiPopupFlags popup_flags = 0);
		void OpenPopupOnItemClick(const std::string& str_id = NULL, GuiPopupFlags popup_flags = 1);
		void CloseCurrentPopup();

		bool BeginPopupContextItem(const std::string& str_id = NULL, GuiPopupFlags popup_flags = 1);
		bool BeginPopupContextWindow(const std::string& str_id = NULL, GuiPopupFlags popup_flags = 1);
		bool BeginPopupContextVoid(const std::string& str_id = NULL, GuiPopupFlags popup_flags = 1);

		bool IsPopupOpen(const std::string& str_id, GuiPopupFlags flags = 0);

		void Columns(int count = 1, const std::string& id = NULL, bool border = true);
		void NextColumn();
		int  GetColumnIndex();
		float GetColumnWidth(int column_index = -1);
		void SetColumnWidth(int column_index, float width);
		float GetColumnOffset(int column_index = -1);
		void SetColumnOffset(int column_index, float offset_x);
		int  GetColumnsCount();

		bool BeginTabBar(const std::string& str_id, GuiTabBarFlags flags = 0);
		void EndTabBar();
		bool BeginTabItem(const std::string& label, bool* p_open = NULL, GuiTabBarFlags flags = 0);
		void EndTabItem();
		bool TabItemButton(const std::string& label, GuiTabBarFlags flags = 0);
		void SetTabItemClosed(const std::string& tab_or_docked_window_label);

		void LogToTTY(int auto_open_depth = -1);
		void LogToFile(int auto_open_depth = -1, const std::string& filename = "");
		void LogToClipboard(int auto_open_depth = -1);
		void LogFinish();
		void LogButtons();
		void LogText(const std::string& fmt, ...);

		void PushClipRect(const Vector2& clip_rect_min, const Vector2& clip_rect_max, bool intersect_with_current_clip_rect);
		void PopClipRect();

		void SetItemDefaultFocus();
		void SetKeyboardFocusHere(int offset = 0);

		bool IsItemHovered(GuiHoveredFlags flags = 0);
		bool IsItemActive();
		bool IsItemFocused();
		bool IsItemClicked(int mouse_button = 0);
		bool IsItemVisible();
		bool IsItemEdited();
		bool IsItemActivated();
		bool IsItemDeactivated();
		bool IsItemDeactivatedAfterEdit();
		bool IsItemToggledOpen();
		bool IsAnyItemHovered();
		bool IsAnyItemActive();
		bool IsAnyItemFocused();
		Vector2        GetItemRectMin();
		Vector2        GetItemRectMax();
		Vector2        GetItemRectSize();
		void          SetItemAllowOverlap();

		bool IsRectVisible(const Vector2& size);
		bool IsRectVisible(const Vector2& rect_min, const Vector2& rect_max);
		double  GetTime();
		int  GetFrameCount();

		std::string GetStyleColorName(GuiCol idx);

		void CalcListClipping(int items_count, float items_height, int& out_items_display_start, int& out_items_display_end);
		bool BeginChildFrame(unsigned int id, const Vector2& size, GuiWindowFlags flags = 0);
		void EndChildFrame();

		Vector2 CalcTextSize(const std::string& text, const std::string& text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
		

		Vector4 ColorConvertU32ToFloat4(unsigned int in);
		unsigned int  ColorConvertFloat4ToU32(const Vector4& in);
		void   ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
		void   ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

		void   CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);

		void CaptureMouseFromApp(bool want_capture_mouse_value = true);

		std::string GetClipboardText();
		void SetClipboardText(const std::string& text);

		void LoadIniSettingsFromDisk(const std::string& ini_filename);
		void LoadIniSettingsFromMemory(const std::string& ini_data, size_t ini_size = 0);
		void SaveIniSettingsToDisk(const std::string& ini_filename);
		std::string SaveIniSettingsToMemory(size_t* out_ini_size = nullptr);
		void  SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL);
		void* MemAlloc(size_t size);
		void  MemFree(void* ptr);

		unsigned int GetColorU32(GuiCol idx, float alpha_mul = 1.0f);             
		unsigned int GetColorU32(const Vector4& col);                               
		unsigned int GetColorU32(unsigned int col);

		bool  ColorEdit3(const std::string& label, Vector3& col, GuiColorEditFlags flags = 0);
		bool  ColorEdit4(const std::string& label, Vector4& col, GuiColorEditFlags flags = 0);
		bool  ColorPicker3(const std::string& label, Vector3& col, GuiColorEditFlags flags = 0);
		bool  ColorPicker4(const std::string& label, Vector4& col, GuiColorEditFlags flags = 0, const float* ref_col = nullptr);
		bool  ColorButton(const std::string& desc_id, const Vector4& col, GuiColorEditFlags flags = 0, Vector2 size = Vector2(0, 0)); // display a color square/button, hover for details, return true when pressed.
		void  SetColorEditOptions(GuiColorEditFlags flags);

		void Line(const Vector2& p1, const Vector2& p2, unsigned int col, float thickness);

		float GetFontSize();

		GuiIO GetIO();
	};
}