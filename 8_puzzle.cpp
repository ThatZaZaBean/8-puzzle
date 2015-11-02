#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <limits>
#include <map>

using namespace std;

class puzzle{
	
	int v[3][3];
	int t_val;	

	public:
		int sol_depth;
		int h_val;
		pair<int, int> zero_pos;

		puzzle(){
			zero_pos = make_pair(2, 2);
			h_val = sol_depth = 0;
			t_val = numeric_limits<int>::max();

		};
		void set_v() {	// sets start state from user input
			int input;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cin >> input; v[i][j] = input;
					if (input == 0)
						zero_pos = make_pair(j, i) ;	// j is x, i is y
				}
			}
		}
		void print();
		void set_state();
		void set_default();	// Set puzzle to default
		void find_zero();	// Stores current "blank" position in zero_pos, might not need
		bool is_equal(puzzle X);	// mostly just to compare to goal

		// Puzzle movement functions
		void move_right();
		void move_left();
		void move_up();
		void move_down();

		int get_t_val();
		void set_t_val();
		void calc_md();		// Returns the Manhattan distance for a given problem.
		puzzle a_star();	// Runs the algorithm
		//vector<int> moves;	// list of moves made

};
// Priority Queue comparator. Hopefully sorts by lowest g(n) + h(n) cost QQ
class Compare {
	public:
		bool operator() (puzzle lhs, puzzle rhs) {
			return lhs.get_t_val() > rhs.get_t_val();
		}
};

puzzle a_star();
puzzle Goal; bool found = false;
priority_queue<puzzle, vector<puzzle>, Compare> open_states;
vector <puzzle> closed_states;

int main(int argc, char* argv[]) {
	// Used for assigning problem size
	//cout << "Enter the size of the puzzle (i.e 4 for 4x4):";
	int size;// cin >> size;
	cout << "Enter numbers in x y z format followed by enter" << endl;

	puzzle start;
	Goal.set_default();	
	start.set_v();

	open_states.push(start);
	puzzle sol = start.a_star();
	cout << endl;
	sol.print();
	/*for (int i = 0; i < sol.moves.size(); i++)
	{
			 
			 if (sol.moves[i] == 1){
				sol.move_right();
				sol.print();
			 }
			 if (sol.moves[i] == 2) {
				 sol.move_left();
				 sol.print();
			 }
			 if (sol.moves[i] == 3) {
				 sol.move_up();
				 sol.print();
			 }
			 if (sol.moves[i] == 4) {
				 sol.move_down();
				 sol.print();
			 }
			 cout << endl;  
	 }*/
}
void puzzle::set_default() {
	 v[0][0] = 1;  v[0][1] = 2;  v[0][2] = 3;
	 v[1][0] = 4;  v[1][1] = 5;  v[1][2] = 6;
	 v[2][0] = 7;  v[2][1] = 8;  v[2][2] = 0;
}
int puzzle::get_t_val() {
	return t_val;
}
void puzzle::set_t_val() {
	t_val = h_val + sol_depth;
}

/* 
    Map mutation functions
	Important to note: first is x-axis
	and second is y-axis. It is somewhat 
	hard to follow if you do not know.
*/
void puzzle::move_right() {
	if (zero_pos.first >= 2){
		// Do nothing, out of bounds
		return;
	}
	else {
		int temp = v[zero_pos.second][zero_pos.first + 1];
		v[zero_pos.second][zero_pos.first] = temp;
		v[zero_pos.second][zero_pos.first + 1] = 0;
		zero_pos = make_pair(zero_pos.first + 1, zero_pos.second);
		return;
	}
}
void puzzle::move_left() {
	if (zero_pos.first <= 0){
		// Do nothing, out of bounds
		return;
	}
	else {
		int temp = v[zero_pos.second][zero_pos.first - 1];
		v[zero_pos.second][zero_pos.first] = temp;
		v[zero_pos.second][zero_pos.first - 1] = 0;
		zero_pos = make_pair(zero_pos.first - 1, zero_pos.second);
		return;
	}
}
/*
	since we go y[0] , move up actually
				y[1] moves down and
				y[2] down moves up

*/
void puzzle::move_up() {
	if (zero_pos.second <= 0){
		// Do nothing, out of bounds
		return;
	}
	else {
		int temp = v[zero_pos.second - 1][zero_pos.first];
		v[zero_pos.second][zero_pos.first] = temp;
		v[zero_pos.second - 1][zero_pos.first] = 0;
		zero_pos = make_pair(zero_pos.first, zero_pos.second - 1);
		return;
	}
}
void puzzle::move_down() {
	if (zero_pos.second >= 2){
		// Do nothing, out of bounds
		return;
	}
	else {
		int temp = v[zero_pos.second + 1][zero_pos.first];
		v[zero_pos.second][zero_pos.first] = temp;
		v[zero_pos.second + 1][zero_pos.first] = 0;
		zero_pos = make_pair(zero_pos.first, zero_pos.second + 1);
		return;
	}
}

// Calculates the manhattan distance for a given map
void puzzle::calc_md() {
	int total = 0, true_x, true_y, curr_val;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{			
			if ((curr_val = v[x][y]) != 0) {	// Don't need to calc blank space
				true_x = (curr_val - 1) / 3;	// Gives the original x-val, rounded down of cv
				true_y = (curr_val - 1) % 3;	// Gives the original y-val of cv

				total += abs(true_x - x) + abs(true_y - y);	// adds distance from origin to total
			}
		}
	}
	h_val = total;
}

bool puzzle::is_equal(puzzle X) { // might be too simple	

	if ((v[0][0] == X.v[0][0]) && (v[0][1] == X.v[0][1]) && (v[0][2] == X.v[0][2])
		&& (v[1][0] == X.v[1][0]) && (v[1][1] == X.v[1][1]) && (v[1][2] == X.v[1][2])
		&& (v[2][0] == X.v[2][0]) && (v[2][1] == X.v[2][1]) && (v[2][2] == X.v[2][2]))
		return true;
	return false;
}


void puzzle::find_zero() {	// might not need

}
void puzzle::set_state(){

}

void puzzle::print() {

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

puzzle puzzle::a_star() {
	puzzle p;
	while (!found) {
		p = open_states.top();
		open_states.pop();
		// Found the end, we done boiz
		if (p.is_equal(Goal)) {
			cout << "We did it!\n"<< p.sol_depth << endl;
			p.print();
			return p;
		}
		else {
			// Update costs
			p.sol_depth = p.sol_depth + 1;
			p.calc_md(); p.set_t_val();
			closed_states.push_back(p);

			if (p.zero_pos.first < 2){	// Can move left
				p.move_right();
				//moves.push_back(1);
				open_states.push(p);
				//moves.pop_back();
				p.move_left();
			}
			if (p.zero_pos.first > 0){	// Can move left
				p.move_left();
				//moves.push_back(2);
				open_states.push(p);
				//moves.pop_back();
				p.move_right();
			}
			if (p.zero_pos.second > 0){	// Can move up
				p.move_up();
				//moves.push_back(3);
				open_states.push(p);
				//moves.pop_back();
				p.move_down();
			}
			if (p.zero_pos.second < 2){	// Can move down
				p.move_down();
				//moves.push_back(4);
				open_states.push(p);
				//moves.pop_back();
				p.move_up();
			}			
		}		
	}
	p.print();
	return p;
}