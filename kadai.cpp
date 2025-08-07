#include <iostream>

int main() {
    // 2つの整数を格納するための変数を宣言します。
    int number1;
    int number2;

    // ユーザーに1つ目の整数の入力を促します。
    std::cout << "1つ目の整数を入力してください: ";

    // ユーザーが入力した値を変数 number1 に格納します。
    std::cin >> number1;

    // ユーザーに2つ目の整数の入力を促します。
    std::cout << "2つ目の整数を入力してください: ";

    // ユーザーが入力した値を変数 number2 に格納します。
    std::cin >> number2;

    // 入力された2つの整数を表示します。
    std::cout << "入力された整数は " << number1 << " と " << number2 << " です。" << std::endl;

    return 0;
}
