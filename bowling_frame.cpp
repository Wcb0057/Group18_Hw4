#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	score1_ = 0;
	score2_ = 0;
	score3_ = 0;          // Stores the third ball in frame 10.
	roll_one_ = false;    // The first ball has not been rolled yet.
	roll_two_ = false;    // The second ball has not been rolled yet.
	roll_three_ = false;  // The third ball has not been rolled yet.
	strike_ = false;
	spare_ = false;
	frame_num_ = fnum;
	frame_score_ = 0;

	next_frame_ = NULL;
	prev_frame_ = NULL;
}

Bowling_Frame::~Bowling_Frame()
{
	// main.cpp deletes each frame, so leave the neighboring frames alone.
	// free(next_frame_);
	// free(prev_frame_);
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame( Bowling_Frame * new_frame ) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame( Bowling_Frame * new_frame ) { prev_frame_ = new_frame; }

int Bowling_Frame::score1()				{ return score1_; }
void Bowling_Frame::score1( int score )	{ score1_ = score; }

int Bowling_Frame::score2()				{ return score2_; }
void Bowling_Frame::score2( int score )	{ score2_ = score; }

int Bowling_Frame::score3() { return score3_; } // Lets main print the third ball.

bool Bowling_Frame::strike() 			{ return strike_; }
void Bowling_Frame::strike(bool value )	{ strike_ = value; }

bool Bowling_Frame::spare() 			{ return spare_; }
void Bowling_Frame::spare(bool value )	{ spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_;
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		if (score1_ == 10) strike_ = true;
	}
	else if (!strike_ && !roll_two_)
	{
		std::uniform_int_distribution<std::mt19937::result_type> distX(0, 10 - score1_);
		score2_ = distX(rng);
		roll_two_ = true;

		if ((score1_ + score2_) == 10) spare_ = true;
	}
	else if (frame_num_ == 10 && strike_ && !roll_two_)
	{
		// A strike in frame 10 gives the second ball a fresh rack.
		score2_ = dist10(rng);
		roll_two_ = true;
	}
	else if (frame_num_ == 10 && (strike_ || spare_) && !roll_three_)
	{
		// A strike or spare in frame 10 earns a third ball.
		int pins_left = 10;

		// If the second ball after a strike leaves pins, roll at those pins.
		if (strike_ && score2_ < 10)
		{
			pins_left = 10 - score2_;
		}

		std::uniform_int_distribution<std::mt19937::result_type> distX(0, pins_left);
		score3_ = distX(rng);
		roll_three_ = true;
	}

	// main.cpp moves to the next frame after this one is complete.
	// else if (next_frame_) next_frame_->update_scores();

	frame_score_ = score1_ + score2_;

	if (frame_num_ == 10)
	{
		// Frame 10 includes its own bonus ball.
		frame_score_ += score3_;
	}
	else if (spare_ && next_frame_)
	{
		// A spare adds the next frame's first ball.
		frame_score_ += next_frame_->score1();
	}
	else if (strike_ && next_frame_)
	{
		// A strike adds the next two balls.
		frame_score_ += next_frame_->score1();

		if (next_frame_->strike() && next_frame_->frame_num() < 10)
		{
			// A strike before frame 10 uses one ball, so look one frame farther.
			if (next_frame_->next_frame())
			{
				frame_score_ += next_frame_->next_frame()->score1();
			}
		}
		else
		{
			// Use the next frame's second ball, including when frame 10 starts with a strike.
			frame_score_ += next_frame_->score2();
		}
	}
}
