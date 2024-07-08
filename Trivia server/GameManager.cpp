#include "GameManager.h"
#include "RoomManager.h"
GameManager* GameManager::instancePtr = nullptr;

GameManager* GameManager::getInstance()
{
    if (instancePtr == nullptr)
    {
        instancePtr = new GameManager();
    }
    return instancePtr;
}

Game& GameManager::createGame(RoomData& room)
{
    for (Game& game : mGames)
    {
        if (game.getGameId() == room.id)
        {
            return game;
        }
    }

    vector<Question> questions;
    for (Question question : mDataBase->getQuestions(room.numOfQuestionsInGame))
    {
        questions.push_back(question);
    }
	Game game(questions, room.id);
    mGames.push_back(game);
    return *std::find(mGames.begin(), mGames.end(), game);

}

void GameManager::deleteGame(const unsigned int gameId)
{
    for (auto it = mGames.begin(); it != mGames.end(); ++it)
    {
        if (it->getGameId() == gameId)
        {
            mGames.erase(it);
            break;
        }
    }
}

std::thread GameManager::startRemoveFinishedGames()
{
    return std::thread(&GameManager::removeFinishedGames, this);
}

void GameManager::removeFinishedGames()
{
    RoomManager* roomManager = RoomManager::getInstance();
    while (true) {
        if (mGames.size() > 0)
        {
            for (int i = 0; i < mGames.size();) 
            {
                if(mGames.at(i).isFinished())
                {
                    std::this_thread::sleep_for(std::chrono::seconds(10)); //Let the users take game results before game is freed.
                    roomManager->deleteRoom(mGames.at(i).getGameId());
                    mGames.erase(mGames.begin() + i);
                }
                else
                {
                    i++;
                }

            }

        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

GameManager::GameManager()
{
    mDataBase = SqliteDataBase::getInstance();
}

GameManager::~GameManager()
{
	delete instancePtr;
    instancePtr = nullptr;
}

