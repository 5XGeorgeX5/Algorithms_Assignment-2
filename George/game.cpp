#include "skipList.cpp"
using namespace std;

class Game
{
private:
    SkipList<string, int> nameSortedList;
    SkipList<pair<int, string>, int> scoreSortedList;

public:
    void addPlayer(string name, int score = 0)
    {
        nameSortedList.insert(name, score);
        scoreSortedList.insert({score, name}, score);
    }

    void removePlayer(string name)
    {
        int score = nameSortedList.remove(name);
        scoreSortedList.remove({score, name});
    }

    void updateScore(string name, int newScore)
    {
        removePlayer(name);
        addPlayer(name, newScore);
    }

    void viewScore(string name)
    {
        cout << name << "'s Score: " << nameSortedList.search(name) << '\n';
    }

    void viewTopPlayers(int count)
    {
        vector<pair<pair<int, string>, int>> players = scoreSortedList.getArray();
        if (players.size() < count)
            count = players.size();
        int lastScore = INT_MAX;
        int repeated = 1;
        int rank = 0;
        cout << "Top " << count << " Players:\n";
        for (auto it = players.rbegin(); it != players.rend(); it++)
        {
            if (it->second == lastScore)
                repeated++;
            else
            {
                lastScore = it->second;
                rank += repeated;
                repeated = 1;
            }
            if (rank > count)
                break;
            cout << "Rank " << rank << ": " << it->first.second << " Score: " << it->second << '\n';
        }
    }
};

int main()
{
    Game game;

    game.addPlayer("Alice", 100);
    game.addPlayer("Bob", 200);
    game.addPlayer("David", 400);
    game.addPlayer("Eve", 500);
    game.addPlayer("Charlie", 300);
    game.viewTopPlayers(4);
    game.updateScore("Alice", 300);
    game.viewTopPlayers(4);
    game.removePlayer("David");
    game.viewTopPlayers(4);

    return 0;
}
