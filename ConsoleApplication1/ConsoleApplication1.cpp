// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_set>
#include <exception>
#include <unordered_map>
#include <stdio.h>

using namespace::std;

//const unsigned int lenX = 3;
int const lenX = 5;
float n = 0.4;


double random(double min, double max)
{
    return (double)(rand()) / RAND_MAX * (max - min) + min;
}

struct IntVector2 
{
    bool X;
    bool Y;

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

bool get(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    return it->second;
}

bool put(Grid& grid, int x, int y, bool it)
{
    grid.emplace(IntVector2(x, y), it);
    return 0;
}

bool random_decision(float Z, float n, float z1) //float &T[k][l]
{

    float psi = random(0, 1) * Z;
    
    //float Z1 = pow(z1, n);

    //float Z1 = pow((get(T, k - 1, l) + get(T, k + 1, l) + get(T, k, l - 1) + get(T, k, l + 1)), n);
    //float Z1 = pow((*T[k - 1][l] + *T[k + 1][l]  + *T[k][l - 1] + *T[k][l + 1]), n);
    if (z1 > Z)
        return true;
    else
        return false;


    //return Z1 > psi;
}

void generate_field(float n, Grid& varLig) //было int вместо Grid& (magic  @_@ )
{
    //Grid Lig;

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
            T[i][j] = i / ((float)(2) * j) + pow(2, j);

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
                if ((get(varLig, i, j) == true) or (get(varLig, i + 1, j - 1) == true) or (get(varLig, i + 1, j + 1) == true) or (get(varLig, i + 2, j) == true)) {

                    put(varLig, i + 1, j, true);
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
                if ((get(varLig, i, j) == true) or (get(varLig, i - 1, j - 1) == true) or (get(varLig, i - 1, j + 1) == true) or (get(varLig, i - 2, j) == true)) {
                    put(varLig, i - 1, j, true);
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
                if ((get(varLig, i, j) == true) or (get(varLig, i - 1, j + 1) == true) or (get(varLig, i, j + 2) == true) or (get(varLig, i + 1, j + 1) == true)) {
                    put(varLig, i, j + 1, true);
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
    for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;
}


int main()
{
    cout << "Hello World!\n";
    

    Grid Lig;
    
    put(Lig, 0, 0, true);
    put(Lig, 1, 0, true);
    put(Lig, 2, 0, true);
    put(Lig, 3, 0, false);

    put(Lig, 0, lenX, true);
    put(Lig, 1, lenX, true);
    put(Lig, 2, lenX, true);
    put(Lig, 3, lenX, false);

    //int d = get(Lig, 4, 6);
    //put(Lig, 5, 6, true); //тест работает ли


    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j) == true)
                cout << "1 ";
            else
                cout << "0 ";
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
