#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

class sudoku
{
public:
    sudoku(int size, int difficulty)
    {
        int k = 0;
        this->difficulty = difficulty;

        this->size = size;
        field = new int* [this->size];
        tempField = new int* [this->size];
        boolField = new bool* [this->size];
        for (int i = 0; i < this->size; i++)
        {
            field[i] = new int[this->size];
            tempField[i] = new int[this->size];
            boolField[i] = new bool[this->size];
        }

        for (int i = 0; i < this->size; i++)
        {
            for (int j = 0; j < this->size; j++)
            {
                boolField[i][j] = false;

                if (i == 0)
                {
                    field[i][j] = j + 1;
                }
                else
                {
                    field[i][j] = field[i - 1][(j + (int)sqrt(this->size) + k) % this->size];
                }
            }

            if (i % (int)sqrt(this->size) == (int)sqrt(this->size) - 1)
            {
                k++;
            }
            else
            {
                k = 0;
            }
        }

        fieldGeterator();
    }

    void startGame()
    {
        show();

        if (cellPicker() == true)
        {
            cout << "Поздравляем с победой! Вы успешно решили судоку размером " << size << " на " << size << " на уровне сложности " << difficulty << endl;
        }
        else
        {
            cout << "Увы, Вы проиграли!\nНа самом деле поле судоку выглядело так: " << endl;
            show(difficulty);
        }

    }

    ~sudoku()
    {
        delete[]field;
        delete[]tempField;
        delete[]boolField;
    }

private:
    int difficulty;
    int size;
    int removedCells;
    int** field;
    int** tempField;
    bool** boolField;

    void show()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (field[i][j] == 0)
                {
                    cout << "* ";
                }
                else
                {
                    cout << field[i][j] << ' ';
                }

                if (j % (int)sqrt(size) == (int)sqrt(size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(size) == (int)sqrt(size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    }

    void show(const int row, const int col)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (field[i][j] == 0)
                {
                    if (i == row && j == col)
                    {
                        cout << "+ ";
                    }
                    else
                    {
                        cout << "* ";
                    }
                }
                else
                {
                    cout << field[i][j] << ' ';
                }

                if (j % (int)sqrt(size) == (int)sqrt(size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(size) == (int)sqrt(size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    }

    void show(const bool mistakes)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cout << tempField[i][j] << ' ';

                if (j % (int)sqrt(size) == (int)sqrt(size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(size) == (int)sqrt(size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    }

    void copyField()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                tempField[i][j] = field[i][j];
            }
        }
    }

    void transposing()
    {
        copyField();

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                field[j][i] = tempField[i][j]; //строки становятся столбцами, а столбцы - строками
            }
        }
    }

    void swapRows()
    {
        int rowOne, rowTwo, sector;

        copyField();

        getRandNum(rowOne, rowTwo, sector, (int)sqrt(size));
        for (int j = 0; j < size; j++)
        {
            field[sector * (int)sqrt(size) + rowOne][j] = tempField[sector * (int)sqrt(size) + rowTwo][j]; //sector * (int)sqrt(size) + rowOne/rowTwo – сектор умножаем на корень квадратный размера, чтобы получить начало нужного сектора и прибавляем номер строки этого сектора
            field[sector * (int)sqrt(size) + rowTwo][j] = tempField[sector * (int)sqrt(size) + rowOne][j];
        }
    }

    void swapCols()
    {
        int colOne, colTwo, sector;

        copyField();

        getRandNum(colOne, colTwo, sector, (int)sqrt(size));
        for (int j = 0; j < size; j++)
        {
            field[j][sector * (int)sqrt(size) + colOne] = tempField[j][sector * (int)sqrt(size) + colTwo]; //sector * (int)sqrt(size) + colOne/colTwo – сектор умножаем на корень квадратный размера, чтобы получить начало нужного сектора и прибавляем номер столбца этого сектора
            field[j][sector * (int)sqrt(size) + colTwo] = tempField[j][sector * (int)sqrt(size) + colOne];
        }
    }

    void swapRowSectors()
    {
        int rowSectorOne, rowSectorTwo;

        copyField();

        getRandNum(rowSectorOne, rowSectorTwo, (int)sqrt(size));
        for (int i = 0; i < (int)sqrt(size); i++)
        {
            for (int j = 0; j < size; j++)
            {

                field[rowSectorOne * (int)sqrt(size) + i][j] = tempField[rowSectorTwo * (int)sqrt(size) + i][j]; //rowSectorOne/rowSectorTwo * (int)sqrt(size) + i - сектор умножаем на корень квадратный из размера и прибаляем номер строки итерируясь по циклу, чтобы все строки сектора поменять на все строки другого сектора
                field[rowSectorTwo * (int)sqrt(size) + i][j] = tempField[rowSectorOne * (int)sqrt(size) + i][j];
            }
        }
    }

    void swapColSectors()
    {
        int colSectorOne, colSectorTwo;

        copyField();

        getRandNum(colSectorOne, colSectorTwo, (int)sqrt(size));
        for (int i = 0; i < (int)sqrt(size); i++)
        {
            for (int j = 0; j < size; j++)
            {

                field[j][colSectorOne * (int)sqrt(size) + i] = tempField[j][colSectorTwo * (int)sqrt(size) + i]; //colSectorOne/colSectorTwo * (int)sqrt(size) + i - сектор умножаем на корень квадратный из размера и прибаляем номер столбца итерируясь по циклу, чтобы все столбцы сектора поменять на все столбцы другого сектора
                field[j][colSectorTwo * (int)sqrt(size) + i] = tempField[j][colSectorOne * (int)sqrt(size) + i];
            }
        }
    }

    void getRandNum(int& firstNum, int& secondNum, const int k) //Генератор двух разных случайных чисел в диапозоне от 0 до k
    {
        firstNum = rand() % k;
        do
        {
            secondNum = rand() % k;
        } while (secondNum == firstNum);

        //cout << "Первое число = " << firstNum << "\tВторое число = " << secondNum << endl;
    }

    void getRandNum(int& firstNum, int& secondNum, int& sector, const int k)//генератор трех случайных чисел два из которых различны между собой. Генерация происходит в диапозоне от 0 до k
    {
        sector = rand() % k;
        firstNum = rand() % k;
        do
        {
            secondNum = rand() % k;
        } while (secondNum == firstNum);

        //cout << "Первое число = " << sector << "\tВторое число = " << firstNum << "\tТретье число = " << secondNum << endl;
    }

    void fieldGeterator()
    {
        int index = 0;
        int swapNumber, randSwapper;

        swapNumber = 1 + rand() % (int)pow(size, 3);
        do
        {
            randSwapper = rand() % 5;
            switch (randSwapper)
            {
                case 0:
                {
                    swapRows();
                }break;

                case 1:
                {
                    swapCols();
                }break;

                case 2:
                {
                    swapRowSectors();
                }break;

                case 3:
                {
                    swapColSectors();
                }

                case 4:
                {
                    transposing();
                }break;
            }

            index++;
        } while (index < swapNumber);

        copyField();
        cellRemover();
    }

    void cellRemover()
    {
        int cellOne, cellTwo, index;

        removedCells = 0.2 * difficulty * 2 * pow(size, 2) / 3 - (rand() % size) / sqrt(size); // формула рассчета количества ячеек, которые будут удалены
        index = 0;

        while (index < removedCells)
        {
            getRandNum(cellOne, cellTwo, size);

            if (boolField[cellOne][cellTwo] == false)
            {
                boolField[cellOne][cellTwo] = true;
                field[cellOne][cellTwo] = 0;
                index++;
            }
            else
            {
                continue;
            } //Если ячейка [cellOne][cellTwo] не была удалена, то мы можем ее удалить, иначе мы ее пропускаем. Для проверки используется булевый массив
        }
    }

    bool cellPicker()
    {
        bool victory = true;
        unsigned int row, col, value, mistakes;
        mistakes = 0;

        for (int i = 0; i < removedCells; )
        {
            do
            {
                cout << "Введите номер строки и столбца, чтобы выбрать пустую ячейку, используя цифры от 1 до " << size << endl;
                cout << "Номер строки: ";
                cin >> row;
                cout << "Номер столбца: ";
                cin >> col;

            } while (size < row || row < 1 || size < col || col < 1 || field[row - 1][col - 1] != 0);

            show(row - 1, col - 1);
            do
            {
                cout << "Введите значение ячейки(" << row << ", " << col << "), используя цифры от 1 до " << size << " или используйте 0 для отмены выбор: ";
                cin >> value;
            } while (size < value);

            if (value == 0)
            {
                continue;
            }
            else if (value == tempField[row - 1][col - 1])
            {
                field[row - 1][col - 1] = value;
                i++;
                show();
            }
            else
            {
                mistakes++;
                cout << "Введенное Вами число оказалось неверным, допущено ошибок " << mistakes << " из " << 5 - (difficulty - 2) << " возможных!" << endl;

                if (mistakes == 5 - (difficulty - 2))
                {
                    victory = false;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }

        return victory;
    }
};

int main()
{
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    unsigned int fieldSize, fieldDiff;
    do
    {
        cout << "Введите размер одного сектора, но не меньше 2: ";
        cin >> fieldSize;
    } while (fieldSize < 2);

    do
    {
        cout << "Выберите сложность игрового поля от 0 до 5: ";
        cin >> fieldDiff;
    } while (fieldDiff > 5);


    sudoku first(pow(fieldSize, 2), fieldDiff);
    first.startGame();

    system("PAUSE");
    return 0;
}

//lol save it ;-;