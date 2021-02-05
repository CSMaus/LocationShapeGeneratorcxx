// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <unordered_set>
#include <exception>
#include <unordered_map>

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

bool put(Grid& grid, int x, int y)
{
    bool it;
    grid.emplace(IntVector2(x, y)) = it;
    //if (it == grid.end())
        //return 0;

    //return it->second;
}


int main()
{
    cout << "Hello World!\n";

    Grid Lig;
    int d = get(Lig, 4, 6);
    put(Lig, 5, 6);
    

    float n = 0.95;

}

int generate_field(float n, Grid& grid)
{
    //float T[lenX][lenX];

    // Set Boundary condition for lighthing
    int Ttop = 45;  // 100
    int Tbottom = 0; // 0
    int Tleft = 15;  // 0
    int Tright = 15;  // 30
    int Tguess = 5; //30


    //создаю массив, в конце выполнения функции он удалится
    auto T = new float* [lenX];
    for (int i = 1; i < lenX - 1; i++)
    {
        T[i] = new float[lenX];
        for (int j = 1; j < lenX - 1; j++)
        {
            T[i][j] = i/(2*j) + pow(2, j);
            //T[i][j] = 0.25 * (T[i + 1][j] + T[i - 1][j] + T[i][j + 1] + T[i][j - 1]);
        }
    }


    Grid Lig;

    int d = get(Lig, 4, 6);


    for (int k = 2; k < lenX - 2; k++)
    {
        for (int l = 2; l < lenX - 2; l++)
        {
            float Z = (T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1]);
            // определяю для x + 1
            if (random_decision(k + 1, l, Z, n, *T[k + 1][l])) {
                
                if ((get(Lig, k, l) == 1) or (get(Lig, k + 1, l - 1) == 1) or (get(Lig, k + 1, l + 1) == 1) or (get(Lig, k + 2, l) == 1)) {

                    put(Lig, k + 1, l)= 1;
                }
                else {
                    get(Lig, k + 1, l) = 0;
                }
            }
            else {
                Lig[k + 1][l] = 0;
            }

            // определяю для x - 1
            if (random_decision(k - 1, l, Z, n)) {
                if ((get(Lig, k][l] == 1) or (get(Lig, k - 1][l - 1] == 1) or (get(Lig, k - 1][l + 1] == 1) or (get(Lig, k - 2][l] == 1)) {
                    get(Lig, k - 1][l] = 1;
                }
                else {
                    get(Lig, k - 1][l] = 0;
                }
            }
            else {
                get(Lig, k - 1][l] = 0;
            }

                // определяю для y + 1 # (get(Lig, k - 1, l] == 0) & (get(Lig, k + 1, l] == 0) |
            if (random_decision(k, l + 1, Z, n)) {
                if ((get(Lig, k][l] == 1) or (get(Lig, k - 1][l + 1] == 1) or (get(Lig, k][l + 2] == 1) or (get(Lig, k + 1][l + 1] == 1)){
                    get(Lig, k][l + 1] = 1;
                }
                else {
                    get(Lig, k][l + 1] = 0;
                }
            }
            else {
                get(Lig, k][l + 1] = 1;
            }
        }
    }

    //удаляю массив - очищаю память
    for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;

    return Lig;
    
}

bool random_decision(int k, int l, float Z, float n, float *T)
{
    float psi = Z * rand();
    float Z1 = pow((*T[k - 1][l] + *T[k + 1][l] + *T[k][l - 1] + *T[k][l + 1]), n);

    return Z1 > psi;
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
