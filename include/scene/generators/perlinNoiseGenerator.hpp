#pragma once

#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

class PerlinNoise
{
    public:
        PerlinNoise(unsigned int seed = std::random_device{}())
        {
            m_p.resize(512);
            for (int i = 0; i < 256; i++)
            {
                m_p[i] = i;
            }

            std::mt19937 gen(seed);
            std::shuffle(m_p.begin(), m_p.begin() + 256, gen);

            for (int i = 0; i < 256; i++)
            {
                m_p[256 + i] = m_p[i];
            }
        }

        double noise(double x, double y)
        {
            int X = static_cast<int>(std::floor(x)) & 255;
            int Y = static_cast<int>(std::floor(y)) & 255;

            x -= std::floor(x);
            y -= std::floor(y);

            double u = fade(x);
            double v = fade(y);

            int A = m_p[X] + Y;
            int B = m_p[X + 1] + Y;

            return lerp(v, lerp(u, grad(m_p[A], x, y),
                                   grad(m_p[B], x - 1, y)),
                           lerp(u, grad(m_p[A + 1], x, y - 1),
                                   grad(m_p[B + 1], x - 1, y - 1)));
        }

        double ocataveNoise(double x, double y, int octaves, double persistence)
        {
            double total = 0;
            double frequency = 1;
            double amplitude = 1;
            double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
            for(int i = 0; i < octaves; i++)
            {
                total += noise(x * frequency, y * frequency) * amplitude;

                maxValue += amplitude;

                amplitude *= persistence;
                frequency *= 2;
            }

            return total / maxValue;
        }

    private:
        std::vector<int> m_p;

        double fade(double t) {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        double lerp(double t, double a, double b) {
            return a + t * (b - a);
        }

        double grad(int hash, double x, double y) {
            int h = hash & 15;
            double grad = 1.0 + (h & 7);
            if (h & 8) grad = -grad;
            return (h & 1 ? x : y) * grad;
        }

};