// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_set>
#include <exception>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // для функции time()

using namespace::std;

//const unsigned int lenX = 3;
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

//сравниваем 
bool random_decision(int k, int l, float Z, float n, float **T)
{
    float r = random(0, 1);
    float psi = r * Z;
    float Z1 = pow((T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1]), n);
    //float Z1 = pow(z1, n); 
    //float Z1 = pow((get(T, k - 1, l) + get(T, k + 1, l) + get(T, k, l - 1) + get(T, k, l + 1)), n);

    return Z1 > psi;
}


//генератор поля
void generate_field(float n, Grid& varLig, int numIter)
{
    //создаю массив, в конце выполнения функции он удалится
    auto T = new float*[lenX];

    int Ttop = 45;  // 100
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
        //cout << "\n ";
        for (int j = 2; j < lenX - 2; j++)
        {
            T[i][j] = (i + 1) / ((float)(2) * (j + 1)) + pow(3, j)/ pow(2, j); //(pow(lenX, 2) + j + 1)
            //T[i][j] = (0.25 * T[i + 1][j] + 0.25 * T[i - 1][j] + 0.25 * T[i][j + 1] + 0.25 * T[i][j - 1]);
            //cout << int(T[i][j]);
            //cout << "  ";
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

                bool r_ip1 = random_decision(i + 1, j, Z, n, T) & (get(varLig, i, j) or get(varLig, i + 1, j - 1) or get(varLig, i + 1, j + 1) or get(varLig, i + 2, j));
                bool r_im1 = random_decision(i - 1, j, Z, n, T) & (get(varLig, i, j) or get(varLig, i - 1, j - 1) or get(varLig, i - 1, j + 1) or get(varLig, i - 2, j));
                bool r_jp1 = random_decision(i, j + 1, Z, n, T) & (get(varLig, i, j) or get(varLig, i - 1, j + 1) or get(varLig, i, j + 2) or get(varLig, i + 1, j + 1));

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


int main()
{
    Grid Lig;


    //int nn = 4;
    //int r = round(random(3, 6));
    //int rand = round(random(1, 3));
    //int ri = round(random(0, 2));
    //int rj = round(random(0, 2));
    //for (int i = 0; i < nn; i++) {
    //    for (int j = 0; j < nn; j++) {
    //        int a = round(ri * (lenX - nn) + i);
    //        int b = round((lenX / r) * (r - rand));
    //        //(lenX / r) * (r - rand)
    //        //(lenX / r) * (r - rand)
    //        put(Lig, a, j, true);
    //            // Lig[ri * (lenX - i) + i, int(lenY / r) * (r - rand)] = 1
    //        put(Lig, i, rj * (lenX - nn) + j, true);
    //    }
    //}
    int numIter = 2 * lenX;
    put(Lig, 1, 3, true); //было 3 до замены на int(lenX* random(0.1, 1))
    put(Lig, 3, 1, true);
    generate_field(n, Lig, numIter);


    /*
    // тест для проверки рандома
    Grid rPrint;
    Grid rPrint2;
    int numIter = 2;
    for (size_t i = 0; i < numIter; i++)
    {
        generate_field(n, Lig, rPrint, rPrint2, numIter);
    }
    
    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            int result = get(rPrint, i, j) - get(rPrint2, i, j);
            cout << result;
            cout << " ";
        }
    }
    cout << " \n\n";*/
    /*put(Lig, 0, 2, true);
    put(Lig, 1, 2, true);
    put(Lig, 2, 2, true);
    put(Lig, 3, 2, true);

    put(Lig, lenX - 2, 0, true);
    put(Lig, lenX - 2, 1, true);
    put(Lig, lenX - 2, 2, true);
    put(Lig, lenX - 2, 3, true);

    put(Lig, lenX, lenX - 2, true);
    put(Lig, lenX - 1, lenX - 2, true);
    put(Lig, lenX - 2, lenX - 2, true);
    put(Lig, lenX - 3, lenX - 2, true);*/

    /*int numIter = 25;
    for (int i = 0; i < numIter; i++) 
    {
        
    }*/

    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j))
                cout << "1 ";
            else
                cout << "0 ";
        }
    }
}