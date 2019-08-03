#pragma once

#include <Utils.hpp>

#include "../memory/tag_groups.hpp"
#include "../memory/local_types.hpp"
#include "filo.hpp"
#include "tags.hpp"

namespace cache
{
	namespace cache_files_windows
	{
		bool read(int a2, LONG tag_offset, DWORD size, LPVOID buffer);
		char *get_build();
		static auto validate = (bool(__cdecl *)(s_cache_file_header * cache_file_header))(0x501950);
		s_cache_file_header *get_header();
		static auto partitions = (uint32_t(__thiscall *)(uint32_t *))(0x502500);
		char load_root_tag(uint32_t tag_index);
		bool open_tags();
		bool setup();
		bool read_tag(LONG tag_offset, DWORD size, LPVOID buffer);
		static auto dispose = (uint32_t(__cdecl *)())(0x502CE0);
		bool load_tags(char *scenario_path);
		static auto sub_5031A0 = (bool(__cdecl *)())(0x5031A0);
		bool initialize(char *scenario_path, s_cache_file_header *cache_file_header);
		static auto dispose_from_old_map = (void *(__cdecl *)(int runtime_resource_index))(0x5AB630);
		static auto initialize_for_new_map = (bool(__cdecl *)(int runtime_resource_index, char *scenario_path))(0x5ABAD0);
		static auto get_tag_runtime_resource_index_of_source_file = (int(__cdecl *)(char *scenario_path))(0x5ABE90);
	}

	bool load(int campaign_id, int map_id, char *scenario_path);
}

inline void SubmitCacheFilesHooks(const char *name)
{
	if (ConfigManager.GetBool("Hooks", name))
	{
		//HookManager.Submit({ 0x1016D0 }, &cache::cache_files_windows::read, "cache::cache_files_windows::read");
		HookManager.Submit({ 0x101940 }, &cache::cache_files_windows::get_build, "cache::cache_files_windows::get_build");
		HookManager.Submit({ 0x102F16 }, &cache::cache_files_windows::load_root_tag, "cache::cache_files_windows::load_root_tag", HookFlags::IsCall);
		HookManager.Submit({ 0x1028C0 }, &cache::cache_files_windows::open_tags, "cache::cache_files_windows::open_tags");
		HookManager.Submit({ 0x102E9B }, &cache::cache_files_windows::setup, "cache::cache_files_windows::setup", HookFlags::IsCall);
		//HookManager.Submit({ 0x102C90 }, &cache::cache_files_windows::read_tag, "cache::cache_files_windows::read_tag");
		HookManager.Submit({ 0x102E1E }, &cache::cache_files_windows::initialize, "cache::cache_files_windows::initialize", HookFlags::IsCall);

		HookManager.Submit({ 0x0EA5E0 }, &cache::load, "cache::load");
		HookManager.Submit({ 0x0EA5EE }, &cache::cache_files_windows::load_tags, "cache::cache_files_windows::load_tags", HookFlags::IsCall);
	}
}

void cache_read_patch()
{
	if (ConfigManager.GetBool("Maps", "UseNewCacheStyle"))
	{
		// patches (g_cache_file_header.ExternalDependencies & 2) to true
		Patch(0x1027C7, { 0xEB }).Apply();
		Patch(0x102C9A, { 0xEB }).Apply();
		//Patch::NopFill(0x102B54, 6);
		//Patch::NopFill(0x102907, 2);
		//Patch::NopFill(0x102F27, 2);
	}
}

inline void SubmitCacheFilesPatches(const char *name)
{
	if (ConfigManager.GetBool("Patches", name))
	{
		PatchManager.Submit(&cache_read_patch, "cache_read_patch");
	}
}

namespace cache
{
	//-	004EA5E0, cache::load
	//-	005016D0, cache::cache_files_windows::read
	//-	00501940, cache::cache_files_windows::get_build, called on draw_watermark
	//	00501950, cache::cache_files_windows::validate
	//	00501B90, cache::cache_files_windows::calculate_load_percentage_jump
	//	00501BF0, cache::cache_files_windows::calculate_load_percentage
	//-	00501F90, cache::cache_files_windows::get_header
	//	00501FA0, cache::cache_files_windows::get_rsa
	//	00501FD0, cache::cache_files_windows::sub_501FD0, used in c_cache_file_tag_resource_runtime_manager::initialize_for_new_map::c_cache_file_page_restorer
	//	00502210, cache::cache_files_windows::hash_validate
	//	00502300, cache::cache_files_windows::sub_502300, used in cache::cache_files_windows::do_work_internal as right side of ^ (bitwise or)
	//	00502500, cache::cache_files_windows::partitions
	//	00502500, cache::cache_files_windows::sub_502550, used at the very beginning of cache::cache_files_windows::sub_501FD0
	//-	00502780, cache::cache_files_windows::load_root_tag
	//-	005028C0, cache::cache_files_windows::open_tags
	//-	00502970, cache::cache_files_windows::read_from_tag_list
	//-	00502B40, cache::cache_files_windows::setup
	//-	00502C90, cache::cache_files_windows::read_tag
	//	00502CE0, cache::cache_files_windows::dispose
	//-	00502DC0, cache::cache_files_windows::load_tags
	//	005031A0, cache::cache_files_windows::sub_5031A0, used in cache::cache_files_windows::load after hash verification is successful
	//	00503200, cache::cache_files_windows::release
	//	00503300, cache::dispose
	//	00503340, cache::initialize
	//	00503470, cache::cache_files_windows::sub_503470, used in cache::cache_files_windows::load_root_tag
	//	005A97C0, cache::cache_files_windows::do_work_internal
	//	005AA060, cache::cache_files_windows::get_resource_runtime_file_handle
	//	005AA0C0, cache::cache_files_windows::get_resource_runtime_file_handle_from_scenario_path
	//	005AA1D0, cache::cache_files_windows::get_io_completion_key_from_scenario_path
	//	005AA260, cache::cache_files_windows::get_cache_file_handle
	//	005AA300, cache::cache_files_windows::get_cache_file_handle2
	//	005AA330, cache::cache_files_windows::get_interop_debug_section_size
	//	005AA3C0, cache::cache_files_windows::get_resource_runtime_file_handle2
	//	005AA420, cache::cache_files_windows::get_resource_runtime_file_handle2_from_scenario_path
	//	005AA560, cache::cache_files_windows::get_interop_debug_section_size_from_scenario_path
	//	005AA660, cache::cache_files_windows::has_valid_cache_file_index
	//-	005AA7C0, cache::cache_files_windows::initialize
	//	005AA8E0, cache::cache_files_windows::get_guid
	//	005AA910, cache::cache_files_windows::get_guid_from_scenario_path
	//	005AAB20, cache::cache_files_windows::do_work
	//	005AAD30, cache::cache_files_windows::copy_map
	//	005AAE70, cache::cache_files_windows::copy_stop
	//	005AAFD0, cache::cache_files_windows::individual_map_progress
	//	005AB0E0, cache::cache_files_windows::get_file_status
	//	005AB190, cache::cache_files_windows::get_load_action
	//	005AB2B0, cache::cache_files_windows::get_individual_map_size
	//	005AB320, cache::cache_files_windows::get_file_length
	//	005AB630, cache::cache_files_windows::dispose_from_old_map
	//	005AB6F0, cache::cache_files_windows::campare_filetime
	//	005AB820, cache::cache_files_windows::get_cache_path
	//	005ABAD0, cache::cache_files_windows::initialize_for_new_map
	//	005ABBD0, cache::cache_files_windows::validation
	//	005ABDF0, cache::cache_files_windows::get_runtime_resource_count
	//	005ABE90, cache::cache_files_windows::get_tag_runtime_resource_index_of_source_file
	//	005AC420, cache::cache_files_windows::campare

	namespace cache_files_windows
	{
		bool read(int a2, LONG tag_offset, DWORD size, LPVOID buffer)
		{
			printf_s("cache::cache_files_windows::read: [tag_offset, 0x%04X, size, 0x%04X]\n", tag_offset, size);

			//return ((char(*)(uint8_t *, int, LONG, DWORD, LPVOID))(0x5016D0))(a1, a2, tag_offset, size, buffer);
			int file_error;
			if (!file_open(global_tag_cache_filo, 1, &file_error))
				return 0;
			if (!file_set_position_hook(global_tag_cache_filo, tag_offset, 0))
				return 0;
			return file_read_hook(global_tag_cache_filo, size, 0, buffer);
		}

		char *get_build()
		{
			auto result = g_cache_file_header->Build;
			//printf_s("cache::cache_files_windows::get_build: [cache_file_header->Build, %s]\n", result);

			return result;
		}
		s_cache_file_header *get_header()
		{
			auto result = g_cache_file_header;
			printf_s("cache::cache_files_windows::get_header: [cache_file_header->ScenarioPath, %s]\n", result->ScenarioPath);

			return result;
		}

		char load_root_tag(uint32_t tag_index)
		{
			printf_s("cache::cache_files_windows::load_root_tag: [tag_index, 0x%04X]\n", tag_index);
			auto result = ((char(*)(uint32_t tag_index))0x502780)(tag_index);

			return result;
		}

		bool open_tags()
		{
			cache_path.Update(ConfigManager.GetBool("Maps", "UseNewCacheStyle"));

			char result = 0;
			if (g_cache_file_header->ExternalDependencies & 2)
			{
				filo_create_hook(global_tag_cache_filo, cache_path.tags.Path.c_str(), 0);
				int file_error;
				result = file_open(global_tag_cache_filo, 1, &file_error);
			}

			cache_path.tags.Print();
			return result;
		}

		bool setup()
		{
			printf_s("cache::cache_files_windows::setup: [cache_file_header->ScenarioPath, %s]\n", g_cache_file_header->ScenarioPath);
			auto result = ((char(*)())0x502B40)();

			return result;
		}

		bool read_tag(LONG tag_offset, DWORD size, LPVOID buffer)
		{
			printf_s("cache::cache_files_windows::read_tag: [tag_offset, 0x%04X, size, 0x%04X]\n", tag_offset, size);
			if (!(g_cache_file_header->ExternalDependencies & 2))
				return read(2, tag_offset, size, buffer);

			int file_error;
			if (!file_open(global_tag_cache_filo, 1, &file_error))
				return 0;
			if (!file_set_position_hook(global_tag_cache_filo, tag_offset, 0))
				return 0;
			return file_read_hook(global_tag_cache_filo, size, 0, buffer);
		}

		struct s_scenario_definition : s_base_definition<0x824> {};
		auto g_scenario_definition = GetStructure<s_scenario_definition>(0x22AAEB4);
		auto g_scenario_tag_index = (uint32_t *)0x189CCF8;

		struct s_globals_definition : s_base_definition<0x608> {};
		auto g_globals_definition = GetStructure<s_globals_definition>(0x22AAEB8);
		auto g_globals_tag_index = (uint32_t *)0x189CCFC;

		auto g_resources_loaded = (bool *)0x22AAFF0;

		static auto main_loop_pregame = (char(__cdecl *)())(0x5063A0);
		static auto global_memory_map_allocate_data = (uint32_t *(__cdecl *)(int, int, int datum_size, uint32_t flags))(0x51D180);
		static auto secure_working_memory_get_cache_data = (uint32_t (__cdecl *)())(0x5018F0);

		bool load_tags(char *scenario_path)
		{
			printf_s("cache::cache_files_windows::load_tags: [scenario_path, %s]\n", scenario_path);

			if (!g_shouldPrint)
				return ((char(*)(char *))0x502DC0)(scenario_path);

			//void *secure_working_memory = 0;
			//int secure_working_memory_size = 0;
			//secure_working_memory_get(0, &secure_working_memory, &secure_working_memory_size);
			//if (secure_working_memory_size >= 0x38C8 && secure_working_memory)
			//	memset(secure_working_memory, 0, 0x38C8u);

			int32_t scenario_tag_index = -1;
			char scenario_loaded = 0;

			int32_t globals_tag_index = -1;
			char globals_loaded = 0;

			if (cache::cache_files_windows::initialize(scenario_path, g_cache_file_header))
			{
				if (cache::cache_files_windows::validate(g_cache_file_header))
				{
					if (*(bool *)0x189CFD0 && g_cache_file_header->TrackedBuild)
						((char(__cdecl *)(char *))0x42E390)(g_cache_file_header->Build);

					//s_cache_file_header cache_file_header;
					//memmove(&cache_file_header, g_cache_file_header, 0x3390u);

					float progress = 1.f;
					((void(__cdecl *)(int, int))0x52EEC0)(1, 1);

					cache::cache_files_windows::open_tags();
					cache::cache_files_windows::partitions((uint32_t *)GetStructure<uint8_t[8]>(0x22AE4D0));
					cache::cache_files_windows::setup();

					size_t tag_size = 4 * *g_cache_info.max_tag_count_ptr;
					*(uint32_t **)g_cache_info.tag_table_ptr = global_memory_map_allocate_data(5, 0, tag_size, 0);
					memset(*(uint32_t **)g_cache_info.tag_table_ptr, 0, tag_size);

					*(uint32_t *)0x22AB004 = 0;
					*g_cache_info.max_resource_count_ptr = 0x4B00000;
					*(uint32_t **)g_cache_info.resource_table_ptr = global_memory_map_allocate_data(5, 0, *g_cache_info.max_resource_count_ptr, 0);
					g_cache_info.resource_index_table_ptr = 0;

					char root_tag_loaded = cache::cache_files_windows::load_root_tag(0) & 1;
					scenario_loaded = cache::cache_files_windows::load_root_tag(g_cache_file_header->ScenarioTagIndex) & root_tag_loaded;

					if (g_cache_file_header->ExternalDependencies & 2)
						file_close(global_tag_cache_filo);

					((void(__cdecl *)())0x52EEF0)();

					if (scenario_loaded)
					{
						main_loop_pregame();

						if (scenario_loaded)
						{
							cache::cache_files_windows::sub_5031A0();
							scenario_tag_index = g_cache_file_header->ScenarioTagIndex;
							*g_resources_loaded = true;
							goto LABEL_25;
						}
						//auto v5 = (s_map_data_unknown_size_of_38C8 *)secure_working_memory_get_cache_data();
						//if (v5)
						//{
						//	
						//	v5->unknown3390[0] = 0;
						//	memset(v5->SHA1_B, 0xBB, 5 * sizeof(int32_t));
						//	memset(v5->SHA1_C, 0xCC, 5 * sizeof(int32_t));
						//	memset(v5->RSA, 0xDD, 64 * sizeof(int32_t));
						//	memmove(v5, &cache_file_header, 0x3390u);
						//	cache_file_header_dispose(&v5->cache_file);
						//	v5->size = 0x3390;
						//	v5->cache_file_ptr = &v5->cache_file;
						//	signed int v9 = 0x3390;
						//	secure_working_memory = v5->SHA;

						//	int SHA1_B;
						//	if (SHA1::Compute(v5->SHA, 1024, 1))
						//	{
						//		do
						//		{
						//			unsigned int v10 = v9;
						//			if (v9 > 0x100000)
						//				v10 = 0x100000;
						//			SHA1::Calculate(secure_working_memory, 1024, v5, v10);
						//			main_loop_pregame();
						//			v9 -= v10;
						//			v5 = (v5 + v10);
						//		} while (v9 > 0);
						//		SHA1_B = secure_working_memory_size;
						//		SHA1::sub_508E80(secure_working_memory, 1024, secure_working_memory_size);
						//	}
						//	else
						//	{
						//		SHA1_B = secure_working_memory_size;
						//	}

						//	int SHA1_C;
						//	ContentHeader::SHA1::Compute(SHA1_B, 20, 1, &SHA1_C);
						//	qmemcpy(v5->RSA, g_cache_file_header->RSA, 256u);
						//	main_loop_pregame();
						//	auto v12 = (s_map_data_unknown_size_of_38C8 *)secure_working_memory_get_cache_data();
						//	scenario_loaded = sub_508F80(v12->SHA1_C, v12->RSA);
						//	

						//	if (scenario_loaded)
						//	{
						//		v12->unknown3390[0] = 1;
						//		cache::cache_files_windows::sub_5031A0();
						//		scenario_tag_index = g_cache_file_header->ScenarioTagIndex;
						//		*g_resources_loaded = true;
						//		goto LABEL_25;
						//	}
						//}
						else
						{
							scenario_loaded = false;
						}
					}

					scenario_tag_index = -1;
				}

				((void(__cdecl *)())0x50CBB0)();
			}

			cache::cache_files_windows::dispose();
			((char(__cdecl *)())0x5A9730)();

			LABEL_25:

			*g_scenario_tag_index = scenario_tag_index;
			if (scenario_tag_index == -1)
				return scenario_loaded;

			g_scenario_definition = (s_scenario_definition *)tag_get_definition_hook('scnr', scenario_tag_index);


			globals_tag_index = ((uint32_t(__cdecl *)(uint32_t group))0x5017E0)('matg');
			*g_globals_tag_index = globals_tag_index;
			if (globals_tag_index == -1)
				return globals_loaded;
			g_globals_definition = (s_globals_definition *)tag_get_definition_hook('matg', globals_tag_index);

			auto rasterizer_definition = ((tag_reference *)(g_globals_definition->data + 0x518))->GetDefinition();
			*(uint32_t *)0x50DD9BC = *(uint32_t *)(rasterizer_definition + 0x50);
			*(uint32_t *)0x50DD9C0 = *(uint32_t *)(rasterizer_definition + 0x54);

			return true;
		}

		bool initialize(char *scenario_path, s_cache_file_header *cache_file_header)
		{
			auto runtime_resource_index = cache::cache_files_windows::get_tag_runtime_resource_index_of_source_file(scenario_path);

			if (((bool(__cdecl *)(char *))0x54C360)(scenario_path))
			{
				if (*(byte *)0x240B1E0)
				{
					if (runtime_resource_index != -1)
						cache::cache_files_windows::dispose_from_old_map(runtime_resource_index);
				}
			}

			if (((bool(__cdecl *)(char *))0x54C360)(scenario_path))
				((char(__cdecl *)(char *, int a2))0x54C330)(scenario_path, 1);

			if (*(byte *)0x240B1E0)
			{
				if (runtime_resource_index == -1)
				{
					runtime_resource_index = 7;

					if (!cache::cache_files_windows::initialize_for_new_map(runtime_resource_index, scenario_path))
						return 0;
				}
			}
			else if (runtime_resource_index == -1)
			{
				return 0;
			}

			printf_s("cache::cache_files_windows::initialize\n{\n");
			for (size_t i = 0; i < 15; i++)
			{
				auto CacheFileHeader = g_cache->cache_file.tag_runtime_resources[i].Header;
				if (CacheFileHeader.ScenarioTagIndex)
					printf_s("\t[tag_runtime_resources[%d].Header(0x%04X, %s)]\n", i, CacheFileHeader.ScenarioTagIndex, CacheFileHeader.ScenarioPath);
			}
			printf_s("\n");
			for (size_t i = 0; i < 15; i++)
			{
				auto CacheFileHeader = g_cache->cache_file.tag_runtime_resources[i].Header;
				if (CacheFileHeader.SourceFile[0] && !CacheFileHeader.ScenarioTagIndex)
					printf_s("\t[tag_runtime_resources[%d].Header(%s)]\n", i, CacheFileHeader.SourceFile);
			}
			printf_s("};\n");

			g_cache->cache_file.runtime_resource_index = runtime_resource_index;
			memmove(cache_file_header, &g_cache->cache_file.tag_runtime_resources[runtime_resource_index].Header, sizeof(s_cache_file_header));
			return 1;
		}
	}

	bool load(int campaign_id, int map_id, char *scenario_path) // dirty_disk_error(TODO: reimplement this function) if returned value is false
	{
		if (cache::cache_files_windows::load_tags(scenario_path)) // TODO: fully reimplement this as a hook, not call hook
		{
			auto scenario = tag_get_definition_hook('scnr', *(uint32_t *)0x189CCF8);
			if ((map_id == -2) || ((uint32_t)(scenario + 4) == campaign_id || campaign_id == -1) && ((uint32_t)(scenario + 8) == map_id || map_id == -1))
			{
				((unsigned int(__cdecl *)())0x4EB6D0)(); // scenario_tags_fixup();
				((bool(__cdecl *)())0x600750)(); // game_startup(), TODO: fixup engine functions that use hf2p functions
				printf_s("map_load: scenario_tags_fixup called\n");

				return true;
			}
			else
			{
				((void *(__cdecl *)())0x503200)(); // cache::cache_files_windows::release();
				printf_s("map_load: cache_file_dispose called\n");

				*(uint32_t *)0x189CCF8 = -1;
				**(uint32_t * *)0x22AAEB4 = 0;
				**(uint32_t * *)0x22AAEB8 = 0;
				*(uint32_t *)0x189CD0C = -1;
				*(uint32_t *)0x22AAEBC = 0;

				return false;
			}
		}

		printf_s("cache::load: map_load_tags failed\n");
		return false;
	}
}