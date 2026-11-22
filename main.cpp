#include <iostream>
#include <sstream>
#include "bowling_frame.h"

using namespace std;

int current_game_total(Bowling_Frame* first_frame)
{
    Bowling_Frame* temp = first_frame;
    int game_total = 0;

    while (temp != nullptr)
    {
        game_total += temp->frame_score();
        temp = temp->next_frame();
    }

    return game_total;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Error: Please enter the number of players.\n";
        cout << "Usage: ./bowling <number of players>\n";
        return 1;
    }

    int player_count = 0;
    char extra;
    istringstream input(argv[1]);

    if (!(input >> player_count) || (input >> extra) || player_count <= 0)
    {
        cout << "Error: Enter a positive whole number of players.\n";
        return 1;
    }

    Bowling_Frame** first_frames = new Bowling_Frame*[player_count];

    // Make and link ten frames for each player.
    for (int player = 0; player < player_count; player++)
    {
        first_frames[player] = new Bowling_Frame(1);
        Bowling_Frame* previous = first_frames[player];

        for (int number = 2; number <= 10; number++)
        {
            Bowling_Frame* frame = new Bowling_Frame(number);
            previous->next_frame(frame);
            frame->prev_frame(previous);
            previous = frame;
        }
    }

    for (int player = 0; player < player_count; player++)
    {
        Bowling_Frame* frame = first_frames[player];

        // Play each frame in order.
        while (frame != nullptr)
        {
            frame->update_scores();

            if (frame->frame_num() == 10)
            {
                frame->update_scores();

                if (frame->strike() || frame->spare())
                {
                    frame->update_scores();
                }
            }
            else if (!frame->strike())
            {
                frame->update_scores();
            }

            frame = frame->next_frame();
        }

        // All rolls are available now, so update and print the scores.
        cout << "\nPlayer " << player + 1 << "\n";
        frame = first_frames[player];

        while (frame != nullptr)
        {
            frame->update_scores();

            cout << "Frame " << frame->frame_num() << ": "
                 << frame->frame_score() << "\n";

            frame = frame->next_frame();
        }

        cout << "Game total: "
             << current_game_total(first_frames[player]) << "\n";

        // Free the frames created for this player.
        frame = first_frames[player];

        while (frame != nullptr)
        {
            Bowling_Frame* next = frame->next_frame();
            delete frame;
            frame = next;
        }
    }

    delete[] first_frames;
    return 0;
}
