#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>

const double PI = 3.14159;
const double HALFPI = PI / 2;

int getPriority(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 2;
        break;
    case '*':
    case '/':
    case '%':
        return 3;
        break;
    case '~':
        return 4;
        break;
    case '^':
        return 5;
        break;
    case 's':
    case 'c':
    case 't':
    case 'k':
    case 'S':
    case 'C':
    case 'T':
    case 'K':
    case char(251):
        return 6;
        break;
    default:
        return -1;
        break;
    }
}

bool isNumber(char symbol)
{
    return (isdigit(symbol) || symbol == '.' || symbol == ',');
}

std::string getStringNumber(std::string exp, int *begin)
{
    // Получаем символ
    char ch = exp[*begin];
    // Переменная цикла
    int *i = begin;
    // Считанное число, начинающееся с уже обнаруженного символа
    std::string number = "";
    number += ch;

    while (*i + 1 <= exp.length() && isNumber(ch = exp[*i + 1]))
    {
        // Записываем цифру в итоговое число
        number += ch;
        *i = *i + 1;
    }

    // Возвращаем число
    return number;
}

std::string getStringFunction(std::string exp, int *begin)
{
    // Получаем символ
    char ch = exp[*begin];
    // Переменная цикла
    int *i = begin;
    // Считанная слово или функция, начинающееся с уже обнаруженного символа
    std::string func = "";
    func += ch;

    while (*i + 1 <= exp.length() && isalpha(ch = exp[*i + 1]) && ch != ' ')
    {
        // Записываем цифру в итоговое число
        func += ch;
        *i = *i + 1;
    }

    // Возвращаем слово
    return func;
}

// Формат строки для последующего чтения
std::string formatExpression(std::string exp)
{
    // Выходная строка
    std::string outexp = "";

    for (int i = 0; i < exp.length(); i++)
    {
        char ch = exp[i];

        // Если это буква
        if (isalpha(ch))
        {
            // Считываем функцию
            std::string func = getStringFunction(exp, &i);

            // Тригонометрические
            if (func == "sin")
            {
                outexp += 's';
            }
            else if (func == "cos")
            {
                outexp += 'c';
            }
            else if (func == "tg" || func == "tan")
            {
                outexp += 't';
            }
            else if (func == "ctg" || func == "ctan")
            {
                outexp += 'k';
            }
            // Обратные тригонометрические
            else if (func == "arcsin" || func == "asin")
            {
                outexp += 'S';
            }
            else if (func == "arccos" || func == "acos")
            {
                outexp += 'C';
            }
            else if (func == "arctg" || func == "arctan" || func == "atg" || func == "atan")
            {
                outexp += 'T';
            }
            else if (func == "arcctg" || func == "arcctan" || func == "actg" || func == "actan")
            {
                outexp += 'K';
            }
            // Корень
            else if (func == "sqrt")
            {
                outexp += char(251);
            }
            // Константы
            else if (func == "pi")
            {
                outexp += std::to_string(PI);
            }
            // Неизвестная структура
            else
            {
                outexp += "Error";
            }
        }
        else if (ch != ' ')
        {
            outexp += ch;
        }

        // Неявный знак умножения перед скобкой
        if (isNumber(ch) && (i + 1) < exp.length() && exp[i + 1] == '(')
        {
            outexp += '*';
        }
    }

    return outexp;
}

std::string getPostfix(std::string inputExpression)
{
    // Выходная строка
    std::string outputString = "";
    // Строка, над которой будем работать
    std::string myexp = "";
    // Вектор операторов
    std::vector<char> opers;

    myexp = formatExpression(inputExpression);

    // Пока есть необработанные символы
    char symbol;
    for (int n = 0; n < myexp.length(); n++)
    {
        // Рассматриваемый символ
        symbol = myexp[n];

        // Если это минус, определяем его унарность
        if (symbol == '-' && (n == 0 || !isNumber(myexp[n - 1])))
        {
            symbol = '~';
        }

        // Если это цифра или точка
        if (isNumber(symbol))
        {
            // Записываем полное число в выходную строку
            outputString += getStringNumber(myexp, &n) + " ";
        }
        // Если это закрывающая скобка
        else if (symbol == ')')
        {
            // Выталкиваем все знаки до открывающей скобки
            while (!opers.empty() && opers.back() != '(')
            {
                // Записывыем
                outputString += opers.back();
                // Удаляем из вектора
                opers.pop_back();
            }
            // Если операторы не закончились, и мы нашли открывающую скобку
            if (!opers.empty() && opers.back() == '(')
            {
                // Удаляем её
                opers.pop_back();
            }
        }
        // Если вектор пустой, либо это открывающая скобка, либо приоритет рассматриваемого оператора больше лежащего на варшине
        else if (opers.empty() || symbol == '(' || getPriority(symbol) > getPriority(opers.back()))
        {
            opers.push_back(symbol);
        }
        // Если приоритеты равны, либо рассматриваемый символ менее приоритетный
        else if (getPriority(symbol) <= getPriority(opers.back()))
        {
            // Выталкиваем все операторы, имеющие больший или такой же приоритет
            int j = 0;
            int vecSize = opers.size() - 1;
            while (vecSize - j >= 0)
            {
                // Сверяем приоритеты
                if (getPriority(opers[vecSize - j]) >= getPriority(symbol))
                {
                    // Записывыем
                    outputString += opers[vecSize - j];
                    // Удаляем из вектора
                    opers.erase(opers.begin() + (vecSize - j));
                }
                // Если это открывающая скобка, то дальше не лезем
                else if (opers[vecSize - j] == '(')
                {
                    break;
                }
                // Идём дальше
                j++;
            }

            // Сохраняем рассматриваемый элемент
            opers.push_back(symbol);
        }
    }

    // Выталкиваем все оставшиеся операторы
    while (!opers.empty())
    {
        outputString += opers.back();
        opers.pop_back();
    }

    return outputString;
}

double calcPosfixExpression(std::string strExp)
{
    // Вектор всех чисел
    std::vector<double> numbers;

    const int expLength = strExp.length();

    // Проходим входную строку
    for (int i = 0; i < expLength; i++)
    {
        // Рассматриваемый символ
        char ch = strExp[i];

        // Пропускаем пробелы
        if (ch == ' ')
            continue;

        // Если это цифра
        if (isNumber(ch))
        {
            // Получаем строку числа
            const std::string strNumber = getStringNumber(strExp, &i);
            // Char в int
            double dblNumber = std::stof(strNumber);

            // Запихиваем в вектор
            numbers.push_back(dblNumber);
        }
        else
        {
            // Результат этапа вычисления
            double localResult;

            // Если есть операнды
            if (!numbers.empty())
            {
                // Правый оператор (последний)
                double rightOperand = numbers[numbers.size() - 1];

                // Унарные операции
                if (ch == '~' || getPriority(ch) == 6)
                {
                    switch (ch)
                    {
                    case '~':
                        localResult = rightOperand * -1;
                        break;
                    case 's':
                        localResult = sin(rightOperand);
                        break;
                    case 'c':
                        localResult = cos(rightOperand);
                        break;
                    case 't':
                        localResult = tan(rightOperand);
                        break;
                    case 'k':
                        localResult = 1 / tan(rightOperand);
                        break;
                    case 'S':
                        localResult = asin(rightOperand);
                        break;
                    case 'C':
                        localResult = acos(rightOperand);
                        break;
                    case 'T':
                        localResult = atan(rightOperand);
                        break;
                    case 'K':
                        localResult = atan(-rightOperand) + HALFPI;
                        break;
                    case char(251):
                        localResult = sqrt(rightOperand);
                        break;
                    }

                    // Удаляем использованные операнды
                    numbers.pop_back();

                    // Добавляем вместо них результат
                    numbers.push_back(localResult);
                }
                // Бинарные операции (минимум два операнда)
                else if (numbers.size() >= 2)
                {
                    // Левый оператор (предпоследний)
                    double leftOperand = numbers[numbers.size() - 2];

                    switch (ch)
                    {
                    case '+':
                        localResult = leftOperand + rightOperand;
                        break;
                    case '-':
                        localResult = leftOperand - rightOperand;
                        break;
                    case '*':
                        localResult = leftOperand * rightOperand;
                        break;
                    case '/':
                        localResult = leftOperand / rightOperand;
                        break;
                    case '%':
                        localResult = fmod(leftOperand, rightOperand);
                        break;
                    case '^':
                        localResult = pow(leftOperand, rightOperand);
                        break;
                    }

                    // Удаляем использованные операнды
                    numbers.pop_back();
                    numbers.pop_back();

                    // Добавляем вместо них результат
                    numbers.push_back(localResult);
                }
            }
        }
    }

    // Если сотались только числа без бинарных знаков (с минусами)
    while (numbers.size() > 1)
    {
        double rightOperand = numbers[numbers.size() - 1];
        double leftOperand = numbers[numbers.size() - 2];

        numbers.pop_back();
        numbers.pop_back();

        numbers.push_back(leftOperand + rightOperand);
    }

    // Возваращем единственное оставшееся число
    return numbers.back();
}

int main()
{
    // // Переменная, хранящая выражение
    // std::string exp = "";

    // // Вводим выражение
    // std::cout << "Enter some math expression: ";
    // std::getline(std::cin, exp);

    // // Вычисляем постфиксную запись и из неё результат
    // std::string postfixExp = getPostfix(exp);

    // std::cout << "The postfix expression: " << postfixExp;

    // double result = calcPosfixExpression(postfixExp);

    // std::cout << "\n= " << result;

    // Выражения и правильные ответы
    std::string expressions[] = {
        "8/2*(2+2)",
        "2+5(5-3/(4+6))",
        "-1(2*(5-4))",
        "(32-16)-23+2^4",
        "(3+2)+4*5 / 5",
        "(2*2/2)-34+(14/2)",
        "-2^2",
        "(58-32)*-2^2",
        "-16^0.5+(28-4)/4",
        "(22/322*42)/8*322",
        "2^2^2",
        "(8^4/2^8-2)/(4-3*(-2)^3)",
        "-(-(4^2 - 3^2))",
        "2(-3)^4",
        "((-2)^4+(-1)^3*7)/(-3)^2",
        "12*(4*3)/2",
        "2+20*10+(2*10^2)/2",
        "cos(-14)+sqrt(5)-16",
        "(22(sin(pi)^2-cos(pi)^2))/4",
        "sqrt(2)",
        "((arccos(1)-0.5)^2)^2+sqrt(2)",
        "(sqrt(4)+2)^2",
        "(1/tg(4))*2.4",
        "pi*6^2",
    };
    double answers[]{
        16,
        25,
        -2,
        9,
        9,
        -25,
        -4,
        -104,
        2,
        115,
        16,
        0,
        7,
        162,
        1,
        72,
        302,
        -13,
        -5,
        1,
        1,
        16,
        2,
        113,
    };

    // Кол-во примеров
    int arrayLength = sizeof(answers) / sizeof(double);
    // Результаты
    std::string postfix;
    double result;
    // Проходимся, сверяем и выводим
    for (int i = 0; i < arrayLength; i++)
    {
        std::string exp = expressions[i];
        double answ = answers[i];

        postfix = getPostfix(exp);
        result = calcPosfixExpression(postfix);

        std::cout << char(186) << std::left << std::setw(10) << (int(result) == int(answ) ? "ok" : "wtf") << char(186);
        std::cout << std::left << std::setw(40) << exp;
        std::cout << std::left << " = " << std::setw(15) << result << char(186);
        std::cout << std::right << std::setw(45) << postfix << "  " << char(186) << std::endl;

        // Рисуем рамку
        std::cout << char(199);
        for (int i = 0; i < 10; i++)
        {
            std::cout << char(196);
        }
        std::cout << char(215);
        for (int i = 0; i < 58; i++)
        {
            std::cout << char(196);
        }
        std::cout << char(215);
        for (int i = 0; i < 47; i++)
        {
            std::cout << char(196);
        }
        std::cout << char(182);
        std::cout << std::endl;
    }

    // Нажать кнопку перед выходом
    getch();

    return 0;
}