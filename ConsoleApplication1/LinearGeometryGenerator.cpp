#include <math.h>
#include <time.h>
#include <iostream>
#include <exception>
#include <unordered_set>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
//#include "LinearGeometryGenerator.h"

using namespace std;


float random(float min, float max)
{
    static int first = -1;
    if ((first = (first < 0)))
        srand(time(NULL)); // + getpid());
    if (min >= max)
        return errno = EDOM, NAN;

    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

bool random_decision(int k, int l, float Z, float n, float** T)
{
    float r = random(0, 1);
    float psi = r * Z;
    float Z1 = pow((T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1]), n);

    return Z1 > psi;
}

struct IntVector2
{
    //bool X;
    //bool Y;
    int X;
    int Y;

    IntVector2(int x, int y)
        :X(x), Y(y)
    {
    }

    IntVector2(const IntVector2& other)
        :X(other.X), Y(other.Y)
    {
    }

    friend bool operator==(const IntVector2& a, const IntVector2& b) {
        return a.X == b.X && a.Y == b.Y;
    }
};

template<> struct std::hash<IntVector2> {
    std::size_t operator()(IntVector2 const& s) const noexcept
    {
        return s.X ^ s.Y;
    }
};

typedef std::unordered_map<IntVector2, int> Grid;

//берём значение по координатам
bool get(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    //возвращаем то, что положили
    return it->second;
}

//кладём значение по кординатам
bool set(Grid& grid, int x, int y, bool it)
{
    grid.insert_or_assign(IntVector2(x, y), it);
    return 0;
}


struct Params
{
public:
    int width = 30;
    int height = 10;

    float powerFill = 0.9;
    int numIter = 1;
    int timeIterLight = int(width + height) * 3;

    int choiseLocation = 0;

    int degP = 1.5;
    int degP2 = 1.2;

    float Spreading = 0.2;
};



void GenerateLinearMap(Grid& lightMap, int location)
{
    Params param;

    if (location == 0)
    {
        float pstart = param.powerFill;
        
        //создаю массив, в конце выполнения функции он удалится
        auto P = new float* [param.width];
        //записываю в потенциальное (температурное) поле нули
        for (int i = 0; i < param.width; i++)
        {
            P[i] = new float[param.height];
            for (int j = 0; j < param.height; j++)
            {
                P[i][j] = 0;
            }
        }


        for (int i = 1; i < param.width; i++)
        {
            P[i] = new float[param.height];
            for (int j = 1; j < param.height; j++)
            {
                if (i == int(param.width / 2))
                {
                    P[i][j] = pstart;
                }
                else if (i > int(param.width / 2))
                {
                    //P[i][j] = (pow(pstart, param.degP) + pow(P[i - 1][j - 1], param.degP)) / 2 - 0.1;
                    //P[i][j] = (pow((P[i - 1][j]), param.degP2) + pow((P[i][j - 1]), param.degP2)) / 2 - 0.25;
                    P[i][j - 1] = (pow(pstart, param.degP) + pow(P[i - 1][j - 1], param.degP)) / 2 - 0.1;
                    P[i][j] = (pow((P[i - 1][j]), param.degP2) + pow((P[i][j - 1]), param.degP2)) / 2 - 0.25;

                }
                else
                {
                    P[i][j] = 0;
                }
                cout << P[i][j];
                cout << ' ';
            }
            cout << '\n';
        }

        for (int i = param.width / 2 - 1; i >= 0; i--)
        {
            for (int j = 1; j < param.height; j += 1)
            {
                P[i][j - 1] = (pow(pstart, param.degP) + pow(P[i + 1][j - 1], param.degP)) / 2 - 0.1;
                P[i][j] = (pow((P[i + 1][j]), param.degP2) + pow((P[i][j - 1]), param.degP2)) / 2 - 0.25;
            }
        }


        //Fill the linear lighting map
        set(lightMap, param.width / 2, 0, true);
        set(lightMap, param.width / 2, 1, true);
        set(lightMap, param.width / 2, 2, true);


        for (int j = 2; j < param.height - 2; j++)
        {
            for (int i = param.width / 2; i >= 2; i--)
            {
                bool thereIsNeib = get(lightMap, i + 1, j) or get(lightMap, i - 1, j) or get(lightMap, i, j - 1);

                if (thereIsNeib)
                {
                    float r = random(0.25, 1);

                    if ((P[i][j] > r) || (get(lightMap, i, j)))
                    {
                        set(lightMap, i, j, true);
                    }
                    else
                    {
                        set(lightMap, i, j, false);
                    }
                }
            }
        }

        for (int j = 2; j < param.height - 2; j++)
        {
            for (int i = param.width / 2; i < param.width - 2; i++)
            {
                bool thereIsNeib = get(lightMap, i + 1, j) || get(lightMap, i - 1, j) || get(lightMap, i, j - 1);

                if (thereIsNeib)
                {
                    float r = random(0.1, 1);

                    if ((P[i][j] > r) || (get(lightMap, i, j)))
                    {
                        set(lightMap, i, j, true);
                    }
                    else
                    {
                        set(lightMap, i, j, false);
                    }
                }
            }
        }
    }

        //P[i][j] = (pow(pstart, param.degP) + pow(P[i - 1][j - 1], param.degP)) / 2 - 0.1;
}


int main()
{
    Grid Lig;

    Params params;
    //начальные условия для молнии-пещеры
    set(Lig, 3, 3, true);
    set(Lig, params.width - 3, 3, true);
    //данные для постобработки путём сжатия
    int squareSize = 2;

    GenerateLinearMap(Lig, 0);
    set(Lig, 3, 3, true);
    set(Lig, params.width - 3, 3, true);

    for (int i = 0; i < params.width; i++) {
        cout << " \n";
        for (int j = 0; j < params.height; j++) {
            if (get(Lig, i, j))
                cout << "  ";
            else
                cout << "||";
        }
    }
}