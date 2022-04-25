// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_set>
#include <exception>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#define PI 	                (3.1415926535897932f)


using namespace::std;

struct Params 
{
    int Width = 30;
    int Heidht = 30;

    float powerFill = 0.3;
    int numIter = 1;
    
    //defines time of cluster raising
    int timeIterLight = 45;

    int choiseLocation = 1;

    int degP = 1.5;
    int degP2 = 3.2;
};



//для молнии 0.976
int const lenX = 30;
//float n = 0.967;
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
bool IsFilled(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    //возвращаем то, что положили
    return it->second;
}

//кладём значение по кординатам

{
    grid.insert_or_assign(IntVector2(x, y), it);
    return 0;
}



int counterOfTrues(Grid& boolField, int squareSize, int k, int l)
{
    int numOfTrue = 0;
    for (int i = k; i < squareSize + k; i++)
    {
        for (int j = l; j < squareSize + l; j++)
        {
            if (IsFilled(boolField, i, j))
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


/*/
//генератор поля
void generate_field(float n, Grid& varLig, int numIter, int squareSize)
{
    //создаю массив, в конце выполнения функции он удалится
    auto T = new float*[lenX];

    //для молнии все значения =5
    int Ttop = 5;  // 100
    int Tbottom = 5; // 0
    int Tleft = 15;  // 0
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
        //cout << "\n";
        for (int j = 2; j < lenX - 2; j++)
        {
            T[i][j] = (i + 1) / ((float)(2) * (j + 1)) + pow(3, j)/ pow(2, j); //(pow(lenX, 2) + j + 1)
            //cout << T[i][j];
            //cout << " ";
        }
    }

    for (int i = 2; i < lenX - 2; i++)
    {
        for (int j = 2; j < lenX - 2; j++)
        {
            float Z = (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);


            //__________________НАЧИНАЕТСЯ КОД ДЛЯ ЗАДАНИЯ ПОЛЯ Lig____________________________

            bool r_ip1 = random_decision(i + 1, j, Z, n, T) && (IsFilled(varLig, i, j) or IsFilled(varLig, i + 1, j - 1) or get(varLig, i + 1, j + 1) or get(varLig, i + 2, j));
            bool r_im1 = random_decision(i - 1, j, Z, n, T) && (IsFilled(varLig, i, j) or IsFilled(varLig, i - 1, j - 1) or get(varLig, i - 1, j + 1) or get(varLig, i - 2, j));
            bool r_jp1 = random_decision(i, j + 1, Z, n, T) && (IsFilled(varLig, i, j) or IsFilled(varLig, i - 1, j + 1) or get(varLig, i, j + 2) or get(varLig, i + 1, j + 1));


            bool thereIsNeib = random_decision(i, j, Z, n, T) && (get(varLig, i - 1, j) or get(varLig, i + 1, j) or get(varLig, i, j + 1) or get(varLig, i, j - 1));

            if (thereIsNeib) {
                set(varLig, i, j, true);
                T[i][j] = 100;
            }
            else {
                set(varLig, i, j, false);
            }
        }
    }

    //удаляю массив - очищаю память //пока не работает
    for (size_t i = 0; i < lenX; i++)
    {
        delete T[i];
    }
    delete T;
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
                        set(varLig, k, l, true);
                    }
                }
            }

            else {
                for (int k = i; k < i + squareSize; k++)
                {
                    for (int l = j; l < j + squareSize; l++)
                    {
                        set(varLig, k, l, false);
                    }
                }
            }
        }
    }
    
}

*/

/*
int main()
{
    Grid FIntVector;


    int numIter =3;

    //начальные условия для молнии-пещеры
    set(Lig, 3, 3, true);
    set(Lig, lenX - 3, 3, true);
    //данные для постобработки путём сжатия
    int squareSize = 2;

    generate_field(n, Lig, numIter, squareSize);
    set(Lig, 3, 3, true);
    set(Lig, lenX - 3, 3, true);

    /*
    int numIter = 2;

    //начальные условия для молнии-пещеры
    put(FIntVector, 3, 3, true);
    put(FIntVector, lenX - 3, 3, true);
    //данные для постобработки путём сжатия
    int squareSize = 2;

    generate_field(n, FIntVector, numIter, squareSize);
    put(FIntVector, 3, 3, true);
    put(FIntVector, lenX - 3, 3, true);
    */

    /*
    filter_field(FIntVector, squareSize, numIter);

    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j))
                cout << "  ";
            else
                cout << "||";
        }
    }*/

    Params mParams;
    int choiseLocation = mParams.choiseLocation;


    if (choiseLocation == 0) {

        float Pstart = 0.999; //mParams.powerFill

        vector<vector<int>> P;
        for (int i = 0; i < mParams.Width; i++)
        {
            vector<int> p;
            for (int j = 0; j < mParams.Heidht; j++)
            {
                p.push_back(0);
            }
            P.push_back(p);
        }

        Set(FIntVector, mParams.Width / 2, 0, true);
        Set(FIntVector, mParams.Width / 2, 1, true);
        Set(FIntVector, mParams.Width / 2, 2, true);

        for (int i = 1; i < mParams.Width; i++)
        {
            for (int j = 1; j < mParams.Heidht; j += 1)
            {
                if (i == mParams.Width / 2)
                {
                    P[i][j] = Pstart;
                }
                else if (i > mParams.Width / 2)
                {
                    P[i][j - 1] = (pow(Pstart, mParams.degP) + pow(P[i - 1][j - 1], mParams.degP)) / 2 - 0.1;
                    P[i][j] = (pow((P[i - 1][j]), mParams.degP2) + pow((P[i][j - 1]), mParams.degP2)) / 2 - 0.25;
                }
            }
        }

        for (int i = mParams.Width / 2 - 1; i >= 0; i--)
        {
            for (int j = 1; j < mParams.Heidht; j += 1)
            {
                P[i][j - 1] = (pow(Pstart, mParams.degP) + pow(P[i + 1][j - 1], mParams.degP)) / 2 - 0.1;
                P[i][j] = (pow((P[i + 1][j]), mParams.degP2) + pow((P[i][j - 1]), mParams.degP2)) / 2 - 0.25;
            }
        }


        for (int j = 2; j < mParams.Heidht - 2; j++)
        {
            for (int i = mParams.Width / 2; i >= 2; i--)
            {
                bool thereIsNeib = IsFilled(FIntVector, i + 1, j) || IsFilled(FIntVector, i - 1, j) || IsFilled(FIntVector, i, j - 1);

                if (thereIsNeib)
                {
                    float r = random(0.25, 1);

                    if ((P[i][j] > r) || (IsFilled(FIntVector,i, j)))
                    {
                        Set(FIntVector,i, j, true);
                        if (!IsFilled(FIntVector, i, j - 1) && random(0, 1) > 0.5 && j < int(mParams.Heidht / 3)) {
                        }
                    }
                    else
                    {
                        Set(FIntVector, i, j, false);
                    }
                }
            }
        }

        for (int j = 2; j < mParams.Heidht - 2; j++)
        {
            for (int i = mParams.Width / 2; i < mParams.Width - 2; i++)
            {
                bool thereIsNeib = IsFilled(FIntVector,i + 1, j) || IsFilled(FIntVector, i - 1, j) || IsFilled(FIntVector, i, j - 1);

                if (thereIsNeib)
                {
                    float r = random(0.25, 1);

                    if ((P[i][j] > r) || (IsFilled(FIntVector, i, j)))
                    {
                        Set(FIntVector,i, j, true);
                    }
                    else
                    {
                        Set(FIntVector,i, j, false);
                    }
                }
            }
        }
    }


    else if (mParams.choiseLocation == 1)
    {
        float Pstart = mParams.powerFill; //0.8

        //vector<FFloatArrayContainer> P;
        vector<vector<int>> P;

        for (int i = 0; i < mParams.Width; i++)
        {
            vector<int> p;
            //P.Add(FFloatArrayContainer());
            for (int j = 0; j < mParams.Heidht; j++)
            {
                p.push_back(Pstart);
            }
            P.push_back(p);
        }

        Set(FIntVector, mParams.Width / 2, 0, true);
        Set(FIntVector,mParams.Width / 2, 1, true);
        Set(FIntVector,mParams.Width / 2, 2, true);
        

        for (int i = 1; i < mParams.Width; i++)
        {
            vector<int> p;
            for (int j = 1; j < mParams.Heidht; j += 1)
            {
                p.push_back(Pstart);
            }
            P.push_back(p);
        }

        P[int(mParams.Width / 2)][0] = 1;
        P[int(mParams.Width / 2)][1] = 1;
        P[int(mParams.Width / 2)][2] = 1;

        for (int o = 0; o < mParams.timeIterLight; o++)
        {
            //пока хотя бы один из них не заполнен
            //do {} while (IsFilled(FIntVector(1:mParams.Width-1, mParams.Heidht - 2, 0)))



            for (int j = 2; j < mParams.Heidht - 2; j++)
            {
                for (int i = mParams.Width / 2; i >= 2; i--)
                {
                    bool thereIsNeib = IsFilled(FIntVector, i + 1, j) || IsFilled(FIntVector, i - 1, j) || IsFilled(FIntVector, i, j - 1);

                    float Z = 0.25 * (P[i + 1][j] + P[i][j + 1] + P[i - 1][j] + P[i][j - 1]);


                    float xmax = 0.937;


                    float V = 40.0;
                    float d = 28.0;
                    float g = 0.5;
                    float ksi = random(0, 1);
                    float alpha = 2 * PI * ksi;
                    float E0 = alpha * V / d;
                    float E = g * log(exp(E0 / g) - ksi * exp(E0 / g) - 1);

                    float Emax = g * log(exp(2 * PI * xmax * V / (d * g)) - xmax * (exp(2 * PI * xmax * V / (d * g)) - 1));

                    if (thereIsNeib)
                    {
                        if ((E / Emax >= Z) || (IsFilled(FIntVector, i, j)))
                        {
                            Set(FIntVector, i, j, true);
                            P[i][j] = E;
                        }
                        else
                        {
                            Set(FIntVector, i, j, false);
                        }
                    }
                }
            }

            for (int j = 2; j < mParams.Heidht - 2; j++)
            {
                for (int i = mParams.Width / 2; i < mParams.Width - 2; i++)
                {
                    bool thereIsNeib = IsFilled(FIntVector, i + 1, j) || IsFilled(FIntVector, i - 1, j) || IsFilled(FIntVector, i, j - 1);

                    float Z = 0.25 * (P[i + 1][j] + P[i][j + 1] + P[i - 1][j] + P[i][j - 1]);


                    float xmax = 0.937;

                    float V = 30.0; //40
                    float d = 15.0; //28
                    float g = 0.5;
                    float ksi = random(0, 1);
                    float alpha = 2 * PI * ksi;
                    float E0 = alpha * V / d;
                    float E = g * log(exp(E0 / g) - ksi * exp(E0 / g) - 1);

                    float Emax = g * log(exp(2 * PI * xmax * V / (d * g)) - xmax * (exp(2 * PI * xmax * V / (d * g)) - 1));

                    if (thereIsNeib)
                    {
                        if ((E / Emax >= Z) || (IsFilled(FIntVector, i, j)))
                        {
                            Set(FIntVector, i, j, true);
                            P[i][j] = E;
                        }
                        else
                        {
                            Set(FIntVector, i, j, false);
                        }
                    }
                }
            }
        }


        bool wasChanging = true;
        while (wasChanging)
        {
            wasChanging = false;
            int step = int(pow((mParams.Width + mParams.Heidht), 0.15));
            for (int i = 1; i < mParams.Width - 3; i += 3)
            {
                for (int j = 1; j < mParams.Heidht - 3; j += 3)
                {

                    bool otherValue1 = IsFilled(FIntVector, i, j) && !IsFilled(FIntVector, i - 1, j) && !IsFilled(FIntVector, i + 1, j) && !IsFilled(FIntVector, i, j - 1) && !IsFilled(FIntVector, i - 1, j - 1);
                    bool otherValue2 = !IsFilled(FIntVector, i, j) && IsFilled(FIntVector, i - 1, j) && IsFilled(FIntVector, i + 1, j) && IsFilled(FIntVector, i, j - 1) && IsFilled(FIntVector, i - 1, j - 1);

                    if (otherValue1)
                    {
                        Set(FIntVector, i, j, false);
                        wasChanging = true;
                    }
                    if (otherValue2)
                    {
                        Set(FIntVector, i, j, true);
                        wasChanging = true;
                    }
                }
            }
        }
    }