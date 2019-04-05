#pragma once
#include "../utils/Utils.hpp"

#include "../memory/local_types.hpp"

auto game_globals_get_campaign_difficulty_get_hook()
{
	auto game_globals = game_globals_get();
	if (game_globals->game_options.MapType.value == e_map_type::_campaign)
		return game_globals->game_options.DifficultyLevel.value;
	return e_difficulty_level::_normal;
}

game_globals *game_globals_update_end_match_wait_time_hook(float a1)
{
	static auto game_time_ticks_per_second_delta_whole = (int(*)(float))(0x564BB0);
	auto game_globals = game_globals_get();
	if (game_globals->unknown24BE4 && game_globals->end_match_wait_time > 0)
		game_globals->end_match_wait_time += game_time_ticks_per_second_delta_whole(a1);
	return game_globals;
}

int game_globals_get_structure_bsp_index_hook()
{
	return game_globals_get()->structure_bsp_index;
}

game_globals *game_globals_set_playback_none_hook()
{
	auto game_globals = game_globals_get();
	game_globals->game_options.GamePlayback.value = e_game_playback::_none;
	return game_globals;
}

e_game_playback *__cdecl game_globals_get_playback_hook()
{
	return &game_globals_get()->game_options.GamePlayback;
}

e_game_playback __cdecl game_globals_set_playback_hook(e_game_playback a1)
{
	game_globals_get()->game_options.GamePlayback = a1;
	return a1;
}

int game_insertion_point_get_hook()
{
	auto game_globals = game_globals_get();
	if (game_globals->game_options.MapType.value == e_map_type::_campaign)
		return game_globals->game_options.InsertionPoint.value;
	return 0;
}

game_globals *game_globals_set_cinematic_is_playing_false_hook()
{
	auto game_globals = game_globals_get();
	game_globals->cinematic_is_playing = false;
	return game_globals;
}

game_globals *game_globals_set_cinematic_is_playing_true_hook()
{
	auto game_globals = game_globals_get();
	game_globals->cinematic_is_playing = true;
	return game_globals;
}

s_progression game_globals_get_game_progression_hook()
{
	return game_progression_get();
}

char game_and_map_variant_initialize_after_load_hook()
{
	static auto game_options_verify_game_variant = (bool(*)(uint32_t *))(0x572920);
	static auto game_options_verify_map_variant = (bool(*)(uint32_t *))(0x586D00);
	auto game_globals = game_globals_get();
	game_options_verify_game_variant((uint32_t *)&game_globals->game_options.GameVariant);

	printf_s("initializing game_variant and map_variant: after_load\n");
	return game_options_verify_map_variant((uint32_t *)&game_globals->game_options.MapVariant);
}

uint32_t *active_camera_initialize_after_load_hook()
{
	static auto active_camera_initialize_after_load_internal = (uint32_t *(*)())(0x5937F0);

	printf_s("initializing active_camera: after_load\n");
	return active_camera_initialize_after_load_internal();
}

inline void AddGameGlobalsHooks(const char *name)
{
	if (ConfigManager.GetBool("Hooks", name))
	{
		AddHook({ 0x130C70 }, &game_globals_get_campaign_difficulty_get_hook, "game_globals_get_campaign_difficulty");
		AddHook({ 0x130FF0 }, &game_globals_update_end_match_wait_time_hook, "game_globals_update_end_match_wait_time");
		AddHook({ 0x131150 }, &game_globals_get_structure_bsp_index_hook, "game_globals_get_structure_bsp_index");
		AddHook({ 0x132280 }, &game_globals_set_playback_none_hook, "game_globals_set_playback_none");

		//AddHook({ 0x132A80 }, &game_globals_get_playback_hook, "game_globals_get_playback");
		//AddHook({ 0x132AA0 }, &game_globals_set_playback_hook, "game_globals_set_playback");

		AddHook({ 0x132BB0 }, &game_globals_set_cinematic_is_playing_false_hook, "game_globals_set_cinematic_is_playing_false");
		AddHook({ 0x132BD0 }, &game_globals_set_cinematic_is_playing_true_hook, "game_globals_set_cinematic_is_playing_true");
		AddHook({ 0x132B90 }, &game_globals_get_game_progression_hook, "game_globals_get_game_progression");

		//AddHook({ 0x1326B0 }, &game_and_map_variant_initialize_after_load_hook, "game_and_map_variant_initialize_after_load");
		//AddHook({ 0x1917D0 }, &active_camera_initialize_after_load_hook, "active_camera_initialize_after_load");
	}
}

inline void AddGameGlobalsPatches(const char *name)
{
	if (ConfigManager.GetBool("Patches", name))
	{

	}
}