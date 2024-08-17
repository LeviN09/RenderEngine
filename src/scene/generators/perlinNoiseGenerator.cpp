#include <algorithm>

#include "scene/generators/perlinNoiseGenerator.hpp"


PerlinNoise::PerlinNoise(uint64_t seed)
{
    m_p.resize(512);
    for (uint64_t i = 0; i < 256; i++)
    {
        m_p[i] = i;
    }

    std::mt19937 gen(seed);
    std::shuffle(m_p.begin(), m_p.begin() + 256, gen);

    for (uint64_t i = 0; i < 256; i++)
    {
        m_p[256 + i] = m_p[i];
    }
}

double_t PerlinNoise::Noise(double_t x, double_t y)
{
    int64_t X = static_cast<int64_t>(std::floor(x)) & 255;
    int64_t Y = static_cast<int64_t>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double_t u = Fade(x);
    double_t v = Fade(y);

    int64_t A = m_p[X] + Y;
    int64_t B = m_p[X + 1] + Y;

    return Lerp(v, Lerp(u, Grad(m_p[A], x, y),
                           Grad(m_p[B], x - 1, y)),
                   Lerp(u, Grad(m_p[A + 1], x, y - 1),
                           Grad(m_p[B + 1], x - 1, y - 1)));
}

double_t PerlinNoise::OcataveNoise(double_t x, double_t y, int64_t octaves, double_t persistence)
{
    double_t total = 0;
    double_t frequency = 1;
    double_t amplitude = 1;
    double_t maxValue = 0;

    for(uint64_t i = 0; i < octaves; i++)
    {
        total += Noise(x * frequency, y * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total / maxValue;
}

double_t PerlinNoise::Fade(double_t t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double_t PerlinNoise::Lerp(double_t t, double_t a, double_t b)
{
    return a + t * (b - a);
}

double_t PerlinNoise::Grad(int64_t hash, double_t x, double_t y)
{
    int64_t h = hash & 15;
    double_t grad = 1.0 + (h & 7);
    if (h & 8)
    {
        grad = -grad;
    }
    return (h & 1 ? x : y) * grad;
}