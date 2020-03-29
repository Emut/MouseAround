#ifndef CSEM_H
#define CSEM_H

#include <mutex>
#include <condition_variable>

class CSem{
private:
	std::mutex m_mtx;
	std::condition_variable m_cv;
	int m_count;

	CSem(const CSem &){};

public:
	CSem();
	void Signal();
	void Wait();
};

#endif