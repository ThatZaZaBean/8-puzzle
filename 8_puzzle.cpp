#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <limits>
#include <map>
#include <ctime>

using namespace std;

class puzzle{
	
	int v[3][3];
	int t_val;	

	public:
		int sol_depth;	// Current depth in the tree
		int h_val;		// Current heuristic value
		int h_type;		// Stores type of heuristic
		int lm;			// The last move the puzzle made
		pair<int, int> zero_pos;	// Location of zero, updated at all points
		int sol_type;	// used for determining heuristic to be used

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
		void set_default();	// Set puzzle to default
		bool is_equal(puzzle X);	// mostly just to compare to goal

		// Puzzle movement functions
		void move_right();
		void move_left();
		void move_up();
		void move_down();

		int get_t_val();
		void set_t_val();	// Calculates h(n) + g(n)
		int  calc_md();		// Returns the Manhattan distance for a given problem.
		int  calc_mt();		// Returns the Misplaced Tile val for a given problem.
		puzzle a_star();	// Runs the algorithm
		vector<int> moves;	// list of moves made
		vector<int> h_moves;	// list of h_val at move location
		vector<int> g_moves;	// list of g_val at move location

};
// Priority Queue comparator. Hopefully sorts by lowest g(n) + h(n) cost QQ
class Compare {
	public:
		bool operator() (puzzle lhs, puzzle rhs) {
			return lhs.get_t_val() > rhs.get_t_val();
		}
};

puzzle a_star();
puzzle Goal;
priority_queue<puzzle, vector<puzzle>, Compare> open_states;
vector <puzzle> closed_states;

int main(int argc, char* argv[]) {
	// Used for assigning problem size
	//cout << "Enter the size of the puzzle (i.e 4 for 4x4):";
	int size;// cin >> size;
	cout << "Enter numbers in x y z format followed by enter three times.\n Or enter all numbers at once with spaces in between!\n Represent a blank space with 0!" << endl;

	puzzle start;
	Goal.set_default();	
	start.set_v();
	cout << endl << "Enter the type of search you want:\n" << " 1. Uniform Cost\n 2. A* w/ Misplaced Tile\n 3. A* w/ Manhattan Distance\n";
	cin >> start.h_type;

	clock_t begin = clock();
	open_states.push(start);
	puzzle sol = start.a_star();
	clock_t end = clock();	
	
	double tot_time = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Puzzle solved in " << tot_time << " seconds\n";
	cout << "Steps displayed below\n\n";
	start.print();

	for (int i = 0; i < sol.moves.size(); i++)  // Takes moves list from
	{                                           // solution and applies it
		if (sol.moves[i] == 1){					// to the start position // so that sol is viewable 
			start.move_right();
			cout << "The best state to expand with a g(n) = " << sol.g_moves[i] << " and h(n) = " << sol.h_moves[i] << " is...\n";
			start.print();
			cout << "Expanding this node!\n";
		}
		if (sol.moves[i] == 2) {
			start.move_left();
			cout << "The best state to expand with a g(n) = " << sol.g_moves[i] << " and h(n) = " << sol.h_moves[i] << " is...\n";
			start.print();
			cout << "Expanding this node!\n";
		}
		if (sol.moves[i] == 3) {
			start.move_up();
			cout << "The best state to expand with a g(n) = " << sol.g_moves[i] << " and h(n) = " << sol.h_moves[i] << " is...\n";
			start.print();
			cout << "Expanding this node!\n";
		}
		if (sol.moves[i] == 4) {
			start.move_down();
			cout << "The best state to expand with a g(n) = " << sol.g_moves[i] << " and h(n) = " << sol.h_moves[i] << " is...\n";
			start.print();
			cout << "Expanding this node!\n";
		}
		cout << endl;
	}
	sol.print();
	cout << endl << "Goal!" << endl;
}
void puzzle::set_default() {
	 v[0][0] = 1;  v[0][1] = 2;  v[0][2] = 3;
	 v[1][0] = 4;  v[1][1] = 5;  v[1][2] = 6;
	 v[2][0] = 7;  v[2][1] = 8;  v[2][2] = 0;
}
int puzzle::get_t_val() {
	return t_val;
}
// Sets total weight based on the desired heuristic
void puzzle::set_t_val() {
	if (h_type == 1) {
		t_val = sol_depth;
		h_moves.push_back(0);
	}
	else if (h_type == 2) {
		t_val = calc_mt() + sol_depth;
		h_moves.push_back(t_val - sol_depth);
	}
	else {
		t_val = calc_md() + sol_depth;
		h_moves.push_back(t_val - sol_depth);
	}
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
int puzzle::calc_md() {
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
	return total;
}

// Calculates the misplaced tiles value for a given map
int puzzle::calc_mt() {
	return ((v[0][0] != 1) + (v[0][1] != 2) + (v[0][2] != 3)	// A very lazy and easy way to tell if 
		+ (v[1][0] != 4) + (v[1][1] != 5) + (v[1][2] != 6)		// the states are in their goal positions
		+ (v[2][0] != 7) + (v[2][1] != 8));
}

bool puzzle::is_equal(puzzle X) { // might be too simple	

	if ((v[0][0] == X.v[0][0]) && (v[0][1] == X.v[0][1]) && (v[0][2] == X.v[0][2])
		&& (v[1][0] == X.v[1][0]) && (v[1][1] == X.v[1][1]) && (v[1][2] == X.v[1][2])
		&& (v[2][0] == X.v[2][0]) && (v[2][1] == X.v[2][1]) && (v[2][2] == X.v[2][2]))
		return true;
	return false;
}

void puzzle::print() {

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (v[i][j] != 0)
				cout << v[i][j] << " ";
			else cout << "b ";
		}
		cout << endl;
	}
}

puzzle puzzle::a_star() {
	puzzle p;
	while (true) {
		p = open_states.top();
		open_states.pop();
		// Found the end, we done boiz
		if (p.is_equal(Goal)) {
			cout << "We did it! Solution found at depth "<< p.sol_depth << "!" << endl;
			cout << "Ending queue size " << closed_states.size() << endl;
			return p;
		}
		else {
			// Update costs
			p.sol_depth = p.sol_depth + 1;
			p.moves.push_back(p.lm);	// Stores the last move made
			p.g_moves.push_back(p.sol_depth);
			p.set_t_val();
			closed_states.push_back(p);

			if (p.zero_pos.first < 2){	// Can move left
				p.move_right();			// Since I neglected to use pointers
				p.lm = 1;				// I have to move the map's position
				open_states.push(p);	// back after pushing it onto the
				p.move_left();			// queue, else states aren't correct
			}
			if (p.zero_pos.first > 0){	// Can move left
				p.move_left();
				p.lm = 2;
				open_states.push(p);
				p.move_right();
			}
			if (p.zero_pos.second > 0){	// Can move up
				p.move_up();
				p.lm = 3;
				open_states.push(p);
				p.move_down();
			}
			if (p.zero_pos.second < 2){	// Can move down
				p.move_down();
				p.lm = 4;
				open_states.push(p);
				p.move_up();
			}			
		}
	}
}