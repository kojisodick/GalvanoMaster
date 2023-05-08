#pragma once


#include <iostream>

template <class T, unsigned int N>
class Queue {

public:
	// キューを初期化する
	void init() {
		m_head = m_tail = 0;
	}

	// キューが空かどうかを判定する
	bool isEmpty() {
		return (m_head == m_tail);
	}

	// スタックが満杯かどうかを判定する
	bool isFull() {
		return (m_head == (m_tail + 1) % N);
	}


	bool push(T v) {
		return enqueue(v);
	}

	T pop() {
		return dequeue();
	}

	// enqueue (tail に要素を格納してインクリメント)
	bool enqueue(T v) {
		if (isFull()) {
			//cout << "error: queue is full." << endl;
			return false;
		}
		m_q[m_tail++] = v;
		if (m_tail == N) m_tail = 0; // リングバッファの終端に来たら 0 に
		return true;
	}

	// dequeue (head にある要素を返して head をインクリメント)
	T dequeue() {
		if (isEmpty()) {
			//cout << "error: stack is empty." << endl;
			return NULL;
		}
		T res = m_q[m_head];
		++m_head;
		if (m_head == N) m_head = 0;
		return res;
	}

private:
	T m_q[N];
	int m_tail;
	int m_head; // キューの要素区間を表す変数

};
