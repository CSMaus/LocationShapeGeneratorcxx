// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_set>
#include <exception>
#include <unordered_map>
//#include "ConsoleApplication1.h"

using namespace::std;

//const unsigned int lenX = 3;
int const lenX = 20;

struct IntVector2 
{
    bool X;
    bool Y;

    IntVector2(int x, int y)
        :X(x), Y(y)
    {
    }
};

template<> struct std::hash<IntVector2> {
    std::size_t operator()(IntVector2 const& s) const noexcept
    {
        return s.X ^ s.Y;
    }
};

typedef std::unordered_map<IntVector2, int> Grid;

bool get(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    return it->second;
}

bool put(Grid& grid, int x, int y, bool it)
{
    //bool it;
    grid.emplace(IntVector2(x, y), it);
    //if (it == grid.end())
        //return 0;

    //return it->second;
}

bool random_decision(float Z, float n, float z1) //float &T[k][l]
{

    float psi = Z * rand();
    float Z1 = pow(z1, n);
    //float Z1 = pow((get(T, k - 1, l) + get(T, k + 1, l) + get(T, k, l - 1) + get(T, k, l + 1)), n);
    //float Z1 = pow((*T[k - 1][l] + *T[k + 1][l]  + *T[k][l - 1] + *T[k][l + 1]), n);

    return Z1 > psi;
}

Grid* generate_field(float n, Grid& grid) //было int вместо Grid& (magic  @_@ )
{
    Grid Lig;

    // Set Boundary condition for lighthing
    int Ttop = 45;  // 100
    int Tbottom = 0; // 0
    int Tleft = 15;  // 0
    int Tright = 15;  // 30
    int Tguess = 5; //30

    //создаю массив, в конце выполнения функции он удалится
    auto T = new float* [lenX];
    //Grid T;
    for (int i = 1; i < lenX - 1; i++)
    {
        T[i] = new float[lenX];
        for (int j = 1; j < lenX - 1; j++)
        {
            //put(T, i, j, i / (2 * j) + pow(2, j));
            T[i][j] = i / (2 * j) + pow(2, j);
            //T[i][j] = 0.25 * (T[i + 1][j] + T[i - 1][j] + T[i][j + 1] + T[i][j - 1]);
        }
    }
    
    for (int i = 2; i < lenX - 2; i++)
    {
        for (int j = 2; j < lenX - 2; j++)
        {
            float Z = (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);
            //float Z = (get(T, i - 1, j) + get(T, i + 1, j) + get(T, i, j - 1) + get(T, i, j + 1));

            // определяю для x + 1 
            //комменьтю пока else т к д быть false, если не задано иное

            int k = i + 1, l = j;
            float z1 = T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1];
            if (random_decision(Z, n, z1)) { //&T[k + 1][l] почему надо так, а не : *T[k + 1][l]

                if ((get(Lig, i, j) == 1) or (get(Lig, i + 1, j - 1) == 1) or (get(Lig, i + 1, j + 1) == 1) or (get(Lig, i + 2, j) == 1)) {

                    put(Lig, i + 1, j, true);
                }
                /*else {
                    get(Lig, k + 1, l) = 0;
                }
            }
            else { 
                Lig[k + 1][l] = 0; */
            }


            // определяю для x - 1
            k = i - 1, l = j;
            z1 = T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1];
            if (random_decision(Z, n, z1)) {
                if ((get(Lig, i, j) == 1) or (get(Lig, i - 1, j - 1) == 1) or (get(Lig, i - 1, j + 1) == 1) or (get(Lig, i - 2, j) == 1)) {
                    put(Lig, i - 1, j, true);
                }
                /*else {
                get(Lig, k - 1][l] = 0;
                }
                }
                else {
                get(Lig, k - 1][l] = 0;
                */
            }

            // определяю для y + 1 # (get(Lig, k - 1, l] == 0) & (get(Lig, k + 1, l] == 0) |
            k = i, l = j + 1;
            z1 = T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1];
            if (random_decision(Z, n, z1)) {
                if ((get(Lig, i, j) == 1) or (get(Lig, i - 1, j + 1) == 1) or (get(Lig, i, j + 2) == 1) or (get(Lig, i + 1, j + 1) == 1)) {
                    put(Lig, i, j + 1, true);
                }
                /*else {
                get(Lig, k][l + 1] = 0;
                }
                }
                else {
                get(Lig, k][l + 1] = 1;
                */
            }
        }
    }

    //удаляю массив - очищаю память
    /*for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;*/

    //return Lig;
}


int main()
{
    cout << "Hello World!\n";
    float n = 0.95;

    Grid Lig;
    Lig = *generate_field(n, Lig);
    //int d = get(Lig, 4, 6);
    //put(Lig, 5, 6, true); //тест работает ли


    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            cout << get(Lig, i, j) + " ";
        }

    }

}




// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
