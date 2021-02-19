// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_set>
#include <exception>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace::std;


//для молнии 0.965
int const lenX = 35;
float n = 0.97;


double random(double min, double max)
{
    static int first = -1;
    if ((first = (first < 0)))
        srand(time(NULL)); // + getpid());
    if (min >= max)
        return errno = EDOM, NAN;

    return min + (double)rand() / ((double)RAND_MAX / (max - min));
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
bool put(Grid& grid, int x, int y, bool it)
{
    if (!it)
    {
        int r = 6;
    }
    grid.emplace(IntVector2(x, y), it);
    return 0;
}


int counterOfTrues(Grid& boolField, int squareSize, int k, int l)
{
    int numOfTrue = 0;
    for (int i = k; i < squareSize + k; i++)
    {
        for (int j = l; j < squareSize + l; j++)
        {
            if (get(boolField, i, j))
            {
                numOfTrue++;
            }
        }
    }
    return numOfTrue;
}

//сравниваем 
bool random_decision(int k, int l, float Z, float n, float **T)
{
    float r = random(0, 1);
    float psi = r * Z;
    float Z1 = pow((T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1]), n);

    return Z1 > psi;
}

//генератор поля
void generate_field(float n, Grid& varLig, int numIter)
{
    //создаю массив, в конце выполнения функции он удалится
    auto T = new float*[lenX];

    //для молнии все значения =5
    int Ttop = 5;  // 100
    int Tbottom = 5; // 0
    int Tleft = 5;  // 0
    int Tright = 5;  // 30

    //записываю в потенциальное (температурное) поле нули
    for (int i = 0; i < lenX; i++)
    {
        T[i] = new float[lenX];
        for (int j = 0; j < lenX; j++)
        {
            T[i][j] = 0;
        }
    }

    //задаю граничные условия для поля потенциалов
    for (int i = 1; i < lenX-1; i++)
    {
        T[i][lenX - 2] = Ttop;
        T[i][0] = int(Tbottom * random(0, i));
        T[lenX - 2][i] = Tright;
        T[0][i] = Tleft;
    }

    //рассчитываю поле
    for (int i = 2; i < lenX - 2; i++)
    {
        for (int j = 2; j < lenX - 2; j++)
        {
            T[i][j] = (i + 1) / ((float)(2) * (j + 1)) + pow(3, j)/ pow(2, j); //(pow(lenX, 2) + j + 1)
        }
    }

    for (int i = 0; i < numIter; i++)
    {
        for (int i = 2; i < lenX - 2; i++)
        {
            for (int j = 2; j < lenX - 2; j++)
            {
                float Z = (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);

                //__________________НАЧИНАЕТСЯ КОД ДЛЯ ЗАДАНИЯ ПОЛЯ Lig____________________________

                bool r_ip1 = random_decision(i + 1, j, Z, n, T) && (get(varLig, i, j) or get(varLig, i + 1, j - 1) or get(varLig, i + 1, j + 1) or get(varLig, i + 2, j));
                bool r_im1 = random_decision(i - 1, j, Z, n, T) && (get(varLig, i, j) or get(varLig, i - 1, j - 1) or get(varLig, i - 1, j + 1) or get(varLig, i - 2, j));
                bool r_jp1 = random_decision(i, j + 1, Z, n, T) && (get(varLig, i, j) or get(varLig, i - 1, j + 1) or get(varLig, i, j + 2) or get(varLig, i + 1, j + 1));

                if (r_ip1) {
                    put(varLig, i + 1, j, true);
                }
                else {
                    put(varLig, i + 1, j, false);
                }

                // определяю для x - 1
                if (r_im1) {
                    put(varLig, i - 1, j, true);
                }
                else {
                    put(varLig, i - 1, j, false);
                }

                // определяю для y + 1
                if (r_jp1) {
                    put(varLig, i, j + 1, true);
                }
                else {
                    put(varLig, i, j + 1, false);
                }
            }
        }
    }


    //удаляю массив - очищаю память //пока не работает
    /*for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;*/
}

//фильтрация лабиринта для более квадратной формы
void filter_field(Grid& varLig, int yPoint, int squareSize)
{
    Grid timingLig;

    //сжимаем
    for (int i = yPoint; i < lenX; i+= squareSize)
    {
        for (int j = yPoint; j < lenX; j+= squareSize)
        {
            if (counterOfTrues(varLig, squareSize, i, j) > 2)
            {
                put(timingLig, i, j, true);
            }
            else 
            {
                put(timingLig, i, j, false);
            }
        }
    }


    //возвращаем нужный размер
}


int main()
{
    Grid Lig;

    int numIter = int(0.2 * lenX);

    //начальные условия для молнии-пещеры
    int startyPoint = int(lenX / 2);
    put(Lig, 3, startyPoint, true);
    
    //данные для постобработки путём сжатия //сейчас написано только для размера 2
    int squareSize = 2;

    //начальные условия для лабиринта
    //put(Lig, 1, 3, true);
    //put(Lig, 3, 2, true);

    generate_field(n, Lig, numIter);

    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j))
                cout << "  ";
            else
                cout << "||";
        }
    }
}