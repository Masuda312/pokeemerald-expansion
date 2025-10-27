#include "infection_table.h"
#include "global.h"
#include "mutation.h"
#include "pokemon.h"
#include "string_util.h"
#include "constants/species.h"

bool8 ShouldDoInfectionMutate()
{
    for (int i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        u32 status = GetMonData(mon, MON_DATA_STATUS);

        if (status & STATUS1_INFECTED)
        return TRUE;
    }
    return FALSE;
}

static void ChangeMonSpeciesKeepEverything(struct Pokemon *mon, u16 newSpecies)
{
    u16 moves [4];
    u8 ppBonuses;
    u8 pp[4];
    u8 level = GetMonData(mon, MON_DATA_LEVEL);
    u32 exp = GetMonData(mon, MON_DATA_EXP);

    for (int i = 0; i < 4; i++)
    {
        moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i);
        pp[i] = GetMonData(mon, MON_DATA_PP1 + i);
    }
    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES);


    SetMonData(mon, MON_DATA_SPECIES, &newSpecies);
    CalculateMonStats(mon);

    for (int i = 0; i < 4; i++)
    {
        SetMonData(mon, MON_DATA_MOVE1 + i, &moves[i]);
        SetMonData(mon, MON_DATA_PP1 + i, &pp[i]);
    }
    SetMonData(mon, MON_DATA_PP_BONUSES, &ppBonuses);
    SetMonData(mon, MON_DATA_LEVEL, &level);
    SetMonData(mon, MON_DATA_EXP, &exp);
}

void ChangeInfectedToDeoxys(void)
{
    for (int i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        u16 species = GetMonData(mon, MON_DATA_SPECIES);
        if (species == SPECIES_NONE || species >= NUM_SPECIES)
            continue;

        u32 status = GetMonData(mon, MON_DATA_STATUS);
        if (!(status & STATUS1_INFECTED))
            continue;

        u16 target = sInfectionMutateMap[species];
        if (target == SPECIES_NONE || target >= NUM_SPECIES)
            continue;

        if (species == target)
        continue;

        StringCopy(gStringVar1, GetSpeciesName(species));
        
        ChangeMonSpeciesKeepEverything(mon, target);
    }
}
