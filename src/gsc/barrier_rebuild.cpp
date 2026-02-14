// barrier_rebuild.cpp
// Native barrier rebuild implementation for T6 GSC Helper
// Allows bots to repair barriers by hooking the game engine

#include <stdafx.hpp>

namespace gsc
{
	namespace barrier
	{
		// Game function signatures (addresses need to be found via reverse engineering)
		// These will be hooked to enable bot barrier repair
		
		// Force repair a specific barrier chunk
		// Bypasses the "use button held" check
		void force_repair_barrier(game::scr_entref_t ent)
		{
			auto* bot = &game::g_entities[ent.entnum];
			auto barrier_entnum = gsc::value::get<int>(0);
			auto chunk_index = gsc::value::get<int>(1);
			
			if(barrier_entnum < 0 || barrier_entnum >= 2048)
			{
				gsc::value::add<int>(0); // Failed
				return;
			}
			
			auto* barrier = &game::g_entities[barrier_entnum];
			
			// TODO: Implement native barrier repair logic
			// This needs to:
			// 1. Check if barrier exists and is damaged
			// 2. Repair the specific chunk directly in memory
			// 3. Award points to bot (typically 10 points per board)
			// 4. Trigger repair sound/animation via game functions
			// 5. Update barrier state in game memory
			// 6. Call proper game repair function if we can find it
			
			// Memory structure (needs verification):
			// barrier->zbarrier->chunk_health[chunk_index]
			
			// For now, return success placeholder
			gsc::value::add<int>(1); // Success
		}
		
		// Repair barrier by directly modifying chunk health
		// This is a memory manipulation approach
		void repair_barrier_chunk_direct(game::scr_entref_t ent)
		{
			auto* bot = &game::g_entities[ent.entnum];
			
			// Get parameters from GSC
			auto barrier_entnum = gsc::value::get<int>(0);
			auto max_health = gsc::value::get<int>(1); // What health should chunks be
			
			if(barrier_entnum < 0 || barrier_entnum >= 2048)
			{
				gsc::value::add<int>(0);
				return;
			}
			
			// TODO: Direct memory write to barrier health
			// This bypasses the use button requirement completely
			// Steps:
			// 1. Find barrier entity in memory
			// 2. Locate zbarrier structure
			// 3. Find chunk_health array
			// 4. Write max_health to all chunks
			// 5. Award points to bot
			// 6. Play repair sound
			
			gsc::value::add<int>(1); // Success
		}
		
		// Check if safe to repair (no zombies too close)
		void is_safe_to_repair(game::scr_entref_t ent)
		{
			auto* bot = &game::g_entities[ent.entnum];
			auto* barrier_pos = gsc::value::get<float*>(0);
			auto safe_distance = gsc::value::get<float>(1);
			
			float safe_dist_sq = safe_distance * safe_distance;
			
			// Check all AI entities for proximity
			for(int i = 0; i < 2048; i++)
			{
				auto* ent_check = &game::g_entities[i];
				
				// Skip invalid entities
				if(!ent_check || ent_check->state.eType != 1) // 1 = ET_ACTOR (AI)
					continue;
				
				// Calculate distance squared
				float dx = ent_check->r.currentOrigin[0] - barrier_pos[0];
				float dy = ent_check->r.currentOrigin[1] - barrier_pos[1];
				float dz = ent_check->r.currentOrigin[2] - barrier_pos[2];
				float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);
				
				if(dist_sq < safe_dist_sq)
				{
					gsc::value::add<int>(0); // Not safe
					return;
				}
			}
			
			gsc::value::add<int>(1); // Safe
		}
		
		// Award points to bot for repair
		void award_repair_points(game::scr_entref_t ent)
		{
			auto* bot = &game::g_entities[ent.entnum];
			auto points = gsc::value::get<int>(0); // Usually 10 per board
			
			// TODO: Call game's point awarding function
			// Function signature: Scr_AddPoints(entity, points, reason)
			// This ensures points are properly tracked and displayed
			
			gsc::value::add<int>(1); // Success
		}
		
		// Initialize barrier rebuild hooks and functions
		void init()
		{
			// Register custom GSC methods
			gsc::method::add("force_repair_barrier", 2, 2, force_repair_barrier);
			gsc::method::add("repair_barrier_chunk_direct", 2, 2, repair_barrier_chunk_direct);
			gsc::method::add("is_safe_to_repair", 2, 2, is_safe_to_repair);
			gsc::method::add("award_repair_points", 1, 1, award_repair_points);
		}
	}
}
