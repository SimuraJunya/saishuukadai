#include <iostream>

int main() {
    // 2�̐������i�[���邽�߂̕ϐ���錾���܂��B
    int number1;
    int number2;

    // ���[�U�[��1�ڂ̐����̓��͂𑣂��܂��B
    std::cout << "1�ڂ̐�������͂��Ă�������: ";

    // ���[�U�[�����͂����l��ϐ� number1 �Ɋi�[���܂��B
    std::cin >> number1;

    // ���[�U�[��2�ڂ̐����̓��͂𑣂��܂��B
    std::cout << "2�ڂ̐�������͂��Ă�������: ";

    // ���[�U�[�����͂����l��ϐ� number2 �Ɋi�[���܂��B
    std::cin >> number2;

    // ���͂��ꂽ2�̐�����\�����܂��B
    std::cout << "���͂��ꂽ������ " << number1 << " �� " << number2 << " �ł��B" << std::endl;

    return 0;
}
