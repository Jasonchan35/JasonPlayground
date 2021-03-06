#include "common.h"

class Knight2 {
public:
	struct Board;
	class Cell;

	struct Request {
		Board* board = nullptr;
		int totalMoves = 0;
		int found = 0;
	};

	class Cell {
		int		_x = -1;
		int		_y = -1;
		int		_state = 0;
		int		_nextCellCount = 0;
		Cell*	_nextCells[8];
	public:
		struct Cmp { 
			bool operator() (const Cell *a, const Cell *b) const  { 
				return a->_nextCellCount < b->_nextCellCount;
			} 
		} cmp;

		void init(int x, int y) {
			_x = x;
			_y = y;
		}

		int state() { return _state; }

		_forceinline
		void addNext(Cell* p) {
			assert(_nextCellCount < 8);
			_nextCells[_nextCellCount] = p;
			_nextCellCount++;
		}

		_forceinline
		int findNextIndex(Cell* p) {
			for (int i = 0; i < _nextCellCount; i++) {
				if (_nextCells[i] != p) continue;
				return i;
			}
			return -1;
		}

		_forceinline
		void removeNext(Cell* p) {
			auto i = findNextIndex(p);
			if (i < 0) {
				assert(false); // not found
				return;
			}

			if (_nextCellCount > 1) {
				_nextCells[i] = _nextCells[_nextCellCount - 1]; // replace by last one
			}
			_nextCellCount--;
		}

		int doStep(Request& req, int steps) {
		#if 1
			if (steps < req.totalMoves - 1) {
				for (int i = 0; i < _nextCellCount; i++) {
					auto* next = _nextCells[i];
					if (next->_nextCellCount == 1)
						return 0;
				}
			}
		#endif

			_state = steps;
			for (int i = 0; i < _nextCellCount; i++) {
				auto* next = _nextCells[i];
				next->removeNext(this);
			}
	
			if (steps >= req.totalMoves) {
				req.found++;
				if (req.found % 100 == 0) {
					printf("found: %d\n", req.found);
				}
				//req.board->dump();
				_state = 0; // restore
				return 1;
			}

			std::sort(_nextCells, _nextCells + _nextCellCount, cmp);

			steps++;
			int solutionCount = 0;
			for (int i = 0; i < _nextCellCount; i++) {
				auto* next = _nextCells[i];
				if (next->_state != 0) continue;

				solutionCount += next->doStep(req, steps);
				if (next->_nextCellCount == 1) {
					if (steps % 2 != req.totalMoves %2) {
						break;
					}
				}
			}

			for (int i = 0; i < _nextCellCount; i++) {
				auto* next = _nextCells[i];
				next->addNext(this);
			}

			_state = 0;
			return solutionCount;
		}

	};

	struct Board {
		Cell* cell(int x, int y) { 
			if (x < 0 || x >= _size) return nullptr;
			if (y < 0 || y >= _size) return nullptr;
			return &_cells[y][x]; 
		}

		int size() { return _size; }

		void init(int size) {
			_size = size;
			const int dx[8] = {1,  1,  2,  2, -1, -1, -2, -2};
			const int dy[8] = {2, -2,  1, -1,  2, -2,  1, -1};

			for (int y = 0; y < _size; y++) {
				for (int x = 0; x < _size; x++) {
					auto* c = cell(x,y);
					c->init(x,y);
					for (int i = 0; i < 8; i++) {
						int nx = x + dx[i];
						int ny = y + dy[i];
						if (auto* next = cell(nx, ny)) {
							c->addNext(next);
						}
					}
				}
			}
		}

		void dump() {
			for (int y = 0; y < _size; y++) {
				for (int x = 0; x < _size; x++) {
					printf("%5d", cell(x,y)->state());
				}
				printf("\n");
			}
			printf("\n");
		}
	private:
		int _size = 0;
		Cell _cells[MaxBoardSize][MaxBoardSize];	
	};

	int totalMoves;
	int found;
	int result[MaxBoardSize][MaxBoardSize];
	Board board;

	Knight2(int n){
		if (n > MaxBoardSize) {
			assert(false);
			return;
		}

		board.init(n);

		totalMoves = n * n;
		found = 0;

		for (int y = 0; y < board.size(); y++) {
			for (int x = 0; x < board.size(); x++) {
				result[y][x] = 0;
			}
		}
	}

	void dumpResult() {
		for (int y = 0; y < board.size(); y++) {
			for (int x = 0; x < board.size(); x++) {
				printf("%5d", result[y][x]);
			}
			printf("\n");
		}
		printf("\n");
	}

	void resolveOne() {
		resolve(0,0);
	}

	void resolveAll() {
		for (int y = 0; y < board.size(); y++) {
			for (int x = 0; x < board.size(); x++) {
				result[y][x] += resolve(x, y);
			}
		}
		dumpResult();
	}

	int resolve(int x, int y) {
		printf("resolve %d %d...\n", x, y);
		auto* cell = board.cell(x, y);
		if (!cell) return 0;

		Request req;
		req.board = &board;
		req.totalMoves = totalMoves;
		int r = cell->doStep(req, 1);
		printf("solution: %d\n", r);
		return r;
	}
};

