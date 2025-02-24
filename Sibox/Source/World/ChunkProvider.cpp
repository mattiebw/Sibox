#include "papipch.h"
#include "World/ChunkProvider.h"
#include "World/TileSets.h"

DefaultChunkProvider::DefaultChunkProvider()
{
    srand(time(0));

    int Cellular_Seed = rand();
    int Perlin_Seed = rand();

    PNoise_1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    PNoise_1.SetSeed(Perlin_Seed);
    PNoise_1.SetFrequency(0.08f);
    PNoise_1.SetFractalType(FastNoiseLite::FractalType_FBm);
    PNoise_1.SetFractalOctaves(5);
    PNoise_1.SetFractalLacunarity(0.15f);
    PNoise_1.SetFractalGain(0.0f);

    PNoise_2.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    PNoise_2.SetSeed(Perlin_Seed);
    PNoise_2.SetFrequency(0.08f);
    PNoise_2.SetFractalType(FastNoiseLite::FractalType_PingPong);
    PNoise_2.SetFractalOctaves(7);
    PNoise_2.SetFractalLacunarity(0.0f);
    PNoise_2.SetFractalGain(0.0f);
    PNoise_2.SetFractalWeightedStrength(1.8f);
    PNoise_2.SetFractalPingPongStrength(2.1f);

    CNoise_1.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    CNoise_1.SetSeed(Cellular_Seed);
    CNoise_1.SetFrequency(0.15f);
    CNoise_1.SetFractalType(FastNoiseLite::FractalType_FBm);
    CNoise_1.SetFractalOctaves(1);
    CNoise_1.SetFractalLacunarity(2.0f);
    CNoise_1.SetFractalGain(0.0f);
    CNoise_1.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Hybrid);
    CNoise_1.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
    CNoise_1.SetCellularJitter(1.0f);

    CNoise_2.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    CNoise_2.SetSeed(Cellular_Seed);
    CNoise_2.SetFrequency(0.15f);
    CNoise_2.SetFractalType(FastNoiseLite::FractalType_FBm);
    CNoise_2.SetFractalOctaves(4);
    CNoise_2.SetFractalLacunarity(0.75f);
    CNoise_2.SetFractalGain(0.2f);
    CNoise_2.SetFractalWeightedStrength(0.2f);
    CNoise_2.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Euclidean);
    CNoise_2.SetCellularReturnType(FastNoiseLite::CellularReturnType_Distance);
    CNoise_2.SetCellularJitter(1.0f);
}

uint16_t DefaultChunkProvider::GetTileAt(int x, int y) const
{
    float Pnoise_1 = PNoise_1.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float Pnoise_2 = PNoise_2.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float Cnoise_1 = CNoise_1.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float Cnoise_2 = CNoise_2.GetNoise(static_cast<float>(x), static_cast<float>(y));
    // uses perlin noise to make snow on mountains
    if (Pnoise_1 > 0.5f || Pnoise_1 < -0.5f)
        return TileSets::Snow;
    // uses perlin noise to make solid stone
    else if (Pnoise_1 < -0.4f && Pnoise_1 >= -0.5f || Pnoise_1 > 0.4f && Pnoise_1 < 0.5f)
        return TileSets::MoutainStone;
    // uses two layers of Cellular noise over the Perlin noise to make ponds
    else if (Cnoise_1 >= 0.85f)
    {
        if (Cnoise_2 >= -0.55f && Cnoise_2 < -0.4f)
        {
            int LWater = rand() % 6;
            switch (LWater)
            {
            case 0:
                return TileSets::LWater1;
                break;
            case 1:
                return TileSets::LWater2;
                break;
            case 2:
                return TileSets::LWater3;
                break;
            case 3:
                return TileSets::LWater4;
                break;
            case 4:
                return TileSets::LWater5;
                break;
            case 5:
                return TileSets::LWater6;
                break;
            }
        }
        else if (Cnoise_2 >= -0.4f)
        {
            return TileSets::Sand;
        }
        else
        {
            int DWater = rand() % 6;
            switch (DWater)
            {
            case 0:
                return TileSets::DWater1;
                break;
            case 1:
                return TileSets::DWater2;
                break;
            case 2:
                return TileSets::DWater3;
                break;
            case 3:
                return TileSets::DWater4;
                break;
            case 4:
                return TileSets::DWater5;
                break;
            case 5:
                return TileSets::DWater6;
                break;
            }
        }
    }
    // continue using perlin noise to make stone floor 
    else if (Pnoise_1 <= -0.3f && Pnoise_1 >= -0.4f || Pnoise_1 >= 0.3f && Pnoise_1 <= 0.4f)
        return TileSets::StoneWall;
    // Paths
    else if (Pnoise_2 <= -0.79f)
    {
        int Mud = rand() % 4;
        switch (Mud)
        {
        case 0:
            return TileSets::Mud1;
            break;
        case 1:
            return TileSets::Mud2;
            break;
        case 2:
            return TileSets::Mud3;
            break;
        case 3:
            return TileSets::Mud4;
            break;
        }
    }
    // else = all remaining tiles are 50% grass and 12.5% for each flower
    else
    {
        int Coin = rand() % 2;

        switch (Coin)
        {
        case 0:
            return TileSets::Grass;
            break;
        case 1:
            int Random_Flowers = rand() % 4;
            switch (Random_Flowers)
            {
            case 0:
                return TileSets::Flowers;
                break;
            case 1:
                return TileSets::Sunflowers;
                break;
            case 2:
                return TileSets::Lilies;
                break;
            case 3:
                return TileSets::Roses;
                break;
            }
            break;
        }
    }
}
// lol hope this works