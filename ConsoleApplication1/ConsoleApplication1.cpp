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
int const lenX = 30;
float n = 0.976;


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
    //grid.insert_or_assign(IntVector2(x, y), it);
    grid.emplace(IntVector2(x, y), it);
    return 0;
}

bool isOrAss(Grid& grid, int x, int y, bool it)
{
    grid.insert_or_assign(IntVector2(x, y), it);
    //grid.emplace(IntVector2(x, y), it);
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
void generate_field(float n, Grid& varLig, int numIter, int squareSize)
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

    /*for (int m = 2; m < lenX - squareSize - 2; m+= squareSize)
    {
        for (int i = m; i < squareSize + m; i++)
        {
            for (int j = m; j < squareSize + m; j++)
            {
                if (counterOfTrues(varLig, squareSize, i, j) > 2)
                {
                    put(varLig, i, j, true);
                }
                else
                {
                    put(varLig, i, j, false);
                }
            }
        }
    }*/


    /*for (int i = 2; i < lenX - squareSize - 2; i+= squareSize)
    {
        for (int j = 2; j < lenX - squareSize - 2; j+= squareSize)
        {
            if (counterOfTrues(varLig, squareSize, i, j) >= 2) {
                for (int k = i; k < i + squareSize; k++)
                {
                    for (int l = j; l < j + squareSize; l++)
                    {
                        put(varLig, k, l, true);                    }
                }
            }

            else {
                for (int k = i; k < i + squareSize; k++)
                {
                    for (int l = j; l < j + squareSize; l++)
                    {
                        put(varLig, k, l, false);
                    }
                }
            }
        }
    }*/

    //удаляю массив - очищаю память //пока не работает
    /*for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;*/
}

//фильтрация лабиринта для более квадратной формы
void filter_field(Grid& varLig, int squareSize, int numIter)
{
    for (int i = 1; i < lenX - 1; i += squareSize)
    {
        for (int j = 1; j < lenX - 1; j += squareSize)
        {
            if (counterOfTrues(varLig, squareSize, i, j) >= 2) {
                for (int k = i; k < i + squareSize; k++)
                {
                    for (int l = j; l < j + squareSize; l++)
                    {
                        isOrAss(varLig, k, l, true);
                    }
                }
            }

            else {
                for (int k = i; k < i + squareSize; k++)
                {
                    for (int l = j; l < j + squareSize; l++)
                    {
                        isOrAss(varLig, k, l, false);
                    }
                }
            }
        }
    }
    
}



int main()
{
    Grid Lig;

    int numIter = int(0.2 * lenX);

    //начальные условия для молнии-пещеры
    put(Lig, 3, int(lenX/2), true);

    //данные для постобработки путём сжатия
    int squareSize = 2;

    generate_field(n, Lig, numIter, squareSize);

    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j))
                cout << "  ";
            else
                cout << "||";
        }
    }

    filter_field(Lig, squareSize, numIter);

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