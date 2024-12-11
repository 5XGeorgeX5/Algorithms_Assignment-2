#include "SkipList.cpp"
using namespace std;

class Game
{
private:
    SkipList<int, int> idSortedList;
    SkipList<pair<int, int>, int> scoreSortedList;
    int size = 0;

public:
    void addPlayer(int id, int score = 0)
    {
        size++;
        idSortedList.insert(id, score);
        scoreSortedList.insert({score, id}, score);
    }

    void removePlayer(int id)
    {
        size--;
        int score = idSortedList.remove(id);
        scoreSortedList.remove({score, id});
    }

    void updateScore(int id, int newScore)
    {
        removePlayer(id);
        addPlayer(id, newScore);
    }

    void viewScore(int id)
    {
        cout << "Player ID: " << id << " Score: " << idSortedList.search(id) << '\n';
    }

    void viewTopPlayers(int count)
    {
        vector<pair<pair<int, int>, int>> topPlayers = scoreSortedList.getArray();
        if (size < count)
            count = size;
        int rank = 1;
        for (auto it = topPlayers.rbegin(); it != topPlayers.rend(); it++)
        {
            if (rank > count)
                break;
            cout << "Rank: " << rank << " Player ID: " << it->first.second << " Score: " << it->first.first << '\n';
            rank++;
        }
    }
};

int main()
{
    Game game;

    game.addPlayer(1);
    game.addPlayer(2);
    game.addPlayer(3);

    game.updateScore(1, 50);
    game.updateScore(2, 30);
    game.updateScore(3, 70);

    game.viewTopPlayers(3);

    game.updateScore(1, -10);
    game.viewTopPlayers(2);

    game.viewScore(1);
    game.removePlayer(2);
    game.viewTopPlayers(3);

    return 0;
}
