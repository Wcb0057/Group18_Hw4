#include <iostream>
#include "bowling_frame.h"
#include <random>
#include <cstdlib>

using namespace std;

std::random_device dev_main;
std::mt19937 rng_main(dev_main());

int current_game_total(Bowling_Frame * first_frame)
{
	Bowling_Frame * temp = first_frame;
	int game_total = 0;
	int temp_total = 0;
	while(temp)
	{
		game_total += temp->frame_score();
		temp = temp->next_frame();
	}
	return game_total;	
}

int main(int argc, char* argv[])
{
	//Require exactly one command line
	if (argc != 2)
	{
		cout << "ERROR: Please provide the number of players." << endl;
		cout << "example: ./bowling 3" << endl;
		return 1;
	}

	int num_players = atoi(argv[1]);

	//creates an array of player pointers
	Bowling_Frame** players = new Bowling_Frame*[num_players];

	//create 10 frames for each player
	for (int p = 0; p < num_players; p++)
	{
		players[p] = new Bowling_Frame(1);

		Bowling_Frame* current = players[p];

		for (int frame = 2; frame <= 10; frame++)
		{
			Bowling_Frame* newFrame = 
				new Bowling_Frame(frame);

			current ->next_frame(newFrame);
			newFrame->prev_frame(current);

			current = newFrame;
		}
	}



	  // PASS 1: Generate all rolls
    for (int p = 0; p < num_players; p++)
    {
        Bowling_Frame* current = players[p];

        while (current)
        {
            current->update_scores();
            current->update_scores();
			current->update_scores();

            current = current->next_frame();
        }
    }

    // Recalculate frame totals after all future frames
	// have their scores populated
	for (int p = 0; p < num_players; p++)
	{
    	Bowling_Frame* current = players[p];

   		while (current)
    	{
        	current->update_scores();
        	current = current->next_frame();
    	}
	}

    // Print results
    for (int p = 0; p < num_players; p++)
    {
        cout << "\n========================" << endl;
        cout << "Player " << (p + 1) << endl;
        cout << "========================" << endl;

        Bowling_Frame* current = players[p];

        while (current)
        {
            cout << "Frame "
                 << current->frame_num()
                 << endl;

            cout << " Score 1: "
                 << current->score1()
                 << endl;

            cout << " Score 2: "
                 << current->score2()
                 << endl;

            if (current->frame_num() == 10)
            {
                cout << " Score 3: "
                     << current->score3()
                     << endl;
            }

            cout << " Frame Total: "
                 << current->frame_score()
                 << endl
                 << endl;

            current = current->next_frame();
        }

        cout << "Game Total: "
             << current_game_total(players[p])
             << endl;
	
    }

    return 0;
}

