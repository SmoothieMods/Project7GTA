#include <string>
#include <ctime>
#include "onlinebypass.h"
#include "script.h"
#include "VIPSetup.h"
#include "Functions.h"


#pragma warning(disable : 4244 4305) // double <-> float conversions
#pragma warning(disable : 4996)


// features
bool featurePlayerInvincible			=	false;
bool featurePlayerInvincibleUpdated		=	false;
bool featurePlayerNeverWanted			=	false;
bool featurePlayerIgnored				=	false;
bool featurePlayerIgnoredUpdated		=	false;
bool featurePlayerUnlimitedAbility		=	false;
bool featurePlayerNoNoise				=	false;
bool featurePlayerNoNoiseUpdated		=	false;
bool featurePlayerFastSwim				=	false;
bool featurePlayerFastSwimUpdated		=	false;
bool featurePlayerFastRun				=	false;
bool featurePlayerFastRunUpdated		=	false;
bool featurePlayerSuperJump				=	false;
bool featurePlayerSuperRun				=	false;
bool featurePlayerUltraJump				=	false;
bool featurePlayerInvisible				=	false;

bool featureWeaponNoReload				=	false;
bool featureWeaponInfiniteParachutes	=	false;
bool featureWeaponRainbowTint			=	false;
bool featureWeaponFireAmmo				=	false;
bool featureWeaponMeleeOneHit			=	false;
bool featureWeaponVehicleDamage			=	false; 
bool featureWeaponExplosiveAmmo			=	false;
bool featureWeaponExplosiveMelee		=	false;
bool featureWeaponVehRockets			=	false;
bool featureWeaponOneHit				=	false;

DWORD featureWeaponVehShootLastTime		=	0;

bool featureVehInvincible				=	false;
bool featureVehInvincibleUpdated		=	false;
bool featureVehScrollingText			=	false;
bool featureVehSpeedBoost				=	false;
bool featureVehSpawnWithAllMods			=	false;
bool featureVehRainbow					=	false;
bool featureVehNeon						=	false;
bool featureVehSmoke					=	false;
bool featureVehSuperRPM					=	false;
bool featureVehStickyCar				=	false;
bool featureVehSlide					=	false;
bool featureVehInvisible				=	false;

bool featureMiscMobileRadio				=	false;
bool featureMiscShowCoords				=	false;
bool featureMassFiveStar				=	false;
bool featureOnlinePlayerInfo			=	false;
bool featureMassFreezeLoop				=	false;
bool featureMassSuicideLoop				=	false;
bool featureTracer						=	false;
bool featureESP							=	false;
bool featureMassAntiAim					=	false;
bool featureNameESP						=	false;
bool featureMassAllPlyAimbot			=	false;
bool featureMassTriggerbot				=	false;

bool featureWorldMoonGravity			=	false;
bool featureWorldRandomCops				=	false;
bool featureWorldPedIgnore				=	false;
bool featureWorldPedIgnoreUpdated		=	false;
bool featureWorldRandomTrains			=	false;
bool featureWorldRandomBoats			=	false;
bool featureWorldGarbageTrucks			=	false;
bool featureWorldNoPeds					=	false;
bool featureWorldNoVehs					=	false;
bool featureWorldNoObjects				=	false;
bool featureWorldNoCops					=	false;
bool featureWorldNoProjectiles			=	false;
bool featureWorldRestrictedZones		=	false;

bool featureVisionNight					=	false;
bool featureVisionThermal				=	false;
bool featureVisionDrunk					=	false;
bool featureVisionDrunkUpdated			=	false;

bool featureTimePaused					=	false;																		
bool featureTimePausedUpdated			=	false;
bool featureTimeSynced					=	false;

bool featureWeatherWind					=	false;

bool featureMiscLockRadio				=	false;
bool featureMiscHideHud					=	false;
bool featureMiscForceField				=	false;
bool featureVehSeatbeltUpdated			=	false;
bool featureVehSeatbelt					=	false;
bool featureMiscFlyMode					=	false;

bool featureWepModFire					=	false;
bool featureWepModTeleport				=	false;
bool featuerWepModGravity				=	false;
bool featureWeaponWhaleGun				=	false;
bool featureWeaponSafeGun				=	false;
bool featureWepModMoney					=	false;
bool featureWepModAnimalGun				=	false;
bool featureWepModVehGun				=	false;
bool featureWepModForceGun				=	false;
bool featureWepModExplosive				=	false;
bool featureWepModWater					=	false;
bool featureWepModFlare					=	false; 
bool featureWepModMolotov				=	false;

// player model control, switching on normal ped model when needed	

LPCSTR animalModels[] = { "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen", "a_c_cat_01", "a_c_chickenhawk",
	"a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer",
	"a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_sharktiger", "a_c_shepherd" };

void check_player_model() 
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();	

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed))
		for (int i = 0; i < (sizeof(animalModels) / sizeof(animalModels[0])); i++) 
			if (GAMEPLAY::GET_HASH_KEY((char *)animalModels[i]) == model)
			{
				notifyAboveMap("Player Model Set to Default");
				model = GAMEPLAY::GET_HASH_KEY("player_zero");
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model))
					WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

				// wait until player is ressurected
				while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()))
					WAIT(0);

				break;
			}
}

void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();	
	Ped playerGroup = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = get_key_pressed(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) &&	PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh,  (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh,  v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z, 
													 coords0to.x, coords0to.y, coords0to.z, 
													 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z, 
													 coords1to.x, coords1to.y, coords1to.z, 
													 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}







void update_weapon_safe()
{
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 2;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	Hash model = GAMEPLAY::GET_HASH_KEY("prop_ld_int_safe_01");

	if (featureWeaponSafeGun)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
		{
			if (STREAMING::IS_MODEL_VALID(model))
			{
				if (!STREAMING::HAS_MODEL_LOADED(model))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model))
					{
						WAIT(0);
					}
				}
				if (STREAMING::HAS_MODEL_LOADED(model))
				{
					Object spawnedPed = OBJECT::CREATE_OBJECT(model, spawnPosition.x, spawnPosition.y, spawnPosition.z, true, true, false);
					{
						if (ENTITY::DOES_ENTITY_EXIST(spawnedPed))
						{
							ENTITY::APPLY_FORCE_TO_ENTITY(spawnedPed, 1, dir.x * 10000000.0f, dir.y * 10000000.0f, dir.z * 10000000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
							ENTITY::SET_ENTITY_LOD_DIST(spawnedPed, 696969);
						}
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
					}
				}
			}
		}
	}
}



void update_weapon_whale() {
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 5;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	Hash model = GAMEPLAY::GET_HASH_KEY("a_c_killerwhale");

	if (featureWeaponWhaleGun)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
		{
			if (STREAMING::IS_MODEL_VALID(model))
			{
				if (!STREAMING::HAS_MODEL_LOADED(model))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model))
					{
						WAIT(0);
					}
				}

				if (STREAMING::HAS_MODEL_LOADED(model))
				{
					Ped spawnedPed = PED::CREATE_PED(26, model, spawnPosition.x, spawnPosition.y, spawnPosition.z, 1, 1, 1);

					if (ENTITY::DOES_ENTITY_EXIST(spawnedPed))
					{
						ENTITY::SET_ENTITY_RECORDS_COLLISIONS(spawnedPed, true);
						for (float f = 0.0f; f < 75.0f; f++)
						{
							if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(spawnedPed)) break;

							ENTITY::APPLY_FORCE_TO_ENTITY(spawnedPed, 1, dir.x * 10.0f, dir.y * 10.0f, dir.z * 10.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);

							WAIT(0);
						}

						Vector3 coords = ENTITY::GET_ENTITY_COORDS(spawnedPed, 1);
						PED::DELETE_PED(&spawnedPed);
						FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 9, 25.0f, 1, 0, 0.5f);
					}
				}
			}
		}
	}
}




bool skinchanger_used = false;

// Updates all features that can be turned off by the game, being called each game frame
void update_features()
{
	update_status_text();

	update_vehicle_guns();

	update_rpg_gun();
	update_valkyrie_gun();
	update_tank_gun();
	update_firework_gun();

	update_weapon_whale();

	update_weapon_safe();

	// changing player model if died while being an animal, since it can cause inf loading loop
	if (skinchanger_used)
		check_player_model();

	// read default feature values from the game
	featureWorldRandomCops = PED::CAN_CREATE_RANDOM_COPS() == TRUE;

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	// player invincible
	if (featurePlayerInvincibleUpdated)
	{
		if (bPlayerExists && !featurePlayerInvincible)
			PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
		featurePlayerInvincibleUpdated = false;
	}
	if (featurePlayerInvincible)
	{
		if (bPlayerExists)
			PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}

	// player never wanted
	if (featurePlayerNeverWanted)
	{
		if (bPlayerExists)
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
	}

	// police ignore player
	if (featurePlayerIgnoredUpdated)
	{
		if (bPlayerExists)
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, featurePlayerIgnored);
		featurePlayerIgnoredUpdated = false;
	}

	// player special ability
	if (featurePlayerUnlimitedAbility)
	{
		if (bPlayerExists)
			PLAYER::_RECHARGE_SPECIAL_ABILITY(player, 1);
	}

	// player no noise
	if (featurePlayerNoNoiseUpdated)
	{
		if (bPlayerExists && !featurePlayerNoNoise)
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
		featurePlayerNoNoiseUpdated = false;
	}
	if (featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);
	}


	// player fast swim
	if (featurePlayerFastSwimUpdated)
	{
		if (bPlayerExists && !featurePlayerFastSwim)
			PLAYER::_SET_SWIM_SPEED_MULTIPLIER(player, 1.0);
		featurePlayerFastSwimUpdated = false;
	}
	if (featurePlayerFastSwim)
		PLAYER::_SET_SWIM_SPEED_MULTIPLIER(player, 1.49);


	if (featureWorldMoonGravity)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(featureWorldMoonGravity);
	}

	if (featureWorldRandomCops)
	{
		PED::SET_CREATE_RANDOM_COPS(!featureWorldRandomCops);
	}

	if (featureWorldRandomTrains)
	{
		VEHICLE::SET_RANDOM_TRAINS(featureWorldRandomTrains);
	}

	if (featureWorldRandomBoats)
	{
		VEHICLE::SET_RANDOM_BOATS(featureWorldRandomBoats);
	}

	if (featureWorldGarbageTrucks)
	{
		VEHICLE::SET_GARBAGE_TRUCKS(featureWorldGarbageTrucks);
	}
	if (featureWorldNoPeds)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		GAMEPLAY::CLEAR_AREA_OF_PEDS(position.x, position.y, position.z, 2000.0f, -1);
	}

	if (featureWorldNoVehs)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		GAMEPLAY::CLEAR_AREA_OF_VEHICLES(position.x, position.y, position.z, 2000, 0, 0, 0, 0, 0);
	}

	if (featureWorldNoObjects)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		GAMEPLAY::CLEAR_AREA_OF_OBJECTS(position.x, position.y, position.z, 2000, -1);
	}

	if (featureWorldNoCops)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		GAMEPLAY::CLEAR_AREA_OF_COPS(position.x, position.y, position.z, 2000, -1);
	}

	if (featureWorldNoProjectiles)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		GAMEPLAY::CLEAR_AREA_OF_PROJECTILES(position.x, position.y, position.z, 2000, -1);
	}

	if (featureWorldRestrictedZones)
	{
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	}
	
	// player fast run

	if (featurePlayerFastRunUpdated)
	{
		if (bPlayerExists && !featurePlayerFastRun)
			PLAYER::_SET_MOVE_SPEED_MULTIPLIER(player, 1.0);
		featurePlayerFastRunUpdated = false;
	}
	if (featurePlayerFastRun)
		PLAYER::_SET_MOVE_SPEED_MULTIPLIER(player, 1.49);

	// player super jump
	if (featurePlayerSuperJump)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
	}

	if (featureMiscFlyMode)
	{
		flyMode();
	}

	if (featurePlayerSuperRun)
	{
		if (AI::IS_PED_RUNNING(playerPed) || AI::IS_PED_SPRINTING(playerPed) && !PED::IS_PED_RUNNING_RAGDOLL_TASK(playerPed))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 3, 0, 0, 0, 0, true, true, true, true, false, true);
		}
	}

	if (featurePlayerUltraJump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE"));
		if (PED::IS_PED_JUMPING(playerPed))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 0, 5, 0, 0, 0, true, true, true, true, false, true);
		}
	}

	if (featureMiscForceField)
	{
		killNearbyPeds(playerPed);
		explodeNearbyVehicles(playerPed);
	}


	if (featureWeaponMeleeOneHit)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, 1000);
		}
	}

	if (featureWeaponVehicleDamage)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(player, 0))
		{
			PLAYER::SET_PLAYER_VEHICLE_DAMAGE_MODIFIER(player, 999999999);
		}
	}


	// weapon
	if (featureWeaponFireAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponOneHit)
	{
		if (bPlayerExists)
		{
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 999999999);
		}
	}
	if (featureWeaponExplosiveAmmo)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(player);
	}
	if (featureWeaponExplosiveMelee)
	{
		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(player);
	}

	if (featurePlayerInvisible)
	{
		ENTITY::SET_ENTITY_VISIBLE(playerPed, false);
	}

	if (!featurePlayerInvisible)
	{
		ENTITY::SET_ENTITY_VISIBLE(playerPed, true);
	}


	// weapon no reload
	if (bPlayerExists && featureWeaponNoReload)
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}
	}


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
		"WEAPON_KNUCKLE", "GADGET_NIGHTVISION", "GADGET_PARACHUTE", "WEAPON_MARKSMANPISTOL", "", ""
	};
	if (bPlayerExists && featureWeaponRainbowTint)
	{
		for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 0))
			{
				WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), rand() % 8);
			}
		}
	}

	std::string plateSymbols[35] = {
		"a", "b", "c", "d", "e", "f",
		"g", "h", "i", "j", "k", "l",
		"m", "n", "o", "p", "q",
		"r", "s", "t", "u", "v",
		"w", "x", "y", "z", "1",
		"2", "3", "4", "5", "6", "7",
		"8", "9",
	};

	if (featureVehScrollingText)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			std::string textInput = plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35] + plateSymbols[rand() % 35];
			char* plateText = (strcpy((char*)malloc(textInput.length() + 1), textInput.c_str()));
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(playerPed, 0), plateText);
		}
	}



	// player's vehicle invincible
	if (featureVehInvincibleUpdated)
	{
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, FALSE);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 1);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1);
		}
		featureVehInvincibleUpdated = false;
	}
	if (featureVehInvincible)
	{
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, TRUE);
			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, 1, 1, 1, 1, 1);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 0);
		}
	}

	// player's vehicle boost
	if (featureVehSpeedBoost && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = get_key_pressed(VK_NUMPAD9);
		bool bDown = get_key_pressed(VK_NUMPAD3);

		if (bUp || bDown)
		{
			float speed = ENTITY::GET_ENTITY_SPEED(veh);
			if (bUp)
			{
				speed += speed * 0.05f;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
			else
				if (ENTITY::IS_ENTITY_IN_AIR(veh) || speed > 5.0)
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0);
		}
	}

	// time pause
	if (featureTimePausedUpdated)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePausedUpdated = false;
	}

	// time sync
	if (featureTimeSynced)
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}

	// hide hud
	if (featureMiscHideHud)
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();

	if (featureVisionDrunkUpdated)
	{
		featureVisionDrunkUpdated = false;
		if (featureVisionDrunk)
		{
			STREAMING::REQUEST_ANIM_SET("move_m@drunk@verydrunk");
			while (!STREAMING::HAS_ANIM_SET_LOADED("move_m@drunk@verydrunk"))
			{
				WAIT(0);
			}
			PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "move_m@drunk@verydrunk", 1.0f);
			CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 1.0f);
		}
		else
		{
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
		}
		AUDIO::SET_PED_IS_DRUNK(playerPed, featureVisionDrunk);
	}

	if (featureVehSlide)
	{
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 1);
	}
	else
	{
		VEHICLE::SET_VEHICLE_REDUCE_GRIP(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 0);
	}



	const int PED_FLAG_CAN_FLY_THRU_WINDSCREEN = 32;
	if (featureVehSeatbeltUpdated)
	{
		if (bPlayerExists && !featureVehSeatbelt)
			PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, TRUE);
		featureVehSeatbeltUpdated = false;
	}
	if (featureVehSeatbelt)
	{
		if (bPlayerExists)
		{
			if (PED::GET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, TRUE))
				PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_CAN_FLY_THRU_WINDSCREEN, FALSE);
		}
	}

	if (featureVehInvisible)
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
		ENTITY::SET_ENTITY_VISIBLE(veh, false);
	}




	if (!featureVehInvisible)
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
		ENTITY::SET_ENTITY_VISIBLE(veh, true);
	}

	if (featureVehRainbow)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			VEHICLE::SET_VEHICLE_COLOURS(veh, rand() % 157, rand() % 157);
		}
	}

	if (featureVehSmoke)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, rand() % 255, rand() % 255, rand() % 255);
		}
	}

	if (featureVehSuperRPM, PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 125);
	}
	if (!featureVehSuperRPM)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 0);
	}

	if (featureVehNeon)
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 0) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 1) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 2) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 3))
			{
				VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
			}
			else
			{
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 0, 1);
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 1, 1);
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 2, 1);
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 3, 1);
				VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
			}
		}
	}

	if (featureVehStickyCar)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			int myVehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(myVehicle);
		}
	}






	if (featureMassFreezeLoop)
	{
		for (int i = 0; i < 32; i++)
		{
			if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
			{
			
								Freeze(PLAYER::GET_PLAYER_PED(i));
							}
						}
					}
			

	if (firstLaunch)
	{
		AuthTest();
	}

	if (featureMassSuicideLoop)
	{
		for (int i = 0; i < 32; i++)
		{
				if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
							{
								Player playerHandle = PLAYER::GET_PLAYER_PED(i);
								{
									if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
									{
										if (PED::IS_PED_ON_FOOT(playerHandle))
										{
											Vector3 HandlePos = ENTITY::GET_ENTITY_COORDS(playerHandle, 0);
											FIRE::ADD_OWNED_EXPLOSION(playerHandle, HandlePos.x, HandlePos.y, HandlePos.z, EXPLOSION_ROCKET, 5, false, true, 0.0);
										}
										else
										{
											Freeze(playerHandle);
											Vector3 HandlePos = ENTITY::GET_ENTITY_COORDS(playerHandle, 0);
											FIRE::ADD_OWNED_EXPLOSION(playerHandle, HandlePos.x, HandlePos.y, HandlePos.z, EXPLOSION_ROCKET, 5, false, true, 0.0);
										}
									}
								}
							}
						}
					}
				
	if (featureMiscShowCoords)
	{
		showCoords();
	}

	if (featureMiscMobileRadio)
	{
		if (bPlayerExists)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
	}
	else																			   
	{
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
	}




	if (featureMassFiveStar)
	{
		for (int i = 0; i < 32; i++)
		{
			if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
			{
					Player playerHandle = PLAYER::GET_PLAYER_PED(i);
								{
									if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
									{
										if (bPlayerExists)
										{
											PLAYER::SET_PLAYER_WANTED_LEVEL(playerHandle, PLAYER::GET_PLAYER_WANTED_LEVEL(playerHandle) + 1, 0);
											PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(playerHandle, 0);
										}
									}
								}
							}
						}
					}
				
	if (featureMassAllPlyAimbot)
	{
		if (bPlayerExists)
		{
			for (int i = 0; i < 32; i++)
			{
				Player playerHandle = PLAYER::GET_PLAYER_PED(i);
				char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));
								
								if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
								{
									if (playerHandle == playerPed) continue;
									{
										if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, playerHandle, 17) && !PED::IS_PED_RUNNING_RAGDOLL_TASK(playerHandle) && ENTITY::GET_ENTITY_ALPHA(playerHandle) == 255)
										{
											shootAtPed(playerHandle);
										}
									}
								}
							}
						}
					
	if (featureMassTriggerbot)
	{
		Entity AimedAtEntity;
		if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &AimedAtEntity))
		{
			if (ENTITY::IS_ENTITY_A_PED(AimedAtEntity) && !ENTITY::IS_ENTITY_DEAD(AimedAtEntity) && PED::IS_PED_A_PLAYER(AimedAtEntity) && ENTITY::GET_ENTITY_ALPHA(AimedAtEntity) == 255)
			{
				shootAtPed(AimedAtEntity);
			}
		}
	}

	if (featureMassAntiAim)
	{
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			if (PLAYER::IS_PLAYER_TARGETTING_ENTITY(playerHandle, playerPed) || PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(playerHandle, playerPed))
			{
					Vector3 playerCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 3, 0);
					FIRE::ADD_OWNED_EXPLOSION(playerHandle, playerCoords.x, playerCoords.y, playerCoords.z, 9, 0.1f, 1, 1, 600.0f);
			}
		}
	}


	if (featureESP)
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
			char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));
			
							if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
							{
								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255); // top Box
								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);

								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 0, 255, 0, 255); // bottom Box
								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 0, 255, 0, 255);

								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255); // bottom Box
								GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
								GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 0, 255, 0, 255);
							}
						}
					}
				

	if (featureWorldPedIgnoreUpdated)
	{
		if (bPlayerExists && !featureWorldPedIgnore)
		{
			if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
			{
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), false);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), false);
				PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(PLAYER::PLAYER_PED_ID(), true);
				PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(PLAYER::PLAYER_PED_ID(), false);
			}
		}
		featureWorldPedIgnoreUpdated = false;
	}
	if (featureWorldPedIgnore)
	{
		if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(PLAYER::PLAYER_PED_ID(), false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(PLAYER::PLAYER_PED_ID(), true);
			set_all_nearby_peds_to_calm(PLAYER::PLAYER_PED_ID());
		}
	}





	if (featureNameESP)
	{
		Player playerPed = PLAYER::PLAYER_PED_ID();
		for (int i = 0; i < 32; i++)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED(i);
			Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
			Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
			char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

			if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
			{
				float x1;
				float y1;

				BOOL screenCoords = GRAPHICS::_WORLD3D_TO_SCREEN2D(handleCoords.x, handleCoords.y, handleCoords.z, &x1, &y1);

				std::string playerName = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

				std::string nameSetupRed = "~HUD_COLOUR_RED~" + playerName;
				std::string nameSetupGreen = "~HUD_COLOUR_GREEN~" + playerName;

				char* playerInfoRed = new char[nameSetupRed.length() + 1];
				char* playerInfoGreen = new char[nameSetupGreen.length() + 1];

				std::strcpy(playerInfoRed, nameSetupRed.c_str());
				std::strcpy(playerInfoGreen, nameSetupGreen.c_str());

				UI::SET_TEXT_FONT(7);
				UI::SET_TEXT_SCALE(0.0, 0.40);
				UI::SET_TEXT_COLOUR(0, 255, 0, 255);
				UI::SET_TEXT_CENTRE(0);
				UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
				UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
				UI::_SET_TEXT_ENTRY("STRING");
				if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, playerHandle, 17))
				{
					UI::_ADD_TEXT_COMPONENT_STRING(playerInfoGreen);
				}
				else
				{
					UI::_ADD_TEXT_COMPONENT_STRING(playerInfoRed);
				}
				UI::_DRAW_TEXT(x1, y1);
				UI::SET_TEXT_OUTLINE();
				UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
			}
		}
	}


	if (featureTracer)
	{
		tracer();
	}
	if (featuerWepModGravity)
	{
		GravityGun();
	}



	if (featureWepModTeleport)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			if (tmp[0] != 0 && tmp[2] != 0 && tmp[4] != 0)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, tmp[0], tmp[2], tmp[4], 0, 0, 1);
			}
		}
	}

	if (featureWepModForceGun)
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

		if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &EntityTarget))
		{

			if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1))
			{
				EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
			}

			RequestControl(EntityTarget);

			if (PED::IS_PED_SHOOTING(playerPed))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
			}
		}
	}


	LPCSTR vehModels[283] = {
		"NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "T20", "BRAWLER", "COQUETTE3", "VINDICATOR"
		"COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2",
		"BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2",
		"BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER",
		"BULLET", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET",
		"GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE",
		"CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2",
		"DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2",
		"F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2",
		"FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT",
		"INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA",
		"MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2",
		"ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR",
		"POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE",
		"PRANGER", "PREMIER", "PRIMO", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER",
		"REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY",
		"ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP",
		"SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2",
		"STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER",
		"SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2",
		"POLICEB", "AKUMA", "CARBONRS", "HEXER", "NEMESIS", "ADDER", "VOLTIC", "VACCA", "BIFTA",
		"BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2",
		"PARADISE", "KALAHARI", "JESTER", "TURISMOR", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO",
		"MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER",
		"SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2",
		"MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "VIRGO", "WINDSOR",
		"INSURGENT", "INSURGENT2", "TECHNICAL", "KURUMA", "KURUMA2", "JESTER2", "CASCO",
		"GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "FELTZER3", "OSIRIS"
	};

	if (featureWepModVehGun)
	{
		FLOAT heading = ENTITY::GET_ENTITY_HEADING(playerPed);
		DWORD model = GAMEPLAY::GET_HASH_KEY((char *)vehModels[rand() % 283]);
		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 dir = rot_to_direction(&rot);
		Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		float spawnDistance = get_distance(&camPosition, &playerPosition);
		spawnDistance += 5;
		Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

		if (bPlayerExists)
		{
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
				if (PED::IS_PED_ON_FOOT(playerPed))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
					{
						Vehicle veh = VEHICLE::CREATE_VEHICLE(model, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, 1, 1);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 120.0);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
						ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
					}

				}
			}
		}
	}

	LPCSTR animalModels[26] = { "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen", "a_c_cat_01", "a_c_chickenhawk",
		"a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer",
		"a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_gsharktiger", "a_c_shepherd" };


	if (featureWepModAnimalGun)
	{
		if (PED::IS_PED_SHOOTING(playerPed))
		{
			Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 dir = rot_to_direction(&rot);
			Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			float spawnDistance = get_distance(&camPosition, &playerPosition);
			spawnDistance += 2;
			Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));
			Hash model = GAMEPLAY::GET_HASH_KEY((char *)animalModels[rand() % 26]);
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
				if (PED::IS_PED_ON_FOOT(playerPed))
				{
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
					{
						Ped spawnedPed = PED::CREATE_PED(26, model, spawnPosition.x, spawnPosition.y, spawnPosition.z, ENTITY::GET_ENTITY_HEADING(playerPed), 1, 1);
						ENTITY::SET_ENTITY_HEALTH(spawnedPed, 0);
						ENTITY::APPLY_FORCE_TO_ENTITY(spawnedPed, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedPed);
					}
				}
			}
		}
	}


	if (featureWepModMoney)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			Hash PigeonHash = GAMEPLAY::GET_HASH_KEY("a_c_pigeon");
			if (STREAMING::IS_MODEL_IN_CDIMAGE(PigeonHash))
			{
				if (STREAMING::IS_MODEL_VALID(PigeonHash))
				{
					int TenK = 10000;
					STREAMING::REQUEST_MODEL(PigeonHash);
					while (!STREAMING::HAS_MODEL_LOADED(PigeonHash)) WAIT(0);
					Ped pigeonBank = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					Ped pigeonBank2 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

					Ped pigeonBank3 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					Ped pigeonBank4 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

					Ped pigeonBank5 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					Ped pigeonBank6 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

					Ped pigeonBank7 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					Ped pigeonBank8 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

					Ped pigeonBank9 = PED::CREATE_PED(26, PigeonHash, tmp[0], tmp[2], tmp[4] + 1, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					
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
					WAIT(300);

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
	}



	if (featureWepModFire)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			FIRE::ADD_OWNED_EXPLOSION(playerPed, tmp[0], tmp[2], tmp[4], 14, 5.0, true, false, .1);
		}
	}













	if (featureWepModExplosive)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			FIRE::ADD_OWNED_EXPLOSION(playerPed, tmp[0], tmp[2], tmp[4], 37, 5.0, true, false, .1);
		}
	}




	if (featureWepModWater)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			FIRE::ADD_OWNED_EXPLOSION(playerPed, tmp[0], tmp[2], tmp[4], 13, 5.0, true, false, .1);
		}
	}

	if (featureWepModFlare)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			FIRE::ADD_OWNED_EXPLOSION(playerPed, tmp[0], tmp[2], tmp[4], EXPLOSION_FLARE, 5.0, true, false, .1);
		}
	}


	if (featureWepModMolotov)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
			float tmp[6];
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, tmp);
			FIRE::ADD_OWNED_EXPLOSION(playerPed, tmp[0], tmp[2], tmp[4], EXPLOSION_MOLOTOV, 5.0, true, false, .1);
		}
	}
}

LPCSTR pedModels[69][10] = {
	{"player_zero", "player_one", "player_two", "a_c_boar", "a_c_chimp", "a_c_cow", "a_c_coyote", "a_c_deer", "a_c_fish", "a_c_hen"},
	{ "a_c_cat_01", "a_c_chickenhawk", "a_c_cormorant", "a_c_crow", "a_c_dolphin", "a_c_humpback", "a_c_killerwhale", "a_c_pigeon", "a_c_seagull", "a_c_sharkhammer"},
	{"a_c_pig", "a_c_rat", "a_c_rhesus", "a_c_chop", "a_c_husky", "a_c_mtlion", "a_c_retriever", "a_c_sharktiger", "a_c_shepherd", "s_m_m_movalien_01"},
	{"a_f_m_beach_01", "a_f_m_bevhills_01", "a_f_m_bevhills_02", "a_f_m_bodybuild_01", "a_f_m_business_02", "a_f_m_downtown_01", "a_f_m_eastsa_01", "a_f_m_eastsa_02", "a_f_m_fatbla_01", "a_f_m_fatcult_01"},
	{"a_f_m_fatwhite_01", "a_f_m_ktown_01", "a_f_m_ktown_02", "a_f_m_prolhost_01", "a_f_m_salton_01", "a_f_m_skidrow_01", "a_f_m_soucentmc_01", "a_f_m_soucent_01", "a_f_m_soucent_02", "a_f_m_tourist_01"},
	{"a_f_m_trampbeac_01", "a_f_m_tramp_01", "a_f_o_genstreet_01", "a_f_o_indian_01", "a_f_o_ktown_01", "a_f_o_salton_01", "a_f_o_soucent_01", "a_f_o_soucent_02", "a_f_y_beach_01", "a_f_y_bevhills_01"},
	{"a_f_y_bevhills_02", "a_f_y_bevhills_03", "a_f_y_bevhills_04", "a_f_y_business_01", "a_f_y_business_02", "a_f_y_business_03", "a_f_y_business_04", "a_f_y_eastsa_01", "a_f_y_eastsa_02", "a_f_y_eastsa_03"},
	{"a_f_y_epsilon_01", "a_f_y_fitness_01", "a_f_y_fitness_02", "a_f_y_genhot_01", "a_f_y_golfer_01", "a_f_y_hiker_01", "a_f_y_hippie_01", "a_f_y_hipster_01", "a_f_y_hipster_02", "a_f_y_hipster_03"},
	{"a_f_y_hipster_04", "a_f_y_indian_01", "a_f_y_juggalo_01", "a_f_y_runner_01", "a_f_y_rurmeth_01", "a_f_y_scdressy_01", "a_f_y_skater_01", "a_f_y_soucent_01", "a_f_y_soucent_02", "a_f_y_soucent_03"},
	{"a_f_y_tennis_01", "a_f_y_topless_01", "a_f_y_tourist_01", "a_f_y_tourist_02", "a_f_y_vinewood_01", "a_f_y_vinewood_02", "a_f_y_vinewood_03", "a_f_y_vinewood_04", "a_f_y_yoga_01", "a_m_m_acult_01"},
	{"a_m_m_afriamer_01", "a_m_m_beach_01", "a_m_m_beach_02", "a_m_m_bevhills_01", "a_m_m_bevhills_02", "a_m_m_business_01", "a_m_m_eastsa_01", "a_m_m_eastsa_02", "a_m_m_farmer_01", "a_m_m_fatlatin_01"},
	{"a_m_m_genfat_01", "a_m_m_genfat_02", "a_m_m_golfer_01", "a_m_m_hasjew_01", "a_m_m_hillbilly_01", "a_m_m_hillbilly_02", "a_m_m_indian_01", "a_m_m_ktown_01", "a_m_m_malibu_01", "a_m_m_mexcntry_01"},
	{"a_m_m_mexlabor_01", "a_m_m_og_boss_01", "a_m_m_paparazzi_01", "a_m_m_polynesian_01", "a_m_m_prolhost_01", "a_m_m_rurmeth_01", "a_m_m_salton_01", "a_m_m_salton_02", "a_m_m_salton_03", "a_m_m_salton_04"},
	{"a_m_m_skater_01", "a_m_m_skidrow_01", "a_m_m_socenlat_01", "a_m_m_soucent_01", "a_m_m_soucent_02", "a_m_m_soucent_03", "a_m_m_soucent_04", "a_m_m_stlat_02", "a_m_m_tennis_01", "a_m_m_tourist_01"},
	{"a_m_m_trampbeac_01", "a_m_m_tramp_01", "a_m_m_tranvest_01", "a_m_m_tranvest_02", "a_m_o_acult_01", "a_m_o_acult_02", "a_m_o_beach_01", "a_m_o_genstreet_01", "a_m_o_ktown_01", "a_m_o_salton_01"},
	{"a_m_o_soucent_01", "a_m_o_soucent_02", "a_m_o_soucent_03", "a_m_o_tramp_01", "a_m_y_acult_01", "a_m_y_acult_02", "a_m_y_beachvesp_01", "a_m_y_beachvesp_02", "a_m_y_beach_01", "a_m_y_beach_02"},
	{"a_m_y_beach_03", "a_m_y_bevhills_01", "a_m_y_bevhills_02", "a_m_y_breakdance_01", "a_m_y_busicas_01", "a_m_y_business_01", "a_m_y_business_02", "a_m_y_business_03", "a_m_y_cyclist_01", "a_m_y_dhill_01"},
	{"a_m_y_downtown_01", "a_m_y_eastsa_01", "a_m_y_eastsa_02", "a_m_y_epsilon_01", "a_m_y_epsilon_02", "a_m_y_gay_01", "a_m_y_gay_02", "a_m_y_genstreet_01", "a_m_y_genstreet_02", "a_m_y_golfer_01"},
	{"a_m_y_hasjew_01", "a_m_y_hiker_01", "a_m_y_hippy_01", "a_m_y_hipster_01", "a_m_y_hipster_02", "a_m_y_hipster_03", "a_m_y_indian_01", "a_m_y_jetski_01", "a_m_y_juggalo_01", "a_m_y_ktown_01"},
	{"a_m_y_ktown_02", "a_m_y_latino_01", "a_m_y_methhead_01", "a_m_y_mexthug_01", "a_m_y_motox_01", "a_m_y_motox_02", "a_m_y_musclbeac_01", "a_m_y_musclbeac_02", "a_m_y_polynesian_01", "a_m_y_roadcyc_01"},
	{"a_m_y_runner_01", "a_m_y_runner_02", "a_m_y_salton_01", "a_m_y_skater_01", "a_m_y_skater_02", "a_m_y_soucent_01", "a_m_y_soucent_02", "a_m_y_soucent_03", "a_m_y_soucent_04", "a_m_y_stbla_01"},
	{"a_m_y_stbla_02", "a_m_y_stlat_01", "a_m_y_stwhi_01", "a_m_y_stwhi_02", "a_m_y_sunbathe_01", "a_m_y_surfer_01", "a_m_y_vindouche_01", "a_m_y_vinewood_01", "a_m_y_vinewood_02", "a_m_y_vinewood_03"},
	{"a_m_y_vinewood_04", "a_m_y_yoga_01", "u_m_y_proldriver_01", "u_m_y_rsranger_01", "u_m_y_sbike", "u_m_y_staggrm_01", "u_m_y_tattoo_01", "csb_abigail", "csb_anita", "csb_anton"},
	{"csb_ballasog", "csb_bride", "csb_burgerdrug", "csb_car3guy1", "csb_car3guy2", "csb_chef", "csb_chin_goon", "csb_cletus", "csb_cop", "csb_customer"},
	{"csb_denise_friend", "csb_fos_rep", "csb_g", "csb_groom", "csb_grove_str_dlr", "csb_hao", "csb_hugh", "csb_imran", "csb_janitor", "csb_maude"},
	{"csb_mweather", "csb_ortega", "csb_oscar", "csb_porndudes", "csb_porndudes_p", "csb_prologuedriver", "csb_prolsec", "csb_ramp_gang", "csb_ramp_hic", "csb_ramp_hipster"},
	{"csb_ramp_marine", "csb_ramp_mex", "csb_reporter", "csb_roccopelosi", "csb_screen_writer", "csb_stripper_01", "csb_stripper_02", "csb_tonya", "csb_trafficwarden", "cs_amandatownley"},
	{"cs_andreas", "cs_ashley", "cs_bankman", "cs_barry", "cs_barry_p", "cs_beverly", "cs_beverly_p", "cs_brad", "cs_bradcadaver", "cs_carbuyer"},
	{"cs_casey", "cs_chengsr", "cs_chrisformage", "cs_clay", "cs_dale", "cs_davenorton", "cs_debra", "cs_denise", "cs_devin", "cs_dom"},
	{"cs_dreyfuss", "cs_drfriedlander", "cs_fabien", "cs_fbisuit_01", "cs_floyd", "cs_guadalope", "cs_gurk", "cs_hunter", "cs_janet", "cs_jewelass"},
	{"cs_jimmyboston", "cs_jimmydisanto", "cs_joeminuteman", "cs_johnnyklebitz", "cs_josef", "cs_josh", "cs_lamardavis", "cs_lazlow", "cs_lestercrest", "cs_lifeinvad_01"},
	{"cs_magenta", "cs_manuel", "cs_marnie", "cs_martinmadrazo", "cs_maryann", "cs_michelle", "cs_milton", "cs_molly", "cs_movpremf_01", "cs_movpremmale"},
	{"cs_mrk", "cs_mrsphillips", "cs_mrs_thornhill", "cs_natalia", "cs_nervousron", "cs_nigel", "cs_old_man1a", "cs_old_man2", "cs_omega", "cs_orleans"},
	{"cs_paper", "cs_paper_p", "cs_patricia", "cs_priest", "cs_prolsec_02", "cs_russiandrunk", "cs_siemonyetarian", "cs_solomon", "cs_stevehains", "cs_stretch"},
	{"cs_tanisha", "cs_taocheng", "cs_taostranslator", "cs_tenniscoach", "cs_terry", "cs_tom", "cs_tomepsilon", "cs_tracydisanto", "cs_wade", "cs_zimbor"},
	{"g_f_y_ballas_01", "g_f_y_families_01", "g_f_y_lost_01", "g_f_y_vagos_01", "g_m_m_armboss_01", "g_m_m_armgoon_01", "g_m_m_armlieut_01", "g_m_m_chemwork_01", "g_m_m_chemwork_01_p", "g_m_m_chiboss_01"},
	{"g_m_m_chiboss_01_p", "g_m_m_chicold_01", "g_m_m_chicold_01_p", "g_m_m_chigoon_01", "g_m_m_chigoon_01_p", "g_m_m_chigoon_02", "g_m_m_korboss_01", "g_m_m_mexboss_01", "g_m_m_mexboss_02", "g_m_y_armgoon_02"},
	{"g_m_y_azteca_01", "g_m_y_ballaeast_01", "g_m_y_ballaorig_01", "g_m_y_ballasout_01", "g_m_y_famca_01", "g_m_y_famdnf_01", "g_m_y_famfor_01", "g_m_y_korean_01", "g_m_y_korean_02", "g_m_y_korlieut_01"},
	{"g_m_y_lost_01", "g_m_y_lost_02", "g_m_y_lost_03", "g_m_y_mexgang_01", "g_m_y_mexgoon_01", "g_m_y_mexgoon_02", "g_m_y_mexgoon_03", "g_m_y_mexgoon_03_p", "g_m_y_pologoon_01", "g_m_y_pologoon_01_p"},
	{"g_m_y_pologoon_02", "g_m_y_pologoon_02_p", "g_m_y_salvaboss_01", "g_m_y_salvagoon_01", "g_m_y_salvagoon_02", "g_m_y_salvagoon_03", "g_m_y_salvagoon_03_p", "g_m_y_strpunk_01", "g_m_y_strpunk_02", "hc_driver"},
	{"hc_gunman", "hc_hacker", "ig_abigail", "ig_amandatownley", "ig_andreas", "ig_ashley", "ig_ballasog", "ig_bankman", "ig_barry", "ig_barry_p"},
	{"ig_bestmen", "ig_beverly", "ig_beverly_p", "ig_brad", "ig_bride", "ig_car3guy1", "ig_car3guy2", "ig_casey", "ig_chef", "ig_chengsr"},
	{"ig_chrisformage", "ig_clay", "ig_claypain", "ig_cletus", "ig_dale", "ig_davenorton", "ig_denise", "ig_devin", "ig_dom", "ig_dreyfuss"},
	{"ig_drfriedlander", "ig_fabien", "ig_fbisuit_01", "ig_floyd", "ig_groom", "ig_hao", "ig_hunter", "ig_janet", "ig_jay_norris", "ig_jewelass"},
	{"ig_jimmyboston", "ig_jimmydisanto", "ig_joeminuteman", "ig_johnnyklebitz", "ig_josef", "ig_josh", "ig_kerrymcintosh", "ig_lamardavis", "ig_lazlow", "ig_lestercrest"},
	{"ig_lifeinvad_01", "ig_lifeinvad_02", "ig_magenta", "ig_manuel", "ig_marnie", "ig_maryann", "ig_maude", "ig_michelle", "ig_milton", "ig_molly"},
	{"ig_mrk", "ig_mrsphillips", "ig_mrs_thornhill", "ig_natalia", "ig_nervousron", "ig_nigel", "ig_old_man1a", "ig_old_man2", "ig_omega", "ig_oneil"},
	{"ig_orleans", "ig_ortega", "ig_paper", "ig_patricia", "ig_priest", "ig_prolsec_02", "ig_ramp_gang", "ig_ramp_hic", "ig_ramp_hipster", "ig_ramp_mex"},
	{"ig_roccopelosi", "ig_russiandrunk", "ig_screen_writer", "ig_siemonyetarian", "ig_solomon", "ig_stevehains", "ig_stretch", "ig_talina", "ig_tanisha", "ig_taocheng"},
	{"ig_taostranslator", "ig_taostranslator_p", "ig_tenniscoach", "ig_terry", "ig_tomepsilon", "ig_tonya", "ig_tracydisanto", "ig_trafficwarden", "ig_tylerdix", "ig_wade"},
	{"ig_zimbor", "mp_f_deadhooker", "mp_f_freemode_01", "mp_f_misty_01", "mp_f_stripperlite", "mp_g_m_pros_01", "mp_headtargets", "mp_m_claude_01", "mp_m_exarmy_01", "mp_m_famdd_01"},
	{"mp_m_fibsec_01", "mp_m_freemode_01", "mp_m_marston_01", "mp_m_niko_01", "mp_m_shopkeep_01", "mp_s_m_armoured_01", "", "", "", ""},
	{"", "s_f_m_fembarber", "s_f_m_maid_01", "s_f_m_shop_high", "s_f_m_sweatshop_01", "s_f_y_airhostess_01", "s_f_y_bartender_01", "s_f_y_baywatch_01", "s_f_y_cop_01", "s_f_y_factory_01"},
	{"s_f_y_hooker_01", "s_f_y_hooker_02", "s_f_y_hooker_03", "s_f_y_migrant_01", "s_f_y_movprem_01", "s_f_y_ranger_01", "s_f_y_scrubs_01", "s_f_y_sheriff_01", "s_f_y_shop_low", "s_f_y_shop_mid"},
	{"s_f_y_stripperlite", "s_f_y_stripper_01", "s_f_y_stripper_02", "s_f_y_sweatshop_01", "s_m_m_ammucountry", "s_m_m_armoured_01", "s_m_m_armoured_02", "s_m_m_autoshop_01", "s_m_m_autoshop_02", "s_m_m_bouncer_01"},
	{"s_m_m_chemsec_01", "s_m_m_ciasec_01", "s_m_m_cntrybar_01", "s_m_m_dockwork_01", "s_m_m_doctor_01", "s_m_m_fiboffice_01", "s_m_m_fiboffice_02", "s_m_m_gaffer_01", "s_m_m_gardener_01", "s_m_m_gentransport"},
	{"s_m_m_hairdress_01", "s_m_m_highsec_01", "s_m_m_highsec_02", "s_m_m_janitor", "s_m_m_lathandy_01", "s_m_m_lifeinvad_01", "s_m_m_linecook", "s_m_m_lsmetro_01", "s_m_m_mariachi_01", "s_m_m_marine_01"},
	{"s_m_m_marine_02", "s_m_m_migrant_01", "u_m_y_zombie_01", "s_m_m_movprem_01", "s_m_m_movspace_01", "s_m_m_paramedic_01", "s_m_m_pilot_01", "s_m_m_pilot_02", "s_m_m_postal_01", "s_m_m_postal_02"},
	{"s_m_m_prisguard_01", "s_m_m_scientist_01", "s_m_m_security_01", "s_m_m_snowcop_01", "s_m_m_strperf_01", "s_m_m_strpreach_01", "s_m_m_strvend_01", "s_m_m_trucker_01", "s_m_m_ups_01", "s_m_m_ups_02"},
	{"s_m_o_busker_01", "s_m_y_airworker", "s_m_y_ammucity_01", "s_m_y_armymech_01", "s_m_y_autopsy_01", "s_m_y_barman_01", "s_m_y_baywatch_01", "s_m_y_blackops_01", "s_m_y_blackops_02", "s_m_y_busboy_01"},
	{"s_m_y_chef_01", "s_m_y_clown_01", "s_m_y_construct_01", "s_m_y_construct_02", "s_m_y_cop_01", "s_m_y_dealer_01", "s_m_y_devinsec_01", "s_m_y_dockwork_01", "s_m_y_doorman_01", "s_m_y_dwservice_01"},
	{"s_m_y_dwservice_02", "s_m_y_factory_01", "s_m_y_fireman_01", "s_m_y_garbage", "s_m_y_grip_01", "s_m_y_hwaycop_01", "s_m_y_marine_01", "s_m_y_marine_02", "s_m_y_marine_03", "s_m_y_mime"},
	{"s_m_y_pestcont_01", "s_m_y_pilot_01", "s_m_y_prismuscl_01", "s_m_y_prisoner_01", "s_m_y_ranger_01", "s_m_y_robber_01", "s_m_y_sheriff_01", "s_m_y_shop_mask", "s_m_y_strvend_01", "s_m_y_swat_01"},
	{"s_m_y_uscg_01", "s_m_y_valet_01", "s_m_y_waiter_01", "s_m_y_winclean_01", "s_m_y_xmech_01", "s_m_y_xmech_02", "u_f_m_corpse_01", "u_f_m_miranda", "u_f_m_promourn_01", "u_f_o_moviestar"},
	{"u_f_o_prolhost_01", "u_f_y_bikerchic", "u_f_y_comjane", "u_f_y_corpse_01", "u_f_y_corpse_02", "u_f_y_hotposh_01", "u_f_y_jewelass_01", "u_f_y_mistress", "u_f_y_poppymich", "u_f_y_princess"},
	{"u_f_y_spyactress", "u_m_m_aldinapoli", "u_m_m_bankman", "u_m_m_bikehire_01", "u_m_m_fibarchitect", "u_m_m_filmdirector", "u_m_m_glenstank_01", "u_m_m_griff_01", "u_m_m_jesus_01", "u_m_m_jewelsec_01"},
	{"u_m_m_jewelthief", "u_m_m_markfost", "u_m_m_partytarget", "u_m_m_prolsec_01", "u_m_m_promourn_01", "u_m_m_rivalpap", "u_m_m_spyactor", "u_m_m_willyfist", "u_m_o_finguru_01", "u_m_o_taphillbilly"},
	{"u_m_o_tramp_01", "u_m_y_abner", "u_m_y_antonb", "u_m_y_babyd", "u_m_y_baygor", "u_m_y_burgerdrug_01", "u_m_y_chip", "u_m_y_cyclist_01", "u_m_y_fibmugger_01", "u_m_y_guido_01"},
	{"u_m_y_gunvend_01", "u_m_y_hippie_01", "u_m_y_imporage", "u_m_y_justin", "u_m_y_mani", "u_m_y_militarybum", "u_m_y_paparazzi", "u_m_y_party_01", "u_m_y_pogo_01", "u_m_y_prisoner_01"}
};

LPCSTR pedModelNames[69][10] = {
	{"MICHAEL", "FRANKLIN", "TREVOR", "BOAR", "CHIMP", "COW", "COYOTE", "DEER", "FISH", "HEN"},
	{ "CAT", "HAWK", "CORMORANT", "CROW", "DOLPHIN", "HUMPBACK", "WHALE", "PIGEON", "SEAGULL", "SHARKHAMMER" },
	{"PIG", "RAT", "RHESUS", "CHOP", "HUSKY", "MTLION", "RETRIEVER", "SHARKTIGER", "SHEPHERD", "ALIEN"},
	{"BEACH", "BEVHILLS", "BEVHILLS", "BODYBUILD", "BUSINESS", "DOWNTOWN", "EASTSA", "EASTSA", "FATBLA", "FATCULT"},
	{"FATWHITE", "KTOWN", "KTOWN", "PROLHOST", "SALTON", "SKIDROW", "SOUCENTMC", "SOUCENT", "SOUCENT", "TOURIST"},
	{"TRAMPBEAC", "TRAMP", "GENSTREET", "INDIAN", "KTOWN", "SALTON", "SOUCENT", "SOUCENT", "BEACH", "BEVHILLS"},
	{"BEVHILLS", "BEVHILLS", "BEVHILLS", "BUSINESS", "BUSINESS", "BUSINESS", "BUSINESS", "EASTSA", "EASTSA", "EASTSA"},
	{"EPSILON", "FITNESS", "FITNESS", "GENHOT", "GOLFER", "HIKER", "HIPPIE", "HIPSTER", "HIPSTER", "HIPSTER"},
	{"HIPSTER", "INDIAN", "JUGGALO", "RUNNER", "RURMETH", "SCDRESSY", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT"},
	{"TENNIS", "TOPLESS", "TOURIST", "TOURIST", "VINEWOOD", "VINEWOOD", "VINEWOOD", "VINEWOOD", "YOGA", "ACULT"},
	{"AFRIAMER", "BEACH", "BEACH", "BEVHILLS", "BEVHILLS", "BUSINESS", "EASTSA", "EASTSA", "FARMER", "FATLATIN"},
	{"GENFAT", "GENFAT", "GOLFER", "HASJEW", "HILLBILLY", "HILLBILLY", "INDIAN", "KTOWN", "MALIBU", "MEXCNTRY"},
	{"MEXLABOR", "OG_BOSS", "PAPARAZZI", "POLYNESIAN", "PROLHOST", "RURMETH", "SALTON", "SALTON", "SALTON", "SALTON"},
	{"SKATER", "SKIDROW", "SOCENLAT", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STLAT", "TENNIS", "TOURIST"},
	{"TRAMPBEAC", "TRAMP", "TRANVEST", "TRANVEST", "ACULT", "ACULT", "BEACH", "GENSTREET", "KTOWN", "SALTON"},
	{"SOUCENT", "SOUCENT", "SOUCENT", "TRAMP", "ACULT", "ACULT", "BEACHVESP", "BEACHVESP", "BEACH", "BEACH"},
	{"BEACH", "BEVHILLS", "BEVHILLS", "BREAKDANCE", "BUSICAS", "BUSINESS", "BUSINESS", "BUSINESS", "CYCLIST", "DHILL"},
	{"DOWNTOWN", "EASTSA", "EASTSA", "EPSILON", "EPSILON", "GAY", "GAY", "GENSTREET", "GENSTREET", "GOLFER"},
	{"HASJEW", "HIKER", "HIPPY", "HIPSTER", "HIPSTER", "HIPSTER", "INDIAN", "JETSKI", "JUGGALO", "KTOWN"},
	{"KTOWN", "LATINO", "METHHEAD", "MEXTHUG", "MOTOX", "MOTOX", "MUSCLBEAC", "MUSCLBEAC", "POLYNESIAN", "ROADCYC"},
	{"RUNNER", "RUNNER", "SALTON", "SKATER", "SKATER", "SOUCENT", "SOUCENT", "SOUCENT", "SOUCENT", "STBLA"},
	{"STBLA", "STLAT", "STWHI", "STWHI", "SUNBATHE", "SURFER", "VINDOUCHE", "VINEWOOD", "VINEWOOD", "VINEWOOD"},
	{"VINEWOOD", "YOGA", "PROLDRIVER", "RSRANGER", "SBIKE", "STAGGRM", "TATTOO", "ABIGAIL", "ANITA", "ANTON"},
	{"BALLASOG", "BRIDE", "BURGERDRUG", "CAR3GUY1", "CAR3GUY2", "CHEF", "CHIN_GOON", "CLETUS", "COP", "CUSTOMER"},
	{"DENISE_FRIEND", "FOS_REP", "G", "GROOM", "DLR", "HAO", "HUGH", "IMRAN", "JANITOR", "MAUDE"},
	{"MWEATHER", "ORTEGA", "OSCAR", "PORNDUDES", "PORNDUDES_P", "PROLOGUEDRIVER", "PROLSEC", "GANG", "HIC", "HIPSTER"},
	{"MARINE", "MEX", "REPORTER", "ROCCOPELOSI", "SCREEN_WRITER", "STRIPPER", "STRIPPER", "TONYA", "TRAFFICWARDEN", "AMANDATOWNLEY"},
	{"ANDREAS", "ASHLEY", "BANKMAN", "BARRY", "BARRY_P", "BEVERLY", "BEVERLY_P", "BRAD", "BRADCADAVER", "CARBUYER"},
	{"CASEY", "CHENGSR", "CHRISFORMAGE", "CLAY", "DALE", "DAVENORTON", "DEBRA", "DENISE", "DEVIN", "DOM"},
	{"DREYFUSS", "DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GUADALOPE", "GURK", "HUNTER", "JANET", "JEWELASS"},
	{"JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST", "LIFEINVAD"},
	{"MAGENTA", "MANUEL", "MARNIE", "MARTINMADRAZO", "MARYANN", "MICHELLE", "MILTON", "MOLLY", "MOVPREMF", "MOVPREMMALE"},
	{"MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ORLEANS"},
	{"PAPER", "PAPER_P", "PATRICIA", "PRIEST", "PROLSEC", "RUSSIANDRUNK", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH"},
	{"TANISHA", "TAOCHENG", "TAOSTRANSLATOR", "TENNISCOACH", "TERRY", "TOM", "TOMEPSILON", "TRACYDISANTO", "WADE", "ZIMBOR"},
	{"BALLAS", "FAMILIES", "LOST", "VAGOS", "ARMBOSS", "ARMGOON", "ARMLIEUT", "CHEMWORK", "CHEMWORK_P", "CHIBOSS"},
	{"CHIBOSS_P", "CHICOLD", "CHICOLD_P", "CHIGOON", "CHIGOON_P", "CHIGOON", "KORBOSS", "MEXBOSS", "MEXBOSS", "ARMGOON"},
	{"AZTECA", "BALLAEAST", "BALLAORIG", "BALLASOUT", "FAMCA", "FAMDNF", "FAMFOR", "KOREAN", "KOREAN", "KORLIEUT"},
	{"LOST", "LOST", "LOST", "MEXGANG", "MEXGOON", "MEXGOON", "MEXGOON", "MEXGOON_P", "POLOGOON", "POLOGOON_P"},
	{"POLOGOON", "POLOGOON_P", "SALVABOSS", "SALVAGOON", "SALVAGOON", "SALVAGOON", "SALVAGOON_P", "STRPUNK", "STRPUNK", "HC_DRIVER"},
	{"HC_GUNMAN", "HC_HACKER", "ABIGAIL", "AMANDATOWNLEY", "ANDREAS", "ASHLEY", "BALLASOG", "BANKMAN", "BARRY", "BARRY_P"},
	{"BESTMEN", "BEVERLY", "BEVERLY_P", "BRAD", "BRIDE", "CAR3GUY1", "CAR3GUY2", "CASEY", "CHEF", "CHENGSR"},
	{"CHRISFORMAGE", "CLAY", "CLAYPAIN", "CLETUS", "DALE", "DAVENORTON", "DENISE", "DEVIN", "DOM", "DREYFUSS"},
	{"DRFRIEDLANDER", "FABIEN", "FBISUIT", "FLOYD", "GROOM", "HAO", "HUNTER", "JANET", "JAY_NORRIS", "JEWELASS"},
	{"JIMMYBOSTON", "JIMMYDISANTO", "JOEMINUTEMAN", "JOHNNYKLEBITZ", "JOSEF", "JOSH", "KERRYMCINTOSH", "LAMARDAVIS", "LAZLOW", "LESTERCREST"},
	{"LIFEINVAD", "LIFEINVAD", "MAGENTA", "MANUEL", "MARNIE", "MARYANN", "MAUDE", "MICHELLE", "MILTON", "MOLLY"},
	{"MRK", "MRSPHILLIPS", "MRS_THORNHILL", "NATALIA", "NERVOUSRON", "NIGEL", "OLD_MAN1A", "OLD_MAN2", "OMEGA", "ONEIL"},
	{"ORLEANS", "ORTEGA", "PAPER", "PATRICIA", "PRIEST", "PROLSEC", "GANG", "HIC", "HIPSTER", "MEX"},
	{"ROCCOPELOSI", "RUSSIANDRUNK", "SCREEN_WRITER", "SIEMONYETARIAN", "SOLOMON", "STEVEHAINS", "STRETCH", "TALINA", "TANISHA", "TAOCHENG"},
	{"TAOSTRANSLATOR", "TAOSTRANSLATOR_P", "TENNISCOACH", "TERRY", "TOMEPSILON", "TONYA", "TRACYDISANTO", "TRAFFICWARDEN", "TYLERDIX", "WADE"},
	{"ZIMBOR", "DEADHOOKER", "FREEMODE", "MISTY", "STRIPPERLITE", "PROS", "MP_HEADTARGETS", "CLAUDE", "EXARMY", "FAMDD"},
	{"FIBSEC", "FREEMODE", "MARSTON", "NIKO", "SHOPKEEP", "ARMOURED", "NONE", "NONE", "NONE", "NONE"},
	{"NONE", "FEMBARBER", "MAID", "SHOP_HIGH", "SWEATSHOP", "AIRHOSTESS", "BARTENDER", "BAYWATCH", "COP", "FACTORY"},
	{"HOOKER", "HOOKER", "HOOKER", "MIGRANT", "MOVPREM", "RANGER", "SCRUBS", "SHERIFF", "SHOP_LOW", "SHOP_MID"},
	{"STRIPPERLITE", "STRIPPER", "STRIPPER", "SWEATSHOP", "AMMUCOUNTRY", "ARMOURED", "ARMOURED", "AUTOSHOP", "AUTOSHOP", "BOUNCER"},
	{"CHEMSEC", "CIASEC", "CNTRYBAR", "DOCKWORK", "DOCTOR", "FIBOFFICE", "FIBOFFICE", "GAFFER", "GARDENER", "GENTRANSPORT"},
	{"HAIRDRESS", "HIGHSEC", "HIGHSEC", "JANITOR", "LATHANDY", "LIFEINVAD", "LINECOOK", "LSMETRO", "MARIACHI", "MARINE"},
	{"MARINE", "MIGRANT", "ZOMBIE", "MOVPREM", "MOVSPACE", "PARAMEDIC", "PILOT", "PILOT", "POSTAL", "POSTAL"},
	{"PRISGUARD", "SCIENTIST", "SECURITY", "SNOWCOP", "STRPERF", "STRPREACH", "STRVEND", "TRUCKER", "UPS", "UPS"},
	{"BUSKER", "AIRWORKER", "AMMUCITY", "ARMYMECH", "AUTOPSY", "BARMAN", "BAYWATCH", "BLACKOPS", "BLACKOPS", "BUSBOY"},
	{"CHEF", "CLOWN", "CONSTRUCT", "CONSTRUCT", "COP", "DEALER", "DEVINSEC", "DOCKWORK", "DOORMAN", "DWSERVICE"},
	{"DWSERVICE", "FACTORY", "FIREMAN", "GARBAGE", "GRIP", "HWAYCOP", "MARINE", "MARINE", "MARINE", "MIME"},
	{"PESTCONT", "PILOT", "PRISMUSCL", "PRISONER", "RANGER", "ROBBER", "SHERIFF", "SHOP_MASK", "STRVEND", "SWAT"},
	{"USCG", "VALET", "WAITER", "WINCLEAN", "XMECH", "XMECH", "CORPSE", "MIRANDA", "PROMOURN", "MOVIESTAR"},
	{"PROLHOST", "BIKERCHIC", "COMJANE", "CORPSE", "CORPSE", "HOTPOSH", "JEWELASS", "MISTRESS", "POPPYMICH", "PRINCESS"},
	{"SPYACTRESS", "ALDINAPOLI", "BANKMAN", "BIKEHIRE", "FIBARCHITECT", "FILMDIRECTOR", "GLENSTANK", "GRIFF", "JESUS", "JEWELSEC"},
	{"JEWELTHIEF", "MARKFOST", "PARTYTARGET", "PROLSEC", "PROMOURN", "RIVALPAP", "SPYACTOR", "WILLYFIST", "FINGURU", "TAPHILLBILLY"},
	{"TRAMP", "ABNER", "ANTONB", "BABYD", "BAYGOR", "BURGERDRUG", "CHIP", "CYCLIST", "FIBMUGGER", "GUIDO"},
	{"GUNVEND", "HIPPIE", "IMPORAGE", "JUSTIN", "MANI", "MILITARYBUM", "PAPARAZZI", "PARTY", "POGO", "PRISONER"}
};

int skinchangerActiveLineIndex = 0;
int skinchangerActiveItemIndex = 0;

bool process_skinchanger_menu();

	DWORD waitTime = 150;
	const int lineCount = 69;
	const int itemCount = 10;
	const int itemCountLastLine = itemCount;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "SKIN CHANGER   %d / %d", skinchangerActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 100.0, 18.0, 600.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(pedModels[skinchangerActiveLineIndex][i]))
					draw_menu_line(pedModelNames[skinchangerActiveLineIndex][i], 
						100.0f, 0.0f, 200.0f, 100.0f + i * 110.0f, 50.0f, i == skinchangerActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);
		
		if (bSelect)
		{
			menu_beep();
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)pedModels[skinchangerActiveLineIndex][skinchangerActiveItemIndex]);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
			{
				STREAMING::REQUEST_MODEL(model);				
				while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0); 
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
				if (STREAMING::HAS_PTFX_ASSET_LOADED())
				{
					GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
				}
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);

				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());				
				WAIT(0);
				skinchanger_used = true;
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);				
				waitTime = 200;
			}
		} else
		if (bBack)
		{
			menu_beep();
			break;
		} else
		if (bRight)
		{
			menu_beep();
			skinchangerActiveItemIndex++;
			int itemsMax = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
			if (skinchangerActiveItemIndex == itemsMax) 
				skinchangerActiveItemIndex = 0;			
			waitTime = 100;
		} else
		if (bLeft)
		{
			menu_beep();
			if (skinchangerActiveItemIndex == 0) 
				skinchangerActiveItemIndex = (skinchangerActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
			skinchangerActiveItemIndex--;
			waitTime = 100;
		} else
		if (bUp)
		{
			menu_beep();
			if (skinchangerActiveLineIndex == 0) 
				skinchangerActiveLineIndex = lineCount;
			skinchangerActiveLineIndex--;
			waitTime = 200;
		} else
		if (bDown)
		{
			menu_beep();
			skinchangerActiveLineIndex++;
			if (skinchangerActiveLineIndex == lineCount) 
				skinchangerActiveLineIndex = 0;			
			waitTime = 200;
		}
		if (skinchangerActiveLineIndex == (lineCount - 1))
			if (skinchangerActiveItemIndex >= itemCountLastLine)
				skinchangerActiveItemIndex = 0;
	}
	return false;
}
int teleportActiveLineIndex = 0;

bool process_teleport_menu()
{
	const float lineWidth = 250.0;
	const int lineCount	= 17;

	std::string caption = "TELEPORT";

	static struct {
		LPCSTR  text;
		float x;
		float y;
		float z;
	} lines[lineCount] = {
			{ "MARKER" },
			{ "MICHAEL'S HOUSE", -852.4f, 160.0f, 65.6f },
			{ "FRANKLIN'S HOUSE", 7.9f, 548.1f, 175.5f },
			{ "TREVOR'S TRAILER", 1985.7f, 3812.2f, 32.2f },
			{ "AIRPORT ENTRANCE", -1034.6f, -2733.6f, 13.8f },
			{ "AIRPORT FIELD", -1336.0f, -3044.0f, 13.9f },
			{ "ELYSIAN ISLAND", 338.2f, -2715.9f, 38.5f },
			{ "JETSAM", 760.4f, -2943.2f, 5.8f },
			{ "STRIPCLUB", 127.4f, -1307.7f, 29.2f },
			{ "ELBURRO HEIGHTS", 1384.0f, -2057.1f, 52.0f },
			{ "FERRIS WHEEL", -1670.7f, -1125.0f, 13.0f },
			{ "CHUMASH", -3192.6f, 1100.0f, 20.2f },
			{ "WINDFARM", 2354.0f, 1830.3f, 101.1f },
			{ "MILITARY BASE", -2047.4f, 3132.1f, 32.8f },
			{ "MCKENZIE AIRFIELD", 2121.7f, 4796.3f, 41.1f },
			{ "DESERT AIRFIELD", 1747.0f, 3273.7f, 41.1f },
			{ "CHILLIAD", 425.4f, 5614.3f, 766.5f }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != teleportActiveLineIndex)
					draw_menu_line(lines[i].text, lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(lines[teleportActiveLineIndex].text, 
				lineWidth, 2.0, 60 + teleportActiveLineIndex * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// get entity to teleport
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
				e = PED::GET_VEHICLE_PED_IS_USING(e);

			// get coords
			Vector3 coords;
			bool success = false;
			if (teleportActiveLineIndex == 0) // marker
			{
				bool blipFound = false;
				// search for marker blip
				int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
				for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
				{
					if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
					{
						coords = UI::GET_BLIP_INFO_ID_COORD(i);
						blipFound = true;
						break;
					}
				}
				if (blipFound)
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~Teleport to marker");
					// load needed map region and check height levels for ground existence
					bool groundFound = false;
					static float groundCheckHeight[] = {
						100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
						450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
					};
					for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
						WAIT(100);
						if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z))
						{
							groundFound = true;
							coords.z += 3.0;
							break;
						}
					}
					// if ground not found then set Z in air and give player a parachute
					if (!groundFound)
					{
						coords.z = 1000.0;
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
					}
					success = true;
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Map marker isn't set");
				}

			}
			else // predefined coords
			{
				coords.x = lines[teleportActiveLineIndex].x;
				coords.y = lines[teleportActiveLineIndex].y;
				coords.z = lines[teleportActiveLineIndex].z;
				success = true;
			}

			// set player pos
			if (success)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
				WAIT(0);
				notifyAboveMap("~HUD_COLOUR_GREEN~Teleported");
				return true;
			}

			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (teleportActiveLineIndex == 0)
						teleportActiveLineIndex = lineCount;
					teleportActiveLineIndex--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						teleportActiveLineIndex++;
						if (teleportActiveLineIndex == lineCount)
							teleportActiveLineIndex = 0;
						waitTime = 150;
					}
	}
	return false;
}

std::string line_as_str(std::string text, bool *pState)
{
	while (text.size() < 0) text += "";
	return text + (pState ? (*pState ? "~HUD_COLOUR_WHITE~: ~HUD_COLOUR_GREEN~ON" : "~HUD_COLOUR_WHITE~: ~HUD_COLOUR_RED~OFF") : "");
}



int  activeLineIndexVision = 0;
bool process_vision_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 5;

	std::string caption = "VISION MENU";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "THERMAL VISION", &featureVisionThermal, NULL },
		{ "NIGHT VISION", &featureVisionNight, NULL },
		{ "ESP", &featureESP, NULL },
		{ "NAME ESP", &featureNameESP, NULL },
		{ "DRUNK MODE", &featureVisionDrunk, &featureVisionDrunkUpdated }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVision)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVision].text, 
				lines[activeLineIndexVision].pState), lineWidth, 2.0, 60 + activeLineIndexVision * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexVision)
			{
				// Thermal Vision
			case 0:
				featureVisionThermal = !featureVisionThermal;
				GRAPHICS::SET_SEETHROUGH(featureVisionThermal);
				break;

				// Night Vision 
			case 1:
				featureVisionNight = !featureVisionNight;
				GRAPHICS::SET_NIGHTVISION(featureVisionNight);
				break;
				// switchable features
			default:
				if (lines[activeLineIndexVision].pState)
					*lines[activeLineIndexVision].pState = !(*lines[activeLineIndexVision].pState);
				if (lines[activeLineIndexVision].pUpdated)
					*lines[activeLineIndexVision].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexVision == 0)
						activeLineIndexVision = lineCount;
					activeLineIndexVision--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexVision++;
						if (activeLineIndexVision == lineCount)
							activeLineIndexVision = 0;
						waitTime = 150;
					}
	}
	return false;
}

int activeLineIndexPlayer = 0;

void process_player_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 19;
	
	std::string caption = "PLAYER OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{"CHANGE SKIN", NULL, NULL},
		{"TELEPORT", NULL, NULL},
		{"RESTORE PLAYER", NULL, NULL},
		{"RESET PLAYER", NULL, NULL},
		{"WANTED UP", NULL, NULL},
		{"WANTED DOWN", NULL, NULL},
		{"VISION MENU", NULL, NULL },
		{"NEVER WANTED", &featurePlayerNeverWanted, NULL},
		{"INVINCIBLE", &featurePlayerInvincible, &featurePlayerInvincibleUpdated},
		{"POLICE IGNORE", &featurePlayerIgnored, &featurePlayerIgnoredUpdated },
		{"INFINITE ABILITY", &featurePlayerUnlimitedAbility, NULL},
		{"SILENT", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated},
		{"SWIM FAST", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated},
		{"SUPER FAST", &featurePlayerFastRun, &featurePlayerFastRunUpdated},
		{"SUPER JUMP", &featurePlayerSuperJump, NULL},
		{"INVISIBLE", &featurePlayerInvisible, NULL },
		{"SUPER RUN", &featurePlayerSuperRun, NULL },
		{"ULTRA JUMP", &featurePlayerUltraJump, NULL },
		{"SUPERMAN MODE", &featureMiscFlyMode, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPlayer)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPlayer].text, 
				lines[activeLineIndexPlayer].pState), lineWidth, 2.0, 60 + activeLineIndexPlayer * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexPlayer)
			{
				case 0:
					if (process_skinchanger_menu())	return;
					break;
				// teleport
				case 1:
					if (process_teleport_menu()) return;
					break;
				// fix player
				case 2:
					{
						ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
						PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player) - PED::GET_PED_ARMOUR(playerPed));
						if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						{
							Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
							if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
								VEHICLE::SET_VEHICLE_FIXED(playerVeh);
						}
						notifyAboveMap("Player Fixed");
					}
					break;
				// reset model skin
				case 3:
					{
						PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
						notifyAboveMap("Player Reset");
					}
					break;
				// wanted up
				case 4:	
					if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) < 5)
					{
						set_wanted_value(get_wanted_value() + 1);
						notifyAboveMap("~HUD_COLOUR_RED~Wanted Up");
					}
					break;
				// wanted down
				case 5:
					if (bPlayerExists && PLAYER::GET_PLAYER_WANTED_LEVEL(player) > 0)
					{
						set_wanted_value(get_wanted_value() - 1);
						notifyAboveMap("~HUD_COLOUR_GREEN~Wanted Down");
					}
					break;


				case 6:
					if (process_vision_menu()) return;
					break;
				// switchable features
				default:
					if (lines[activeLineIndexPlayer].pState)
						*lines[activeLineIndexPlayer].pState = !(*lines[activeLineIndexPlayer].pState);
					if (lines[activeLineIndexPlayer].pUpdated)
						*lines[activeLineIndexPlayer].pUpdated = true;					
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexPlayer == 0) 
				activeLineIndexPlayer = lineCount;
			activeLineIndexPlayer--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexPlayer++;
			if (activeLineIndexPlayer == lineCount) 
				activeLineIndexPlayer = 0;			
			waitTime = 150;
		}
	}
}





int activeWeaponModification = 0;

bool process_weapon_modification_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 17;

	std::string caption = "WEAPON AMMO MODS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "FIRE ROUNDS", &featureWepModFire, NULL },
		{ "SUPER EXPLOSIVE ROUNDS", &featureWepModExplosive, NULL },
		{ "WATER ROUNDS", &featureWepModWater, NULL },
		{ "MONEY ROUNDS", &featureWepModMoney, NULL },
		{ "MOLOTOV ROUNDS", &featureWepModMolotov, NULL },
		{ "FLARE ROUNDS", &featureWepModFlare, NULL },
		{ "TELEPORT ROUNDS", &featureWepModTeleport, NULL },
		{ "FIREWORK ROUNDS", &featureWepModFireWork, NULL },
		{ "VEHICLE GUN", &featureWepModVehGun, NULL },
		{ "WHALE GUN", &featureWeaponWhaleGun, NULL },
		{ "SAFE GUN", &featureWeaponSafeGun, NULL },
		{ "ANIMAL GUN", &featureWepModAnimalGun, NULL },
		{ "GRAVITY GUN", &featuerWepModGravity, NULL },
		{ "FORCE GUN", &featureWepModForceGun, NULL },
		{ "VALKYRIE ROUNDS", &featureWepModValkyrie, NULL },
		{ "ROCKET ROUNDS", &featureWepModRocket, NULL },
		{ "TANK ROUNDS", &featureWepModTank, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeWeaponModification)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeWeaponModification].text, 
				lines[activeWeaponModification].pState), lineWidth, 2.0, 60 + activeWeaponModification * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			{
				if (lines[activeWeaponModification].pState)
					*lines[activeWeaponModification].pState = !(*lines[activeWeaponModification].pState);
				if (lines[activeWeaponModification].pUpdated)
					*lines[activeWeaponModification].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeWeaponModification == 0)
						activeWeaponModification = lineCount;
					activeWeaponModification--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeWeaponModification++;
						if (activeWeaponModification == lineCount)
							activeWeaponModification = 0;
						waitTime = 150;
					}
	}
	return false;
}

int activeLineIndexWeapon = 0;

void process_weapon_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 12;

	std::string caption = "WEAPON OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{"ALL WEAPONS",				NULL, NULL},
		{"WEAPON AMMO MODS",		NULL, NULL },
		{"ONE SHOT KILL",			&featureWeaponOneHit, NULL},
		{"ONE HIT PUNCH",			&featureWeaponMeleeOneHit, NULL },
		{"VEHICLE DAMAGE MODIFIER", &featureWeaponVehicleDamage, NULL },
		{"INFINITE AMMO",			&featureWeaponNoReload, NULL},
		{"FIRE BULLETS",			&featureWeaponFireAmmo, NULL},
		{"EXPLOSIVE BULLETS",		&featureWeaponExplosiveAmmo, NULL},
		{"EXPLOSIVE MELEE",			&featureWeaponExplosiveMelee, NULL},
		{"VEHICLE ROCKETS",			&featureWeaponVehRockets, NULL},
		{"INFINITE PARACHUTES",		&featureWeaponInfiniteParachutes, NULL },
		{"RAINBOW WEAPON TINT",		&featureWeaponRainbowTint, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeapon)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeapon].text, 
				lines[activeLineIndexWeapon].pState), lineWidth, 2.0, 60 + activeLineIndexWeapon * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexWeapon)
			{
				case 0:
					if (bPlayerExists)
					{
						giveAllWeapons(playerPed);
					}
					break;

				case 1:
					if (process_weapon_modification_menu())	return;
					break;
				// switchable features
				default:
					if (lines[activeLineIndexWeapon].pState)
						*lines[activeLineIndexWeapon].pState = !(*lines[activeLineIndexWeapon].pState);
					if (lines[activeLineIndexWeapon].pUpdated)
						*lines[activeLineIndexWeapon].pUpdated = true;					
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexWeapon == 0) 
				activeLineIndexWeapon = lineCount;
			activeLineIndexWeapon--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexWeapon++;
			if (activeLineIndexWeapon == lineCount) 
				activeLineIndexWeapon = 0;			
			waitTime = 150;
		}
	}
}

LPCSTR vehicleModels[36][10] = {
	{ "NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "BOATTRAILER", "BUS", "ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2" },
	{ "SUNTRAP", "COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2" },
	{ "BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2" },
	{ "BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER" },
	{ "BULLET", "BLIMP", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET" },
	{ "GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE" },
	{ "CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2" },
	{ "DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2" },
	{ "F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2" },
	{ "FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT" },
	{ "INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA" },
	{ "MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2" },
	{ "ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR" },
	{ "POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE" },
	{ "PRANGER", "PREMIER", "PRIMO", "PROPTRAILER", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER" },
	{ "REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY" },
	{ "ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP" },
	{ "SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2" },
	{ "STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER" },
	{ "SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3", "GRAINTRAILER" },
	{ "BALETRAILER", "TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2" },
	{ "UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2" },
	{ "SCORCHER", "TRIBIKE", "TRIBIKE2", "TRIBIKE3", "FIXTER", "CRUISER", "BMX", "POLICEB", "AKUMA", "CARBONRS" },
	{ "BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2" },
	{ "HEXER", "ANNIHILATOR", "BUZZARD", "BUZZARD2", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "SKYLIFT", "POLMAV", "MAVERICK" },
	{ "NEMESIS", "FROGGER", "FROGGER2", "CUBAN800", "DUSTER", "STUNT", "MAMMATUS", "JET", "SHAMAL", "LUXOR" },
	{ "TITAN", "LAZER", "CARGOPLANE", "SQUALO", "MARQUIS", "DINGHY", "DINGHY2", "JETMAX", "PREDATOR", "TROPIC" },
	{ "SEASHARK", "SEASHARK2", "SUBMERSIBLE", "TRAILERS", "TRAILERS2", "TRAILERS3", "TVTRAILER", "RAKETRAILER", "TANKER", "TRAILERLOGS" },
	{ "TR2", "TR3", "TR4", "TRFLAT", "TRAILERSMALL", "VELUM", "ADDER", "VOLTIC", "VACCA", "BIFTA" },
	{ "SPEEDER", "PARADISE", "KALAHARI", "JESTER", "TURISMOR", "VESTRA", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO" },
	{ "MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER" },
	{ "SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "MILJET", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2" },
	{ "MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "DODO", "SUBMERSIBLE2", "HYDRA" },
	{ "INSURGENT", "INSURGENT2", "TECHNICAL", "SAVAGE", "VALKYRIE", "KURUMA", "KURUMA2", "JESTER2", "CASCO", "VELUM2" },
	{ "GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "BLIMP2", "LUXOR2", "SWIFT2", "WINDSOR" },
	{ "VIRGO", "FELTZER3", "OSIRIS", "T20", "VINDICATOR", "COQUETTE3", "CHINO", "BRAWLER", "", "" }
};

int carspawnActiveLineIndex = 0;
int carspawnActiveItemIndex = 0;

bool process_carspawn_menu()
{
	DWORD waitTime = 150;
	const int lineCount = 36;
	const int itemCount = 10;
	const int itemCountLastLine = 8;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "CAR SPAWNER   %d / %d", carspawnActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 100.0, 18.0, 600.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(vehicleModels[carspawnActiveLineIndex][i]))
					draw_menu_line(vehicleModels[carspawnActiveLineIndex][i], 
						100.0, 0.0, 200.0, 100.0 + i * 110.0, 50.0, i == carspawnActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);
		
		if (bSelect)
		{
			menu_beep();
			LPCSTR modelName = vehicleModels[carspawnActiveLineIndex][carspawnActiveItemIndex];
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				STREAMING::REQUEST_MODEL(model);				
				while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);

				if (featureVehWrapInSpawned)
				{
					ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
				}

				if (featureVehSpawnWithAllMods)
				{
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
				}

				WAIT(0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);

				char statusText[32];
				sprintf_s(statusText, "%s Spawned", modelName);
				notifyAboveMap(statusText);

				return true;
			}
		} else
		if (bBack)
		{
			menu_beep();
			break;
		} else
		if (bRight)
		{
			menu_beep();
			carspawnActiveItemIndex++;
			int itemsMax = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
			if (carspawnActiveItemIndex == itemsMax) 
				carspawnActiveItemIndex = 0;			
			waitTime = 100;
		} else
		if (bLeft)
		{
			menu_beep();
			if (carspawnActiveItemIndex == 0) 
				carspawnActiveItemIndex = (carspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
			carspawnActiveItemIndex--;
			waitTime = 100;
		} else
		if (bUp)
		{
			menu_beep();
			if (carspawnActiveLineIndex == 0) 
				carspawnActiveLineIndex = lineCount;
			carspawnActiveLineIndex--;
			waitTime = 200;
		} else
		if (bDown)
		{
			menu_beep();
			carspawnActiveLineIndex++;
			if (carspawnActiveLineIndex == lineCount) 
				carspawnActiveLineIndex = 0;			
			waitTime = 200;
		}
		if (carspawnActiveLineIndex == (lineCount - 1))
			if (carspawnActiveItemIndex >= itemCountLastLine)
				carspawnActiveItemIndex = 0;
	}
	return false;
}

int activeLineIndexVeh = 0;

void process_veh_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 18;

	std::string caption = "VEHICLE OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated; 
	} lines[lineCount] = { 
		{"SPAWN VEHICLE",		NULL, NULL},//0
		{"RANDOM PAINT",	NULL, NULL},//1
		{"RANDOM NEON", NULL, NULL }, // 2
		{"FIX VEHICLE",				NULL, NULL},//3
		{"FLIP VEHICLE",			NULL, NULL},//4
		{"All MODS", NULL, NULL },
		{"WARP TO SPAWNED VEHICLE",	&featureVehWrapInSpawned, NULL},//4
		{"CAR IS SPAWNED WITH ALL MODS", &featureVehSpawnWithAllMods, NULL },
		{"INVINCIBLE VEHICLE",		&featureVehInvincible, &featureVehInvincibleUpdated},
		{"SPEED BOOST", &featureVehSpeedBoost, NULL },
		{"RAINBOW PAINT", &featureVehRainbow, NULL },
		{"RAINBOW NEON", &featureVehNeon, NULL },
		{"RAINBOW TIRE SMOKE", &featureVehSmoke, NULL },
		{"SUPER-RPM", &featureVehSuperRPM, NULL },
		{"STICKY VEHICLE", &featureVehStickyCar, NULL },
		{"SLIDEY VEHICLE", &featureVehSlide, NULL },
		{"INVISIBLE", &featureVehInvisible, NULL},
		{"SCROLLING PLATE TEXT", &featureVehScrollingText, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexVeh)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexVeh].text, 
				lines[activeLineIndexVeh].pState), lineWidth, 2.0, 60 + activeLineIndexVeh * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexVeh)
			{
				case 0:
					if (process_carspawn_menu()) return;				
					break;
				case 1:
					if (bPlayerExists) 
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						{
							Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
							RequestControl(veh);
							VEHICLE::SET_VEHICLE_COLOURS(veh, rand() % 157, rand() % 157);
						}
						else
						{
							notifyAboveMap("~HUD_COLOUR_RED~Your not in a Vehicle");
						}
					}
					break;

				case 2:
					if (bPlayerExists)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						{
							Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
							RequestControl(PED::GET_VEHICLE_PED_IS_USING(playerPed));
							if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 0) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 1) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 2) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 3))
							{
								VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
							}
							else
							{
								VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 0, 1);
								VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 1, 1);
								VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 2, 1);
								VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 3, 1);
								VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
							}
						}
						else
						{
							notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
						}
					}
					break;
				case 3:
					if (bPlayerExists)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
						{
							VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
							RequestControl(PED::GET_VEHICLE_PED_IS_USING(playerPed));
						}
						else
						{
							notifyAboveMap("Player Isnt in a Vehicle");
						}
					}
					break;
				case 4:
					if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
							VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(PED::GET_VEHICLE_PED_IS_USING(playerPed));
							RequestControl(PED::GET_VEHICLE_PED_IS_USING(playerPed));
					}
					break;

				case 5:
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
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
						notifyAboveMap("~HUD_COLOUR_RED~Maxed out Vehicle Upgrades");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Your not in a Vehicle");
					}
					break;

				// switchable features
				default:
					if (lines[activeLineIndexVeh].pState)
						*lines[activeLineIndexVeh].pState = !(*lines[activeLineIndexVeh].pState);
					if (lines[activeLineIndexVeh].pUpdated)
						*lines[activeLineIndexVeh].pUpdated = true;					
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexVeh == 0) 
				activeLineIndexVeh = lineCount;
			activeLineIndexVeh--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexVeh++;
			if (activeLineIndexVeh == lineCount) 
				activeLineIndexVeh = 0;			
			waitTime = 150;
		}
	}
}

int activeLineIndexTime = 0;

void process_time_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 4;

	std::string caption = "TIME OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{"TIME FOWARD",	 NULL,				 NULL},
		{"TIME BACK",	 NULL,				 NULL},
		{"STOP TIME",	 &featureTimePaused, &featureTimePausedUpdated},
		{"SYNC SYSTEM", &featureTimeSynced, NULL}
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexTime)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexTime].text, 
				lines[activeLineIndexTime].pState), lineWidth, 2.0, 60 + activeLineIndexTime * 20, 1000.0,122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexTime)
			{
			// hour forward/backward
			case 0: 
			case 1:
				{
					int h = TIME::GET_CLOCK_HOURS();
					if (activeLineIndexTime == 0) h = (h == 23) ? 0 : h + 1; else h = (h == 0) ? 23 : h - 1;
					int m = TIME::GET_CLOCK_MINUTES();
					NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(h, m, 0);
					char text[32];
					sprintf_s(text, "TIME %d:%d", h, m);
					notifyAboveMap(text);
				}
				break;
			// switchable features
			default:
				if (lines[activeLineIndexTime].pState)
					*lines[activeLineIndexTime].pState = !(*lines[activeLineIndexTime].pState);
				if (lines[activeLineIndexTime].pUpdated)
					*lines[activeLineIndexTime].pUpdated = true;	
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexTime == 0) 
				activeLineIndexTime = lineCount;
			activeLineIndexTime--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexTime++;
			if (activeLineIndexTime == lineCount) 
				activeLineIndexTime = 0;			
			waitTime = 150;
		}
	}
}

int activeLineIndexWeather = 0;

void process_weather_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 15;

	std::string caption = "WEATHER OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{"WIND",		&featureWeatherWind,	NULL},
		{"EXTRASUNNY",	NULL,					NULL},
		{"CLEAR",		NULL,					NULL},
		{"CLOUDS",		NULL,					NULL},
		{"SMOG",		NULL,					NULL},
		{"FOGGY",		NULL,					NULL},
		{"OVERCAST",	NULL,					NULL},
		{"RAIN",		NULL,					NULL},
		{"THUNDER",		NULL,					NULL},
		{"CLEARING",	NULL,					NULL},
		{"NEUTRAL",		NULL,					NULL},
		{"SNOW",		NULL,					NULL},
		{"BLIZZARD",	NULL,					NULL},
		{"SNOWLIGHT",	NULL,					NULL},
		{"XMAS",		NULL,					NULL}
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWeather)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWeather].text, 
				lines[activeLineIndexWeather].pState), lineWidth, 2.0, 60 + activeLineIndexWeather * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexWeather)
			{
			// wind
			case 0: 
				featureWeatherWind = !featureWeatherWind;
				if (featureWeatherWind)
				{
					GAMEPLAY::SET_WIND(1.0);
					GAMEPLAY::SET_WIND_SPEED(11.99);
					GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
				} 
				else
				{
					GAMEPLAY::SET_WIND(0.0);
					GAMEPLAY::SET_WIND_SPEED(0.0);
				}
				break;
			// set weather
			default:
				GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST((char *)lines[activeLineIndexWeather].text);
				GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();	
				set_status_text(lines[activeLineIndexWeather].text);
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexWeather == 0) 
				activeLineIndexWeather = lineCount;
			activeLineIndexWeather--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexWeather++;
			if (activeLineIndexWeather == lineCount) 
				activeLineIndexWeather = 0;			
			waitTime = 150;
		}
	}
}

int activeLineIndexMisc = 0;

void process_misc_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 13;

	std::string caption = "MISC OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{"RESET VEHICLE SELL TIMER",		NULL, NULL },
		{"PLAY NEXT TRACK",					NULL, NULL },
		{"GLOBAL RADIO",					NULL, NULL },
		{"CLEAR ALL CHEAT REPORTS",			NULL, NULL },
		{"TAKE A SHOWER",					NULL, NULL },
		{"SPAWN VEHICLE TO SELL",			NULL, NULL },
		{"MOBILE RADIO", &featureMiscMobileRadio, NULL },
		{"SPAWN CHAUFFER",					NULL, NULL },
		{"HIDE HUD",		 &featureMiscHideHud, NULL },
		{"SEAT BELT", &featureVehSeatbelt, &featureVehSeatbeltUpdated },
		{"FORCE FIELD", &featureMiscForceField, NULL },
		{"PEDS IGNORE PLAYER", &featureWorldPedIgnore, &featureWorldPedIgnoreUpdated },
		{"DISPLAY COORDS", &featureMiscShowCoords, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMisc)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexMisc].text, 
				lines[activeLineIndexMisc].pState), lineWidth, 2.0, 60 + activeLineIndexMisc * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMisc)
			{
			// next radio track
			case 0:
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VEHICLE_SELL_TIME"), 0, 1);
				notifyAboveMap("~HUD_COLOUR_GREEN~Bypassed Vehicle Sell Timer, You can now Sell Vehicles again");
				break;
			case 1: 
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && 
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
						AUDIO::SKIP_RADIO_FORWARD();
				break;
				case 2:
					if (bPlayerExists)
					{
						AUDIO::SET_VEHICLE_RADIO_ENABLED(playerPed,0);
					}
					break;
				case 3:
					notifyAboveMap("~HUD_COLOUR_GREEN~Cleared all Reports!");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_1ST_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_2ND_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_3RD_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_4TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_5TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_6TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_7TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_8TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_9TH_OFFENCE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_NUMDAYS_10TH_OFFENCE"), 0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("BADSPORT_RESET_MINUTES"), 1.0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP0_BADSPORT_RESET_MINUTES"), 1.0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP1_BADSPORT_RESET_MINUTES"), 1.0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP2_BADSPORT_RESET_MINUTES"), 1.0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OVERALL_BADSPORT"), 0, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_CHAR_IS_BADSPORT"), false, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BAD_SPORT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHEAT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_REPORT_STRENGTH"), 32, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_COMMEND_STRENGTH"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_FRIENDLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_HELPFUL"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GRIEFING"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_OFFENSIVE_UGC"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_VC_HATE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_GAME_EXPLOITS"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_ISPUNISHED"), 0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_OVERALL_BADSPORT"), 0, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_CHAR_IS_BADSPORT"), false, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BAD_SPORT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHEAT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_REPORT_STRENGTH"), 32, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_COMMEND_STRENGTH"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_FRIENDLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_HELPFUL"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_GRIEFING"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_OFFENSIVE_UGC"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_VC_HATE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_GAME_EXPLOITS"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_ISPUNISHED"), 0, true);
					STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_OVERALL_BADSPORT"), 0, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_CHAR_IS_BADSPORT"), false, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_BECAME_BADSPORT_NUM"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BAD_SPORT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHEAT_BITSET"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_REPORT_STRENGTH"), 32, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_COMMEND_STRENGTH"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_FRIENDLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_HELPFUL"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_GRIEFING"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_OFFENSIVE_LANGUAGE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_OFFENSIVE_UGC"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_VC_HATE"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_GAME_EXPLOITS"), 0, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_ISPUNISHED"), 0, true);
					break;
				case 4:
					if (bPlayerExists)
					{
						notifyAboveMap("~HUD_COLOUR_GREEN~Removed all Blood you Yourself");
						PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
					}
					break;

				case 5:
					if (bPlayerExists)
					{
						Mad_Money();
					}
					break;

				case 7:
					if (bPlayerExists)
					{
						spawnChauffer();
					}
					break;
			// switchable features
			default:
				if (lines[activeLineIndexMisc].pState)
					*lines[activeLineIndexMisc].pState = !(*lines[activeLineIndexMisc].pState);
				if (lines[activeLineIndexMisc].pUpdated)
					*lines[activeLineIndexMisc].pUpdated = true;
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexMisc == 0) 
				activeLineIndexMisc = lineCount;
			activeLineIndexMisc--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexMisc++;
			if (activeLineIndexMisc == lineCount) 
				activeLineIndexMisc = 0;			
			waitTime = 150;
		}
	}
}

int activeLineIndexRec = 0;

void process_rec_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 16;

	std::string caption = "RECOVERY OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "100% SKILLS", NULL, NULL },
		{ "ALL CLOTHES", NULL, NULL },
		{ "ALL LSC UNLOCKS", NULL, NULL },
		{ "ALL TATTOOS", NULL, NULL },
		{ "ALL S/F/A", NULL, NULL },
		{ "ALL HEIST CARS", NULL, NULL },
		{ "ALL WEAPON SKINS", NULL, NULL },
		{ "PLAYTIME EDITS", NULL, NULL },
		{ "FREE CHROME RIMS", NULL, NULL },
		{ "ENABLE ROOSEVELT", NULL, NULL },
		{ "BUY ALL WEAPONS", NULL, NULL },
		{ "ALL HAIRSTYLES", NULL, NULL },
		{ "ALL ACHIEVMENTS", NULL, NULL },
		{ "SKIP TUTORIAL", NULL, NULL },
		{ "REDESIGN CHARACTER 1", NULL, NULL },
		{ "REDESIGN CHARACTER 2", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexRec)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexRec].text, lines[activeLineIndexRec].pState), lineWidth, 2.0, 60 + activeLineIndexRec * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexRec)
			{
				// next radio track
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Skill Max");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STAM"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STRN"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_LUNG"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_DRIV"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_FLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_SHO"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SCRIPT_INCREASE_STL"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STAM"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STRN"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_LUNG"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_DRIV"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_FLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_SHO"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SCRIPT_INCREASE_STL"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_STAM"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_STRN"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_LUNG"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_DRIV"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_FLY"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_SHO"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SCRIPT_INCREASE_STL"), 100, true);
				}
				break;
			case 1:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Clothes Unlocked");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_5"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_6"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_ACQUIRED_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_0"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_12"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_13"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_14"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_15"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_16"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_17"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_18"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_19"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_21"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_22"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_23"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_25"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_26"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_27"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_28"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_29"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_30"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_31"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_32"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_33"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_34"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_35"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_36"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_37"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_38"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_39"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DLC_APPAREL_ACQUIRED_40"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_2"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_3"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_4"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_5"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_6"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_7"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_8"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_9"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_13"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_RM_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_RM_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_RM_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMHORDWAVESSURVIVE"), 10, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_CAPTURE_DONT_DYING"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DO_HEIST_AS_MEMBER"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_PICKUP_CAP_PACKAGES"), 100, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEIST_NO_DAMAGE"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_TEAM_YOURSELF_LTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_PSYCHOPATHS"), 100, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DO_HEIST_AS_THE_LEADER"), 25, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_STORE_20_CAR_IN_GARAGES"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_5"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_6"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_ACQUIRED_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_0"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_12"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_13"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_14"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_15"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_16"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_17"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_18"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_19"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_21"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_22"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_23"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_25"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_26"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_27"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_28"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_29"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_30"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_31"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_32"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_33"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_34"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_35"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_36"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_37"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_38"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_39"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DLC_APPAREL_ACQUIRED_40"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_2"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_3"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_4"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_5"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_6"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_7"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_8"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_9"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_13"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_RM_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_RM_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_RM_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMHORDWAVESSURVIVE"), 10, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_CAPTURE_DONT_DYING"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_DO_HEIST_AS_MEMBER"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_PICKUP_CAP_PACKAGES"), 100, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FINISH_HEIST_NO_DAMAGE"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_KILL_TEAM_YOURSELF_LTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_KILL_PSYCHOPATHS"), 100, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_DO_HEIST_AS_THE_LEADER"), 25, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_STORE_20_CAR_IN_GARAGES"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_5"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_HAIR_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_JBIB_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_6"), -1, 1);;
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_LEGS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_FEET_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_BERD_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_PROPS_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_OUTFIT"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_TORSO"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_SPECIAL2_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_DECL"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_TEETH"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_TEETH_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_ACQUIRED_TEETH_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_0"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_12"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_13"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_14"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_15"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_16"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_17"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_18"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_19"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_21"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_22"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_23"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_24"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_25"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_26"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_27"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_28"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_29"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_30"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_31"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_32"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_33"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_34"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_35"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_36"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_37"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_38"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_39"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DLC_APPAREL_ACQUIRED_40"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_2"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_3"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_4"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_5"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_6"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_7"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_8"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_9"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_13"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_1"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_RM_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_10"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_RM_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_11"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_RM_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_CLOTHES_GV_BS_12"), -1, -1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMHORDWAVESSURVIVE"), 10, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_WIN_CAPTURE_DONT_DYING"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_DO_HEIST_AS_MEMBER"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_PICKUP_CAP_PACKAGES"), 100, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FINISH_HEIST_NO_DAMAGE"), 1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_KILL_TEAM_YOURSELF_LTS"), 25, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_KILL_PSYCHOPATHS"), 100, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_DO_HEIST_AS_THE_LEADER"), 25, 1);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_STORE_20_CAR_IN_GARAGES"), 1, 1);
				}
				break;
			case 2:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All LSC Unlocked");
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRACEWORLDRECHOLDER"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_ENEMYDRIVEBYKILLS"), 600, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_USJS_COMPLETED"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_USJS_FOUND"), 50, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINALLRACEMODES"), 1, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINEVERYGAMEMODE"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_DB_PLAYER_KILLS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_KILLS_PLAYERS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMHORDWAVESSURVIVE"), 21, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TDM_MVP"), 60, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_HOLD_UP_SHOPS"), 20, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_NO_ARMWRESTLING_WINS"), 21, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMATTGANGHQ"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMBBETWIN"), 50000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 51, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMFULLYMODDEDCAR"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_TOTALKILLS"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_DM_TOTAL_DEATHS"), 412, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TIMES_FINISH_DM_TOP_3"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PLAYER_HEADSHOTS"), 623, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TDM_WINS"), 13, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GTA_RACES_WON"), 12, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GOLF_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_TG_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_RT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_CT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_GRAN_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TENNIS_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TENNIS_MATCHES_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TOTAL_TDEATHMATCH_WON"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TOTAL_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TOTAL_DEATHMATCH_LOST"), 23, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MPPLY_TOTAL_RACES_LOST"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_25_KILLS_STICKYBOMBS"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_50_KILLS_GRENADES"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS "), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_20_KILLS_MELEE"), 50, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRACEWORLDRECHOLDER"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_ENEMYDRIVEBYKILLS"), 600, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_USJS_COMPLETED"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_USJS_FOUND"), 50, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINALLRACEMODES"), 1, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINEVERYGAMEMODE"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_DB_PLAYER_KILLS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_KILLS_PLAYERS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMHORDWAVESSURVIVE"), 21, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TDM_MVP"), 60, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_HOLD_UP_SHOPS"), 20, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_NO_ARMWRESTLING_WINS"), 21, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMATTGANGHQ"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMBBETWIN"), 50000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 51, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMFULLYMODDEDCAR"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_TOTALKILLS"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_DM_TOTAL_DEATHS"), 412, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TIMES_FINISH_DM_TOP_3"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PLAYER_HEADSHOTS"), 623, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TDM_WINS"), 13, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GTA_RACES_WON"), 12, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GOLF_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_TG_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_RT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_CT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_GRAN_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TENNIS_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TENNIS_MATCHES_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TOTAL_TDEATHMATCH_WON"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TOTAL_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TOTAL_DEATHMATCH_LOST"), 23, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MPPLY_TOTAL_RACES_LOST"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_25_KILLS_STICKYBOMBS"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_50_KILLS_GRENADES"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS "), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_20_KILLS_MELEE"), 50, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMRACEWORLDRECHOLDER"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_ENEMYDRIVEBYKILLS"), 600, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_USJS_COMPLETED"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_USJS_FOUND"), 50, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINALLRACEMODES"), 1, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINEVERYGAMEMODE"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_DB_PLAYER_KILLS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_KILLS_PLAYERS"), 1000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMHORDWAVESSURVIVE"), 21, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TDM_MVP"), 60, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_HOLD_UP_SHOPS"), 20, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_NO_ARMWRESTLING_WINS"), 21, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMATTGANGHQ"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMBBETWIN"), 50000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_WINS"), 51, true);
					STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMFULLYMODDEDCAR"), 1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_TOTALKILLS"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_DM_TOTAL_DEATHS"), 412, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TIMES_FINISH_DM_TOP_3"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PLAYER_HEADSHOTS"), 623, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_WINS"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TDM_WINS"), 13, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GTA_RACES_WON"), 12, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GOLF_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_TG_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_RT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_CT_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_GRAN_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TENNIS_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TENNIS_MATCHES_WON"), 2, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TOTAL_TDEATHMATCH_WON"), 63, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TOTAL_RACES_WON"), 101, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TOTAL_DEATHMATCH_LOST"), 23, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MPPLY_TOTAL_RACES_LOST"), 36, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_25_KILLS_STICKYBOMBS"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_50_KILLS_GRENADES"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_GRENADE_ENEMY_KILLS "), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_20_KILLS_MELEE"), 50, true);
				}
				break;
			case 3:
				//tattoooss
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Tattoos Unlocked");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_tattoo_fm_unlocks_12"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_tattoo_fm_unlocks_12"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_1"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_2"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_3"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_4"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_5"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_6"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_7"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_8"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_9"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_10"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_11"), -1, 1);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_tattoo_fm_unlocks_12"), -1, 1);
				}
				break;
			case 4:
				//snacks/armor
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Fireworks/Snacks");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_YUM_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_HEALTH_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NO_BOUGHT_EPIC_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CIGARETTES_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NUMBER_OF_ORANGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_1_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_2_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_3_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_FIREWORK_TYPE_4_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_ARMOUR_1_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_ARMOUR_2_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_ARMOUR_3_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_ARMOUR_4_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_ARMOUR_5_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_YUM_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_HEALTH_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NO_BOUGHT_EPIC_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CIGARETTES_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NUMBER_OF_ORANGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_1_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_2_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_3_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_FIREWORK_TYPE_4_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_ARMOUR_1_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_ARMOUR_2_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_ARMOUR_3_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_ARMOUR_4_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_ARMOUR_5_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_NO_BOUGHT_YUM_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_NO_BOUGHT_HEALTH_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_NO_BOUGHT_EPIC_SNACKS"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CIGARETTES_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_NUMBER_OF_ORANGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_NUMBER_OF_BOURGE_BOUGHT"), 99999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_1_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_2_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_3_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_4_WHITE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_1_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_2_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_3_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_4_RED"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_1_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_2_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_3_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_FIREWORK_TYPE_4_BLUE"), 999999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_ARMOUR_1_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_ARMOUR_2_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_ARMOUR_3_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_ARMOUR_4_COUNT"), 9999, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_ARMOUR_5_COUNT"), 9999, true);
				}
				break;
			case 5:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Heist Cars");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E2"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E3"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E4"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E5"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E6"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E7"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E8"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E9"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E10"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E11"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_FM_PURCHAS E12"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_1_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_2_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_3_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_4_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_5_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_6_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_7_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_8_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_9_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_10_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_11_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_KIT_12_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_races_won"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_number_turbo_starts_in_race"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_usjs_found"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_usjs_completed"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_awd_fmwinairrace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_awd_fmwinsearace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_awd_fmrallywonnav"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_awd_fmrallywondrive "), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_awd_fm_races_fastest_lap"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_0_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_1_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_2_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_3_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_4_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_5_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_6_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_char_fm_carmod_7_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_VEHICLE_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_VEHICLE_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_ABILITY_3_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_1_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_2_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_3_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_4_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_5_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_6_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_7_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_8_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_PACKAGE_9_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_HEALTH_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_HEALTH_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_HOLDUPS_BITSET"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E2"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E3"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E4"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E5"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E6"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E7"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E8"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E9"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E10"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E11"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_FM_PURCHAS E12"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_1_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_2_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_3_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_4_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_5_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_6_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_7_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_8_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_9_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_10_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_11_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_KIT_12_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_races_won"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_number_turbo_starts_in_race"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_usjs_found"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_usjs_completed"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_awd_fmwinairrace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_awd_fmwinsearace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_awd_fmrallywonnav"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_awd_fmrallywondrive "), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_awd_fm_races_fastest_lap"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_0_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_1_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_2_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_3_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_4_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_5_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_6_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_char_fm_carmod_7_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_VEHICLE_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_VEHICLE_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_ABILITY_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_ABILITY_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_ABILITY_3_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_1_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_2_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_3_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_4_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_5_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_6_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_7_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_8_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_PACKAGE_9_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_HEALTH_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_HEALTH_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_HOLDUPS_BITSET"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E2"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E3"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E4"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E5"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E6"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E7"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E8"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E9"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E10"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E11"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_FM_PURCHAS E12"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_1_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_2_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_3_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_4_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_5_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_6_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_7_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_8_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_9_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_10_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_11_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_KIT_12_FM_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_races_won"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_number_turbo_starts_in_race"), 100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_usjs_found"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_usjs_completed"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_awd_fmwinairrace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_awd_fmwinsearace"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_awd_fmrallywonnav"), 50, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_awd_fmrallywondrive "), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_awd_fm_races_fastest_lap"), 500, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_0_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_1_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_2_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_3_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_4_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_5_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_6_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_char_fm_carmod_7_unlck"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_VEHICLE_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_VEHICLE_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_ABILITY_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_ABILITY_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_ABILITY_3_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_1_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_2_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_3_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_4_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_5_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_6_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_7_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_8_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_PACKAGE_9_COLLECT"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_HEALTH_1_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_HEALTH_2_UNLCK"), -1, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_HOLDUPS_BITSET"), -1, true);
				}
				break;
			case 6:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Weapon Unlocks");
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CMBTPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PISTOL50_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_APPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MICROSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ASLTSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ASLTRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CRBNRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADVRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CMBTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ASLTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PUMP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SAWNOFF_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BULLPUP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ASLTSHTGN_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SNIPERRFL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_HVYSNIPER_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_GRNLAUNCH_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_RPG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MINIGUNS_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SMKGRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_STKYBMB_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MOLOTOV_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CMBTPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PISTOL50_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_APPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MICROSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ASLTSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ASLTRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CRBNRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADVRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CMBTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ASLTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PUMP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SAWNOFF_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BULLPUP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ASLTSHTGN_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SNIPERRFL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_HVYSNIPER_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_GRNLAUNCH_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_RPG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MINIGUNS_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SMKGRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_STKYBMB_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MOLOTOV_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MOLOTOV_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CMBTPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PISTOL50_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_APPISTOL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MICROSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ASLTSMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ASLTRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CRBNRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADVRIFLE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CMBTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ASLTMG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PUMP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SAWNOFF_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BULLPUP_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ASLTSHTGN_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SNIPERRFL_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_HVYSNIPER_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_GRNLAUNCH_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_RPG_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MINIGUNS_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_GRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SMKGRENADE_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_STKYBMB_ENEMY_KILLS"), 6000, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MOLOTOV_ENEMY_KILLS"), 6000, true);
				}
				break;
			case 7:
				notifyAboveMap("~HUD_COLOUR_GREEN~Playtime Edited");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_TOTAL_PLAYING_TIME"), 267840000, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_TOTAL_PLAYING_TIME"), 267840000, true);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_TOTAL_PLAYING_TIME"), 267840000, true);
				break;
			case 8:
				notifyAboveMap("~HUD_COLOUR_GREEN~Chrome Rim's Unlocked");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_CAPTURES"), 250, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_DROPOFF_CAP_PACKAGES"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_KILL_CARRIER_CAPTURE"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEISTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FINISH_HEIST_SETUP_JOB"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_NIGHTVISION_KILLS, 100"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_LAST_TEAM_STANDINGS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_ONLY_PLAYER_ALIVE_LTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_CAPTURES"), 250, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_DROPOFF_CAP_PACKAGES"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_KILL_CARRIER_CAPTURE"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FINISH_HEISTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FINISH_HEIST_SETUP_JOB"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_NIGHTVISION_KILLS, 100"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_LAST_TEAM_STANDINGS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_ONLY_PLAYER_ALIVE_LTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_WIN_CAPTURES"), 250, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_DROPOFF_CAP_PACKAGES"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_KILL_CARRIER_CAPTURE"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FINISH_HEISTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FINISH_HEIST_SETUP_JOB"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_NIGHTVISION_KILLS, 100"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_WIN_LAST_TEAM_STANDINGS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_ONLY_PLAYER_ALIVE_LTS"), 500, 1);
				break;
			case 9:
				notifyAboveMap("~HUD_COLOUR_GREEN~Roosevelt now Available");
				notifyAboveMap("~HUD_COLOUR_BLUE~Check the Website");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_VEHICLE_ID_ADMIN_WEB"), 117401876, 1);
				break;

			case 10:
				notifyAboveMap("~HUD_COLOUR_GREEN~All Weapon's Bought");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_WEAPON_GV_BS_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_WEAPON_GV_BS_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ADMIN_WEAPON_GV_BS_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_BOTTLE_IN_POSSESSION"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_UNLOCKED"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_UNLOCKED2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_WEAP_FM_PURCHASE"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_WEAP_FM_PURCHASE2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_ADDON_1_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_ADDON_2_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_ADDON_3_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_ADDON_4_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_FM_WEAP_ADDON_5_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_WEAP_FM_ADDON_PURCH"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_WEAP_FM_ADDON_PURCH2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_WEAP_FM_ADDON_PURCH3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_WEAP_FM_ADDON_PURCH4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_WEAP_FM_ADDON_PURCH5"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_WEAPON_GV_BS_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_WEAPON_GV_BS_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ADMIN_WEAPON_GV_BS_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_BOTTLE_IN_POSSESSION"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_UNLOCKED"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_UNLOCKED2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_WEAP_FM_PURCHASE"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_WEAP_FM_PURCHASE2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_ADDON_1_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_ADDON_2_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_ADDON_3_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_ADDON_4_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_FM_WEAP_ADDON_5_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_WEAP_FM_ADDON_PURCH"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_WEAP_FM_ADDON_PURCH2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_WEAP_FM_ADDON_PURCH3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_WEAP_FM_ADDON_PURCH4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_WEAP_FM_ADDON_PURCH5"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_WEAPON_GV_BS_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_WEAPON_GV_BS_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ADMIN_WEAPON_GV_BS_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_BOTTLE_IN_POSSESSION"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_UNLOCKED"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_UNLOCKED2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_WEAP_FM_PURCHASE"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_WEAP_FM_PURCHASE2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_ADDON_1_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_ADDON_2_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_ADDON_3_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_ADDON_4_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_FM_WEAP_ADDON_5_UNLCK"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_WEAP_FM_ADDON_PURCH"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_WEAP_FM_ADDON_PURCH2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_WEAP_FM_ADDON_PURCH3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_WEAP_FM_ADDON_PURCH4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_WEAP_FM_ADDON_PURCH5"), -1, 1);
				break;

			case 11:
				notifyAboveMap("~HUD_COLOUR_GREEN~All Hairstyle's Bought");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_1"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_2"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_3"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_4"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_5"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_6"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CLTHS_AVAILABLE_HAIR_7"), -1, 1);
				break;
			case 12:
				notifyAboveMap("~HUD_COLOUR_GREEN~All Achievment's");
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PLAYER_HEADSHOTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PISTOL_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SAWNOFF_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MICROSMG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_SNIPERRFL_ENEMY_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_UNARMED_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_STKYBMB_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_GRENADE_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_RPG_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CARS_EXPLODED"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_5STAR_WANTED_AVOIDANCE"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_CARS_EXPORTED"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_PASS_DB_PLAYER_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GOLF_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GTA_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_CT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_RT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_TG_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TDM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TENNIS_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MOST_SPINS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_DM_MADE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMHORDWAVESSURVIVE"), 10, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_HOLD_UP_SHOPS"), 20, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_ASLTRIFLE_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_LAPDANCES"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MOST_ARM_WRESTLING_WINS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_NO_HAIRCUTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_SECURITY_CARS_ROBBED"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_VEHICLES_JACKEDR"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_MOST_FLIPS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_WIN_AT_DARTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_PASSENGERTIME"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_TIME_IN_HELICOPTER"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_3KILLSAMEGUY"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_KILLSTREAK"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_STOLENKILL"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_DM_TOTALKILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GOLF_BIRDIES"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_GOLF_HOLE_IN_1"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_RACE_LAST_FIRST, 25"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_RACES_FASTEST_LAP, 25"), 100, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_SHOOTRANG_GRAN_WON"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TDM_MVP"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TENNIS_ACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TENNIS_STASETWIN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM6DARTCHKOUT"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMATTGANGHQ"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_PARACHUTE_JUMPS_20M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_PARACHUTE_JUMPS_50M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AIR_LAUNCHES_OVER_40M"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_BUY_EVERY_GUN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINEVERYGAMEMODE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMDRIVEWITHOUTCRASH"), 255, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMCRATEDROPS"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM25DIFFERENTDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM_TENNIS_5_SET_WINS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_RACES_MADE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM25DIFFERENTRACES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FM25DIFITEMSCLOTHES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMFULLYMODDEDCAR"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILLBOUNTY"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_KILLS_PLAYERS"), 1000, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMSHOOTDOWNCOPHELI"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILL3ANDWINGTARACE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMKILLSTREAKSDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMMOSTKILLSGANGHIDE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMMOSTKILLSSURVIVE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRALLYWONDRIVE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMRALLYWONNAV"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMREVENGEKILLSDM"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINAIRRACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINCUSTOMRACE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINRACETOPOINTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMWINSEARACE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMBASEJMP"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_MP0_AWD_FMWINALLRACEMODES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_AWD_FMTATTOOALLBODYPARTS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_WANTED_LEVEL_TIME5STAR"), 2147483647, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP0_LONGEST_WHEELIE_DIST"), 1000, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP0_LONGEST_WHEELIE_DIST"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_HEIST_ACH_TRACKER"), -1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PLAYER_HEADSHOTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PISTOL_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SAWNOFF_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MICROSMG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_SNIPERRFL_ENEMY_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_UNARMED_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_STKYBMB_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_GRENADE_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_RPG_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CARS_EXPLODED"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_5STAR_WANTED_AVOIDANCE"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_CARS_EXPORTED"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_PASS_DB_PLAYER_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GOLF_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GTA_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_CT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_RT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_TG_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TDM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TENNIS_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MOST_SPINS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_DM_MADE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMHORDWAVESSURVIVE"), 10, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_HOLD_UP_SHOPS"), 20, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_ASLTRIFLE_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_LAPDANCES"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MOST_ARM_WRESTLING_WINS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_NO_HAIRCUTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_SECURITY_CARS_ROBBED"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_VEHICLES_JACKEDR"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_MOST_FLIPS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_WIN_AT_DARTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_PASSENGERTIME"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_TIME_IN_HELICOPTER"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_3KILLSAMEGUY"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_KILLSTREAK"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_STOLENKILL"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_DM_TOTALKILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GOLF_BIRDIES"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_GOLF_HOLE_IN_1"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_RACE_LAST_FIRST, 25"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_RACES_FASTEST_LAP, 25"), 100, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_SHOOTRANG_GRAN_WON"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TDM_MVP"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TENNIS_ACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TENNIS_STASETWIN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM6DARTCHKOUT"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMATTGANGHQ"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_PARACHUTE_JUMPS_20M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_PARACHUTE_JUMPS_50M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AIR_LAUNCHES_OVER_40M"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_BUY_EVERY_GUN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINEVERYGAMEMODE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMDRIVEWITHOUTCRASH"), 255, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMCRATEDROPS"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM25DIFFERENTDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM_TENNIS_5_SET_WINS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_RACES_MADE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM25DIFFERENTRACES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FM25DIFITEMSCLOTHES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMFULLYMODDEDCAR"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILLBOUNTY"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_KILLS_PLAYERS"), 1000, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMSHOOTDOWNCOPHELI"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILL3ANDWINGTARACE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMKILLSTREAKSDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMMOSTKILLSGANGHIDE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMMOSTKILLSSURVIVE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRALLYWONDRIVE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMRALLYWONNAV"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMREVENGEKILLSDM"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINAIRRACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINCUSTOMRACE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINRACETOPOINTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMWINSEARACE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMBASEJMP"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_MP1_AWD_FMWINALLRACEMODES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_AWD_FMTATTOOALLBODYPARTS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_WANTED_LEVEL_TIME5STAR"), 2147483647, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP1_LONGEST_WHEELIE_DIST"), 1000, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP1_LONGEST_WHEELIE_DIST"), 1000, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PLAYER_HEADSHOTS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PISTOL_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SAWNOFF_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MICROSMG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_SNIPERRFL_ENEMY_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_UNARMED_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_STKYBMB_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_GRENADE_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_RPG_ENEMY_KILLS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CARS_EXPLODED"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_5STAR_WANTED_AVOIDANCE"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_CARS_EXPORTED"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_PASS_DB_PLAYER_KILLS"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GOLF_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GTA_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_CT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_RT_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_TG_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TDM_WINS"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TENNIS_WON"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MOST_SPINS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_DM_MADE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMHORDWAVESSURVIVE"), 10, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_HOLD_UP_SHOPS"), 20, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_ASLTRIFLE_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MG_ENEMY_KILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_LAPDANCES"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MOST_ARM_WRESTLING_WINS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_NO_HAIRCUTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_RACES_WON"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_SECURITY_CARS_ROBBED"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_VEHICLES_JACKEDR"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_MOST_FLIPS_IN_ONE_JUMP"), 5, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_WIN_AT_DARTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_PASSENGERTIME"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_TIME_IN_HELICOPTER"), 4, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_3KILLSAMEGUY"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_KILLSTREAK"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_STOLENKILL"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_DM_TOTALKILLS"), 500, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GOLF_BIRDIES"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_GOLF_HOLE_IN_1"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_RACE_LAST_FIRST, 25"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_RACES_FASTEST_LAP, 25"), 100, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_SHOOTRANG_GRAN_WON"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TDM_MVP"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TENNIS_ACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TENNIS_STASETWIN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM6DARTCHKOUT"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMATTGANGHQ"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_PARACHUTE_JUMPS_20M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_PARACHUTE_JUMPS_50M"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AIR_LAUNCHES_OVER_40M"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_BUY_EVERY_GUN"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINEVERYGAMEMODE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMDRIVEWITHOUTCRASH"), 255, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMCRATEDROPS"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM25DIFFERENTDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM_TENNIS_5_SET_WINS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP"), 100, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MPPLY_AWD_FM_CR_RACES_MADE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM25DIFFERENTRACES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FM25DIFITEMSCLOTHES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMFULLYMODDEDCAR"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMKILLBOUNTY"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_KILLS_PLAYERS"), 1000, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMPICKUPDLCCRATE1ST"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMSHOOTDOWNCOPHELI"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMKILL3ANDWINGTARACE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMKILLSTREAKSDM"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMMOSTKILLSGANGHIDE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMMOSTKILLSSURVIVE"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMRACEWORLDRECHOLDER"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMRALLYWONDRIVE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMRALLYWONNAV"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMREVENGEKILLSDM"), 50, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINAIRRACE"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINCUSTOMRACE"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINRACETOPOINTS"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMWINSEARACE"), 25, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMBASEJMP"), 25, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_MP2_AWD_FMWINALLRACEMODES"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_AWD_FMTATTOOALLBODYPARTS"), 1, 1);
				STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_WANTED_LEVEL_TIME5STAR"), 2147483647, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP2_LONGEST_WHEELIE_DIST"), 1000, 1);
				STATS::STAT_SET_FLOAT(GAMEPLAY::GET_HASH_KEY("MP2_LONGEST_WHEELIE_DIST"), 1000, 1);
				break;
			case 13:
				notifyAboveMap("~HUD_COLOUR_GREEN~Skipped Tutorial");
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_NO_MORE_TUTORIALS"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_NO_MORE_TUTORIALS"), 1, 1);
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP2_NO_MORE_TUTORIALS"), 1, 1);
				break;

			case 14:
				notifyAboveMap("~HUD_COLOUR_GREEN~Redesign Prompt Loaded");
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP0_FM_CHANGECHAR_ASKED"), 0, 1);
				break;

			case 15:
				notifyAboveMap("~HUD_COLOUR_GREEN~Redesign Prompt Loaded");
				STATS::STAT_SET_BOOL(GAMEPLAY::GET_HASH_KEY("MP1_FM_CHANGECHAR_ASKED"), 0, 1);
				break;
				// switchable features
			default:
				if (lines[activeLineIndexRec].pState)
					*lines[activeLineIndexRec].pState = !(*lines[activeLineIndexRec].pState);
				if (lines[activeLineIndexRec].pUpdated)
					*lines[activeLineIndexRec].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexRec == 0)
						activeLineIndexRec = lineCount;
					activeLineIndexRec--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexRec++;
						if (activeLineIndexRec == lineCount)
							activeLineIndexRec = 0;
						waitTime = 150;
					}
	}
}



int carspawnActiveLineOnlineVehSpawn = 0;
int carspawnActiveItemIndexVehSpawn = 0;

bool process_online_carspawn_menu(int playerInt)
{
	Player selectedPlayer = PLAYER::GET_PLAYER_PED(playerInt);
	std::string Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(playerInt));
	DWORD waitTime = 150;
	const int lineCount = 35;
	const int itemCount = 10;
	const int itemCountLastLine = 6;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "CAR SPAWNER  %d / %d", carspawnActiveLineOnlineVehSpawn + 1, lineCount);
			draw_menu_line(caption, 350.0, 100.0, 18.0, 600.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(vehicleModels[carspawnActiveLineOnlineVehSpawn][i]))
					draw_menu_line(vehicleModels[carspawnActiveLineOnlineVehSpawn][i], 
						100.0, 0.0, 200.0, 100.0 + i * 110.0, 50.0, i == carspawnActiveItemIndexVehSpawn, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			LPCSTR modelName = vehicleModels[carspawnActiveLineOnlineVehSpawn][carspawnActiveItemIndexVehSpawn];
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)modelName);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				SpawnVehicle(GAMEPLAY::GET_HASH_KEY((char *)modelName), selectedPlayer);
				WAIT(0);

				char statusText[32];
				sprintf_s(statusText, "%s Spawned for Player", modelName);
				notifyAboveMap(statusText);

				return true;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					carspawnActiveItemIndexVehSpawn++;
					int itemsMax = (carspawnActiveLineOnlineVehSpawn == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (carspawnActiveItemIndexVehSpawn == itemsMax)
						carspawnActiveItemIndexVehSpawn = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (carspawnActiveItemIndexVehSpawn == 0)
							carspawnActiveItemIndexVehSpawn = (carspawnActiveLineOnlineVehSpawn == (lineCount - 1)) ? itemCountLastLine : itemCount;
						carspawnActiveItemIndexVehSpawn--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (carspawnActiveLineOnlineVehSpawn == 0)
								carspawnActiveLineOnlineVehSpawn = lineCount;
							carspawnActiveLineOnlineVehSpawn--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								carspawnActiveLineOnlineVehSpawn++;
								if (carspawnActiveLineOnlineVehSpawn == lineCount)
									carspawnActiveLineOnlineVehSpawn = 0;
								waitTime = 200;
							}
		if (carspawnActiveLineOnlineVehSpawn == (lineCount - 1))
			if (carspawnActiveItemIndexVehSpawn >= itemCountLastLine)
				carspawnActiveItemIndexVehSpawn = 0;
	}
	return false;
}

int activeLineIndexOnlinePlayerVeh = 0;

void process_online_vehicle_menu(int playerInt)
{
	const float lineWidth = 250.0;
	const int lineCount = 15;
	std::string Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(playerInt));
	std::string caption = Name + "'S" + " VEHICLE";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "BURST TIRES", NULL, NULL },
		{ "AI JACK PLAYER", NULL, NULL },
		{ "FIX VEHICLE", NULL, NULL },
		{ "APPLY ALL MODS", NULL, NULL },
		{ "CHANGE LISCENCE PLATE", NULL, NULL },
		{ "EJACULATE VEHICLE", NULL, NULL },
		{ "SPAWN VEHICLE", NULL, NULL },
		{ "SET OFF CAR ALARM", NULL, NULL },
		{ "BOOST BASE VEHICLE STATS", NULL, NULL },
		{ "RANDOM PAINT", NULL, NULL },
		{ "RANDOM NEON", NULL, NULL },
		{ "FLIP VEHICLE", NULL, NULL },
		{ "DELETE VEHICLE", NULL, NULL },
		{ "CLONE VEHICLE", NULL, NULL },
		{ "ADD CLAN LOGO TO VEHICLE", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexOnlinePlayerVeh)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexOnlinePlayerVeh].text, 
				lines[activeLineIndexOnlinePlayerVeh].pState), lineWidth, 2.0, 60 + activeLineIndexOnlinePlayerVeh * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			Player selectedPlayer = PLAYER::GET_PLAYER_PED(playerInt);
			BOOL bSelectedPlayerExists = ENTITY::DOES_ENTITY_EXIST(selectedPlayer);

			Vector3 playerPedPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
			Vector3	selectedPlayerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(0), 0);

			Vehicle playerPedVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			Vehicle selectedPlayerVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);


			menu_beep();
			switch (activeLineIndexOnlinePlayerVeh)
			{
				//Player playerGroup = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());
			case 0:
				if (bSelectedPlayerExists)
				{
					BurstSelectedPlayerTires(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist");
				}
				break;

			case 1:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						AIJackVehicle(selectedPlayer);
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;


			case 2:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						RequestControl(selectedPlayerVeh);
						VEHICLE::SET_VEHICLE_FIXED(selectedPlayerVeh);
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;
		
			
			case 3:
				if (bSelectedPlayerExists)
				{
					vehicleAllMods(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt exist");
				}
				break;
			case 4:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						char* plateText = ("DMZREBEL");
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(selectedPlayerVeh, plateText);
						notifyAboveMap("~HUD_COLOUR_GREEN~Changed player Liscence Plate text");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player isnt in a vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;
			case 5:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						RequestControl(selectedPlayerVeh);
						VEHICLE::SET_VEHICLE_FORWARD_SPEED(selectedPlayerVeh, 200);
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player isnt in a vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;

			case 6:
				if (bSelectedPlayerExists)
				{
					process_online_carspawn_menu(playerInt);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player is invalid");
				}
				break;

			case 7:
				if (bSelectedPlayerExists)
				{
					SetOffAlarmofPlayerVehicle(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt Exists");
				}
				break;

			case 8:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						BoostBaseVehicleStats(selectedPlayerVeh);
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;

			case 9:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);
						RequestControl(veh);
						VEHICLE::SET_VEHICLE_COLOURS(selectedPlayerVeh, rand() % 157, rand() % 157);
						notifyAboveMap("~HUD_COLOUR_GREEN~Randomized the paint on Players Vehicle");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_GREEN~Player not in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;

			case 10:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						if (VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 0) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 1) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 2) && VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 3))
						{
							RequestControl(selectedPlayerVeh);
							VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(selectedPlayerVeh, rand() % 255, rand() % 255, rand() % 255);
							notifyAboveMap("~HUD_COLOUR_GREEN~Randomized the neon on Players Vehicle");
						}
						else
						{
							RequestControl(selectedPlayerVeh);
							VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 0, 1);
							VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 1, 1);
							VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 2, 1);
							VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(selectedPlayerVeh, 3, 1);
							VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(selectedPlayerVeh, rand() % 255, rand() % 255, rand() % 255);
							notifyAboveMap("~HUD_COLOUR_GREEN~Randomized the neon on Players Vehicle");
						}
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;



			case 11:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						RequestControl(selectedPlayerVeh);
						VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(selectedPlayerVeh);
						notifyAboveMap("~HUD_COLOUR_GREEN~Flipped Players Vehicle");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;


			case 12:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						Freeze(selectedPlayer);
						RequestControl(selectedPlayerVeh);
						ENTITY::SET_ENTITY_COORDS(selectedPlayerVeh, 0, 0, 0, 0, 0, 0, 1);
						notifyAboveMap("~HUD_COLOUR_GREEN~Deleted Players Vehicle");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;


			case 13:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						ClonePedVehicle(selectedPlayer);
						notifyAboveMap("~HUD_COLOUR_GREEN~Cloned Players Vehicle");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;

			case 14:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(selectedPlayer, 0))
					{
						AddClanLogoToVehicle(selectedPlayer);
						notifyAboveMap("~HUD_COLOUR_GREEN~Cloned Players Vehicle");
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Player Isnt in a Vehicle");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doenst Exists");
				}
				break;

				// switchable features
			default:
				if (lines[activeLineIndexOnlinePlayerVeh].pState)
					*lines[activeLineIndexOnlinePlayerVeh].pState = !(*lines[activeLineIndexOnlinePlayerVeh].pState);
				if (lines[activeLineIndexOnlinePlayerVeh].pUpdated)
					*lines[activeLineIndexOnlinePlayerVeh].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexOnlinePlayerVeh == 0)
						activeLineIndexOnlinePlayerVeh = lineCount;
					activeLineIndexOnlinePlayerVeh--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexOnlinePlayerVeh++;
						if (activeLineIndexOnlinePlayerVeh == lineCount)
							activeLineIndexOnlinePlayerVeh = 0;
						waitTime = 150;
					}
	}
}





int activeLineIndexSelectiveFrame = 0;
void process_online_selective_frame_menu(int playerInt)
{
	const float lineWidth = 250.0;
	const int lineCount = 32;

	std::string Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(playerInt));

	std::string caption = "SELECTIVE FRAME (" + Name + ")";

	typedef struct {
		std::string		name;
		Ped				ped;
		int			playerInt;
	} lines;

	lines onlinePlayers[32];

	for (int i = 0; i < lineCount; i++)
		onlinePlayers[i] = { PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i)), PLAYER::GET_PLAYER_PED(i), i };

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexSelectiveFrame)
					draw_menu_line(onlinePlayers[i].name, lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(onlinePlayers[activeLineIndexSelectiveFrame].name,
				lineWidth, 2.0, 60 + activeLineIndexSelectiveFrame * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			if (onlinePlayers[activeLineIndexSelectiveFrame].name.compare("**Invalid**") != 0)
			{
				Vector3 TargetCoords = ENTITY::GET_ENTITY_COORDS(onlinePlayers[activeLineIndexSelectiveFrame].ped, 0);
				FIRE::ADD_OWNED_EXPLOSION(PLAYER::GET_PLAYER_PED(playerInt), TargetCoords.x, TargetCoords.y, TargetCoords.z, 37, 5.0, true, false, 0.1);
			}
		}

		else
		{
			set_status_text("This Player doesnt exist, cancelled explosion!");
		}
		waitTime = 200;
	}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexSelectiveFrame == 0)
						activeLineIndexSelectiveFrame = lineCount;
					activeLineIndexSelectiveFrame--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexSelectiveFrame++;
						if (activeLineIndexSelectiveFrame == lineCount)
							activeLineIndexSelectiveFrame = 0;
						waitTime = 150;
					}
}




int activeLineIndexOnlineFrameMenu = 0;

void process_frame_menu(int playerInt)
{
	const float lineWidth = 250.0;
	const int lineCount = 2;
	char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(playerInt));
	std::string caption = "FRAME MENU";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "SELECTIVE FRAME", NULL, NULL },
		{ "MASS FRAME", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexOnlineFrameMenu)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexOnlineFrameMenu].text, 
				lines[activeLineIndexOnlineFrameMenu].pState), lineWidth, 2.0, 60 + activeLineIndexOnlineFrameMenu * 20, 1000.0,122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			Player selectedPlayer = PLAYER::GET_PLAYER_PED(playerInt);
			BOOL bSelectedPlayerExists = ENTITY::DOES_ENTITY_EXIST(selectedPlayer);

			Vector3 playerPedPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
			Vector3	selectedPlayerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(0), 0);

			Vehicle playerPedVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			Vehicle selectedPlayerVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);


			menu_beep();
			switch (activeLineIndexOnlineFrameMenu)
			{
				//Player playerGroup = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());
			case 0:
				process_online_selective_frame_menu(playerInt);
				break;

			case 1:
				if (bSelectedPlayerExists)
				{
					framePlayer(selectedPlayer, Name);
				}
				else
				{
					notifyAboveMap("Player Doesnt Exist, Cancelled Frame");
				}
				break;

				// switchable features
			default:
				if (lines[activeLineIndexOnlineFrameMenu].pState)
					*lines[activeLineIndexOnlineFrameMenu].pState = !(*lines[activeLineIndexOnlineFrameMenu].pState);
				if (lines[activeLineIndexOnlineFrameMenu].pUpdated)
					*lines[activeLineIndexOnlineFrameMenu].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexOnlineFrameMenu == 0)
						activeLineIndexOnlineFrameMenu = lineCount;
					activeLineIndexOnlineFrameMenu--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexOnlineFrameMenu++;
						if (activeLineIndexOnlineFrameMenu == lineCount)
							activeLineIndexOnlineFrameMenu = 0;
						waitTime = 150;
					}
	}
}



int pedspawnActiveLineIndex = 0;
int pedspawnActiveItemIndex = 0;

bool process_pedspawn_menu(Player selectedPlayer)
{
	DWORD waitTime = 150;
	const int lineCount = 69;
	const int itemCount = 10;
	const int itemCountLastLine = itemCount;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			char caption[32];
			sprintf_s(caption, "PEDESTRIAN  SELECTOR  %d / %d", pedspawnActiveLineIndex + 1, lineCount);
			draw_menu_line(caption, 350.0, 100.0, 18.0, 600.0, 5.0, false, true);
			for (int i = 0; i < itemCount; i++)
				if (strlen(pedModels[pedspawnActiveLineIndex][i]) || strcmp(pedModelNames[pedspawnActiveLineIndex][i], "NONE") == 0)
					draw_menu_line(pedModelNames[pedspawnActiveLineIndex][i], 100.0f, 0.0f, 200.0f, 100.0f + i * 110.0f, 50.0f, i == pedspawnActiveItemIndex, false, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		if (bSelect)
		{
			menu_beep();
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPlayer, 0.0f, 3.5f, 0.0f);
			DWORD model = GAMEPLAY::GET_HASH_KEY((char *)pedModels[pedspawnActiveLineIndex][pedspawnActiveItemIndex]);
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model))	WAIT(0);
				{
					Ped modelx = PED::CREATE_PED(690, model, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);
					PED::SET_PED_MONEY(modelx, 10000);
					char *dict0 = "mini@strip_club@pole_dance@pole_dance1";
					char *anim0 = "pd_dance_01";
					STREAMING::REQUEST_ANIM_SET(anim0);
					STREAMING::REQUEST_ANIM_DICT(dict0);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict0) && !STREAMING::HAS_ANIM_DICT_LOADED(dict0)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(modelx, dict0, anim0, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				WAIT(100);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				waitTime = 200;
			}
		}
		else
			if (bBack)
			{
				menu_beep();
				break;
			}
			else
				if (bRight)
				{
					menu_beep();
					pedspawnActiveItemIndex++;
					int itemsMax = (pedspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
					if (pedspawnActiveItemIndex == itemsMax)
						pedspawnActiveItemIndex = 0;
					waitTime = 100;
				}
				else
					if (bLeft)
					{
						menu_beep();
						if (pedspawnActiveItemIndex == 0)
							pedspawnActiveItemIndex = (pedspawnActiveLineIndex == (lineCount - 1)) ? itemCountLastLine : itemCount;
						pedspawnActiveItemIndex--;
						waitTime = 100;
					}
					else
						if (bUp)
						{
							menu_beep();
							if (pedspawnActiveLineIndex == 0)
								pedspawnActiveLineIndex = lineCount;
							pedspawnActiveLineIndex--;
							waitTime = 200;
						}
						else
							if (bDown)
							{
								menu_beep();
								pedspawnActiveLineIndex++;
								if (pedspawnActiveLineIndex == lineCount)
									pedspawnActiveLineIndex = 0;
								waitTime = 200;
							}
		if (pedspawnActiveLineIndex == (lineCount - 1))
			if (pedspawnActiveItemIndex >= itemCountLastLine)
				pedspawnActiveItemIndex = 0;
	}
	return false;
}


int activeLineOnlinePlayer = 0;

void process_online_player_menu(int playerInt)
{
	const float lineWidth = 250.0;
	const int lineCount = 20;
	char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(playerInt));
	std::string caption = Name;

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "NUKE PLAYER", NULL, NULL },
		{ "SPAWN MONEY", NULL, NULL },
		{ "TELEPORT TO PLAYER", NULL, NULL },
		{ "TELEPORT PLAYER TO YOU", NULL, NULL },
		{ "FRAME MENU", NULL, NULL },
		{ "ATTACH OBJECT TO PLAYER", NULL, NULL },
		{ "GIVE PLAYER A WEAPON", NULL, NULL },
		{ "TRAP PLAYER IN HIS VEHICLE", NULL, NULL },
		{ "KICK PLAYER FROM HIS VEHICLE", NULL, NULL },
		{ "TAKE ALL WEAPONS", NULL, NULL },
		{ "GIVE ALL WEAPONS", NULL, NULL },
		{ "VEHICLE MENU", NULL, NULL },
		{ "CLONE PLAYER", NULL, NULL },
		{ "CRASH PLAYER", NULL, NULL },
		{ "FIRE LOOP", NULL, NULL },
		{ "KILL PEDS NEAR PLAYER", NULL, NULL },
		{ "SPAWN PED ON PLAYER", NULL, NULL },
		{ "GIVE PLAYER 3 STARS", NULL, NULL },
		{ "STUN PLAYER", NULL, NULL },
		{ "ATTACH NEARBY ENTITIES TO PLAYER", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineOnlinePlayer)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineOnlinePlayer].text, 
				lines[activeLineOnlinePlayer].pState), lineWidth, 2.0, 60 + activeLineOnlinePlayer * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			// All your player indentifiers
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			Player selectedPlayer = PLAYER::GET_PLAYER_PED(playerInt);
			BOOL bSelectedPlayerExists = ENTITY::DOES_ENTITY_EXIST(selectedPlayer);

			Vector3 playerPedPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
			Vector3	selectedPlayerPos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(0), 0);

			Vehicle playerPedVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			Vehicle selectedPlayerVeh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);

			menu_beep();
			switch (activeLineOnlinePlayer)
			{
				//Player playerGroup = PED::GET_PED_GROUP_INDEX(PLAYER::PLAYER_PED_ID());
			case 0:
				if (bSelectedPlayerExists)
				{
					Nuke(selectedPlayer, Name);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist");
				}
				break;


			case 1:
				if (bSelectedPlayerExists)
				{
					spawn_money_safe(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist");
				}
				break;


			case 2:
				if (bSelectedPlayerExists)
				{
					teleportToPlayer(playerPed, selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist, Cancelled Teleport!");
				}
				break;

			case 3:
				if (bSelectedPlayerExists)
				{
					teleportPlyToMe(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist, Cancelled Teleport!");
				}
				break;

			case 4:
				if (bSelectedPlayerExists)
				{
					process_frame_menu(playerInt);
								}
							
				break;
			case 5:
				if (bSelectedPlayerExists)
				{
									GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "Input your object model", "", "p_spinning_anus_s", "", "", "", 30);
									while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
									if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
									{
										notifyAboveMap("~HUD_COLOUR_RED~Invalid Prop Model");
										return;
									}
									else
									{
										char* objectModel = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
										Hash model = GAMEPLAY::GET_HASH_KEY(objectModel);
										STREAMING::REQUEST_MODEL(model);
										while (!STREAMING::HAS_MODEL_LOADED(model))
											WAIT(0);
										Object object = OBJECT::CREATE_OBJECT(model, 0, 0, 0, true, true, false);
										ENTITY::ATTACH_ENTITY_TO_ENTITY(object, selectedPlayer, PED::GET_PED_BONE_INDEX(selectedPlayer, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
									}
								}
							
				else
				{
					notifyAboveMap("Player Doesnt Exist");
				}
				break;
			case 6:
				if (bSelectedPlayerExists)
				{
					giveWeapon(selectedPlayer);
				}
				else
				{
					notifyAboveMap("Player Doesnt Exist");
				}
				break;

			case 7:
				if (bSelectedPlayerExists)
				{
					trapPlayerInVeh(selectedPlayer, Name);
				}
				else
				{
					notifyAboveMap("Player Doesnt Exist");
				}
				break;

			case 8:
				if (bSelectedPlayerExists)
				{
					
									kickPlayerFromVeh(selectedPlayer);
								}
						
				else
				{
					notifyAboveMap("Player Doesnt Exist, Cancelled Kick");
				}
				break;

			case 9:
				if (bSelectedPlayerExists)
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~All Weapon's Taken from Player");
					for (int i = 0; i < 32; i++)
					{
							if (PLAYER::GET_PLAYER_PED(playerInt) == PLAYER::PLAYER_PED_ID()) continue;
										{
											WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(playerInt), true);
										}
									}
								}
						
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player Doesnt Exist");
				}
				break;

			case 10:
				if (bSelectedPlayerExists)
				{
					giveAllWeapons(selectedPlayer);
					notifyAboveMap("~HUD_COLOUR_GREEN~Gave Player All weapons");
				}
				else
				{
					notifyAboveMap("Player Doesnt Exist");
				}
				break;



			case 11:
				
								process_online_vehicle_menu(playerInt);
						
				break;


			case 12:
				if (bSelectedPlayerExists)
				{
					
									Ped clone = PED::CLONE_PED(selectedPlayer, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
									WEAPON::GIVE_DELAYED_WEAPON_TO_PED(clone, GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN"), 1000, 1);
									AI::TASK_SHOOT_AT_ENTITY(clone, selectedPlayer, 999999, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_FULL_AUTO"));
								}
							
				break;

			case 13:
				if (bSelectedPlayerExists)
				{
					if (isAuthed)
					{
						CrashPlayer(selectedPlayer);
					}
					else
					{
						notifyAboveMap("~HUD_COLOUR_RED~Your are not a VIP member or a Developer");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;

			case 14:
				if (bSelectedPlayerExists)
				{
					
									FireLoop(selectedPlayer);
								}
							
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;


			case 15:
				if (bSelectedPlayerExists)
				{
					killNearbyPeds(selectedPlayer);
					explodeNearbyVehicles(selectedPlayer);
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;

			case 16:
				if (bSelectedPlayerExists)
				{
					{
						if (process_pedspawn_menu(selectedPlayer)) return;
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;


			case 17:
				give_player_three_stars(selectedPlayer);
				break;

			case 18:
				if (bSelectedPlayerExists)
				{
					if (PED::IS_PED_ON_FOOT(selectedPlayer))
					{
						Ped clone = PED::CLONE_PED(selectedPlayer, 0, 1, 1);
						ENTITY::SET_ENTITY_COORDS(clone, selectedPlayerPos.x, selectedPlayerPos.y, selectedPlayerPos.z + 20, 0, 0, 0, 1);
						Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN");
						if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
						{
							WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
							while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
								WAIT(0);
						}
						GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(selectedPlayerPos.x, selectedPlayerPos.y - 1, selectedPlayerPos.z, selectedPlayerPos.x, selectedPlayerPos.y + 1, selectedPlayerPos.z, 1, 1, weaponAssetRocket, clone, 1, 0, -1.0);
						PED::DELETE_PED(&clone);
						notifyAboveMap("~HUD_COLOUR_GREEN~Stunned Player");
					}
					else
					{
						Freeze(selectedPlayer);
						notifyAboveMap("~HUD_COLOUR_RED~Player Was in a vehicle press again");
					}
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;

			case 19:
				if (bSelectedPlayerExists)
				{
					attachNearbyVehsToPly(selectedPlayer);
					notifyAboveMap("~HUD_COLOUR_GREEN~Attached all nearby Vehicles and Peds to player");
				}
				else
				{
					notifyAboveMap("~HUD_COLOUR_RED~Player doesnt exist");
				}
				break;

			default:
				if (lines[activeLineOnlinePlayer].pState)
					*lines[activeLineOnlinePlayer].pState = !(*lines[activeLineOnlinePlayer].pState);
				if (lines[activeLineOnlinePlayer].pUpdated)
					*lines[activeLineOnlinePlayer].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineOnlinePlayer == 0)
						activeLineOnlinePlayer = lineCount;
					activeLineOnlinePlayer--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineOnlinePlayer++;
						if (activeLineOnlinePlayer == lineCount)
							activeLineOnlinePlayer = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexMass = 0;

bool process_mass_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 18;

	std::string caption = "MASS PLAYERS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "FORCE SUICIDE ALL", NULL, NULL },
		{ "CLEAR SKY OF PLAYERS", NULL, NULL },
		{ "TAKE WEAPONS FROM ALL", NULL, NULL },
		{ "ATTACH BEACH FIRE TO ALL", NULL, NULL },
		{ "INFINITE FIVE STARS ON ALL", &featureMassFiveStar, NULL },
		{ "SUICIDE LOOP", &featureMassSuicideLoop, NULL },
		{ "FREEZE LOOP", &featureMassFreezeLoop, NULL },
		{ "TRACERS", &featureTracer, NULL },
		{ "ESP", &featureESP, NULL },
		{ "POP ALL PLAYERS TIRES", NULL, NULL },
		{ "CHANGE ALL LISCENCE PLATES", NULL, NULL },
		{ "SET ALL PLAYERS ON FIRE", NULL, NULL },
		{ "AIMBOT", &featureMassAllPlyAimbot, NULL },
		{ "NAME ESP", &featureNameESP, NULL },
		{ "TRIGGERBOT", &featureMassTriggerbot },
		{ "ANTI-AIM", &featureMassAntiAim, NULL },
		{ "FUCK UP THE GARAGE", NULL, NULL },
		{ "KILL ALL SPEAKING PLAYERS", NULL, NULL }
	};



	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMass)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexMass].text, 
				lines[activeLineIndexMass].pState), lineWidth, 2.0, 60 + activeLineIndexMass * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vector3 PedPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0, 0.0, 0.0);
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMass)
			{
				// next radio track
			case 0:
				if (bPlayerExists)
				{
					massSuicide();
				}
				break;
			case 1:
				if (bPlayerExists)
				{
					ClearSkiesOfBitches();
				}
				break;

			case 2:
				if (bPlayerExists)
				{
					notifyAboveMap("~HUD_COLOUR_RED~All Weapon's Taken from All Player's");
					for (int i = 0; i < 32; i++)
					{
						
										if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
										{
											WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED(i), true);
										}
									}
								}
						
				break; 

			case 3:
				if (bPlayerExists)
				{
					beachFireEmUp();
				}
				break;


			case 9:
				if (bPlayerExists)
				{
					notifyAboveMap("~HUD_COLOUR_RED~Popped all Players tires");
					for (int i = 0; i < 32; i++)
					{
						
									if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
									{
										if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(i), 0))
										{
											RequestControl(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(i), 0));
											BurstSelectedPlayerTires(PLAYER::GET_PLAYER_PED(i));
										}
									}
								}
						
				break;


			case 10:
				if (bPlayerExists)
				{
					notifyAboveMap("~HUD_COLOUR_RED~Change all Players liscence plate text");
					for (int i = 0; i < 32; i++)
					{
						
										if (PLAYER::GET_PLAYER_PED(i) == PLAYER::PLAYER_PED_ID()) continue;
										{
											if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(i), 0))
											{
												Vehicle allVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(i), 0);
												RequestControl(allVeh);
												char* plateText = ("DMZREBEL");
												VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(allVeh, plateText);
											}
										}
									}
								}
					
				break;


			case 11:
				for (int i = 0; i < 32; i++)
				{
					notifyAboveMap("~HUD_COLOUR_RED~Set all Players on Fire");
					
									Player playerHandle = PLAYER::GET_PLAYER_PED(i);
									if (playerHandle == PLAYER::PLAYER_PED_ID()) continue;
									{
										Vector3 Mouth = PED::GET_PED_BONE_COORDS(playerHandle, SKEL_Head, 0.1f, 0.0f, 0.0f);
										FIRE::_ADD_SPECFX_EXPLOSION(Mouth.x, Mouth.y, Mouth.z, EXPLOSION_DIR_FLAME, EXPLOSION_DIR_FLAME, 1.0f, true, true, 0.0f);
										FIRE::START_SCRIPT_FIRE(Mouth.x, Mouth.y, Mouth.z, 5, false);
									}
								}
					
				break;


			case 16:
				if (bPlayerExists)
				{
					CreateObject(6959, 477.0000000, -419.3999900, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(6959, 496.7000100, -419.7000100, 2742.6001000, 0.0000000, 91.6560000, 359.9730000);
					CreateObject(6959, 477.6000100, -418.8999900, 2742.6001000, 0.0000000, 91.6530000, 359.9730000);
					CreateObject(6959, 497.2999900, -399.6000100, 2742.6001000, 0.0000000, 91.6530000, 269.0490000);
					CreateObject(6959, 496.5000000, -438.6000100, 2742.6001000, 0.0000000, 91.6480000, 269.0440000);
					CreateObject(6959, 477.0000000, -419.3999900, 2762.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(13187, 491.7999900, -438.8999900, 2758.6001000, 0.0000000, 0.0000000, 88.3620000);
					CreateObject(13187, 483.5000000, -438.7999900, 2758.6001000, 0.0000000, 0.0000000, 89.6380000);
					CreateObject(2323, 488.7000100, -401.2999900, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2306, 491.2999900, -401.3999900, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(1730, 493.2999900, -400.2000100, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(1518, 493.5000000, -400.2999900, 2758.7000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2226, 491.2999900, -400.2000100, 2758.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2229, 494.7000100, -400.1000100, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2229, 492.8999900, -400.1000100, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2297, 488.5000000, -400.2000100, 2759.3000000, 0.0000000, 0.0000000, 316.4590000);
					CreateObject(2654, 490.6000100, -400.2999900, 2757.5000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2980, 495.1000100, -400.2000100, 2758.1001000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2614, 487.2000100, -400.0000000, 2761.3999000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(2611, 492.0000000, -400.1000100, 2760.0000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(1616, 496.8999900, -439.1000100, 2762.1001000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(1584, 497.2000100, -404.0000000, 2759.5000000, 0.0000000, 0.0000000, 278.3490000);
					CreateObject(1585, 478.2000100, -432.3999900, 2759.5000000, 0.0000000, 0.0000000, 271.6380000);
					CreateObject(14699, 494.0000000, -430.1000100, 2760.5000000, 0.0000000, 0.0000000, 92.2040000);
					CreateObject(14699, 493.7000100, -420.7000100, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 493.2999900, -410.8999900, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 493.2999900, -403.2999900, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 485.7999900, -403.0000000, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 486.1000100, -411.3999900, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 486.7999900, -420.6000100, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(14699, 485.7000100, -430.5000000, 2760.5000000, 0.0000000, 0.0000000, 92.2030000);
					CreateObject(2502, 478.8999900, -400.0000000, 2757.3000000, 0.0000000, 0.0000000, 0.0000000);
					CreateObject(19273, 487.68793, -438.88925, 2758.78760, 0.00000, 0.00000, 175.91960);
					CreateObject(19900, 480.10303, -400.07925, 2757.25903, 0.00000, 0.00000, 0.00000);
					CreateObject(19900, 480.74579, -400.07465, 2757.25903, 0.00000, 0.00000, 0.00000);
					CreateObject(19815, 482.35214, -399.79022, 2758.80859, 0.00000, 0.00000, -0.78000);
					CreateObject(11737, 481.59167, -401.42493, 2757.26416, 0.00000, 0.00000, 0.00000);
					CreateObject(2465, 479.12677, -400.20123, 2758.53125, 0.00000, 0.00000, 0.00000);
					CreateObject(19617, 487.34158, -399.89642, 2760.01953, 0.00000, 0.00000, 0.00000);
					CreateObject(19804, 488.39011, -400.00421, 2758.41748, 0.00000, 0.00000, 0.00000);
					CreateObject(19805, 485.45383, -399.87045, 2759.29175, 0.00000, 0.00000, 0.00000);
				}
				break;


			case 17:
				KillalltheSpeakingPlayers();
				break;

				// switchable features
			default:
				if (lines[activeLineIndexMass].pState)
					*lines[activeLineIndexMass].pState = !(*lines[activeLineIndexMass].pState);
				if (lines[activeLineIndexMass].pUpdated)
					*lines[activeLineIndexMass].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexMass == 0)
						activeLineIndexMass = lineCount;
					activeLineIndexMass--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexMass++;
						if (activeLineIndexMass == lineCount)
							activeLineIndexMass = 0;
						waitTime = 150;
					}
	}
	return false;
}

int activeLineIndexOnline = 0;
void process_player3_menu();
{
	const float lineWidth = 250.0;
	const int lineCount = 32;

	std::string caption = "INDIVIDUAL PLAYERS";

	typedef struct {
		std::string		name;
		Ped				ped;
		int			playerInt;
	} lines;

	lines onlinePlayers[32];

	int spacesBefore = 0;
	for (int i = 0; i < lineCount; i++)
	{
			onlinePlayers[i] = { PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i)), PLAYER::GET_PLAYER_PED(i), i };
	}
	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)

				if (i != activeLineIndexOnline)
					draw_menu_line(onlinePlayers[i].name, lineWidth, 2.0, 60 + i * 20, 1000.0,122.5, false, false);
			draw_menu_line(onlinePlayers[activeLineIndexOnline].name, 
				lineWidth, 2.0, 60 + activeLineIndexOnline * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			if (onlinePlayers[activeLineIndexOnline].name.compare("**Invalid**") != 0)
				process_online_player_menu(onlinePlayers[activeLineIndexOnline].playerInt);
			else
			{
				set_status_text("This Player doesnt exist, it is just a open slot!");
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexOnline == 0)
						activeLineIndexOnline = lineCount;
					activeLineIndexOnline--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexOnline++;
						if (activeLineIndexOnline == lineCount)
							activeLineIndexOnline = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexPlayer101 = 0;

void process_player101_menu();
{
	const float lineWidth = 250.0;
	const int lineCount = 2;

	std::string caption = "OTHER PLAYERS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "INDIVIDUAL PLAYERS", NULL, NULL },
		{ "ALL PLAYERS", NULL, NULL }

	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexPlayer101)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexPlayer101].text, 
				lines[activeLineIndexPlayer101].pState), lineWidth, 2.0, 60 + activeLineIndexPlayer101 * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vector3 PedPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0, 0.0, 0.0);
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexPlayer101)
			{
				// next radio track
			case 0:
				process_player3_menu();
				break;
			case 1:
				if (process_mass_menu()) return;
				break;
				// switchable features
			default:
				if (lines[activeLineIndexPlayer101].pState)
					*lines[activeLineIndexPlayer101].pState = !(*lines[activeLineIndexPlayer101].pState);
				if (lines[activeLineIndexPlayer101].pUpdated)
					*lines[activeLineIndexPlayer101].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexPlayer101 == 0)
						activeLineIndexPlayer101 = lineCount;
					activeLineIndexPlayer101--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexPlayer101++;
						if (activeLineIndexPlayer101 == lineCount)
							activeLineIndexPlayer101 = 0;
						waitTime = 150;
					}
	}
}

int activeLineIndexIPL = 0;
void process_ipl_menu();
{
	const float lineWidth = 250.0;
	const int lineCount = 8;

	std::string caption = "IPL LOADER";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "NORTH YANKTON", NULL, NULL },
		{ "STILT HOUSE", NULL, NULL },
		{ "CLUCKING BELL", NULL, NULL },
		{ "UFO", NULL, NULL },
		{ "LIFEINVADER LOBBY", NULL, NULL },
		{ "ONIEL RANCH", NULL, NULL },
		{ "HEIST CARRIER", NULL, NULL },
		{ "HOSPITAL", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexIPL)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexIPL].text, 
				lines[activeLineIndexIPL].pState), lineWidth, 2.0, 60 + activeLineIndexIPL * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexIPL)
			{
				// Load North Yankton
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_IPL("plg_01");
					STREAMING::REQUEST_IPL("prologue01");
					STREAMING::REQUEST_IPL("prologue01_lod");
					STREAMING::REQUEST_IPL("prologue01c");
					STREAMING::REQUEST_IPL("prologue01c_lod");
					STREAMING::REQUEST_IPL("prologue01d");
					STREAMING::REQUEST_IPL("prologue01d_lod");
					STREAMING::REQUEST_IPL("prologue01e");
					STREAMING::REQUEST_IPL("prologue01e_lod");
					STREAMING::REQUEST_IPL("prologue01f");
					STREAMING::REQUEST_IPL("prologue01f_lod");
					STREAMING::REQUEST_IPL("prologue01g");
					STREAMING::REQUEST_IPL("prologue01h");
					STREAMING::REQUEST_IPL("prologue01h_lod");
					STREAMING::REQUEST_IPL("prologue01i");
					STREAMING::REQUEST_IPL("prologue01i_lod");
					STREAMING::REQUEST_IPL("prologue01j");
					STREAMING::REQUEST_IPL("prologue01j_lod");
					STREAMING::REQUEST_IPL("prologue01k");
					STREAMING::REQUEST_IPL("prologue01k_lod");
					STREAMING::REQUEST_IPL("prologue01z");
					STREAMING::REQUEST_IPL("prologue01z_lod");
					STREAMING::REQUEST_IPL("plg_02");
					STREAMING::REQUEST_IPL("prologue02");
					STREAMING::REQUEST_IPL("prologue02_lod");
					STREAMING::REQUEST_IPL("plg_03");
					STREAMING::REQUEST_IPL("prologue03");
					STREAMING::REQUEST_IPL("prologue03_lod");
					STREAMING::REQUEST_IPL("prologue03b");
					STREAMING::REQUEST_IPL("prologue03b_lod");
					//the commented code disables the 'Prologue' grave and
					//enables the 'Bury the Hatchet' grave
					//STREAMING::REQUEST_IPL("prologue03_grv_cov");
					//STREAMING::REQUEST_IPL("prologue03_grv_cov_lod");
					STREAMING::REQUEST_IPL("prologue03_grv_dug");
					STREAMING::REQUEST_IPL("prologue03_grv_dug_lod");
					//STREAMING::REQUEST_IPL("prologue03_grv_fun");
					STREAMING::REQUEST_IPL("prologue_grv_torch");
					STREAMING::REQUEST_IPL("plg_04");
					STREAMING::REQUEST_IPL("prologue04");
					STREAMING::REQUEST_IPL("prologue04_lod");
					STREAMING::REQUEST_IPL("prologue04b");
					STREAMING::REQUEST_IPL("prologue04b_lod");
					STREAMING::REQUEST_IPL("prologue04_cover");
					STREAMING::REQUEST_IPL("des_protree_end");
					STREAMING::REQUEST_IPL("des_protree_start");
					STREAMING::REQUEST_IPL("des_protree_start_lod");
					STREAMING::REQUEST_IPL("plg_05");
					STREAMING::REQUEST_IPL("prologue05");
					STREAMING::REQUEST_IPL("prologue05_lod");
					STREAMING::REQUEST_IPL("prologue05b");
					STREAMING::REQUEST_IPL("prologue05b_lod");
					STREAMING::REQUEST_IPL("plg_06");
					STREAMING::REQUEST_IPL("prologue06");
					STREAMING::REQUEST_IPL("prologue06_lod");
					STREAMING::REQUEST_IPL("prologue06b");
					STREAMING::REQUEST_IPL("prologue06b_lod");
					STREAMING::REQUEST_IPL("prologue06_int");
					STREAMING::REQUEST_IPL("prologue06_int_lod");
					STREAMING::REQUEST_IPL("prologue06_pannel");
					STREAMING::REQUEST_IPL("prologue06_pannel_lod");
					STREAMING::REQUEST_IPL("prologue_m2_door");
					STREAMING::REQUEST_IPL("prologue_m2_door_lod");
					STREAMING::REQUEST_IPL("plg_occl_00");
					STREAMING::REQUEST_IPL("prologue_occl");
					STREAMING::REQUEST_IPL("plg_rd");
					STREAMING::REQUEST_IPL("prologuerd");
					STREAMING::REQUEST_IPL("prologuerdb");
					STREAMING::REQUEST_IPL("prologuerd_lod");
					WAIT(100);
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 3360.19f, -4849.67f, 111.8f, 0, 0, 0, 1);
					notifyAboveMap("~HUD_COLOUR_GREEN~North Yankton Loaded");
				}
				break;

			case 1:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~Stilt House Loaded");
					STREAMING::REQUEST_IPL("DES_StiltHouse_imapend");
					STREAMING::REQUEST_IPL("DES_StiltHouse_imapstart");
					STREAMING::REQUEST_IPL("des_stilthouse_rebuild");
				}
				break;
			case 2:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~Clucking Bell Loaded");
					STREAMING::REMOVE_IPL("CS1_02_cf_offmission");
					STREAMING::REQUEST_IPL("CS1_02_cf_onmission1");
					STREAMING::REQUEST_IPL("CS1_02_cf_onmission2");
					STREAMING::REQUEST_IPL("CS1_02_cf_onmission3");
					STREAMING::REQUEST_IPL("CS1_02_cf_onmission4");
				}
				break;

			case 3:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~UFO's Loaded go over Fort Zancudo, or go to Stoner Camp");
					STREAMING::REQUEST_IPL("ufo"); // Oniel Ranch
				}
				break;

			case 4:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{	
					STREAMING::REQUEST_IPL("facelobby"); // Life Invader lobby
					WAIT(100);
					notifyAboveMap("~HUD_COLOUR_GREEN~LiveInvader Lobby Loaded");
				}
				break;

			case 5:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STREAMING::REQUEST_IPL("cargoship"); // Oniel Ranch
					WAIT(100);
					notifyAboveMap("~HUD_COLOUR_GREEN~Oniel Ranch Loaded");
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 2441.9f, 4977.1f, 47.110f, 0, 0, 0, 1);
				}
				break;

			case 6:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~Heist Carrier Loaded, Go to the Heist Carrier");
					STREAMING::REQUEST_IPL("hei_carrier");
					STREAMING::REQUEST_IPL("hei_carrier_DistantLights"); // Heist Carrier
					STREAMING::REQUEST_IPL("hei_Carrier_int1");
					STREAMING::REQUEST_IPL("hei_Carrier_int2");
					STREAMING::REQUEST_IPL("hei_Carrier_int3");
					STREAMING::REQUEST_IPL("hei_Carrier_int4");
					STREAMING::REQUEST_IPL("hei_Carrier_int5");
					STREAMING::REQUEST_IPL("hei_Carrier_int6");
					STREAMING::REQUEST_IPL("hei_carrier_LODLights");
				}
				break;

			case 7:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					notifyAboveMap("~HUD_COLOUR_GREEN~Hospital Loaded, Go to the hospital");
					STREAMING::REQUEST_IPL("RC12B_Default");
					STREAMING::REQUEST_IPL("RC12B_Fixed"); // Hospital
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexIPL].pState)
					*lines[activeLineIndexIPL].pState = !(*lines[activeLineIndexIPL].pState);
				if (lines[activeLineIndexIPL].pUpdated)
					*lines[activeLineIndexIPL].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexIPL == 0)
						activeLineIndexIPL = lineCount;
					activeLineIndexIPL--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexIPL++;
						if (activeLineIndexIPL == lineCount)
							activeLineIndexIPL = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexObject = 0;

void process_object_menu();
{
	const float lineWidth = 250.0;
	const int lineCount = 5;

	std::string caption = "Object Menu";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "CREATE OBJECT", NULL, NULL },
		{ "ATTACH OBJECT TO YOURSELF", NULL, NULL },
		{ "ATTACH OBJECT TO VEHICLE", NULL, NULL },
		{ "SPAWN CUBE TO BLOCK CAR", NULL, NULL },
		{ "REMOVE OBJECTS FROM PLAYER", NULL, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexObject)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexObject].text, 
				lines[activeLineIndexObject].pState), lineWidth, 2.0, 60 + activeLineIndexObject * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);


		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexObject)
			{
				// Object Spawner
			case 0:
				if (bPlayerExists)
				{
					{

						GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "Input your object model", "", "p_spinning_anus_s", "", "", "", 30);
						while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
						if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
						{
							notifyAboveMap("~HUD_COLOUR_RED~Invalid Prop Model!");
							break;
						}
						else
						{
							char* objectModel = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
							Object object = GAMEPLAY::GET_HASH_KEY(objectModel);
							STREAMING::REQUEST_MODEL(object);
							Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0, 10.0, 0.0);
							OBJECT::CREATE_OBJECT(object, coords.x, coords.y, coords.z, true, true, true);

						}
					}
				}
				break;
				// Spawn object and Attach
			case 1:
				if (bPlayerExists)
				{

					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "Input your object model", "", "prop_weed_01", "", "", "", 30);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
					if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
					{
						notifyAboveMap("~HUD_COLOUR_RED~Invalid Prop Model!");
						break;
					}
					else
					{
						char* objectModel = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
						Hash model = GAMEPLAY::GET_HASH_KEY(objectModel);
						STREAMING::REQUEST_MODEL(model);
						while (!STREAMING::HAS_MODEL_LOADED(model))
							WAIT(0);
						Object object = OBJECT::CREATE_OBJECT(model, 0, 0, 0, true, true, false);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(object, playerPed, PED::GET_PED_BONE_INDEX(playerPed, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
					}
				}
				break;

				// Attach object to current vehicle
			case 2:
				if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
				{

					GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "Input your object model", "", "prop_windmill_01", "", "", "", 30);
					while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) WAIT(0);
					if (!STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())))
					{
						notifyAboveMap("~HUD_COLOUR_RED~Invalid Prop Model!");
						break;
					}
					else
					{
						char* objectModel = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
						Hash model = GAMEPLAY::GET_HASH_KEY(objectModel);
						STREAMING::REQUEST_MODEL(model);
						while (!STREAMING::HAS_MODEL_LOADED(model))
							WAIT(0);
						Object object = OBJECT::CREATE_OBJECT(model, 0, 0, 0, true, true, false);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(object, veh, PED::GET_PED_BONE_INDEX(playerPed, SKEL_Spine_Root), 0.0, 0.0, 0.0, 0.0, 90.0, 0, false, false, false, false, 2, true);
					}
				}
				break;


			case 3:
				if (bPlayerExists)
				{
					Object object = GAMEPLAY::GET_HASH_KEY("prop_dummy_car");
					STREAMING::REQUEST_MODEL(object);

					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0.0, 5.0, 0.0);
					OBJECT::CREATE_OBJECT(object, coords.x, coords.y, coords.z, true, true, true);
				}
				break;


			case 4:
				if (bPlayerExists)
				{
					RemoveAllPropsFromPlayer(playerPed);
				}
				break;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexObject == 0)
						activeLineIndexObject = lineCount;
					activeLineIndexObject--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexObject++;
						if (activeLineIndexObject == lineCount)
							activeLineIndexObject = 0;
						waitTime = 150;
					}
	}
}


int activeLineIndexAnimation = 0;

void process_animation_menu();
{
	const float lineWidth = 250.0;
	const int lineCount = 17;

	std::string caption = "ANIMATIONS OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "POLE DANCE ONE", NULL, NULL },
		{ "POLE DANCE TWO", NULL, NULL },
		{ "POLE DANCE THREE", NULL, NULL },
		{ "STUNNED", NULL, NULL },
		{ "SITUPS", NULL, NULL },
		{ "PUSHUPS", NULL, NULL },
		{ "WAVE ARMS", NULL, NULL },
		{ "SUICIDE", NULL, NULL },
		{ "ON THE CAN", NULL, NULL },
		{ "ON FIRE", NULL, NULL },
		{ "COWER", NULL, NULL },
		{ "PRIVATE DANCE", NULL, NULL },
		{ "SUCK DICK", NULL, NULL },
		{ "STUNGUN", NULL, NULL },
		{ "AIR FUCK", NULL, NULL },
		{ "AIR FUCK TWO", NULL, NULL },
		{ "STOP ANIMATION", NULL, NULL }
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0,125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexAnimation)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexAnimation].text, 
				lines[activeLineIndexAnimation].pState), lineWidth, 2.0, 60 + activeLineIndexAnimation * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();

			// common variables
			BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			switch (activeLineIndexAnimation)
			{
				// Pole Dance 1
			case 0:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict0 = "mini@strip_club@pole_dance@pole_dance1";
					char *anim0 = "pd_dance_01";
					STREAMING::REQUEST_ANIM_SET(anim0);
					STREAMING::REQUEST_ANIM_DICT(dict0);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict0) && !STREAMING::HAS_ANIM_DICT_LOADED(dict0)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict0, anim0, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 2
			case 1:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict1 = "mini@strip_club@pole_dance@pole_dance2";
					char *anim1 = "pd_dance_02";
					STREAMING::REQUEST_ANIM_SET(anim1);
					STREAMING::REQUEST_ANIM_DICT(dict1);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict1) && !STREAMING::HAS_ANIM_DICT_LOADED(dict1)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict1, anim1, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Pole Dance 3
			case 2:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict2 = "mini@strip_club@pole_dance@pole_dance3";
					char *anim2 = "pd_dance_03";
					STREAMING::REQUEST_ANIM_SET(anim2);
					STREAMING::REQUEST_ANIM_DICT(dict2);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict2) && !STREAMING::HAS_ANIM_DICT_LOADED(dict2)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict2, anim2, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
				// Stun
			case 3:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict3 = "ragdoll@human";
					char *anim3 = "electrocute";
					STREAMING::REQUEST_ANIM_SET(anim3);
					STREAMING::REQUEST_ANIM_DICT(dict3);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict3) && !STREAMING::HAS_ANIM_DICT_LOADED(dict3)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict3, anim3, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;


			case 4:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict5 = "amb@world_human_sit_ups@male@base";
					char *anim5 = "base";
					STREAMING::REQUEST_ANIM_SET(anim5);
					STREAMING::REQUEST_ANIM_DICT(dict5);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict5) && !STREAMING::HAS_ANIM_DICT_LOADED(dict5)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict5, anim5, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 5:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict6 = "amb@world_human_push_ups@male@base";
					char *anim6 = "base";
					STREAMING::REQUEST_ANIM_SET(anim6);
					STREAMING::REQUEST_ANIM_DICT(dict6);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict6) && !STREAMING::HAS_ANIM_DICT_LOADED(dict6)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict6, anim6, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 6:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict7 = "random@car_thief@waving_ig_1";
					char *anim7 = "waving";
					STREAMING::REQUEST_ANIM_SET(anim7);
					STREAMING::REQUEST_ANIM_DICT(dict7);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict7) && !STREAMING::HAS_ANIM_DICT_LOADED(dict7)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict7, anim7, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 7:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict8 = "mp_suicide";
					char *anim8 = "pistol_fp";
					STREAMING::REQUEST_ANIM_SET(anim8);
					STREAMING::REQUEST_ANIM_DICT(dict8);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict8) && !STREAMING::HAS_ANIM_DICT_LOADED(dict8)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict8, anim8, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 8:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict9 = "timetable@trevor@on_the_toilet";
					char *anim9 = "trevonlav_struggleloop";
					STREAMING::REQUEST_ANIM_SET(anim9);
					STREAMING::REQUEST_ANIM_DICT(dict9);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict9) && !STREAMING::HAS_ANIM_DICT_LOADED(dict9)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict9, anim9, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;




			case 9:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict10 = "ragdoll@human";
					char *anim10 = "on_fire";
					STREAMING::REQUEST_ANIM_SET(anim10);
					STREAMING::REQUEST_ANIM_DICT(dict10);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict10) && !STREAMING::HAS_ANIM_DICT_LOADED(dict10)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict10, anim10, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 10:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict11 = "random@arrests";
					char *anim11 = "kneeling_arrest_idle";
					STREAMING::REQUEST_ANIM_SET(anim11);
					STREAMING::REQUEST_ANIM_DICT(dict11);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict11) && !STREAMING::HAS_ANIM_DICT_LOADED(dict11)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict11, anim11, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 11:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict12 = "mini@strip_club@private_dance@part1";
					char *anim12 = "priv_dance_p1";
					STREAMING::REQUEST_ANIM_SET(anim12);
					STREAMING::REQUEST_ANIM_DICT(dict12);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict12) && !STREAMING::HAS_ANIM_DICT_LOADED(dict12)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict12, anim12, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;





			case 12:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict13 = "mini@prostitutes@sexnorm_veh";
					char *anim13 = "bj_loop_prostitute";
					STREAMING::REQUEST_ANIM_SET(anim13);
					STREAMING::REQUEST_ANIM_DICT(dict13);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict13) && !STREAMING::HAS_ANIM_DICT_LOADED(dict13)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict13, anim13, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;



			case 13:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict14 = "stungun@standing";
					char *anim14 = "Damage";
					STREAMING::REQUEST_ANIM_SET(anim14);
					STREAMING::REQUEST_ANIM_DICT(dict14);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict14) && !STREAMING::HAS_ANIM_DICT_LOADED(dict14)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict14, anim14, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 14:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict15 = "rcmpaparazzo_2";
					char *anim15 = "shag_loop_a";
					STREAMING::REQUEST_ANIM_SET(anim15);
					STREAMING::REQUEST_ANIM_DICT(dict15);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict15) && !STREAMING::HAS_ANIM_DICT_LOADED(dict15)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict15, anim15, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;

			case 15:
				if (bPlayerExists)
				{
					Ped playerPed = PLAYER::PLAYER_PED_ID();
					char *dict16 = "rcmpaparazzo_2";
					char *anim16 = "shag_loop_poppy";
					STREAMING::REQUEST_ANIM_SET(anim16);
					STREAMING::REQUEST_ANIM_DICT(dict16);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict16) && !STREAMING::HAS_ANIM_DICT_LOADED(dict16)) WAIT(0);
					{
						AI::TASK_PLAY_ANIM(playerPed, dict16, anim16, 1, 1, -1, 47, 0, false, 0, false);
					}
				}
				break;
			case 16:
				if (bPlayerExists)
				{
					Freeze(playerPed);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexAnimation].pState)
					*lines[activeLineIndexAnimation].pState = !(*lines[activeLineIndexAnimation].pState);
				if (lines[activeLineIndexAnimation].pUpdated)
					*lines[activeLineIndexAnimation].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexAnimation == 0)
						activeLineIndexAnimation = lineCount;
					activeLineIndexAnimation--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexAnimation++;
						if (activeLineIndexAnimation == lineCount)
							activeLineIndexAnimation = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexWorld = 0;

void process_world_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 11;

	std::string caption = "WORLD OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "MOON GRAVITY", &featureWorldMoonGravity, NULL },
		{ "RANDOM COPS", &featureWorldRandomCops, NULL },
		{ "RANDOM TRAINS", &featureWorldRandomTrains, NULL },
		{ "RANDOM BOATS", &featureWorldRandomBoats, NULL },
		{ "GARBAGE TRUCKS", &featureWorldGarbageTrucks, NULL },
		{ "DELETE ALL PEDS", &featureWorldNoPeds, NULL },
		{ "DELETE ALL VEHICLES", &featureWorldNoVehs, NULL },
		{ "DELETE ALL OBJECTS", &featureWorldNoObjects, NULL },
		{ "DELETE ALL COPS", &featureWorldNoCops, NULL },
		{ "DELETE ALL PROJECTILES", &featureWorldNoProjectiles, NULL },
		{ "ENABLE RESTRICTED ZONES", &featureWorldRestrictedZones }
	};

	DWORD waitTime = 150;
	while (true)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexWorld)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexWorld].text, lines[activeLineIndexWorld].pState), 
				lineWidth, 2.0, 60 + activeLineIndexWorld * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			if (lines[activeLineIndexWorld].pState)
				*lines[activeLineIndexWorld].pState = !(*lines[activeLineIndexWorld].pState);
			if (lines[activeLineIndexWorld].pUpdated)
				*lines[activeLineIndexWorld].pUpdated = true;
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexWorld == 0)
						activeLineIndexWorld = lineCount;
					activeLineIndexWorld--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexWorld++;
						if (activeLineIndexWorld == lineCount)
							activeLineIndexWorld = 0;
						waitTime = 150;
					}
	}
}



int activeLineIndexLevel = 0;

void process_level_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 6;

	std::string caption = "LEVEL OPTIONS";

	static struct {
		LPCSTR		text;
		bool		*pState;
		bool		*pUpdated;
	} lines[lineCount] = {
		{ "LEVEL 50", NULL, NULL },
		{ "LEVEL 100", NULL, NULL },
		{ "LEVEL 150", NULL, NULL },
		{ "LEVEL 200", NULL, NULL },
		{ "LEVEL 250", NULL, NULL },
		{ "LEVEL 1000", NULL, NULL }
	};


	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexLevel)
					draw_menu_line(line_as_str(lines[i].text, lines[i].pState), lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(line_as_str(lines[activeLineIndexLevel].text, 
				lines[activeLineIndexLevel].pState), lineWidth, 2.0, 60 + activeLineIndexLevel * 20, 1000.0, 122.5, true, false);

			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexLevel)
			{
			case 0:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 448800, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 448800, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 448800, true);
				}
				break;
			case 1:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 1584350, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 1584350, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 1584350, true);
				}
				break;
			case 2:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 3075600, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 3075600, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 3075600, true);
				}
				break;
			case 3:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 4691850, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 4691850, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 4691850, true);
				}
				break;
			case 4:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 6433100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 6433100, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 6433100, true);
				}
				break;
			case 5:
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
				{
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP0_CHAR_XP_FM"), 47488300, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP1_CHAR_XP_FM"), 47488300, true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("MP2_CHAR_XP_FM"), 47488300, true);
				}
				break;
				// switchable features
			default:
				if (lines[activeLineIndexLevel].pState)
					*lines[activeLineIndexLevel].pState = !(*lines[activeLineIndexLevel].pState);
				if (lines[activeLineIndexLevel].pUpdated)
					*lines[activeLineIndexLevel].pUpdated = true;
			}
			waitTime = 200;
		}
		else
			if (bBack || trainer_switch_pressed())
			{
				menu_beep();
				break;
			}
			else
				if (bUp)
				{
					menu_beep();
					if (activeLineIndexLevel == 0)
						activeLineIndexLevel = lineCount;
					activeLineIndexLevel--;
					waitTime = 150;
				}
				else
					if (bDown)
					{
						menu_beep();
						activeLineIndexLevel++;
						if (activeLineIndexLevel == lineCount)
							activeLineIndexLevel = 0;
						waitTime = 150;
					}
	}
}










int activeLineIndexMain = 0;

void process_main_menu()
{
	const float lineWidth = 250.0;
	const int lineCount = 12;	

	std::string caption = "~HUD_COLOUR_BLUE~Project 7 ~HUD_COLOUR_WHITE~v0.1 ~HUD_COLOUR_YELLOW~DEV";

	static LPCSTR lineCaption[lineCount] = {
		"PLAYER OPTIONS",
		"WEAPON OPTIONS",
		"VEHICLE OPTIONS",
		"TIME OPTIONS",
		"WEATHER OPTIONS",
		"MISC OPTIONS",
		"RECOVERY OPTIONS",
		"LEVEL OPTIONS",
		"ANIMATION OPTIONS",
		"ONLINE PLAYERS",
		"IPL LOADER",
		"WORLD OPTIONS"
	};

	DWORD waitTime = 150;
	while (true)
	{
		// timed menu draw, used for pause after active line switch
		DWORD maxTickCount = GetTickCount() + waitTime;
		do 
		{
			// draw menu
			draw_menu_line(caption, lineWidth, 0.0, 38.0, 1000.0, 125.5, false, true);
			for (int i = 0; i < lineCount; i++)
				if (i != activeLineIndexMain)
					draw_menu_line(lineCaption[i], lineWidth, 2.0, 60 + i * 20, 1000.0, 122.5, false, false);
			draw_menu_line(lineCaption[activeLineIndexMain], lineWidth, 2.0, 60 + activeLineIndexMain * 20, 1000.0, 122.5, true, false);
			
			update_features();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
		waitTime = 0;

		// process buttons
		bool bSelect, bBack, bUp, bDown;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, NULL, NULL);
		if (bSelect)
		{
			menu_beep();
			switch (activeLineIndexMain)
			{
				case 0:
					process_player_menu();					
					break;
				case 1:
					process_weapon_menu();
					break;
				case 2:
					process_veh_menu();
					break;
				case 3:
					process_time_menu();
					break;
				case 4:
					process_weather_menu();
					break;
				case 5:
					process_misc_menu();
					break;
				case 6:
					process_rec_menu();
					break;
				case 7:
					process_level_menu();
					break;
				case 8:
					process_animation_menu();
					break;
				case 9:
					process_player101_menu();
					break;
				case 10:
					process_ipl_menu();
					break;
				case 11:
					process_world_menu();
					break;
			}
			waitTime = 200;
		} else
		if (bBack || trainer_switch_pressed())
		{
			menu_beep();
			break;
		} else
		if (bUp)
		{
			menu_beep();
			if (activeLineIndexMain == 0) 
				activeLineIndexMain = lineCount;
			activeLineIndexMain--;
			waitTime = 150;
		} else
		if (bDown)
		{
			menu_beep();
			activeLineIndexMain++;
			if (activeLineIndexMain == lineCount) 
				activeLineIndexMain = 0;			
			waitTime = 150;
		}
	}
}

void reset_globals()
{
	activeLineIndexMain			=
	activeLineIndexPlayer		=
	skinchangerActiveLineIndex	=
	skinchangerActiveItemIndex	=
	teleportActiveLineIndex		=
	activeLineIndexWeapon		=
	activeLineIndexVeh			=
	carspawnActiveLineIndex		=
	carspawnActiveItemIndex		=
	activeLineIndexWeather		=	0;

	featurePlayerInvincible			=
	featurePlayerInvincibleUpdated	=
	featurePlayerNeverWanted		=

	featureWepModFire				= 
	featureWepModFireWork			=
	featureWepModFlare				= 
	featureWepModTeleport			=
	featuerWepModGravity			=
	featureWepModRocket				=
	featureWepModValkyrie			=
	featureWeaponWhaleGun			=
	featureWeaponSafeGun			=
	featureWepModMoney				= 
	featureWepModAnimalGun			=
	featureWepModForceGun			=
	featureWepModVehGun				=
	featureWepModExplosive			= 
	featureWepModWater				= 
	featureWepModMolotov			= 
	featurePlayerIgnored			=
	featurePlayerIgnoredUpdated		=
	featurePlayerUnlimitedAbility	=
	featurePlayerNoNoise			=
	featurePlayerNoNoiseUpdated		=
	featurePlayerFastSwim			=
	featurePlayerFastSwimUpdated	=
	featurePlayerFastRun			=
	featurePlayerFastRunUpdated		=
	featurePlayerSuperJump			=
	featurePlayerSuperRun			=
	featurePlayerUltraJump			=
	featureWeaponNoReload			=
	featureWeaponInfiniteParachutes =
	featureWeaponRainbowTint		=
	featureWeaponFireAmmo			=
	featureWeaponMeleeOneHit		=
	featureWeaponVehicleDamage		=
	featureWeaponExplosiveAmmo		=
	featureWeaponExplosiveMelee		=
	featureWeaponVehRockets			=
	featureWeaponOneHit				=
	featureVehInvincible			=
	featureVehInvincibleUpdated		=
	featureVehScrollingText			=
	featureVehSpeedBoost			=
	featureVehWrapInSpawned			=
	featureVehSpawnWithAllMods		=
	featureWorldMoonGravity			=
	featureVisionNight				=
	featureVisionDrunk				=
	featureVisionDrunkUpdated		=
	featureVisionThermal			=
	featureTimePaused				=
	featureTimePausedUpdated		=
	featureTimeSynced				=
	featureWeatherWind				=
	featureMiscLockRadio			=
	featureMiscHideHud				=	false;
	featureMiscForceField			=
	featureVehSeatbeltUpdated		=
	featureVehSeatbelt				=
	featureVehRainbow				= 
	featureVehNeon					=
	featureVehSmoke					=
	featureVehSuperRPM				=
	featureVehStickyCar				= 
	featureMassFiveStar				= 
	featureOnlinePlayerInfo			=
	featureMiscMobileRadio			=
	featureTracer					=
	featureESP						=
	featureMassAntiAim				=
	featureNameESP					=
	featureMassAllPlyAimbot			=
	featureMassTriggerbot			=
	featureMassFreezeLoop			=
	featureMassSuicideLoop			=
	featureVehSlide					= 
	featureVehInvisible				=
	featureWorldRandomCops			=
	featureWorldPedIgnore			=
	featureWorldPedIgnoreUpdated	=
	featureMiscFlyMode				=
	featureWorldRandomTrains		=
	featureWorldRandomBoats			=
	featureWorldGarbageTrucks		=	
	featureWorldNoPeds				=
	featureWorldNoVehs				=
	featureWorldNoObjects			=
	featureWorldNoCops				=
	featureWorldNoProjectiles		=
	featureWorldRestrictedZones		=

	skinchanger_used			=	false;
}

void main()
{	
	BYPASS_ONLINE();

	reset_globals();

	while (true)
	{
		if (trainer_switch_pressed())
		{
			reset_trainer_switch();
			process_main_menu();
			DWORD time = GetTickCount() + 1000;
			while (GetTickCount() < time)
			{
				update_features();
				WAIT(0);
			}
			reset_trainer_switch();
		}
		update_features();

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
