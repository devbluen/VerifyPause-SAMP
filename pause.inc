
		// Include
#include <YSI_Coding\y_hooks>

		// Forwards
forward UpdatePause_Tick(playerid);
forward OnPlayerPause(playerid);
forward OnPlayerResume(playerid, time);

static enum E_PAUSE {
	E_PAUSE_TIMER,
	bool:E_PAUSE_UPDATE,
	bool:E_PAUSE_PAUSED,
	E_PAUSE_TICK,
	E_PAUSE_RATE
};

static e_Pause[MAX_PLAYERS][E_PAUSE];

hook OnPlayerConnect(playerid) {
	e_Pause[playerid][E_PAUSE_TIMER] = SetTimerEx("UpdatePause_Tick", 360, true, "i", playerid);
	return true;
}

hook OnPlayerDisconnect(playerid, reason) {
	KillTimer(e_Pause[playerid][E_PAUSE_TIMER]);
	return true;
}

hook OnPlayerUpdate(playerid) {

	if(!e_Pause[playerid][E_PAUSE_UPDATE])
		e_Pause[playerid][E_PAUSE_UPDATE] = true;

	e_Pause[playerid][E_PAUSE_RATE] = GetConnectedTime(playerid);
	return true;
}

public UpdatePause_Tick(playerid) {

	switch(GetPlayerState(playerid)) {
		case PLAYER_STATE_ONFOOT, PLAYER_STATE_DRIVER, PLAYER_STATE_PASSENGER: {

			if(e_Pause[playerid][E_PAUSE_UPDATE]) {
				if(e_Pause[playerid][E_PAUSE_PAUSED]) {
					CallLocalFunction("OnPlayerResume", "ii", playerid, GetConnectedTime(playerid) - e_Pause[playerid][E_PAUSE_TICK]);
				}

				e_Pause[playerid][E_PAUSE_PAUSED] = false;
				e_Pause[playerid][E_PAUSE_UPDATE] = false;
			}

			if(!e_Pause[playerid][E_PAUSE_PAUSED] && (GetConnectedTime(playerid) - e_Pause[playerid][E_PAUSE_RATE]) >= 2000) {
				CallLocalFunction("OnPlayerPause", "i", playerid);
				e_Pause[playerid][E_PAUSE_TICK] = GetConnectedTime(playerid);
				e_Pause[playerid][E_PAUSE_PAUSED] = true;
			}
		}
	}
	return true;
}

stock GetConnectedTime(playerid)
	return (NetStats_GetConnectedTime(playerid));

stock bool:IsPlayerPaused(playerid)
	return (e_Pause[playerid][E_PAUSE_PAUSED]);
