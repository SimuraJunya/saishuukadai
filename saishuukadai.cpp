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
        cout << "自己ベスト更新！" << endl;
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
        cout << "掛け合わされていると思う" << num_factors << "つの素数をスペースで区切って入力してください: ";
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

        cout << "こんにちは、" << player.getName() << "さん！" << endl;
        if (player.getBestTime() > 0) {
            cout << "あなたの自己ベストは " << fixed << setprecision(2) << player.getBestTime() << " 秒です。" << endl;
        }
        else {
            cout << "自己ベスト記録に挑戦しましょう！" << endl;
        }

        cout << "\n掛け合わせると " << this->product << " になる" << num_factors << "つの素数を当ててください。" << endl;

        auto start_time = chrono::high_resolution_clock::now();
        vector<int> guess = getPlayerGuess();
        auto end_time = chrono::high_resolution_clock::now();

        if (guess.size() != num_factors) {
            cout << "\nエラー: 有効な" << num_factors << "つの素数を入力してください。" << endl;
            return;
        }

        cout << "\n--- 結果 ---" << endl;
        if (checkAnswer(guess)) {
            chrono::duration<double> elapsed_time = end_time - start_time;
            double current_time = elapsed_time.count();
            cout << "正解です！" << endl;
            cout << "タイム: " << fixed << setprecision(2) << current_time << " 秒" << endl;

            if (player.getBestTime() < 0 || current_time < player.getBestTime()) {
                saveScore(current_time);
            }
        }
        else {
            cout << "不正解です。" << endl;
            vector<int> sorted_factors = secret_factors;
            sort(sorted_factors.begin(), sorted_factors.end());
            cout << "正解は ";
            for (auto it = sorted_factors.begin(); it != sorted_factors.end(); ++it) {
                cout << *it << (next(it) != sorted_factors.end() ? ", " : "");
            }
            cout << " でした。" << endl;
        }
    }
};

int main() {
    string name;
    cout << "プレイヤー名を入力してください: ";
    cin >> name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Player player(name);
    Game game(player);

    char play_again_choice;
    do {
        game.playRound();

        cout << "\nもう一度トライしますか？ (はい: y, いいえ: n): ";
        cin >> play_again_choice;
        if (cin.peek() != '\n') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "----------------------------------------" << endl;

    } while (play_again_choice == 'y');

    cout << "ゲームを終了します。" << endl;
}