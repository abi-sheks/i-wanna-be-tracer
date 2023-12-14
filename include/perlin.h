#pragma once
#include "utilities.h"
#include "vec3.h"

class Perlin
{
public:
    Perlin()
    {
        rand_vec = new vec3[point_count];
        for (int i = 0; i < point_count; i++)
        {
            rand_vec[i] = normalize(vec3::random(-1, 1));
        }
        perm_x = generatePerm();
        perm_y = generatePerm();
        perm_z = generatePerm();
    }
    ~Perlin()
    {
        delete[] rand_vec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }
    double turbulence(const Point3 &point, int depth = 7) const
    {
        double result = 0.0;
        auto temp_point = point;
        auto weight = 1.0;
        for (int i = 0; i < depth; i++)
        {
            result += weight*noise(temp_point);
            weight *= 0.5;
            temp_point *= 2;
        }
        //eliminates possibility of negative, so no need to handle NaN in texture.
        //Otherwise HANDLE
        return fabs(result);
    }
    double noise(const Point3 &point) const
    {
        // for interpolation
        auto u = point.x() - floor(point.x());
        auto v = point.y() - floor(point.y());
        auto w = point.z() - floor(point.z());
        // rounding off interpolation with Hermite cubic for smoothness
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);

        auto i = static_cast<int>(floor(point.x()));
        auto j = static_cast<int>(floor(point.y()));
        auto k = static_cast<int>(floor(point.z()));
        vec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = rand_vec[perm_x[(i + di) & 255] ^
                                             perm_y[(j + dj) & 255] ^
                                             perm_z[(k + dk) & 255]];
                }
            }
        }
        return perlinLerp(c, u, v, w);
    }

private:
    static const int point_count = 256;
    vec3 *rand_vec;
    int *perm_x;
    int *perm_y;
    int *perm_z;

    static int *generatePerm()
    {
        auto p = new int[point_count];
        for (int i = 0; i < Perlin::point_count; i++)
        {
            p[i] = i;
        }
        permute(p, point_count);
        return p;
    }
    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = randomInt(0, i);
            swapInt(&p[target], &p[i]);
        }
    }
    static double perlinLerp(vec3 c[2][2][2], double u, double v, double w)
    {
        double result = 0.0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    vec3 weight = vec3(u - i, v - j, w - k);
                    result += c[i][j][k].dot(weight) * (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) * (w * k + (1 - w) * (1 - k));
                }
            }
        }
        return result;
    }
};