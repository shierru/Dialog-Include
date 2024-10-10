#if defined _INC_dialog
    #endinput
#endif
#define _INC_dialog

/**
 * <library name="dialog" summary="Include for dialog">
 *   <license>
 *     (c) Copyright 2024, shierru (Nikita).
 *   </license>
 *   <summary pawndoc="true">
 *     This library uses the enhanced <em>pawndoc.xsl</em> from
 *     <a href="https://github.com/pawn-lang/pawndoc">pawn-lang/pawndoc</a>.
 *     This XSL has features such as library and markdown support, and will not
 *     render this message when used.
 *   </summary>
 * </library>
 */

/// <p/>

#pragma tabsize 4

/**
 * <library>dialog</library>
 */
#if !defined DEFAULT_DIALOG_ID
    #define DEFAULT_DIALOG_ID    (32000)
#endif

/**
 * <library>dialog</library>
 */
#if !defined 
    #define MAX_DIALOG_FUNCTION  (__namemax)
#endif

/**
 * <library>dialog</library>
 */
#if !defined isnull
	#define isnull(%1)          ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

/**
 * <library>dialog</library>
 */
#define DialogCreate:%0(%1)     forward @D_CREATE_%0(%1);public @D_CREATE_%0(%1)
#define DialogResponse:%0(%1)   forward @D_RESPONSE_%0(%1);public @D_RESPONSE_%0(%1)
#define DialogInterrupt:%0(%1)  forward @D_INTERRUPT_%0(%1);public @D_INTERRUPT_%0(%1) 

#define Dialog: #

/**
 * <library>dialog</library>
 */
#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}
#define CHAIN_NEXT(%0) @CO_%0

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

CHAIN_HOOK(DIALOG)
#undef CHAIN_ORDER
#define CHAIN_ORDER CHAIN_NEXT(DIALOG)

static stock _DIALOG_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _DIALOG_IncludeStates() <_ALS : _ALS_go> {}

/**
 * <library>dialog</library>
 * <summary>Call Types</summary>
 */
enum DIALOG_CALL_TYPES
{
    DIALOG_CALL_TYPE_UNKNOWN            = -1,
    DIALOG_CALL_TYPE_NONE,
    DIALOG_CALL_TYPE_CREATE,
    DIALOG_CALL_TYPE_RESPONSE,
    DIALOG_CALL_TYPE_INTERRUPT
};
#define MAX_DIALOG_CALL_TYPES           (DIALOG_CALL_TYPES)

#define DIALOG_CALL_TYPE_UNKNOWN        (DIALOG_CALL_TYPES:-1)
#define DIALOG_CALL_TYPE_NONE           (DIALOG_CALL_TYPES:0)
#define DIALOG_CALL_TYPE_CREATE         (DIALOG_CALL_TYPES:1)
#define DIALOG_CALL_TYPE_RESPONSE       (DIALOG_CALL_TYPES:2)
#define DIALOG_CALL_TYPE_INTERRUPT      (DIALOG_CALL_TYPES:3)

/**
 * <library>dialog</library>
 * <summary>Static variables</summary>
 */
static 
    s_PlayerAddress[MAX_PLAYERS][MAX_DIALOG_CALL_TYPES] = {cellmin, ...},
    s_PlayerAddressDefault[MAX_DIALOG_CALL_TYPES] = {cellmin, ...};

static s_PlayerFuncName[MAX_PLAYERS][MAX_DIALOG_FUNCTION] = {"", ...};

static bool:s_IsPlayerOpenDialog[MAX_PLAYERS] = {false, ...};

/**
 * <library>dialog</library>
 * <summary>Show the player a "create" dialog.</summary>
 * <param name="playerid">The identifier of the player you want to show the dialog from</param>
 * <param name="func">The name of the dialog to show to the player</param>
 */
stock bool:Dialog_Show(playerid, const func[])
{
    Dialog_Process(playerid, func, DIALOG_CALL_TYPE_CREATE);

    Dialog_Call(playerid, DIALOG_CALL_TYPE_CREATE, playerid);

    return true;
}

/**
 * <library>dialog</library>
 * <summary>Show the player a dialog.</summary>
 * <param name="playerid">The identifier of the player you want to show the dialog from</param>
 * <param name="func">The name of the dialog to show to the player</param>
 * <param name="style">The style of the dialog</param>
 * <param name="title">The title at the top of the dialog.  The length of the title can not exceed
 * more than 64 characters before it starts to cut off</param>
 * <param name="body">The text to display in the main dialog.  Use <b><c>\n</c></b> to start a new line
 * and <b><c>\t</c></b> to tabulate.  May be optionally formatted.</param>
 * <param name="button1">The text on the left button</param>
 * <param name="button2">The text on the right button.  Leave it blank ( "" ) to hide it</param>
 */
stock bool:Dialog_Open(playerid, const func[], const DIALOG_STYLE:style, const caption[], const info[], const button1[], const button2[], va_args<>)
{
	memcpy(s_PlayerFuncName[playerid], func, 0, (strlen(func) * cellbytes));

	// Let's try to get the addresses of Response and Interrupt functions and memorize them.
    Dialog_Process(playerid, func, DIALOG_CALL_TYPE_RESPONSE);
    Dialog_Process(playerid, func, DIALOG_CALL_TYPE_INTERRUPT);

    // Format
    if(YSI_CheckNumargs__(7))
        return ShowPlayerDialog(playerid, DEFAULT_DIALOG_ID, style, caption, info, button1, button2);

    format(UNSAFE_HUGE_STRING, UNSAFE_HUGE_LENGTH, info, va_start<7>);
	return ShowPlayerDialog(playerid, DEFAULT_DIALOG_ID, style, caption, UNSAFE_HUGE_STRING, button1, button2);
}

/**
 * <library>dialog</library>
 * <summary>Close the dialog to the player.</summary>
 * <param name="playerid">The identifier of the player you want to close the dialog from</param>
 */
stock bool:Dialog_Close(playerid)
{
    if(!Dialog_IsOpen(playerid))
        return false;

	// Call the Interrupt function
	Dialog_Call(playerid, DIALOG_CALL_TYPE_INTERRUPT, playerid);

	s_PlayerAddress[playerid] = s_PlayerAddressDefault;

	s_IsPlayerOpenDialog[playerid] = false;

    #if defined HidePlayerDialog
        return HidePlayerDialog(playerid);
    #else
        return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, " ", " ", " ", "");
    #endif
}

/**
 * <library>dialog</library>
 * <summary>Check if the player has a dialog open.</summary>
 * <param name="playerid">Identifier of the player for whom you want to check</param>
 * <param name="func">The name of dialog</param>
 */
stock bool:Dialog_IsOpen(playerid, const func[] = "")
{
	if(isnull(func))
		return s_IsPlayerOpenDialog[playerid];

    return !strcmp(s_PlayerFuncName[playerid], func);
}

// Static func's
static stock void:Dialog_Process(playerid, const func[], DIALOG_CALL_TYPES:calltype)
{
    if(isnull(func))
        return;

    new name[MAX_DIALOG_FUNCTION];

    switch(calltype)
    {
        case DIALOG_CALL_TYPE_CREATE:
            strcat(name, "@D_CREATE_");

        case DIALOG_CALL_TYPE_RESPONSE:
            strcat(name, "@D_RESPONSE_");

        case DIALOG_CALL_TYPE_INTERRUPT:
            strcat(name, "@D_INTERRUPT_");

        default: { return; }
    }
    strcat(name, func);

    s_PlayerAddress[playerid][calltype] = GetPublicAddressFromIndex(funcidx(name));
}

static stock void:Dialog_Call(playerid, DIALOG_CALL_TYPES:calltype, va_args<>)
{
    if(!Dialog_IsAddresValid(playerid, calltype))
        return;

    if(YSI_CheckNumargs__(2))
        CallFunction(s_PlayerAddress[playerid][calltype]);
    else
        CallFunction(s_PlayerAddress[playerid][calltype], va_start<2>);
}

static stock bool:Dialog_IsAddresValid(playerid, DIALOG_CALL_TYPES:calltype)
{
	return (s_PlayerAddress[playerid][calltype] != cellmin ? true : false);
}

// Hook's
stock Dialog_ShowPlayerDialog(playerid, dialogid, DIALOG_STYLE:style, const title[], const body[], const button1[], const button2[])
{
	if(dialogid != DEFAULT_DIALOG_ID)
		return 0;

	s_IsPlayerOpenDialog[playerid] = true;

    return Dialog_ShowPlayerDialog(playerid, dialogid, style, title, body, button1, button2);
}
#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog Dialog_ShowPlayerDialog

// Callback's
public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid != DEFAULT_DIALOG_ID)
        return 0;
	
    Dialog_Call(playerid, DIALOG_CALL_TYPE_RESPONSE, playerid, response, listitem, ref(inputtext));

    return Dialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}
#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif
#define OnDialogResponse(%0) CHAIN_PUBLIC:Dialog_OnDialogResponse(%0)
CHAIN_FORWARD:Dialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) = 1;

public OnPlayerDisconnect(playerid, reason)
{
    s_PlayerAddress[playerid] = s_PlayerAddressDefault;

	s_PlayerFuncName[playerid][0] = '\0';

	s_IsPlayerOpenDialog[playerid] = false;

    return Dialog_OnPlayerDisconnect(playerid, reason);
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:Dialog_OnPlayerDisconnect(%0)
CHAIN_FORWARD:Dialog_OnPlayerDisconnect(playerid, reason) = 1;