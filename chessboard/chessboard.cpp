#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

class ChessBoardArray {
protected:
	class Row {
	public:
		Row(ChessBoardArray &a, int i) : chessboardarray(a), row(i) {}
		int &operator[](int i) const {
			if (i-chessboardarray.base<0 || i-chessboardarray.base>=chessboardarray.size) throw out_of_range("");
			return chessboardarray.select(row, i);
		}
	private:
		ChessBoardArray &chessboardarray;
		int row;
	};
	class ConstRow {
	public:
		ConstRow(const ChessBoardArray &a, int i): chessboardarray(a), row(i) {}
		const int operator[](int i) const {
			if (i-chessboardarray.base<0 || i-chessboardarray.base>=chessboardarray.size) throw out_of_range("");
			return chessboardarray.select(row, i);
		}
	private:
		const ChessBoardArray &chessboardarray;
		int row;
	};

public:
	ChessBoardArray(unsigned size = 0, unsigned base = 0, int default_value=0)
	  : base(base), size(size), even(size%2==0) {
		if (even) length = size*size/2;
		else	  length = size*size/2 + 1;
		data = new int[length];
		for (unsigned i=0; i<length; ++i) data[i] = default_value;
	}
	ChessBoardArray(const ChessBoardArray &a)
	  : base(a.base), length(a.length), size(a.size), even(a.size%2 == 0) {

		data = new int[length];
		for (unsigned i = 0; i < length; ++i) data[i] = a.data[i];
	}
	~ChessBoardArray() { if (size!=0) delete[] data;}
	ChessBoardArray & operator=(const ChessBoardArray &a) {
		delete [] data;
		base = a.base;
		length = a.length;
		size = a.size;
		even = (size%2==0);
		data = new int[length];
		for (unsigned i=0; i<length; ++i) data[i] = a.data[i];
		return *this;
	}

	int & select(int i, int j) {
		if (i-base<0 || j-base<0 || i-base>=size ||j-base>=size) throw out_of_range("This tile is out of the board(3).\n");
		return data[loc(i,j)];
	}
	const int select(int i, int j) const {
		if (i-base<0 || j-base<0 || i-base>=size ||j-base>=size) throw out_of_range("This tile is out ouf the board(4).\n");
		return data[loc(i,j)];
	}
	const Row operator[](int i) {
		if (i-base<0 || i-base>=size) throw out_of_range("This tile is out ouf the board(5).\n");
		return Row(*this, i);
	}
	const ConstRow operator[](int i) const {
		if (i-base<0 || i-base>=size) throw out_of_range("This tile is out ouf the board(6).\n");
		return ConstRow(*this, i);
	}

	friend ostream & operator<<(ostream &out, const ChessBoardArray &a) {
		bool evenrow=true;
		if (a.size==0) ;
		else if (a.even) {
			for (unsigned i=0; i<a.size; ++i)
				if (evenrow) {
					for (unsigned j=0; j<a.size; j+=2)
						out << setw(4) << a[i+a.base][j+a.base] << setw(4) << 0;
					out << endl;
					evenrow = !evenrow;
				}
				else {
					for (unsigned j=1; j<a.size; j+=2)
						out << setw(4) << 0 << setw(4) << a[i+a.base][j+a.base];
					out << endl;
					evenrow = !evenrow;
				}
		}
		else {
			for (unsigned i=0; i<a.size; ++i)
				if (evenrow) {
					for (unsigned j=0; j<a.size-2; j+=2)
						out << setw(4) << a[i+a.base][j+a.base] << setw(4) << 0;

					out << setw(4) << a[i+a.base][a.size-1+a.base] << endl;
					evenrow = !evenrow;
				}
				else {
					for (unsigned j=1; j<a.size; j+=2)
						out << setw(4) << 0 << setw(4) << a[i+a.base][j+a.base];
					out << setw(4) << 0 << endl;
					evenrow = !evenrow;
				}
		}
		return out;
	}

private:
	unsigned base, length, size;
	bool even;
	int *data;
	unsigned loc(int i, int j) const {
		if ((i%2==0) != (j%2==0)) throw out_of_range("This is a black tile(0).\n");
		else if (i<0||j<0) throw out_of_range("This tile is out of the board(1).\n");
		else if (i>=(signed)length || j>=(signed)length) throw out_of_range("This tile is out of the board(2).\n");

		i -= base, j -= base;
		if (even) return (size/2)*i + j/2;
		else
			if (i%2==0) return   size*(i/2)   + j/2;
			else		return (size/2 + 1)*i + j/2;
	}
};

#ifndef CONTEST
int main() {
	ChessBoardArray a(5, 1, 420);
	cout << a;
}
#endif
