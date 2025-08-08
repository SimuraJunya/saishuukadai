#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>
#include <limits>
#include <fstream>
#include <map>
#include <iomanip> 
using namespace std;

class Player {
private:
    string name;
    double best_time;

public:
    Player(const string& player_name) : name(player_name), best_time(-1.0) {}

    string getName() const { return name; }
    double getBestTime() const { return best_time; }

    void setBestTime(double time) { best_time = time; }
};

class Game {
private:
    Player& player; 
    mt19937 gen; 

    const vector<int> source_numbers = { 2, 3, 5, 7 };
    const int num_factors = 3;

    vector<int> secret_factors;
    long long product;

    map<string, double> loadAllScores() {
        map<string, double> scores;
        ifstream score_file("scores.txt");
        if (score_file.is_open()) {
            string name;
            double time;
            while (score_file >> name >> time) {
                scores[name] = time;
            }
            score_file.close();
        }
        return scores;
    }

    void loadPlayerBestTime() {
        map<string, double> scores = loadAllScores();
        auto it = scores.find(player.getName());
        if (it != scores.end()) {
            player.setBestTime(it->second);
        }
    }

    void saveScore(double new_time) {
        map<string, double> scores = loadAllScores();
        scores[player.getName()] = new_time;

        ofstream score_file("scores.txt");
        for (const auto& pair : scores) {
            score_file << pair.first << " " << fixed << setprecision(2) << pair.second << endl;
        }
        score_file.close();
        player.setBestTime(new_time); 
        cout << "���ȃx�X�g�X�V�I" << endl;
    }

    void generateQuestion() {
        uniform_int_distribution<> distrib(0, 3);
        secret_factors.clear();
        product = 1;
        for (int i = 0; i < num_factors; ++i) {
            int chosen_number = source_numbers[distrib(gen)];
            secret_factors.push_back(chosen_number);
            product *= chosen_number;
        }
    }

    vector<int> getPlayerGuess() const {
        cout << "�|�����킳��Ă���Ǝv��" << num_factors << "�̑f�����X�y�[�X�ŋ�؂��ē��͂��Ă�������: ";
        vector<int> guess;

        for (int i = 0; i < num_factors; ++i) {
            int number;
            cin >> number;

            if (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                return {}; 
            }
            guess.push_back(number);
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        return guess;
    }

    bool checkAnswer(vector<int>& player_guess) const {
        vector<int> sorted_secret = secret_factors;
        sort(sorted_secret.begin(), sorted_secret.end());
        sort(player_guess.begin(), player_guess.end());
        return sorted_secret == player_guess;
    }

public:
    Game(Player& p) : player(p), gen(random_device{}()) {
        loadPlayerBestTime();
    }

    void playRound() {
        generateQuestion();

        cout << "����ɂ��́A" << player.getName() << "����I" << endl;
        if (player.getBestTime() > 0) {
            cout << "���Ȃ��̎��ȃx�X�g�� " << fixed << setprecision(2) << player.getBestTime() << " �b�ł��B" << endl;
        }
        else {
            cout << "���ȃx�X�g�L�^�ɒ��킵�܂��傤�I" << endl;
        }

        cout << "\n�|�����킹��� " << this->product << " �ɂȂ�" << num_factors << "�̑f���𓖂ĂĂ��������B" << endl;

        auto start_time = chrono::high_resolution_clock::now();
        vector<int> guess = getPlayerGuess();
        auto end_time = chrono::high_resolution_clock::now();

        if (guess.size() != num_factors) {
            cout << "\n�G���[: �L����" << num_factors << "�̑f������͂��Ă��������B" << endl;
            return;
        }

        cout << "\n--- ���� ---" << endl;
        if (checkAnswer(guess)) {
            chrono::duration<double> elapsed_time = end_time - start_time;
            double current_time = elapsed_time.count();
            cout << "�����ł��I" << endl;
            cout << "�^�C��: " << fixed << setprecision(2) << current_time << " �b" << endl;

            if (player.getBestTime() < 0 || current_time < player.getBestTime()) {
                saveScore(current_time);
            }
        }
        else {
            cout << "�s�����ł��B" << endl;
            vector<int> sorted_factors = secret_factors;
            sort(sorted_factors.begin(), sorted_factors.end());
            cout << "������ ";
            for (auto it = sorted_factors.begin(); it != sorted_factors.end(); ++it) {
                cout << *it << (next(it) != sorted_factors.end() ? ", " : "");
            }
            cout << " �ł����B" << endl;
        }
    }
};

int main() {
    string name;
    cout << "�v���C���[������͂��Ă�������: ";
    cin >> name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Player player(name);
    Game game(player);

    char play_again_choice;
    do {
        game.playRound();

        cout << "\n������x�g���C���܂����H (�͂�: y, ������: n): ";
        cin >> play_again_choice;
        if (cin.peek() != '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "----------------------------------------" << endl;

    } while (play_again_choice == 'y');

    cout << "�Q�[�����I�����܂��B" << endl;
}