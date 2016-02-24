#if !defined STREAMER_TYPE_OBJECT
    #error streamer-tp requires streamer to be included.
#endif

static
    Float:TP_AwaitingPos[MAX_PLAYERS][3],
    TP_AwaitingTime[MAX_PLAYERS];

static Float:TP_GetDistanceFromAwaiting(playerid)
{
    new Float:pX, Float:pY, Float:pZ;
    GetPlayerPos(playerid, pX, pY, pZ);
    return (floatsqroot(floatpower(pX - TP_AwaitingPos[playerid][0], 2.0) + floatpower(pY - TP_AwaitingPos[playerid][1], 2.0) + floatpower(pZ - TP_AwaitingPos[playerid][2], 2.0)));
}

public OnPlayerConnect(playerid)
{
    TP_AwaitingPos[playerid][0] = 0.0;
    
	#if defined TP_OnPlayerConnect
	    return TP_OnPlayerConnect(playerid);
	#else
	    return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect TP_OnPlayerConnect

public OnPlayerUpdate(playerid)
{
    if (_:TP_AwaitingPos[playerid][0] && (!TP_AwaitingTime[playerid] || TP_AwaitingTime[playerid] > GetTickCount())) {
        if (TP_GetDistanceFromAwaiting(playerid) < 0.2) {
            TogglePlayerControllable(playerid, true);
            Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_OBJECT, true);
            
            TP_AwaitingPos[playerid][0] = 0.0;
            TP_AwaitingTime[playerid] = 0;
        }
    }
    
	#if defined TP_OnPlayerUpdate
	    return TP_OnPlayerUpdate(playerid);
	#else
	    return 1;
	#endif
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate TP_OnPlayerUpdate

stock SetPlayerCompensatedPos(playerid, Float:X, Float:Y, Float:Z, wait_time = 1000)
{
    if (!IsPlayerConnected(playerid))
        return 0;
    
    TP_AwaitingPos[playerid][0] = X;
    TP_AwaitingPos[playerid][1] = Y;
    TP_AwaitingPos[playerid][2] = Z;
    
    TP_AwaitingTime[playerid] = wait_time ? (GetTickCount() + wait_time) : 0;
    
    TogglePlayerControllable(playerid, false);
    Streamer_UpdateEx(playerid, X, Y, Z, -1, -1, STREAMER_TYPE_OBJECT);
    Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_OBJECT, false);
    
    return SetPlayerPos(playerid, X, Y, Z - 0.3);
}
