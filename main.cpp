#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

int getPriority(char ch)
{
    switch (ch)
    {
    case '(':
        return 0;
        break;
    case ')':
        return 0;
        break;
    case '+':
        return 1;
        break;
    case '-':
        return 1;
        break;
    case '*':
        return 2;
        break;
    case '/':
        return 2;
        break;
    case '%':
        return 2;
        break;
    case '^':
        return 3;
        break;

    default:
        return -1;
        break;
    }
}

std::string toPostfix(std::string strExpression)
{
    std::string postfixExpression = "";
    std::vector<char> mathOpers; // Вектор операторов

    const int expLength = strExpression.length();

    // Проходим входную строку
    for (int i = 0; i < expLength; i++)
    {
        // Рассматриваемый символ
        char ch = strExpression[i];

        // Если это цифра
        if (isdigit(ch))
        {
            // Забираем всё число
            do
            {
                // Записываем цифру в выходную строку
                postfixExpression += ch;
                // Смотрим цифры дальше
                ++i;
                // Присваиваем новое значение знаку и проверяем условие цикла
            } while (isdigit(ch = strExpression[i]));
            // Если дальше не цифра, то на позицию назад
            --i;
            // Разделяем числа пробелом
            postfixExpression += ' ';
        }
        // Если это не цифра и не пробел
        else if (ch != ' ')
        {

            // Если это арифметический знак
            if (getPriority(ch) != -1)
            {
                // Сохраняем, если вектор пустой или это открывающая скобка или
                // Если у рассматриваемого знака приоритет больше чем у последнего в стеке
                if (mathOpers.empty() || ch == '(' || getPriority(ch) > getPriority(mathOpers.back()))
                {
                    mathOpers.push_back(ch);
                }
                // Если это закрывающая скобка
                else if (ch == ')')
                {
                    // Тогда выталкиваем всё, до открывающей

                    // Смотрим все операторы в векторе с крайнего
                    for (int j = mathOpers.size(); j > 0; j--)
                    {
                        // Если приоритет больше или равен
                        if (mathOpers[j - 1] != '(' && !mathOpers.empty())
                        {
                            // Выталкиваем
                            postfixExpression += mathOpers[j - 1];
                            postfixExpression += ' ';

                            // Удаляем вытолкнутые из стека
                            auto begin = mathOpers.cbegin();
                            mathOpers.erase(begin + j - 1);
                        }
                        else if (!mathOpers.empty())
                        {
                            // Удаляем открывающую скобку из стека
                            auto begin = mathOpers.cbegin();
                            mathOpers.erase(begin + j - 1);
                        }
                    }
                }
                // Если приоритет меньше рассматриваемого меньше чем у последнего
                else if (getPriority(ch) <= getPriority(mathOpers.back()))
                {
                    // Смотрим все операторы в векторе с крайнего
                    for (int j = mathOpers.size(); j > 0; j--)
                    {
                        // Если приоритет больше или равен
                        if (getPriority(mathOpers[j - 1]) >= getPriority(ch) && !mathOpers.empty())
                        {
                            // Выталкиваем
                            postfixExpression += mathOpers[j - 1];
                            postfixExpression += ' ';

                            // Удаляем вытолкнутые из стека
                            auto begin = mathOpers.cbegin();
                            mathOpers.erase(begin + j - 1);

                            // Компенсируем удалённый элемент
                            ++j;
                        }
                    }

                    // Сохраняем текущий оператор
                    mathOpers.push_back(ch);
                }
            }
            else
            {
                // Неизвестный символ
                return "Error, unknown character!";
            }
        }
    }

    // Выталкиваем все оставшиеся операторы
    if (!mathOpers.empty())
    {
        for (int i = mathOpers.size(); i > 0; i--)
        {
            postfixExpression += mathOpers[i - 1];
            postfixExpression += ' ';
        }
    }

    return postfixExpression;
}

int main()
{
    std::string exp;

    std::cout << "Enter some math expression: ";
    std::cin >> exp;

    std::cout << "The postfix expression: ";
    std::cout << toPostfix(exp);

    std::cout << "\nResult: ";

    getch();

    return 0;
}