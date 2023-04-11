#include <iostream>
#include <cmath>
using namespace std;
class Polynomial {
protected:
	class Term {
	protected:
		int exponent, coefficient;
		Term *next;
		Term(int exp, int coeff, Term *n=nullptr): exponent(exp), coefficient(coeff), next(n) {}
		friend class Polynomial;
		friend Polynomial operator+ (const Polynomial &p, const Polynomial &q);
		friend Polynomial operator* (const Polynomial &p, const Polynomial &q);
		friend ostream & operator<< (ostream &out, const Polynomial &p);
	};
public:
	Polynomial(): head(nullptr) {}
	Polynomial(const Polynomial &p) {
		purge();
		if (p.head!=nullptr) {
			Term *b, *x;
			for (Term *a=p.head; a!=nullptr; a=a->next) {
				b = new Term(a->exponent, a->coefficient);
				a==p.head ? head=b : x->next=b;
				a = a->next;
				x = b;
			}
		}
	}
	~Polynomial() { purge(); }
	Polynomial & operator=(const Polynomial &p) {
		purge();
		if (p.head!=nullptr) {
			Term *b, *x;
			for (Term *a = p.head; a!=nullptr; a = a->next) {
				b = new Term(a->exponent, a->coefficient);
				a == p.head ? head = b : x->next = b;
				x = b;
			}
		}
		return *this;
	}
	void addTerm(int expon, int coeff) {
		if (coeff==0) return;
		if (head==nullptr) {
			head = new Term(expon, coeff);
			return;
		}
		Term *a=head;
		Term *b=a;
		while (expon < a->exponent) {
			b = a;
			if ((a = a->next)==nullptr) {
				a = new Term(expon, coeff);
				b->next = a;
				return;
			}
		}
		if (expon == a->exponent) {
			if ((a->coefficient += coeff) == 0) {
				if (head->next==nullptr) {
					delete head;
					head = nullptr;
				}
				else if (a==head) {
					head = a->next;
					delete a;
				}
				else {
					b->next = a->next;
					delete a;
				}
			}
		}
		else if (a==head) {
			b = new Term(expon, coeff, a);
			head = b;
			if (head->next->coefficient==0) {
				delete head->next;
			}
		}
		else b->next = new Term(expon, coeff, a);
	}
	double evaluate(double x) {
		if (head==nullptr) return 0.0;
		if (head->exponent==0) return head->coefficient;
		double result = 0.0;
		Term *a = head;
		do result += a->coefficient * pow(x, a->exponent);
		while ((a = a->next)!=nullptr);
		return result;
	}
	friend Polynomial operator+ (const Polynomial &p, const Polynomial &q) {
		Polynomial w;
		Term *a = p.head;
		Term *b = q.head;
		while (a!=nullptr && b!=nullptr) {
			if (a->exponent == b->exponent) {
				w.addTerm(a->exponent, a->coefficient + b->coefficient);
				a = a->next;
				b = b->next;
			}
			else if (a->exponent > b->exponent) {
				w.addTerm(a->exponent, a->coefficient);
				a = a->next;
			}
			else {
				w.addTerm(b->exponent, b->coefficient);
				b = b->next;
			}
		}
		while (b!=nullptr) {
			w.addTerm(b->exponent, b->coefficient);
			b = b->next;
		}
		while (a!=nullptr) {
			w.addTerm(a->exponent, a->coefficient);
			a = a->next;
		}
		return w;
	}
	friend Polynomial operator* (const Polynomial &p, const Polynomial &q) {
		Polynomial w;

		if (p.head==nullptr || q.head==nullptr) return w;
		Term *a = p.head;
		Term *b;
		do {
			b = q.head;
			do w.addTerm(a->exponent + b->exponent, a->coefficient * b->coefficient);
			while ((b = b->next)!=nullptr);
		} while ((a = a->next)!=nullptr);
		return w;
	}
	friend ostream & operator<< (ostream &out, const Polynomial &p) {
		if (p.head==nullptr) return out << 0;
		for (Term *a=p.head; a!=nullptr; a=a->next) {
			if (a->coefficient > 0) {
				if (a!=p.head) out << "+ ";
				if (a->coefficient!=1) out << a->coefficient;
				else if (a->exponent==0) {
					out << 1;
					break;
				}
				if (a->exponent==0) break;
				if (a->exponent!=1) out << "x^" << a->exponent;
				else out << "x";
				if (a->next!=nullptr) out << " ";
			}
			else {
				out << "- ";
				if (a->coefficient!=-1) out << abs(a->coefficient);
				else if (a->exponent==0) {
					out << 1;
					break;
				}
				if (a->exponent==0) break;
				if (a->exponent!=1) out << "x^" << a->exponent;
				else out << "x";
				if (a->next!=nullptr) out << " ";
			}
		}
		return out;
	}
private:
	Term *head;
	void purge() {
		Term *p = head, *q;
		while (p!=nullptr) {
			q = p->next;
			delete p;
			p=q;
		}
		head = nullptr;
	}
};

#ifndef CONTEST
int main() {
	Polynomial p;
	p.addTerm(1, 3);
	p.addTerm(2, 1);
	p.addTerm(0, -1);

	Polynomial q(p);
	q.addTerm(1, -3);

	cout << "P(x) = " << p << endl;
	cout << "P(1) = " << p.evaluate(1) << endl;
	cout << "Q(x) = " << q << endl;
	cout << "Q(1) = " << q.evaluate(1) << endl;
	cout << "(P+Q)(x) = " << p+q << endl;
	cout << "(P*Q)(x) = " << p*q << endl;
}
#endif
