#include <string>
#include <ctime>
#include <cmath>
#include "script.h"

#define INI_Name "./DMZ.ini"


#pragma warning(disable : 4244 4305) // double <-> float conversions
#pragma warning(disable : 4996)


void draw_rect(float A_0, float A_1, float A_2, float A_3, int Red, int Green, int Blue, int Alpha)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, Red, Green, Blue, Alpha);
}

void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true)
{
	// default values
	int text_col[4] = { 255,255,255,255 },
		rect_col[4] = { 0,0,0,175 };
	float text_scale = 0.350;
	int font = 0;
	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 255;
		text_col[2] = 0;
		text_col[3] = 225;

		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;
		rect_col[3] = 200;

		if (rescaleText) text_scale = 0.355;
	}
	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;
		rect_col[3] = 0;
		if (rescaleText)text_scale = 0.60;
		font = 7;
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height
	float lineLeftScaled = lineLeft / (float)screen_w;
	//center menu
	UI::SET_TEXT_CENTRE(1);
	//
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_OUTLINE();
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));
	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_OUTLINE();
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled*.5f, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));
	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.001f),
		lineWidthScaled, ((((float)(num25)*UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled*.350f)) + 0.0045f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
}


bool get_key_pressed(int nVirtKey)
{
	//return (GetKeyState(nVirtKey) & 0x8000) != 0;
	return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
}

void notifyAboveMap(char* msg)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(msg);
	UI::_DRAW_NOTIFICATION(FALSE, FALSE);
}

DWORD trainerResetTime = 0;

bool trainer_switch_pressed()
{
	int openMenu = GetPrivateProfileInt("CONTROLS", "Trainer_Key", 0x78, INI_Name);
	return (GetTickCount() > trainerResetTime + 1000) && get_key_pressed(openMenu);
}

void reset_trainer_switch()
{
	trainerResetTime = GetTickCount();
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	if (a) *a = get_key_pressed(VK_NUMPAD5);
	if (b) *b = get_key_pressed(VK_NUMPAD0);
	if (up) *up = get_key_pressed(VK_NUMPAD8);
	if (down) *down = get_key_pressed(VK_NUMPAD2);
	if (r) *r = get_key_pressed(VK_NUMPAD6);
	if (l) *l = get_key_pressed(VK_NUMPAD4);
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NO", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_FONT(7);
		UI::SET_TEXT_OUTLINE();
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(0, 204, 0, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(0);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		}
		else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void notifyAboveMap_Status_Text(char* msg, char* Name){
	char statusText[32];
	sprintf_s(statusText, msg, Name);
	notifyAboveMap(statusText);
}

void RequestControl(Entity input)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(input);

	int tick = 0;
	while (tick <= 50)
	{
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(input))
			WAIT(0);
		else
			return;
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(input);
		tick++;
	}
}

void RequestControlOfid(Entity selectedPlayer)
{
	Any netid = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(selectedPlayer);
	int tick = 0;

	while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 50)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
		tick++;
	}
}



void shootAtPed(Player selectedPlayer)
{
	Vector3 Mouth = PED::GET_PED_BONE_COORDS(selectedPlayer, SKEL_Head, 0.1f, 0.0f, 0.0f);
	PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), Mouth.x, Mouth.y, Mouth.z, true);
}

void BruteForceWeaponAddons(Ped ped, Hash weaponHash, bool bSilencer)
{
	static Hash weaponAddons[] = { COMPONENT_AT_SCOPE_MACRO, COMPONENT_AT_SCOPE_MACRO_02, COMPONENT_AT_SCOPE_SMALL, COMPONENT_AT_SCOPE_SMALL_02, COMPONENT_AT_SCOPE_MEDIUM, COMPONENT_AT_SCOPE_LARGE, COMPONENT_AT_SCOPE_MAX, COMPONENT_AT_RAILCOVER_01, COMPONENT_AT_AR_AFGRIP, COMPONENT_AT_PI_FLSH, COMPONENT_AT_AR_FLSH, COMPONENT_PISTOL_CLIP_02, COMPONENT_COMBATPISTOL_CLIP_02, COMPONENT_APPISTOL_CLIP_02, COMPONENT_MICROSMG_CLIP_02, COMPONENT_SMG_CLIP_02, COMPONENT_ASSAULTRIFLE_CLIP_02, COMPONENT_CARBINERIFLE_CLIP_02, COMPONENT_ADVANCEDRIFLE_CLIP_02, COMPONENT_MG_CLIP_02, COMPONENT_COMBATMG_CLIP_02, COMPONENT_ASSAULTSHOTGUN_CLIP_02, COMPONENT_PISTOL50_CLIP_02, COMPONENT_ASSAULTSMG_CLIP_02, COMPONENT_SNSPISTOL_CLIP_02, COMPONENT_COMBATPDW_CLIP_02, COMPONENT_HEAVYPISTOL_CLIP_02, COMPONENT_SPECIALCARBINE_CLIP_02, COMPONENT_BULLPUPRIFLE_CLIP_02, COMPONENT_VINTAGEPISTOL_CLIP_02, COMPONENT_MARKSMANRIFLE_CLIP_02, COMPONENT_HEAVYSHOTGUN_CLIP_02, COMPONENT_GUSENBERG_CLIP_02 };
	for each (Hash addonHash in weaponAddons)
	{
		if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(weaponHash, addonHash))
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, addonHash);
	}
	if (bSilencer)
	{
		static Hash silencers[] = { COMPONENT_AT_PI_SUPP, COMPONENT_AT_PI_SUPP_02, COMPONENT_AT_AR_SUPP, COMPONENT_AT_SR_SUPP, COMPONENT_AT_AR_SUPP_02 };
		for each (Hash silencerHash in silencers)
		{
			if (WEAPON::_CAN_WEAPON_HAVE_COMPONENT(weaponHash, silencerHash)) {
				if (weaponHash != WEAPON_ADVANCEDRIFLE && WEAPON::GET_WEAPONTYPE_GROUP(weaponHash) != WEAPON_TYPE_GROUP_SHOTGUN)
					WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, silencerHash);
			}
		}
		if (weaponHash == WEAPON_SNIPERRIFLE || weaponHash == WEAPON_MICROSMG)
			WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, COMPONENT_AT_AR_SUPP_02);
	}
}

void giveWeapon(Ped selectedPed)
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
	{
		GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "Input your weapon ID", "", "WEAPON_RAILGUN", "", "", "", 30);
		while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
		if (!WEAPON::IS_WEAPON_VALID(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
		{
			notifyAboveMap("~HUD_COLOUR_BLUE~Invalid Weapon");
			return;
		}
		else
		{
			char* weapon = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
			Hash pedWeapon = GAMEPLAY::GET_HASH_KEY(weapon);
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(selectedPed, pedWeapon, 0, true);
			WEAPON::SET_PED_AMMO(selectedPed, pedWeapon, 1000);
			notifyAboveMap_Status_Text("Gave Player a %s", weapon);
		}
	}
}





void vehicleAllMods(Player selectedPlayer)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);
		RequestControl(veh);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, 5);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 3);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 19, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 10, 1, 0);
		notifyAboveMap("~HUD_COLOUR_GREEN~Added all Mods to Players Vehicle");
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Player isnt in Vehicle");
	}
}


void SetOffAlarmofPlayerVehicle(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Entity selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		RequestControl(PED::GET_VEHICLE_PED_IS_USING(selectedPed));
		VEHICLE::SET_VEHICLE_ALARM(selectedVehicle, true);
		VEHICLE::START_VEHICLE_ALARM(selectedVehicle);
		notifyAboveMap("~HUD_COLOUR_GREEN~Set off alarm of Players vehicle");
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Player isnt in a Vehicle");
	}
}



void BurstSelectedPlayerTires(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		notifyAboveMap("~HUD_COLOUR_GREEN~Bursted Players Tires");
		Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		RequestControl(selectedVehicle);
		if (VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(selectedVehicle))
		{
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(selectedVehicle, TRUE);
		}
		static int tireID = 0;
		for (tireID = 0; tireID < 8; tireID++)
		{
			RequestControl(selectedVehicle);
			VEHICLE::SET_VEHICLE_TYRE_BURST(selectedVehicle, tireID, true, 1000.0);
		}
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Player isnt an any vehicle");
	}
}
void framePlayer(Ped selectedPed, char* Name)
{
	for (int i = 0; i < 32; i++)
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		Player playerHandle = PLAYER::GET_PLAYER_PED(i);
		
						if (ENTITY::DOES_ENTITY_EXIST(selectedPed))
						{
							if (!PLAYER::GET_PLAYER_PED(i) == selectedPed) continue;
							notifyAboveMap_Status_Text("Framed %s", Name);
							Vector3 PlyPos1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(i), 0.0, 0.0, 0.0);
							FIRE::ADD_OWNED_EXPLOSION(selectedPed, PlyPos1.x, PlyPos1.y, PlyPos1.z, 37, 5.0, true, false, 0.1);
						}
					}
				}
		

void beachFireEmUp()
{ // prop_beach_fire
	for (int i = 0; i < 32; i++)
	{
		Player playerHandle = PLAYER::GET_PLAYER_PED(i);
		Hash beach_fire = GAMEPLAY::GET_HASH_KEY("prop_beach_fire");
		STREAMING::REQUEST_MODEL(beach_fire);
		while (!STREAMING::HAS_MODEL_LOADED(beach_fire)) WAIT(0);
		notifyAboveMap("~HUD_COLOUR_RED~Attached Beach Fires to All Players");
		if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
		{
			
							if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
							{
								Object object = OBJECT::CREATE_OBJECT(beach_fire, 0, 0, 0, true, true, false);
								ENTITY::ATTACH_ENTITY_TO_ENTITY(object, playerHandle, PED::GET_PED_BONE_INDEX(playerHandle, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
							}
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(beach_fire);
						}
					}
				}
		

void Freeze(Ped selectedPed)
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
	AI::CLEAR_PED_SECONDARY_TASK(selectedPed);
}

void kickPlayerFromVeh(Ped selectedPed)
{
	if (ENTITY::DOES_ENTITY_EXIST(selectedPed))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, 0))
		{
			notifyAboveMap("Kicked Player from Vehicle");
			Freeze(selectedPed);
		}
		else
		{
			notifyAboveMap("Player isnt in a Vehicle");
		}
	}
	else
	{
		notifyAboveMap("Player doesnt Exist");
	}
}

void tracer()
{
	for (int i = 0; i < 32; i++)
	{
		if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
		{
			
							if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED(i)))
							{
								Player playerHandle = PLAYER::GET_PLAYER_PED(i);
								Player playerPed = PLAYER::PLAYER_PED_ID();
								int VehHandle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
								Vector3 getcoords1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
								Vector3 getcoords3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 0, 0);
								GRAPHICS::DRAW_LINE(getcoords1.x, getcoords1.y, getcoords1.z, getcoords3.x, getcoords3.y, getcoords3.z, 255, 0, 0, 255);
							}
						}
					}
				}
	


void trapPlayerInVeh(Ped selectedPed, char* Name)
{
	for (int i = 0; i < 32; i++)
	{
		
						Ped playerOneVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
						if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
						{
							notifyAboveMap("~HUD_COLOUR_GREEN~Kicked Player from his Vehicle");
							RequestControl(playerOneVeh);
							VEHICLE::SET_VEHICLE_DOORS_LOCKED(playerOneVeh, 4);
							VEHICLE::SET_VEHICLE_ENGINE_HEALTH(playerOneVeh, -1);
							VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(playerOneVeh, -1);
						}
						else
						{
							notifyAboveMap("~HUD_COLOUR_GREEN~Player isnt in a Vehicle");
						}
					}
				}
	

void massSuicide()
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
	{
		notifyAboveMap("~HUD_COLOUR_RED~Forced Everyone to Suicide");
		for (int i = 0; i < 32; i++)
		{
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED(i)))
			{
				if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
				{
					
									if (PED::IS_PED_ON_FOOT(PLAYER::GET_PLAYER_PED(i)))
									{
										Player playerHandle = PLAYER::GET_PLAYER_PED(i);
										Vector3 PlyPos1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0.0, 0.0, 0.0);
										FIRE::ADD_OWNED_EXPLOSION(playerHandle, PlyPos1.x, PlyPos1.y, PlyPos1.z, 37, 5.0, false, true, 0.0);
									}
									else
									{
										Player playerHandle = PLAYER::GET_PLAYER_PED(i);
										Freeze(playerHandle);
										WAIT(0);
										Vector3 PlyPos1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0.0, 0.0, 0.0);
										FIRE::ADD_OWNED_EXPLOSION(playerHandle, PlyPos1.x, PlyPos1.y, PlyPos1.z, 37, 5.0, false, true, 0.0);
									}
								}
							}
						}
					}
				}
		



Vector3 GetCoordsFromCam(int distance)
{
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 coord = CAM::GET_GAMEPLAY_CAM_COORD();

    int tZ = rot.z * 0.0174532924;
	int tX = rot.x * 0.0174532924;
	int num = std::abs(std::cos(tX));

	coord.x = coord.x + (-std::sin(tZ)) * (num + distance);
	coord.y = coord.y + (std::cos(tZ)) * (num + distance);
	coord.z = coord.z + std::sin(tX) * 8;

	return coord;
}

void BoostBaseVehicleStats(Vehicle vehicle)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
	ENTITY::SET_ENTITY_INVINCIBLE(vehicle, TRUE); //That should do the trick.
	ENTITY::SET_ENTITY_PROOFS(vehicle, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
	VEHICLE::SET_VEHICLE_STRONG(vehicle, TRUE); //2stronk
	VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(vehicle, !TRUE); //I don't think this really works, but fuck it. Call it anyway.
	VEHICLE::SET_VEHICLE_CAN_BREAK(vehicle, !FALSE); //Hopefully this fixes the car blowing up after getting hit by a train...
	VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(vehicle, !FALSE);
	VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE); //What seems to be the officer, problem? *le9gagmemeface*
	VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, FALSE); //Bulletproof Tires.
	Hash vehicleModel = ENTITY::GET_ENTITY_MODEL(vehicle);
	if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModel) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicleModel))
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0); //IDK what this does, but I guess it allows individual mods to be added? It's what the game does before calling SET_VEHICLE_MOD.
		VEHICLE::SET_VEHICLE_HAS_STRONG_AXLES(vehicle, TRUE); //6stronk9meme
		VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ENGINE, MOD_INDEX_FOUR, TRUE); //6fast9furious
		VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_BRAKES, MOD_INDEX_THREE, TRUE); //GOTTA STOP FAST
		VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_TRANSMISSION, MOD_INDEX_THREE, TRUE); //Not when I shift in to MAXIMUM OVERMEME!
		VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_SUSPENSION, MOD_INDEX_FOUR, TRUE); //How low can you go?
		VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ARMOR, MOD_INDEX_FIVE, TRUE); //100% armor.
		VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TURBO, TRUE); //Forced induction huehuehue
	}
	VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, 1000.0f); //This is what the game does
	notifyAboveMap("~HUD_COLOUR_GREEN~Boosted Base Vehicle stats");
}



void CrashPlayer(Ped selectedPed)
{
	const int maxPeds = 70;
	Ped ClonedPed[maxPeds];
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 selectedPedPosition = ENTITY::GET_ENTITY_COORDS(selectedPed, FALSE);
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);


	// check distance so i dont crash myself
	if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerPosition.x, playerPosition.y, playerPosition.z, selectedPedPosition.x, selectedPedPosition.y, selectedPedPosition.z, false) > 250.0f)
	{
		notifyAboveMap("~HUD_COLOUR_GREEN~This task will take a minute.");
		notifyAboveMap("~HUD_COLOUR_GREEN~Dont worry if the menu disappears its normal");
		//spawn peds
		for (int i = 0; i < maxPeds; i++)
		{
			ClonedPed[i] = PED::CLONE_PED(selectedPed, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
			WAIT(60);
			AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
		}
		//I may need a wait function?

		//and delete the evidence
		for (int i = 0; i < maxPeds; i++)
		{
			RequestControl(ClonedPed[i]);
			PED::DELETE_PED(&ClonedPed[i]);
			AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ClonedPed[i]);
		}
		notifyAboveMap("~HUD_COLOUR_GREEN~Crashed Player");
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~To close to the player your trying to crash!");
		notifyAboveMap("~HUD_COLOUR_RED~Move farther away!");
	}
}


void RemoveAllPropsFromPlayer(Ped ped)
{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	Hash modelHashes[] = { 0x2E28CA22 /*p_tram_crash_s*/, 0xA50DDDD0/*prop_bball_arcade_01*/,
		0xEFC4165A/*prop_food_van_01*/, 0x8E8C7A5B/*prop_crashed_heli*/,
		0x456AA864/*prop_dj_deck_01*/, 0xBE862050/*prop_portacabin01*/,
		0xB20E5785/*prop_sculpt_fix*/, 0x58D3B4EA/*prop_micro_01*/,
		0xC42C019A/*prop_ld_ferris_wheel*/, 0x8AF58425/*prop_lev_des_barge_01*/,
		0x3DC31836/*prop_tv_flat_01*/, 0xA9BD0D16 /*prop_coke_block_01*/,
		0x1AFA6A0A /*Prop_weed_01*/, 0x4B3D240F /*prop_wheelchair_01*/,
		0x40F52369 /*p_v_43_safe_s*/, 0xF830B63E /*prop_swiss_ball_01*/,
		0xD541462D /*p_ld_soc_ball_01*/, 0x532B1DD1 /*prop_rub_trolley01a*/,
		0x0E3BA450 /*prop_xmas_tree_int*/, 0xFB631122 /*prop_bumper_car_01*/,
		0x5571173D /*prop_beer_neon_01*/, 0x6AD326C2 /*prop_space_rifle*/,
		0x7FFBC1E2 /*prop_dummy_01*/, 0x678FC2DB /*prop_wheelchair_01_s*/,
		0x5869A8F8 /*prop_large_gold*/, 0xE6CB661E /*PROP_CS_DILDO_01*/,
		0x2AE13DFA /*prop_armchair_01*/, 0x29CB0F3C /*prop_armour_pickup*/,
		0x922C2A43 /*prop_big_shit_01*/, 0xFA686C0E /*prop_bin_04a*/,
		0x1F550C17 /*prop_chair_01a*/, 0x5B5C4263 /*prop_chip_fryer*/,
		0x39885BB5 /*prop_chickencoop_a*/, 0x16A39A90 /*prop_dog_cage_01*/,
		0xE3CE09E2 /*prop_dummy_plane*/, 0x927A5723 /*prop_fan_01*/,
		0x34D5D3FD /*prop_golf_bag_01*/, 0xB467C540 /*p_spinning_anus_s*/,
		0x745F3383 /*prop_windmill_01*/, 0x392D62AA /*prop_gold_cont_01*/,
		0x07121AC4 /*prop_xmas_ext*/, 0x0E8032E4 /*prop_weed_pallet*/,
		0xD44295DD /*p_cablecar_s*/, 0x6F9939C7 /*prop_ld_toilet_01*/,
		0x9C762726 /*prop_lev_des_barge_02*/, 0x8973A868 /*prop_air_bigradar*/,
		0xC2BC19CD /*p_cs_mp_jet_01_s*/, 651101403 /*garbage bin*/,
		1840863642 /*baskedball */ };
	for each (Hash modelHash in modelHashes)
	{
		Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(playerPosition.x, playerPosition.y, playerPosition.z, 3.0f, modelHash, TRUE, 0, 0);
		if (ENTITY::DOES_ENTITY_EXIST(obj))
		{
			RequestControl(obj);
			ENTITY::DELETE_ENTITY(&obj);
		}
	}
}


void FireLoop(Player selectedPlayer)
{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(selectedPlayer, FALSE);
	Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(playerPosition.x, playerPosition.y, playerPosition.z, 3.0f, GAMEPLAY::GET_HASH_KEY("prop_beach_fire"), TRUE, 0, 0);
	if (ENTITY::DOES_ENTITY_EXIST(obj))
	{
		RequestControl(obj);
		OBJECT::DELETE_OBJECT(&obj);
		notifyAboveMap("~HUD_COLOUR_RED~Fire loop stopped!");
	}
	else
	{
		Hash beach_fire_hash = GAMEPLAY::GET_HASH_KEY("prop_beach_fire");
		STREAMING::REQUEST_MODEL(beach_fire_hash);
		while (!STREAMING::HAS_MODEL_LOADED(beach_fire_hash)) WAIT(0);
		{
			Object beach_fire = OBJECT::CREATE_OBJECT(beach_fire_hash, playerPosition.x, playerPosition.y, playerPosition.z, true, true, false);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(beach_fire, selectedPlayer, PED::GET_PED_BONE_INDEX(selectedPlayer, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
			notifyAboveMap("~HUD_COLOUR_GREEN~Fire loop started!");
		}
	}
}

void teleportPlyToMe(Player selectedPlayer)
{
	Vector3 playerPedPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
	if (ENTITY::DOES_ENTITY_EXIST(selectedPlayer))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
		{
			Vehicle selectedPlayerVeh = PED::GET_VEHICLE_PED_IS_IN(selectedPlayer, 0);
			RequestControl(selectedPlayerVeh);
			ENTITY::SET_ENTITY_COORDS(selectedPlayerVeh, playerPedPos.x, playerPedPos.y, playerPedPos.z, 0, 0, 0, 1);
			notifyAboveMap("~HUD_COLOUR_GREEN~Teleported Player to You");
		}
		else
		{
			notifyAboveMap("~HUD_COLOUR_RED~This function only works if a player is in a vehicle");
		}
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
	}
}


bool IsPlayerFriend(Player player)
{
	BOOL BplayerFriend = false;
	bool bplayerFriend = false;
	int handle[76]; //var num3 = sub_34009(A_0, (A_1) + 264, (A_1) + 272);
	NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
	if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
	{
		BplayerFriend = NETWORK::NETWORK_IS_FRIEND(&handle[0]);
	}
	if (BplayerFriend = 1)
		bplayerFriend = true;
	else
		bplayerFriend = false;

	return bplayerFriend;
}


void AIJackVehicle(Ped selectedPlayer)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, FALSE))
	{
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPlayer);
		WAIT(50);
		Ped Driver = PED::CLONE_PED(selectedPlayer, 0.0f, true, true);
		WAIT(50);
		PED::SET_PED_INTO_VEHICLE(Driver, playerVeh, SEAT_DRIVER);
		PED::SET_PED_DIES_WHEN_INJURED(Driver, false);
		AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(Driver, true);
		WAIT(50);
		AI::TASK_VEHICLE_DRIVE_TO_COORD(Driver, playerVeh, -2000.0f, -1000.0f, 0.0f, 100.0f, 1, ENTITY::GET_ENTITY_MODEL(playerVeh), 0, 0xC00AB, -1);
		notifyAboveMap("~HUD_COLOUR_GREEN~Jacked Players Vehicle");
	}
}

void giveAllWeapons(Player selectedPlayer)
{
	static LPCSTR weaponNames[] = {
		"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
		"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
		"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
		"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
		"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
		"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
		"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
		"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
		"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
		"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN", "WEAPON_FLAREGUN",
		"" };
	for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(selectedPlayer, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 1000, 0);
		BruteForceWeaponAddons(selectedPlayer, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), true);
		WEAPON::SET_PED_WEAPON_TINT_INDEX(selectedPlayer, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), WEAPONTINT_PINK);
		notifyAboveMap("All Weapons Added");
	}
}

void ClearSkiesOfBitches()
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
	{
		notifyAboveMap("~HUD_COLOUR_RED~Cleared the Sky of Bitches");
		for (int i = 0; i < 32; i++)
		{
			
							if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
							{
								if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED(i)))
								{
									if (PED::IS_PED_IN_FLYING_VEHICLE(PLAYER::GET_PLAYER_PED(i)))
									{
										Vehicle playerOneVeh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(i));
										VEHICLE::SET_VEHICLE_DOORS_LOCKED(playerOneVeh, 4);
										VEHICLE::SET_VEHICLE_ENGINE_HEALTH(playerOneVeh, -1);
										VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(playerOneVeh, -1);
									}
								}
							}
						}
					}
				}
	



void killAllBounties()
{
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
	{
		notifyAboveMap("~HUD_COLOUR_RED~Cleared the Sky of Bitches");
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			
							if (playerHandle == PLAYER::PLAYER_PED_ID()) continue;
							{
								if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
								{
									int Blip = UI::GET_BLIP_FROM_ENTITY(playerHandle);
									if (Blip == BLIP_BOUNTY)
									{
										Freeze(playerHandle);
										Vector3 coords = ENTITY::GET_ENTITY_COORDS(playerHandle, 0);
										FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 37, 5.0, true, false, .1);
									}
								}
							}
						}
					}
				}
		


void teleportToPlayer(Ped playerPed, Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		int numSeats = VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(selectedVehicle);
		for (int i = numSeats; i >= -1; i--)
		{
			PED::SET_PED_INTO_VEHICLE(playerPed, selectedVehicle, i);
		}
	}
	else
	{
		if (PED::IS_PED_ON_FOOT(selectedPed))
		{
			Vector3 playerTwoPos = ENTITY::GET_ENTITY_COORDS(selectedPed, 0);
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
			{
				RequestControl(PED::GET_VEHICLE_PED_IS_USING(playerPed));
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), playerTwoPos.x, playerTwoPos.y, playerTwoPos.z + 0.5, 0, 0, 1);
			}
			else
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, playerTwoPos.x, playerTwoPos.y, playerTwoPos.z + 0.5, 0, 0, 1);
			}
		}
	}
}






void spawnPickupModel(char *propName, char *pickupName, Ped selectedPed, int value)
{
	Vector3 ENTPOS = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPed, 0, 0, 0.5);
	static Any health_pack = GAMEPLAY::GET_HASH_KEY(propName);

	STREAMING::REQUEST_MODEL(health_pack);

	while (!STREAMING::HAS_MODEL_LOADED(health_pack)) WAIT(0);

	OBJECT::CREATE_AMBIENT_PICKUP(GAMEPLAY::GET_HASH_KEY(pickupName), ENTPOS.x, ENTPOS.y, ENTPOS.z, 0, value, GAMEPLAY::GET_HASH_KEY(propName), 0, 1);

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(health_pack);
}



void Nuke(Ped selectedPed, char* Name)
{
	for (int i = 0; i < 32; i++)
	{
		
						Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPed, 0.0, 0.0, 0.0);
						notifyAboveMap_Status_Text("Nuked %s", Name);
						if (PED::IS_PED_ON_FOOT(selectedPed))
						{
							FIRE::ADD_OWNED_EXPLOSION(selectedPed, coords.x, coords.y, coords.z, 37, 5.0, true, false, .1);
						}
						else
						{
							if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, 0))
							{
								Freeze(selectedPed);
								FIRE::ADD_OWNED_EXPLOSION(selectedPed, coords.x, coords.y, coords.z, 37, 5.0, true, false, .1);
							}
						}
					}
				}
	








bool featureVehWrapInSpawned = false;
bool featureVehSpawnedMaxMod = false;
void SpawnVehicle(Hash hash, Player selectedPlayer)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash))
	{
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPlayer, 0.0, 5.0, 0.0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), 1, 1);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
		char* plateText = ("DMZREBEL");
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);

		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120);
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rand() % 255, rand() % 255, rand() % 255);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 5);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 14, 2);
		VEHICLE::SET_VEHICLE_MOD(veh, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 10, 1, 0);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 0, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 1, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 2, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 3, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, PLATE_YANKTON);

		WAIT(0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Vehicle hash doesn't exist");
	}
}


void spawnChauffer()
{
	int WaypointHandle = UI::GET_FIRST_BLIP_INFO_ID(8);
	if (UI::DOES_BLIP_EXIST(WaypointHandle))
	{
		Vector3 waypoint1 = UI::GET_BLIP_COORDS(WaypointHandle);
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("marshall"));
		while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("marshall"))) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("marshall"), pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
		if (veh != 0)
		{
			Ped Driver = PED::CREATE_RANDOM_PED_AS_DRIVER(veh, false);
			PED::SET_PED_INTO_VEHICLE(Driver, veh, -1);
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, 0);
			AI::TASK_VEHICLE_DRIVE_TO_COORD(Driver, veh, waypoint1.x, waypoint1.y, waypoint1.z, 40, 1, ENTITY::GET_ENTITY_MODEL(veh), 7, 6, -1);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, PLATE_YANKTON);
			char* plateText = ("   DMZ   ");
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
			notifyAboveMap("~HUD_COLOUR_GREEN~Chauffer Spawned!");
		}
	}
	else
		notifyAboveMap("~HUD_COLOUR_RED~Please choose a waypoint");
}

void Mad_Money()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	if (bPlayerExists)
	{
		ENTITY::SET_ENTITY_COORDS(playerPed, -365.425f, -131.809f, 37.873f, 0, 0, 0, 1);
		WAIT(100);
		Hash hash = GAMEPLAY::GET_HASH_KEY("buffalo2");
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
		VEHICLE::SET_VEHICLE_COLOURS(veh, 120, 120);
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rand() % 255, rand() % 255, rand() % 255);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, 22, 1);
		VEHICLE::SET_VEHICLE_MOD(veh, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 14, 14, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 5);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, 14, 2);
		VEHICLE::SET_VEHICLE_MOD(veh, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(veh, 10, 1, 0);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), 0, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), 1, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), 2, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), 3, 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, PLATE_YANKTON);
		char* plateText = ("   DMZ   ");
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plateText);
		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		WAIT(60);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);


		notifyAboveMap("~HUD_COLOUR_GREEN~Vehicle Spawned! Sell it!");


		STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VEHICLE_SELL_TIME"), 0, true);
		notifyAboveMap("~HUD_COLOUR_GREEN~Sell Time Bypassed!");
		WAIT(10);

	}
}

void VectorToFloat(Vector3 unk, float *Out)
{
	Out[0] = unk.x;
	Out[1] = unk.y;
	Out[2] = unk.z;
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = "~HUD_COLOUR_GREEN~" + str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

void set_permstatus_text(std::string str, bool isGxtEntry = false)
{
	statusText = str;
	statusTextGxtEntry = isGxtEntry;
}



void explodeNearbyVehicles(Player selectedPlayer)
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int vehs[arrSize];
	//0 index is the size of the array
	vehs[0] = numElements;

	int count = PED::GET_PED_NEARBY_VEHICLES(selectedPlayer, vehs);
	for (int i = 0; i < count; ++i)
	{
		int offsettedID = i * 2 + 2;
		if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
		{
			if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_IN(selectedPlayer, 0)) continue;
			{
				if (VEHICLE::GET_VEHICLE_ENGINE_HEALTH(vehs[offsettedID]) > 1)
				{
					Vector3 nearbyPedPos = ENTITY::GET_ENTITY_COORDS(vehs[offsettedID], 0);
					FIRE::ADD_OWNED_EXPLOSION(selectedPlayer, nearbyPedPos.x, nearbyPedPos.y, nearbyPedPos.z, EXPLOSION_STICKYBOMB, 2, 0, 1, 0);
				}
			}
		}
	}
}


void killNearbyPeds(Player selectedPlayer)
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int peds[arrSize];
	//0 index is the size of the array
	peds[0] = numElements;

	int count = PED::GET_PED_NEARBY_PEDS(selectedPlayer, peds, -1);
	for (int i = 0; i < count; ++i)
	{
		int offsettedID = i * 2 + 2;
		if (ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
		{
			if ((peds[offsettedID]) == selectedPlayer) continue;
			{
				if (!ENTITY::IS_ENTITY_DEAD((peds[offsettedID])))
				{
					Vector3 nearbyPedPos = ENTITY::GET_ENTITY_COORDS(peds[offsettedID], 0);
					FIRE::ADD_OWNED_EXPLOSION(selectedPlayer, nearbyPedPos.x, nearbyPedPos.y, nearbyPedPos.z, EXPLOSION_BULLET, 2, 0, 1, 0);
				}
			}
		}
	}
}



void spawn_money_safe(Player selectedPlayer)
{
	Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPlayer, 0.0f, 0.0f, 0.5f);
	Hash PigeonHash = GAMEPLAY::GET_HASH_KEY("a_c_pigeon");
	if (STREAMING::IS_MODEL_IN_CDIMAGE(PigeonHash))
	{
		if (STREAMING::IS_MODEL_VALID(PigeonHash))
		{
			int TenK = 10000;
			STREAMING::REQUEST_MODEL(PigeonHash);
			while (!STREAMING::HAS_MODEL_LOADED(PigeonHash)) WAIT(0);
			Ped pigeonBank = PED::CREATE_PED(26, PigeonHash, coords.x, coords.y + 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);
			Ped pigeonBank2 = PED::CREATE_PED(26, PigeonHash, coords.x, coords.y - 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);

			Ped pigeonBank3 = PED::CREATE_PED(26, PigeonHash, coords.x + 0.15, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);
			Ped pigeonBank4 = PED::CREATE_PED(26, PigeonHash, coords.x - 0.15, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);

			Ped pigeonBank5 = PED::CREATE_PED(26, PigeonHash, coords.x + 0.15, coords.y + 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);
			Ped pigeonBank6 = PED::CREATE_PED(26, PigeonHash, coords.x + 0.15, coords.y - 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);

			Ped pigeonBank7 = PED::CREATE_PED(26, PigeonHash, coords.x - 0.15, coords.y + 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);
			Ped pigeonBank8 = PED::CREATE_PED(26, PigeonHash, coords.x - 0.15, coords.y - 0.15, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);

			Ped pigeonBank9 = PED::CREATE_PED(26, PigeonHash, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(selectedPlayer), TRUE, TRUE);

			PED::SET_PED_MONEY(pigeonBank, TenK);
			PED::SET_PED_MONEY(pigeonBank2, TenK);
			PED::SET_PED_MONEY(pigeonBank3, TenK);
			PED::SET_PED_MONEY(pigeonBank4, TenK);
			PED::SET_PED_MONEY(pigeonBank5, TenK);
			PED::SET_PED_MONEY(pigeonBank6, TenK);
			PED::SET_PED_MONEY(pigeonBank7, TenK);
			PED::SET_PED_MONEY(pigeonBank8, TenK);
			PED::SET_PED_MONEY(pigeonBank9, TenK);

			ENTITY::SET_ENTITY_HEALTH(pigeonBank, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank2, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank3, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank4, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank5, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank6, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank7, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank8, 0);
			ENTITY::SET_ENTITY_HEALTH(pigeonBank9, 0);
			WAIT(200);

			PED::DELETE_PED(&pigeonBank);
			PED::DELETE_PED(&pigeonBank2);
			PED::DELETE_PED(&pigeonBank3);
			PED::DELETE_PED(&pigeonBank4);
			PED::DELETE_PED(&pigeonBank5);
			PED::DELETE_PED(&pigeonBank6);
			PED::DELETE_PED(&pigeonBank7);
			PED::DELETE_PED(&pigeonBank8);
			PED::DELETE_PED(&pigeonBank9);
		}
	}
}


void set_all_nearby_peds_to_calm(Player selectedPlayer)
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int peds[arrSize];
	//0 index is the size of the array
	peds[0] = numElements;

	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), peds, -1);
	for (int i = 0; i < count; ++i)
	{
		for (int n = 0; n < 32; n++)
		{
			int offsettedID = i * 2 + 2;
			if (ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
			{
				if ((peds[offsettedID]) == selectedPlayer) continue;
				{
					PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(peds[offsettedID], true);
					PED::SET_PED_FLEE_ATTRIBUTES(peds[offsettedID], 0, 0);
					PED::SET_PED_COMBAT_ATTRIBUTES(peds[offsettedID], 17, 1);
				}
			}
		}
	}
}


void ApplyForceToEntity(Player ped_id, int x, int y, int z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(ped_id, 1, 0, x, y, z, 0, 0, 0, 1, 1, 1, 0, 1);
}

void flyMode()
{
	Hash hash = GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE");
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1, 1);
	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), 1);
	PED::SET_PED_TO_RAGDOLL_WITH_FALL(PLAYER::PLAYER_PED_ID(), 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);

	if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
	{
		if (GetAsyncKeyState(0x57)) // W key
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 0, 0);
		}

		if (GetAsyncKeyState(0x53)) // S key
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 6, 0);
		}
		if (GetAsyncKeyState(VK_SHIFT))
		{
			ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 6, 0, 0);
		}
	}
}



Vector3 rot_to_direction(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)std::sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radiansZ)) * (double)num);
	dir.z = (float)std::sin((double)radiansX);
	return dir;
}

Vector3 add(Vector3* vectorA, Vector3* vectorB)
{
	Vector3 result;
	result.x = vectorA->x;
	result.y = vectorA->y;
	result.z = vectorA->z;

	result.x += vectorB->x;
	result.y += vectorB->y;
	result.z += vectorB->z;

	return result;
}
Vector3 multiply(Vector3* vector, float x)
{
	Vector3 result;
	result.x = vector->x;
	result.y = vector->y;
	result.z = vector->z;

	result.x *= x;
	result.y *= x;
	result.z *= x;

	return result;
}
float get_distance(Vector3* pointA, Vector3* pointB)
{
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;

	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;

	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);

	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;

	return (float)std::sqrt(sum_2 + z_ba);
}

float get_vector_length(Vector3* vector)
{
	double x = (double)vector->x;
	double y = (double)vector->y;
	double z = (double)vector->z;

	return (float)std::sqrt(x * x + y * y + z * z);
}





Object CreateObject(Hash Hash, float X, float Y, float Z, float Pitch, float Roll, float Yaw)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(Hash) && STREAMING::IS_MODEL_VALID(Hash))
	{
		STREAMING::REQUEST_MODEL(Hash);
		while (!STREAMING::HAS_MODEL_LOADED(Hash))
			WAIT(0);
		Object object = OBJECT::CREATE_OBJECT(Hash, X, Y, Z, 1, 1, 0);
		ENTITY::SET_ENTITY_ROTATION(object, Pitch, Roll, Yaw, 2, 1);
		ENTITY::FREEZE_ENTITY_POSITION(object, 1);
		ENTITY::SET_ENTITY_LOD_DIST(object, 696969);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&object);
		return object;
	}
	return true;
}





bool grav_target_locked = false;

void GravityGun()
{
	Entity EntityTarget;
	DWORD equippedWeapon;
	WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &equippedWeapon, 1);
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 5;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));


	Player tempPed = PLAYER::PLAYER_ID();
	if (equippedWeapon == WEAPON_PISTOL)
	{
		if (grav_target_locked)
		{
			if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &EntityTarget) && GetAsyncKeyState(VK_RBUTTON))
			{
				Vector3 EntityTargetPos = ENTITY::GET_ENTITY_COORDS(EntityTarget, 0);
				PLAYER::DISABLE_PLAYER_FIRING(tempPed, true);
				if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1))
				{
					EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
				}

				RequestControl(EntityTarget);

				if (ENTITY::IS_ENTITY_A_VEHICLE(EntityTarget)) ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));

				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(EntityTarget, spawnPosition.x, spawnPosition.y, spawnPosition.z, 0, 0, 0);

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Foot_Swish", EntityTarget, "docks_heist_finale_2a_sounds", 0, 0);
					ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(tempPed));
					ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
					grav_target_locked = false;
					PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
				}
			}
		}
		if (!PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &EntityTarget))
		{
			grav_target_locked = true;
			PLAYER::DISABLE_PLAYER_FIRING(tempPed, false);
		}
	}
}



bool featureWepModRocket		=	false;
bool featureWepModValkyrie		=	false;
bool featureWepModTank			=	false;
bool featureWepModFireWork		=	false;


void update_firework_gun()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 0.25;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	float endDistance = get_distance(&camPosition, &playerPosition);
	endDistance += 1000;
	Vector3 endPosition = add(&camPosition, &multiply(&dir, endDistance));

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWepModFireWork) return;

	if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
	{
		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
	}
}

void update_rpg_gun()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 0.25;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	float endDistance = get_distance(&camPosition, &playerPosition);
	endDistance += 100;
	Vector3 endPosition = add(&camPosition, &multiply(&dir, endDistance));

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWepModRocket) return;

	if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
	{
		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
	}
}


void update_valkyrie_gun()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 0.25;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	float endDistance = get_distance(&camPosition, &playerPosition);
	endDistance += 1000;
	Vector3 endPosition = add(&camPosition, &multiply(&dir, endDistance));

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWepModValkyrie) return;

	if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
	{
		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_NOSE_TURRET_VALKYRIE");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
	}
}



void update_tank_gun()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 0.25;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	float endDistance = get_distance(&camPosition, &playerPosition);
	endDistance += 1000;
	Vector3 endPosition = add(&camPosition, &multiply(&dir, endDistance));

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWepModTank) return;

	if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
	{
		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_TANK");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
	}
}

int get_wanted_value()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	return PLAYER::GET_PLAYER_WANTED_LEVEL(player);
}

void set_wanted_value(int wantedLevel)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	int currentWantLev = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
	PLAYER::SET_PLAYER_WANTED_LEVEL(player, wantedLevel, 0);
	PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
}



void give_player_three_stars(Player selectedPlayer)
{
	if (ENTITY::DOES_ENTITY_EXIST(selectedPlayer))
	{
		const int maxPeds = 40;
		Ped ClonedPed[maxPeds];
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedPlayer, 0);
		//spawn peds
		for (int i = 0; i < maxPeds; i++)
		{
			ClonedPed[i] = PED::CLONE_PED(selectedPlayer, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
			ENTITY::SET_ENTITY_COORDS(ClonedPed[i], coords.x, coords.y, coords.z + 25, 0, 0, 0, 1);
			PED::SET_PED_AS_COP(ClonedPed[i], true);
			WAIT(20);
			Vector3 cloneCoord = ENTITY::GET_ENTITY_COORDS(ClonedPed[i], 0);
			FIRE::ADD_OWNED_EXPLOSION(selectedPlayer, cloneCoord.x, cloneCoord.y, cloneCoord.z, 9, 5.0f, 0, 1, 0.0f);
			WAIT(20);
			PED::DELETE_PED(&ClonedPed[i]);
		}

		notifyAboveMap("~HUD_COLOUR_GREEN~Set Players wanted level to Three Stars");
	}
	else
	{
		notifyAboveMap("~HUD_COLOUR_RED~Player doesnt Exist");
	}
}

void showCoords()
{
	Vector3 playerCoord = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
	std::string xMsg = "~HUD_COLOUR_GREEN~X " + std::to_string(playerCoord.x);
	std::string yMsg = " Y " + std::to_string(playerCoord.y);
	std::string zMsg = " Z " + std::to_string(playerCoord.z);
	std::string coordMessage = xMsg + yMsg + zMsg;
	char* xCoord = strcpy((char*)malloc(coordMessage.length() + 1), coordMessage.c_str());
	UI::SET_TEXT_FONT(7);
	UI::_SET_TEXT_ENTRY_2("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(xCoord);
	UI::_DRAW_SUBTITLE_TIMED(1, 1);
}




void AddClanLogoToVehicle(Ped playerPed)
{
	Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	vector3_t x, y, z;
	float scale;
	Hash modelHash = ENTITY::GET_ENTITY_MODEL(playerVeh);
	if (GetVehicleInfoForClanLogo(modelHash, x, y, z, scale))
	{
		int alpha = 200;
		if (modelHash == VEHICLE_WINDSOR)
			alpha = 255;
		GRAPHICS::_ADD_CLAN_DECAL_TO_VEHICLE(playerVeh, playerPed, ENTITY::_GET_ENTITY_BONE_INDEX(playerVeh, "chassis_dummy"), x.x, x.y, x.z, y.x, y.y, y.z, z.x, z.y, z.z, scale, 0, alpha);
	}
}


Vehicle ClonePedVehicle(Ped ped)
{
	Vehicle pedVeh = NULL;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
		pedVeh = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
	else 
		pedVeh = PED::GET_VEHICLE_PED_IS_IN(ped, TRUE);
	if (ENTITY::DOES_ENTITY_EXIST(pedVeh))
	{
		Hash vehicleModelHash = ENTITY::GET_ENTITY_MODEL(pedVeh);
		STREAMING::REQUEST_MODEL(vehicleModelHash); 
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);
		Vehicle playerVeh = VEHICLE::CREATE_VEHICLE(vehicleModelHash, playerPosition.x, playerPosition.y, playerPosition.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
		PED::SET_PED_INTO_VEHICLE(playerPed, playerVeh, SEAT_DRIVER);
		int primaryColor, secondaryColor;
		VEHICLE::GET_VEHICLE_COLOURS(pedVeh, &primaryColor, &secondaryColor);
		VEHICLE::SET_VEHICLE_COLOURS(playerVeh, primaryColor, secondaryColor);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModelHash) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicleModelHash))
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(playerVeh, 0);
			VEHICLE::SET_VEHICLE_WHEEL_TYPE(playerVeh, VEHICLE::GET_VEHICLE_WHEEL_TYPE(pedVeh));
			for (int i = 0; i <= MOD_BACKWHEELS; i++)
			{
				if (i > MOD_ARMOR && i < MOD_FRONTWHEELS)
					VEHICLE::TOGGLE_VEHICLE_MOD(playerVeh, i, VEHICLE::IS_TOGGLE_MOD_ON(pedVeh, i));
				else
					VEHICLE::SET_VEHICLE_MOD(playerVeh, i, VEHICLE::GET_VEHICLE_MOD(pedVeh, i), VEHICLE::GET_VEHICLE_MOD_VARIATION(pedVeh, i));
			}
			int tireSmokeColor[3], pearlescentColor, wheelColor;
			VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(pedVeh, &tireSmokeColor[0], &tireSmokeColor[1], &tireSmokeColor[2]);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(playerVeh, tireSmokeColor[0], tireSmokeColor[1], tireSmokeColor[2]);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(playerVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(pedVeh));
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(playerVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(pedVeh));
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(pedVeh, &pearlescentColor, &wheelColor);
			VEHICLE::SET_VEHICLE_EXTRA_COLOURS(playerVeh, pearlescentColor, wheelColor);
			if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(pedVeh, 0))
			{
				int convertableState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(pedVeh);
				if (convertableState == 0 || convertableState == 3 || convertableState == 5)
					VEHICLE::RAISE_CONVERTIBLE_ROOF(playerVeh, 1);
				else
					VEHICLE::LOWER_CONVERTIBLE_ROOF(playerVeh, 1);
			}
			for (int i = 0; i <= NEON_BACK; i++)
			{
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(playerVeh, i, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(pedVeh, i));
			}
			for (int i = 0; i <= 11; i++)
			{
				if (VEHICLE::DOES_EXTRA_EXIST(pedVeh, i))
					VEHICLE::SET_VEHICLE_EXTRA(playerVeh, i, !VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(pedVeh, i));
			}
			if ((VEHICLE::GET_VEHICLE_LIVERY_COUNT(pedVeh) > 1) && VEHICLE::GET_VEHICLE_LIVERY(pedVeh) >= 0)
			{
				VEHICLE::SET_VEHICLE_LIVERY(playerVeh, VEHICLE::GET_VEHICLE_LIVERY(pedVeh));
			}
			int neonColor[3];
			VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(pedVeh, &neonColor[0], &neonColor[1], &neonColor[2]);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(playerVeh, neonColor[0], neonColor[1], neonColor[2]);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(playerVeh, VEHICLE::GET_VEHICLE_WINDOW_TINT(pedVeh));
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(playerVeh, VEHICLE::GET_VEHICLE_DIRT_LEVEL(pedVeh));
			if (GRAPHICS::_HAS_VEHICLE_GOT_DECAL(pedVeh, 0) == TRUE)
			{
				AddClanLogoToVehicle(ped);
			}
		}
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicleModelHash);
	}
	return pedVeh;
}



void KillalltheSpeakingPlayers()
{
	for (int i = 0; i < 32; i++)
	{
		Ped playerHandle = PLAYER::GET_PLAYER_PED(i);
		if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
		{
			if (NETWORK::NETWORK_IS_PLAYER_TALKING(playerHandle))
			{
				Vector3 playerPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0.0, 0.0, 0.0);
				Freeze(playerHandle);
				FIRE::ADD_OWNED_EXPLOSION(playerHandle, playerPos.x, playerPos.y, playerPos.z, 37, 5.0, true, false, 0.1);
			}
		}
	}
	notifyAboveMap("~HUD_COLOUR_RED~Killed all the Speaking Players");
}




void attachNearbyVehsToPly(Player selectedPlayer)
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int vehs[arrSize];
	//0 index is the size of the array
	vehs[0] = numElements;

	int count = PED::GET_PED_NEARBY_VEHICLES(selectedPlayer, vehs);
	for (int i = 0; i < count; ++i)
	{
		int offsettedID = i * 2 + 2;
		if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
		{
			if ((vehs[offsettedID]) == PED::GET_VEHICLE_PED_IS_IN(selectedPlayer, 0)) continue;
			{
				ENTITY::ATTACH_ENTITY_TO_ENTITY(vehs[offsettedID], selectedPlayer, PED::GET_PED_BONE_INDEX(selectedPlayer, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
			}
		}
	}
}


void attachNearbyPedsToPly(Player selectedPlayer)
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;  //Start at index 2, and the odd elements are padding
	int peds[arrSize];

	peds[0] = numElements;

	int count = PED::GET_PED_NEARBY_PEDS(selectedPlayer, peds, -1);
	for (int i = 0; i < count; ++i)
	{
		int offsettedID = i * 2 + 2;
		if (ENTITY::DOES_ENTITY_EXIST(peds[offsettedID]))
		{
			if ((peds[offsettedID]) == selectedPlayer) continue;
			{
				if (!ENTITY::IS_ENTITY_DEAD((peds[offsettedID])))
				{
					ENTITY::ATTACH_ENTITY_TO_ENTITY(peds[offsettedID], selectedPlayer, PED::GET_PED_BONE_INDEX(selectedPlayer, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
				}
			}
		}
	}
}