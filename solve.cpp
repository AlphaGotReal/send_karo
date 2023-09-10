#include <bits/stdc++.h>

using namespace std;

class point{
public:
	double x;
	double y;
	
	point(){
		this->x = 0;
		this->y = 0;
	}

	point(double x, double y){
		this->x = x;
		this->y = y;
	}
};

class twist{
public:
	double linear;
	double angular;

	twist(){
		this->linear = 0;
		this->angular = 0;
	}

	twist(double linear, double angular){
		this->linear = linear;
		this->angular = angular;
	}
};

class vehicle{
public:
	point position;
	double orientation;

	vehicle(){
		this->position = point(0, 0);
		this->orientation = 0;
	}

	vehicle(point position, double orientation){
		this->position = position;
		this->orientation = orientation;
	}

	void step(point dr, double ang){
		this->position.x += dr.x;
		this->position.y += dr.y;
		this->orientation += ang;
		if (this->orientation > 2*M_PI) this->orientation -= 2*M_PI;
		else if (this->orientation < 0) this->orientation += 2*M_PI;
	}

	void step_back(point dr, double ang){
		this->position.x -= dr.x;
		this->position.y -= dr.y;
		this->orientation -= ang;
		if (this->orientation > 2*M_PI) this->orientation -= 2*M_PI;
		else if (this->orientation < 0) this->orientation += 2*M_PI;
	}
};

class graph{
public:
	int w;
	int h;
	vector<vector<int>> occupancy_grid;
	double resolution;

	graph(){}

	graph(vector<vector<int>> occupancy_grid){
		this->w = occupancy_grid[0].size();
		this->h = occupancy_grid.size();
		this->occupancy_grid = occupancy_grid;
	}

	vector<int> neighbors(int node){
		int x = node%this->w;
		int y = node/this->w;
		vector<int> bors;
		for (int dx = -1; dx < 2; dx++){
			for (int dy = -1; dy < 2; dy++){
				if (dx == dy && dy == 0) continue;
				if (this->is_valid(x+dx, y+dy)){
					if (this->occupancy_grid[y+dy][x+dx] == 0){
						bors.push_back(x + dx + this->w*(y + dy));
					}
				}
			}
		}return bors;
	}

	vector<point> a_star(int start, int end){
		int sx = start%this->w;
		int sy = start/this->w;

		int ex = end%this->w;
		int ey = end%this->w;

		vector<point> path;
		vector<int> parent(this->w * this->h, -1);
		vector<bool> explored(this->w * this->h, false);
		for (int t = 0; t < this->w * this->h; t++){
			parent[t] = t;
		}

		vector<double> gc(this->w * this->h, INFINITY);
		vector<double> fc(this->w * this->h, INFINITY);

		double g = this->g_cost(start, start);

		gc[start] = g;
		fc[start] = this->h_cost(start, end) - g;

		priority_queue<tuple<double, double, int>> open; //-f_cost, g_cost, node
		open.push(make_tuple(-fc[start], gc[start], start));

		tuple<double, double, int> curr;
		while (!open.empty()){
			curr = open.top();
			if (get<2>(curr) == end){
				int pth = end;
				while (parent[pth] != pth){
					path.push_back(point(pth%this->w * this->resolution, (int)(pth/this->w) * this->resolution));
					pth = parent[pth];
				}return path;
			}open.pop();
			explored[get<2>(curr)] = true;
			vector<int> neighbors_ = this->neighbors(get<2>(curr));
			for (int node: neighbors_){
				double expected_g_cost = get<1>(curr) + this->g_cost(get<2>(curr), node);
				if (expected_g_cost < gc[node]){
					gc[node] = expected_g_cost;
					fc[node] = expected_g_cost - this->h_cost(node, end);
					parent[node] = get<2>(curr);
					if (explored[node]) continue;
					open.push(make_tuple(-fc[node], gc[node], node));
				}
			}
		}
		return path;
	}

private:
	bool is_valid(int x, int y){
		bool one = (x < this->w && x >= 0);
		bool two = (y < this->h && y >= 0);
		return one && two;
	}

	double digna(int n1, int n2){
		if (n1 == n2) return 0.0;
		int X = this->w;
		int dx = abs(n1%X - n2%X);
		int dy = abs(n1/X - n2/X);

		return min(dx, dy) * sqrt(2) + abs(dx - dy);
	}

	double distance(int n1, int n2){
		if (n1 == n2) return 0.0;
		int X = this->w;
		int dx = abs(n1%X - n2%X);
		int dy = abs(n1/X - n2/X);
	
		return dx*dx + dy*dy;
	}

	double h_cost(int node, int end){
		return this->distance(node, end);
	}

	double g_cost(int node, int next){
		return this->digna(node, next);
	}
};

double max_v = 1.0;
double min_v = 0.1;
double max_w = 0.7;
double min_w = -0.7;

double v_buckets = 5;
double w_buckets = 5;

double dv = (double)(max_v - min_v)/(v_buckets-1);
double dw = (double)(max_w - min_w)/(w_buckets-1);

double sigmoid(double x){
	return 1/(1 + exp(x));
}

namespace cost{
	double alpha = 0.05;
	double beta = 0.3;
	double gamma = 0.01;
	double Obstacle(vehicle bot, vector<vector<int>> grid){
		double distance = INFINITY;
		for (int y = 0; y < grid.size(); y++){
			for (int x = 0; x < grid[0].size(); x++){
				if (grid[y][x]){
					double dx = bot.position.x - x - 0.5;
					double dy = bot.position.y - y - 0.5;
					distance = min(distance, dx*dx + dy*dy);
				}
			}
		}
		return -distance;
	}

	double Path(vehicle bot, vector<point> path){
		double distance = INFINITY;
		for (point p: path){
			double dx = bot.position.x - p.x;
			double dy = bot.position.y - p.y;
			distance = min(distance, dx*dx+dy*dy);
		}return distance;
	}

	double Goal(vehicle bot, point goal){
		double dx = goal.x - bot.position.x;
		double dy = goal.y - bot.position.y;
		return (dx*dx + dy*dy);
	}

	double Total(vehicle bot, vector<vector<int>> grid, vector<point> path, point goal){
		return alpha * Obstacle(bot, grid) + 
			beta * Path(bot, path) + 
			gamma * Goal(bot, goal);
	}
}

twist get_best_action(
		vehicle bot, 
		vector<vector<int>> grid, 
		vector<point> path, 
		point goal, 
		double dt){
	twist speed(0, 0);
	double min_cost = INFINITY, curr_cost;
	for (double v = min_v; v <= max_v; v += dv){
		for (double w = min_w; w <= max_w; w += dw){
			//calculate dr
			point dr(v*dt*cos(bot.orientation+w*dt), v*dt*sin(bot.orientation+w*dt));

			//simulate dt secs into the future with dr
			bot.step(dr, w*dt);
			if (bot.position.x < 0 || bot.position.x >= grid[0].size() || bot.position.y < 0 || bot.position.y >= grid.size()){
				bot.step_back(dr, w*dt);
				continue;
			}

			curr_cost = cost::Total(bot, grid, path, goal);

			if (min_cost > curr_cost){
				speed.linear = v;
				speed.angular = w;
				min_cost = curr_cost;
			}bot.step_back(dr, w*dt);
		}
	}
	return speed;
}

void display_grid(vector<vector<int>> grid){
	for (int r = grid.size()-1; r >= 0; r--){
		for (int c: grid[r]){
			if (c == 1){
				cout << "# ";
			}else {
				cout << ". ";
			}
		}cout << endl;
	}
}

int main(){

	int w, h;
	cin >> w >> h;

	double resolution = 1;
	int W = w/resolution;
	int H = h/resolution;

	vector<vector<int>> occupancy_grid(h, vector<int>(w));
	vector<vector<int>> finer_grid(H, vector<int>(W));
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			cin >> occupancy_grid[h-y-1][x];
			for (int Y = y/resolution; Y < (y+1)/resolution; Y++){
				for (int X = x/resolution; X < (x+1)/resolution; X++ ){
					finer_grid[h/resolution - Y - 1][X] = occupancy_grid[h-y-1][x];
				}
			}
		}
	}

	//spawn info
	point position(0, 0);
	double orientation = 0;
	vehicle bot(position, orientation);

	//goal info
	point goal(w, h);
	
	//display_grid(occupancy_grid);

	graph space(occupancy_grid);
	space.resolution = resolution;

	double dt = 1;
	vector<point> path = space.a_star(w, w*h/resolution/resolution -1);

	while (cost::Goal(bot, goal) > 1){
		twist action = get_best_action(bot, occupancy_grid, path, goal, dt);
		point dr(action.linear*dt*cos(bot.orientation), action.linear*dt*sin(bot.orientation));
		double angle = action.angular*dt;
		bot.step(dr, angle);
		cout << action.linear << ' ' << action.angular << endl;
	}

	return 0;
}


