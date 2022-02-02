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
            cout << "����������� � �������! �� ������� ������ ������ �������� " << size << " �� " << size << " �� ������ ��������� " << difficulty << endl;
        }
        else
        {
            cout << "���, �� ���������!\n�� ����� ���� ���� ������ ��������� ���: " << endl;
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
                field[j][i] = tempField[i][j]; //������ ���������� ���������, � ������� - ��������
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
            field[sector * (int)sqrt(size) + rowOne][j] = tempField[sector * (int)sqrt(size) + rowTwo][j]; //sector * (int)sqrt(size) + rowOne/rowTwo � ������ �������� �� ������ ���������� �������, ����� �������� ������ ������� ������� � ���������� ����� ������ ����� �������
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
            field[j][sector * (int)sqrt(size) + colOne] = tempField[j][sector * (int)sqrt(size) + colTwo]; //sector * (int)sqrt(size) + colOne/colTwo � ������ �������� �� ������ ���������� �������, ����� �������� ������ ������� ������� � ���������� ����� ������� ����� �������
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

                field[rowSectorOne * (int)sqrt(size) + i][j] = tempField[rowSectorTwo * (int)sqrt(size) + i][j]; //rowSectorOne/rowSectorTwo * (int)sqrt(size) + i - ������ �������� �� ������ ���������� �� ������� � ��������� ����� ������ ���������� �� �����, ����� ��� ������ ������� �������� �� ��� ������ ������� �������
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

                field[j][colSectorOne * (int)sqrt(size) + i] = tempField[j][colSectorTwo * (int)sqrt(size) + i]; //colSectorOne/colSectorTwo * (int)sqrt(size) + i - ������ �������� �� ������ ���������� �� ������� � ��������� ����� ������� ���������� �� �����, ����� ��� ������� ������� �������� �� ��� ������� ������� �������
                field[j][colSectorTwo * (int)sqrt(size) + i] = tempField[j][colSectorOne * (int)sqrt(size) + i];
            }
        }
    }

    void getRandNum(int& firstNum, int& secondNum, const int k) //��������� ���� ������ ��������� ����� � ��������� �� 0 �� k
    {
        firstNum = rand() % k;
        do
        {
            secondNum = rand() % k;
        } while (secondNum == firstNum);

        //cout << "������ ����� = " << firstNum << "\t������ ����� = " << secondNum << endl;
    }

    void getRandNum(int& firstNum, int& secondNum, int& sector, const int k)//��������� ���� ��������� ����� ��� �� ������� �������� ����� �����. ��������� ���������� � ��������� �� 0 �� k
    {
        sector = rand() % k;
        firstNum = rand() % k;
        do
        {
            secondNum = rand() % k;
        } while (secondNum == firstNum);

        //cout << "������ ����� = " << sector << "\t������ ����� = " << firstNum << "\t������ ����� = " << secondNum << endl;
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

        removedCells = 0.2 * difficulty * 2 * pow(size, 2) / 3 - (rand() % size) / sqrt(size); // ������� �������� ���������� �����, ������� ����� �������
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
            } //���� ������ [cellOne][cellTwo] �� ���� �������, �� �� ����� �� �������, ����� �� �� ����������. ��� �������� ������������ ������� ������
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
                cout << "������� ����� ������ � �������, ����� ������� ������ ������, ��������� ����� �� 1 �� " << size << endl;
                cout << "����� ������: ";
                cin >> row;
                cout << "����� �������: ";
                cin >> col;

            } while (size < row || row < 1 || size < col || col < 1 || field[row - 1][col - 1] != 0);

            show(row - 1, col - 1);
            do
            {
                cout << "������� �������� ������(" << row << ", " << col << "), ��������� ����� �� 1 �� " << size << " ��� ����������� 0 ��� ������ �����: ";
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
                cout << "��������� ���� ����� ��������� ��������, �������� ������ " << mistakes << " �� " << 5 - (difficulty - 2) << " ���������!" << endl;

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
        cout << "������� ������ ������ �������, �� �� ������ 2: ";
        cin >> fieldSize;
    } while (fieldSize < 2);

    do
    {
        cout << "�������� ��������� �������� ���� �� 0 �� 5: ";
        cin >> fieldDiff;
    } while (fieldDiff > 5);


    sudoku first(pow(fieldSize, 2), fieldDiff);
    first.startGame();

    system("PAUSE");
    return 0;
}

//lol save it ;-;