#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <cmath>

class PerlinNoise
{
    public:
        PerlinNoise(uint64_t seed = std::random_device{}());

        double_t Noise(double_t x, double_t y);

        double_t OcataveNoise(double_t x, double_t y, int64_t octaves, double_t persistence);

    private:
        std::vector<int64_t> m_p;

        double_t Fade(double_t t);
        double_t Lerp(double_t t, double_t a, double_t b);
        double_t Grad(int64_t hash, double_t x, double_t y);
};